//using devkitPro textured_cube example as starting point
//needs to be in c++, not c, for tiny obj loader's dependencies

#include <3ds.h>
#include <citro3d.h>
#include <tex3ds.h>
#include <string.h>
#include "vshader_shbin.h"
#include "grid_t3x.h"		//image texture needs to be a square and power of two, like 64x64 or 128x128

//for tiny obj loader
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define CLEAR_COLOR 0x68B0D8FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

#define OBJ_PATH "romfs:/T-Block.obj"

typedef struct { float position[3]; float texcoord[2]; float normal[3]; } vertex;	//keep this as-is from the example

/*
Loads a .obj file and returns a flat array of vertices ready for citro3d

citro3d needs an array with the vertices of every triangle, even ones that
might be considered 'duplicates' because multiple triangles have a vertex
at that point. 

Parameters:
	path		- romfs path to the .obj file, like this: "romfs:/cube.obj"
	out_count	- pointer to a variable that stores the number of vertices

Returns:
	heap-allocated array of vertex structs or nullptr if loading failed (notes 1)

*/
vertex* load_obj(const char* path, int* out_count)
{
	//tiny_obj_loader output containers
	//these 4 lines are generally always going to exist like this
	tinyobj::attrib_t attrib;						//holds positions, UVs, and normals
    std::vector<tinyobj::shape_t> shapes;			//holds the meshes
    std::vector<tinyobj::material_t> materials;		//holds materials (unused here)
    std::string err;								//receives any errors from the loader

	//load the .obj file and fill the containers defined above
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path)) {
		*out_count = 0;
		return nullptr;
	}

	//count the number of indices, including duplicates, within each shape in the file
	int tot = 0;
	for (const auto& shape : shapes)		//equivalent to "for shape in shapes" in Python
		tot += shape.mesh.indices.size();

	//allocate the flat vertex array on the heap
	//the caller will need to free() it later
	vertex* verts = (vertex*)malloc(sizeof(vertex) * tot);
	int verts_index = 0;	//tracks which output vertex index is being filled

	//loop over every shape in the file
	for (const auto& shape : shapes) {

		//loop over every index in the shape's mesh and link the data (notes 2)
		//mesh.indices allows us to iterate over each vertex and skip over
		//the grouping by face
		for (const auto& index : shape.mesh.indices) {

			// --- POSITION ---
			//vertex positions are stored in a flat float array: [x0, y0, z0, x1, y1, z1, ...]
			//so vertex V starts at index V*3
			verts[verts_index].position[0] = attrib.vertices[3 * index.vertex_index + 0]; // X
            verts[verts_index].position[1] = attrib.vertices[3 * index.vertex_index + 1]; // Y
            verts[verts_index].position[2] = attrib.vertices[3 * index.vertex_index + 2]; // Z

			// --- TEXTURE COORDINATES ---
			//texture coordinates are stored in a flat float array: [u0, v0, u1, v1, ...]
            //so texture coordinate C starts at index C*2
			if (index.texcoord_index >= 0) {
                verts[verts_index].texcoord[0] =        attrib.texcoords[2 * index.texcoord_index + 0];	// U
                verts[verts_index].texcoord[1] = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1];	// V (flipped)
				//see notes 2 for details on why flipping is needed
            } else {
                //if the texcoord_index is -1, there is no UV so we default to (0,0)
                verts[verts_index].texcoord[0] = 0.0f;
                verts[verts_index].texcoord[1] = 0.0f;
            }

			// --- NORMALS ---
			//normals are stores in a flat float array: [nx0, ny0, nz0, nx1, ny1, nz1, ...]
			//so normal N starts at index N*3
			if (index.normal_index >= 0) {
                verts[verts_index].normal[0] = attrib.normals[3 * index.normal_index + 0]; // NX
                verts[verts_index].normal[1] = attrib.normals[3 * index.normal_index + 1]; // NY
                verts[verts_index].normal[2] = attrib.normals[3 * index.normal_index + 2]; // NZ
            } else {
                //if the normal_index is -1, there are no normals so we default to (0,0,1)
                verts[verts_index].normal[0] = 0.0f;
                verts[verts_index].normal[1] = 0.0f;
                verts[verts_index].normal[2] = 1.0f;
            }

			verts_index++; //move to the next vertex index
		}
	}

	*out_count = tot;	//set variable out_count to the number of vertices
	return verts;
}

static DVLB_s* vshader_dvlb;
static shaderProgram_s program;
static int uLoc_projection, uLoc_modelView;
static int uLoc_lightVec, uLoc_lightHalfVec, uLoc_lightClr, uLoc_material;
static int uLoc_viewPos;
static C3D_Mtx projection;
static C3D_Mtx material =
{
	{
	{ { 0.0f, 0.2f, 0.2f, 0.2f } }, // Ambient
	{ { 0.0f, 0.4f, 0.4f, 0.4f } }, // Diffuse
	{ { 0.0f, 0.8f, 0.8f, 0.8f } }, // Specular
	{ { 1.0f, 0.0f, 0.0f, 0.0f } }, // Emission
	}
};

static void* vbo_data;
static C3D_Tex grid_tex;
static float angleX = 0.0, angleY = 0.0;

