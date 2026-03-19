# Project 1: citro3d Fundementals Report

### Section 1: PICA200 vs. Standard OpenGL

The Graphics Processing Unit (GPU) inside the Nintendo 3DS, 2DS, New 3DS, and New 2DS lines of handheld game consoles is a PICA200. The PICA200 is a hybrid GPU, featuring a fixed-function fragment stage in addition to four programmable vertex processors. The vertex processors can be programmed using picasso, which is a specialized assembly language made specifically for the PICA200. It features standard instructions such as **mov**, **add**, and **mul**, but is notably missing some like **sub**. It also has dedicated instructions for 3d and 4d dot products (**dp3** and **dp4**, respectively), and allows negating values in registers as a hardware supported action. 

While the PICA200 is advertised as supporting OpenGL ES 1.1, it does have a few notable quirks when compared to that GPU Application Programming Interface (API). The most notable one, and the one that I specifically ran into while working on this project, is that the pixel dimensions of textures are expected to be powers of two. A texture with a non-standard resolution, like 500x500, can still be used, but it will result in an offset when rendering and gaps along the seams of faces. This kind of problem is a non-issue in modern OpenGL, which allows textures of arbitrary size to be applied to meshes. 

---

### Section 2: Alpha Blending on PICA200

Alpha blending is a graphics technique that simulates transparency by combining the color of an object with the color of whatever is behind it. To achieve this with the PICA200, the **C3D_AlphaBlend** command is used to set the alpha blending pattern. In my case, I did

```c++
C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD,
		GPU_SRC_ALPHA, GPU_ONE_MINUS_SRC_ALPHA,
		GPU_SRC_ALPHA, GPU_ONE_MINUS_SRC_ALPHA);
```

which sets the mode to additive blending, using the alpha from the source. The source in this case is the image texture. If the texture does not have an alpha channel or is completely opaque, like mine is, it is possible to manually set the alpha level and pass that through to the PICA200. That is enough to enforce uniform transparency across an entire texture, but if modulated alpha blending is required, it is necessary to write some additions to the generic picasso assembly code. This assembly code is also called a shader file.

For this project, I modified the shader file to implement the Fresnel Effect. To do this, the necessary calculations were done and then applied to the alpha channel of the pre-clamped output value. My modifications to the code are shown below:

```
...
mov r5,		r1	        ; save normalized view-space normal into r5
...
; Apply Fresnel effect
dp3 r4, r3, r3			; squared length
rsq r4, r4				; 1/sqrt
mul r3, r4, r3			; normalized view direction
dp3 r4.x, r3, r5		; Fresnel = dot(viewDir, normal)
mov r1.w, r4.x			; put Fresnel into alpha channel before clamping
```

---

### Section 3: Reflection

This project allowed me to learn about the 3DS rendering pipeline, from importing geometry to creating custom shader behavior. Along the way a few things surprised me or were otherwise interesting. The fact that .obj files have an inverted vertical axis for texture coordinates, for instance, was very surprising. The industry standard for image UVs is to have the origin at the top left of the image with the horizontal axis extending to the right and the vertical axis extending down. But in .obj files, the origin is at the bottom left, with the horizontal and vertical axes extending to the right and up, respectively. This felt like an odd design choice since most rendering applications and GPU APIs use the standard format.

Another interesting thing I learned was the way that rendering tasks are split between the Central Processing Unit (CPU) and the GPU. I knew on a theoretical level that the CPU handled draw calls and buffer allocation while the GPU just crunched numbers and did linear algebra before engaging in this project, but actually programming the behavior myself instead of just relying on a high-level abstraction really made it sink in.