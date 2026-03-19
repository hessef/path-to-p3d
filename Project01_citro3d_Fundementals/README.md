# Project 1: citro3d Fundementals Report

### Section 1: PICA200 vs. Standard OpenGL

The Graphics Processing Unit (GPU) inside the Nintendo 3DS, 2DS, New 3DS, and New 2DS lines of handheld game consoles is a PICA200. The PICA200 is a hybrid GPU, featuring a fixed-function fragment stage in addition to four programmable vertex processors. The vertex processors can be programmed using picasso, which is a specialized assembly language made specifically for the PICA200. It features standard instructions such as **mov**, **add**, and **mul**, but is notably missing some like **sub**. It also has dedicated instructions for 3d and 4d dot products (**dp3** and **dp4**, respectively), and allows negating values in registers as a hardware supported action. 

While the PICA200 is advertised as supporting OpenGL ES 1.1, it does have a few notable quirks when compared to that GPU Application Programming Interface (API). The most notable one, and the one that I specifically ran into while working on this project, is that the pixel dimensions of textures are expected to be powers of two. A texture with a non-standard resolution, like 500x500, can still be used, but it will result in an offset when rendering and gaps along the seams of faces. This kind of problem is a non-issue in modern OpenGL, which allows textures of arbitrary size to be applied to meshes. 

---

### Section 2: Alpha Blending on PICA200

Alpha blending is a graphics technique that simulates transparency by combining the color of an object with the color of whatever is behind it. 