// Helper function for loading a texture from memory
static bool loadTextureFromMem(C3D_Tex* tex, C3D_TexCube* cube, const void* data, size_t size)
{
	Tex3DS_Texture t3x = Tex3DS_TextureImport(data, size, tex, cube, false);
	if (!t3x)
		return false;

	// Delete the t3x object since we don't need it
	Tex3DS_TextureFree(t3x);
	return true;
}

/*
Changes compared to sample:
	- Now loads an object from .obj file instead of using hard-coded vertices
	- now returns vertex_count (number of vertices)
*/
static int sceneInit(void)
{
	// Load the vertex shader, create a shader program and bind it
	vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&program);

	// Get the location of the uniforms
	uLoc_projection   	= shaderInstanceGetUniformLocation(program.vertexShader, "projection");
	uLoc_modelView    	= shaderInstanceGetUniformLocation(program.vertexShader, "modelView");
	uLoc_lightVec     	= shaderInstanceGetUniformLocation(program.vertexShader, "lightVec");
	uLoc_lightHalfVec 	= shaderInstanceGetUniformLocation(program.vertexShader, "lightHalfVec");
	uLoc_lightClr     	= shaderInstanceGetUniformLocation(program.vertexShader, "lightClr");
	uLoc_material     	= shaderInstanceGetUniformLocation(program.vertexShader, "material");
	uLoc_viewPos	  	= shaderInstanceGetUniformLocation(program.vertexShader, "viewPos");

	// Configure attributes for use with the vertex shader
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=texcoord
	AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // v2=normal

	// Compute the projection matrix
	Mtx_PerspTilt(&projection, C3D_AngleFromDegrees(80.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, false);

	//create vertex_list and calculate size
	int vertex_count = 0;
	vertex* vertex_list = load_obj(OBJ_PATH, &vertex_count);

	// Create the VBO (vertex buffer object)
	vbo_data = linearAlloc(sizeof(vertex) * vertex_count);			//use sizeof the struct vertex
	memcpy(vbo_data, vertex_list, sizeof(vertex) * vertex_count);

	//now free the vertex list from the heap (notes 1)
	free(vertex_list);

	// Configure buffers
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vbo_data, sizeof(vertex), 3, 0x210);

	// Load the texture and bind it to the first texture unit
	if (!loadTextureFromMem(&grid_tex, NULL, grid_t3x, grid_t3x_size))
		svcBreak(USERBREAK_PANIC);
	C3D_TexSetFilter(&grid_tex, GPU_LINEAR, GPU_NEAREST);
	C3D_TexBind(0, &grid_tex);

	//global render state — set once, affects all draw calls
	C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD,
		GPU_SRC_ALPHA, GPU_ONE_MINUS_SRC_ALPHA,
		GPU_SRC_ALPHA, GPU_ONE_MINUS_SRC_ALPHA);

	//set up rendering environment
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvColor(env, 0x80FFFFFF); //set 50% transparency (remember first two bytes are alpha)

	//set RGB and Alpha sources and functions separately
	C3D_TexEnvSrc(env, C3D_RGB, GPU_TEXTURE0, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
	C3D_TexEnvSrc(env, C3D_Alpha, GPU_CONSTANT, GPU_CONSTANT, GPU_CONSTANT);
	C3D_TexEnvFunc(env, C3D_RGB, GPU_MODULATE);
	C3D_TexEnvFunc(env, C3D_Alpha, GPU_REPLACE);

	//disable tests to make sure it renders properly
	C3D_AlphaTest(false, GPU_ALWAYS, 0);

	return vertex_count;
}

/*
Changes compared to sample:
	- Now takes vertex_count (number of vertices) as an argument
*/
static void sceneRender(int vertex_count)
{
	// Calculate the modelView matrix
	C3D_Mtx modelView;
	Mtx_Identity(&modelView);
	Mtx_Translate(&modelView, 0.0, 0.0, -2.0 + 0.5*sinf(angleX), true);
	Mtx_RotateX(&modelView, angleX, true);
	Mtx_RotateY(&modelView, angleY, true);

	// Rotate the cube each frame
	angleX += M_PI / 180;
	angleY += M_PI / 360;

	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView,  &modelView);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_material,   &material);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightVec,     0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightHalfVec, 0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightClr,     1.0f, 1.0f,  1.0f, 1.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_viewPos, 0.0f, 0.0f, 0.0f, 1.0f);			//just pass the origin directly

	//ensure alpha blend is not reset
	C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD,
		GPU_SRC_ALPHA, GPU_ONE_MINUS_SRC_ALPHA,
		GPU_SRC_ALPHA, GPU_ONE_MINUS_SRC_ALPHA);

	// Draw the VBO
	C3D_DrawArrays(GPU_TRIANGLES, 0, vertex_count);
}

static void sceneExit(void)
{
	// Free the texture
	C3D_TexDelete(&grid_tex);

	// Free the VBO
	linearFree(vbo_data);

	// Free the shader program
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);
}

int main()
{
	// Initialize graphics
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	//initialize romfs
	romfsInit();

	// Initialize the render target
	C3D_RenderTarget* target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	// Initialize the scene
	int vertex_count = sceneInit();

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C3D_RenderTargetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
			C3D_FrameDrawOn(target);
			sceneRender(vertex_count);
		C3D_FrameEnd(0);
	}

	// Deinitialize the scene
	sceneExit();

	// Deinitialize romfs
	romfsExit();

	// Deinitialize graphics
	C3D_Fini();
	gfxExit();
	return 0;
}
