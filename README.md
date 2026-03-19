# PERSONA 3D
### A Self-Directed Course in Console Porting
#### Porting Persona 3 FES to the Nintendo 3DS — Fritz Hesser · Spring 2025 – Ongoing

> *"Persona 3D — because the name demands it exist."*

---

## Course Overview

This repository documents a self-directed course in console game porting. The end goal is a playable port of Persona 3 FES running on a New Nintendo 2DS XL — informally known as Persona 3D.

The course is structured as a ladder of projects, each building skills and producing artifacts that feed directly into the next. Every project is scoped to be completable in available free time (weekends and evenings), with focused sprint periods where possible. Each completed project is independently useful, publicly releasable, and documented here.

This README serves as the living syllabus for the project. Completed projects link to their respective folders and write-ups. The capstone section describes the full port architecture and how each ladder project feeds into it.

---

## Prerequisites (Already Satisfied)

> **This course begins with the following already in place:**
> - C/C++ proficiency at a systems level
> - 3DS homebrew development with devkitPro/libctru ([Rokenbok control project](https://github.com/hessef/3ds-rokenbok-control))
> - Cross-platform systems integration (3DS ↔ Python server ↔ Arduino)
> - Interpreter/VM design (Initiate Protocol scripting language)
> - Hardware architecture fundamentals (64-bit ARM CPU in Verilog)
> - Real-time data pipelines and DSP (JANUS modulator, TI radar pipeline)
> - 3D content creation in Blender
> - Binary protocol and format parsing

---

## Project Ladder

| Phase | Project & Focus | Status |
|---|---|---|
| Phase 1 — Graphics Foundation | [Project 1: citro3d Fundamentals](#phase-1--project-1--citro3d-fundamentals) | 🟩 Completed 03/19/2026 |
| Phase 2 — PS2 Fluency | [Project 2: PS2 Pipeline Study + P3 Asset Extraction](#phase-2--project-2--ps2-pipeline-study--persona-3-asset-extraction) | 🔲 Not started |
| Phase 3 — Dry Run Port | [Project 3: Port a Small PS2 Homebrew to 3DS](#phase-3--project-3--port-a-small-ps2-homebrew-to-3ds) | 🔲 Not started |
| Phase 4 — Media Systems | [Project 4: ADX Audio + MVD Video Subsystem](#phase-4--project-4--adx-audio--mvd-video-subsystem) | 🔲 Not started |
| Phase 5 — Real Assets | [Project 5: Persona 3 Model/Map Viewer on 3DS](#phase-5--project-5--persona-3-modelmap-viewer-on-3ds) | 🔲 Not started |
| Phase 6 — Core Gameplay | [Project 6: Persona 3 Combat Prototype on 3DS](#phase-6--project-6--persona-3-combat-prototype-on-3ds) | 🔲 Not started |
| Capstone | [Persona 3D: Full Port Integration](#capstone-persona-3d) | 🔲 Not started |

---

## Estimated Timeline

Assuming ~10–15 hours/week average with periodic focused sprints:

| Milestone | Cumulative Estimate |
|---|---|
| Projects 1–2 complete | 4–8 weeks from start |
| Project 3 complete (dry run port done) | 3–4 months from start |
| Project 4 complete (audio + video on 3DS) | 4–6 months from start |
| Projects 5–6 complete (assets + combat) | 8–12 months from start |
| Persona 3D: first full scene playable | 1.5–2 years from start |
| Persona 3D: full game playable (rough) | 2–3 years from start |
| Persona 3D: polished release | 3–4 years from start |

---

---

# Phase 1 · Project 1 · citro3d Fundamentals
### Mastering the 3DS GPU Pipeline

| Attribute | Value |
|---|---|
| Estimated Time | 1–2 focused weekends (8–20 hrs total) |
| Difficulty | Intermediate |
| Status | 🟩 Completed 03/19/2026 |

### [Project 1 Folder](Project01_citro3d_Fundamentals) 

## Overview

Build a real-time 3D scene on the New 2DS XL using citro3d. The suggested deliverable is a stained glass ornament — a translucent, textured 3D object with correct alpha blending and lighting. This forces engagement with the parts of citro3d the standard cube example skips: alpha blend state, draw ordering, and routing the alpha channel through the TexEnv correctly. By the end, the full PICA200 rendering pipeline should be understood well enough to use deliberately rather than by copying examples.

## Background & Motivation

The Rokenbok project proves the 3DS toolchain and hardware aren't foreign territory. What's new here is the GPU itself. The PICA200 is a fixed-function + programmable hybrid that predates modern OpenGL ES conventions in several ways and diverges from them in others.

citro3d is the community C library wrapping PICA200 into a usable API. It handles vertex buffer objects, texture binding, shader programs, and render targets. picasso is the PICA200 shader assembler used to write custom vertex shaders in PICA200 assembly.

The stained glass ornament is a better deliverable than a simple lit cube because it forces two things the cube example avoids: alpha blending (requiring explicit blend state setup on the CPU side and correct alpha routing through TexEnv) and draw ordering (transparent objects must be drawn back-to-front or blending breaks). Both are real constraints that will appear throughout the port.

One known gotcha to expect: textures must be in Morton/Z-order (tiled) format, not standard linear row-major layout. tex3ds handles this at build time and the kitten example already uses it — but understanding why it exists and what happens if you get it wrong is worth the time.

## Learning Objectives

By the end of this project you will be able to:

- Explain the PICA200 rendering pipeline from vertex buffer to framebuffer
- Write and compile a basic PICA200 vertex shader using picasso
- Implement correct alpha blending for a transparent object including draw order management
- Load an external 3D model (OBJ or similar) and render it with correct textures and lighting
- Profile frame timing on real hardware and identify GPU vs CPU bottlenecks
- Convert linear textures to Morton/tiled format required by the PICA200

## Required Deliverables

> **All of the following must be present to consider this project complete:**
> 1. A running .3dsx homebrew app rendering a textured, lit, transparent 3D object (e.g. stained glass ornament) at 60fps on real New 2DS XL hardware
> 2. Geometry loaded from a file at runtime (OBJ or similar) — not a hardcoded vertex list
> 3. At least one custom vertex shader written in picasso PICA200 assembly
> 4. A post-mortem README in the project folder covering: what PICA200 does differently from standard OpenGL, how alpha blending works on PICA200, and what surprised you
> 5. Source on GitHub with a working build system

## Resources

**Primary Documentation**
- [devkitPro wiki (toolchain setup)](https://devkitpro.org/wiki/Getting_Started)
- [citro3d GitHub repository and API docs](https://github.com/devkitPro/citro3d)
- [PICA200 documentation (3DBrew wiki)](https://www.3dbrew.org/wiki/GPU)
- [picasso shader assembler](https://github.com/devkitPro/picasso)

**Example Codebases**
- [citro3d examples (official — includes textured cube, lighting)](https://github.com/devkitPro/3ds-examples)

**Useful Libraries**
- [tinyobjloader (single-header OBJ loader)](https://github.com/tinyobjloader/tinyobjloader)
- [tex3ds (build-time texture conversion tool)](https://github.com/devkitPro/tex3ds)

**Key Concepts to Study**
- Morton/Z-order texture layout — 3DBrew GPU texture format page
- Alpha blending state in citro3d — `C3D_AlphaBlend` and `GPU_BLEND_*` constants
- TexEnv configuration for alpha passthrough vs modulation
- Back-to-front draw ordering for transparent geometry
- PICA200 vertex attribute binding — how position, UV, normal map to v0/v1/v2

## Getting Started

> Clone the citro3d examples repo and get the textured cube running on hardware first. Read every line of it — the cube example is dense with things that aren't explained in comments. Then swap the cube geometry for an OBJ file exported from Blender using tinyobjloader. That substitution forces you to understand vertex buffer layout and texture loading.
>
> Once you have an opaque textured model working, adding transparency is the next step: enable alpha blending via `C3D_AlphaBlend`, ensure your texture has an alpha channel with actual transparency data, and update the TexEnv to pass alpha through correctly. The moment it breaks because you drew faces in the wrong order is the moment draw ordering becomes concrete rather than theoretical.

## Career Connection

Low-level GPU pipeline knowledge applies to any display system work in embedded and defense electronics. Performance profiling — understanding how to hit a hard real-time deadline and budget CPU vs GPU work — transfers directly to real-time signal processing systems where missing a timing deadline has real consequences.

---

---

# Phase 2 · Project 2 · PS2 Pipeline Study & Persona 3 Asset Extraction
### Understanding Both Ends of the Port

| Attribute | Value |
|---|---|
| Estimated Time | 2–3 focused weekends (15–25 hrs total) |
| Difficulty | Intermediate |
| Status | 🔲 Not started |

## Overview

Two parallel tracks. Track A: understand the PS2 graphics pipeline — GS, VIF, GIF — by reading PCSX2 source and writing minimal PS2 homebrew in PCSX2. Track B: extract and inspect Persona 3 FES's actual game data using community tools, document what's covered and what isn't, and identify gaps. The Persona 3 Portable PC port on Steam is an additional reference for game logic systems shared between the two versions.

## Background & Motivation

The PS2 Emotion Engine is a MIPS R5900. Having implemented an ARM CPU in Verilog, the architecture won't be alien — the same fetch-decode-execute mental model applies. What's genuinely unusual about the PS2 is the rendering path: geometry goes through the VIF units, which run microcode on VU0/VU1, which emit GIF packets consumed by the GS rasterizer. It's DMA-driven all the way down, with no conventional draw call API.

Understanding this pipeline matters because when reading a Persona 3 draw call in PCSX2's debugger, the GIF packets and GS register writes need to be legible. PCSX2's GS and VIF source code is the reference implementation — it documents exact hardware behavior in runnable code.

The Persona 3 Portable PC port (available on Steam) is a useful secondary reference for game logic. The shared systems — combat, Tartarus, Social Links, calendar — are almost certainly the same underlying logic compiled for a different target. x86-64 Windows binaries are significantly more readable in Ghidra/IDA than MIPS R5900 PS2 binaries, so for pure game logic research the PC port is the easier starting point. FES-specific content (FMV cutscenes, The Answer) still requires PS2-only research.

## Learning Objectives

By the end of this project you will be able to:

- Explain the PS2 rendering pipeline from game code to pixel: EE → VIF → VU → GIF → GS
- Read and understand GIF packet structure and GS register writes
- Write a minimal PS2 homebrew program in PCSX2 that renders geometry using the GS directly
- Extract all major asset categories from a Persona 3 FES ISO using community tools
- Produce a documented format inventory: what's known, what's unknown, what tools cover it
- Successfully open a Persona 3 character model and texture in Blender

## Required Deliverables

> **All of the following must be present to consider this project complete:**
> 1. A running PS2 homebrew demo in PCSX2 that renders geometry using the GS pipeline
> 2. A format inventory document cataloguing every major Persona 3 FES file type, its purpose, and whether community tooling covers it
> 3. At least one Persona 3 character model and one texture extracted and viewable in Blender
> 4. A personal notes document on the PS2 pipeline: plain-English explanation of VIF/GIF/GS in your own words

## Resources

**PS2 Hardware Reference**
- [PCSX2 source — GS plugin](https://github.com/PCSX2/pcsx2)
- [ps2dev SDK (PS2 homebrew toolchain)](https://github.com/ps2dev/ps2sdk)
- [ps2tek (community PS2 hardware documentation)](https://psi-rockin.github.io/ps2tek/)
- [ps2dev Docker image (easy toolchain setup)](https://github.com/ps2dev/ps2dev)

**Atlus / Persona 3 Asset Tools**
- [Amicitia](https://github.com/tge-was-taken/Amicitia)
- [AtlusScriptToolchain](https://github.com/tge-was-taken/Atlus-Script-Tools)
- [PersonaEditor](https://github.com/Secre-C/PersonaEditor)
- [GFD-Studio](https://github.com/tge-was-taken/GFD-Studio)
- [Atlus Modding Discord](https://discord.gg/naoto)

**Game Logic Reference**
- [Persona 3 Portable PC port (Steam)](https://store.steampowered.com/app/1557840/) — use Ghidra or x64dbg to read Windows binary for combat formulas, calendar logic, and other shared systems. Cross-reference against PCSX2 for any behavior that may differ between Portable and FES.

## Getting Started

> Start with Track B. Download Amicitia and AtlusScriptCompiler, point them at a Persona 3 FES ISO, and extract things. Open textures, look at models in GFD-Studio, decompile a script file. Do this for a few hours before touching PS2 code. Knowing what the data looks like makes the pipeline research feel concrete.
>
> Then move to Track A. Read ps2tek's GS documentation, then open PCSX2's GSState.cpp and find where GIF packets are parsed. You're pattern-matching the documentation to the code — once a GIF packet is readable and the GS registers it writes are legible, the goal is achieved.

## Career Connection

The PS2's DMA-driven rendering pipeline is conceptually identical to how high-performance embedded systems move data: command lists, ring buffers, and DMA channels appear throughout DSP hardware, FPGAs, and defense sensing equipment. This is DMA-centric system design in a game console context — the pattern is universal.

---

---

# Phase 3 · Project 3 · Port a Small PS2 Homebrew to 3DS
### The Dry Run — Full End-to-End Porting Experience

| Attribute | Value |
|---|---|
| Estimated Time | 2–3 months (part-time weekends/evenings) |
| Difficulty | Advanced |
| Status | 🔲 Not started |

## Overview

Find a small open-source PS2 homebrew game or well-featured tech demo and port it to run on the New 2DS XL. The port doesn't need to be perfect — wireframe rendering, missing audio, and simplified controls are acceptable. The goal is to go through every category of problem present in the real Persona 3 port, at a scale where the codebase is small enough to actually finish. This is the most important project in the ladder.

## Background & Motivation

Everything up to this point has been building vocabulary. This project is the first sentence. The fundamental challenge of porting from PS2 to 3DS is that every hardware-specific assumption in the source code is wrong. PS2 DMA channels don't exist on 3DS. The GS doesn't exist on 3DS. The SPU2 doesn't exist on 3DS.

The solution is a platform abstraction layer (PAL): a set of interfaces describing what the game needs (render this geometry, play this sound, read this input) implemented differently for each target platform. Designing this layer is the core intellectual challenge of porting. Getting it wrong on a small codebase is cheap. Getting it wrong on Persona 3 FES is expensive.

This project is also the first encounter with practical problems unique to 3DS homebrew: stack size limits biting recursive code, the absence of a real debugger, floating-point behavior differences, and global state initialized differently on PS2. Better to discover all of these on a 500-line codebase than a 500,000-line one.

## Learning Objectives

By the end of this project you will be able to:

- Design and implement a platform abstraction layer separating game logic from hardware-specific code
- Replace PS2 GS rendering calls with citro3d equivalents while preserving visual output
- Handle PS2-to-3DS control mapping and input system differences
- Debug undefined behavior, stack overflows, and memory issues in a bare-metal 3DS environment
- Set up a dual-target build system compiling for both PCSX2 and 3DS from the same source
- Document every PS2 API dependency and the 3DS equivalent or workaround used

## Required Deliverables

> **All of the following must be present to consider this project complete:**
> 1. The ported homebrew running on real New 2DS XL hardware — gameplay must be recognizable even if graphics are simplified
> 2. A clean platform abstraction layer in the codebase separating game logic from PS2/3DS specifics
> 3. A dual-target Makefile or CMakeLists.txt building for both PCSX2 and 3DS from the same source
> 4. A porting log: a running record of every PS2-specific problem encountered and how it was solved (personal reference for the Persona 3 port)
> 5. Source on GitHub with a README documenting the port

## Resources

**PS2 Homebrew Sources**
- [ps2homebrew collection](https://github.com/ps2homebrew)
- [ps2sdk samples (well-commented, isolated subsystems)](https://github.com/ps2dev/ps2sdk/tree/master/samples)
  - Target: a project with a game loop, basic 3D rendering, input handling, and under ~2000 lines of game logic

**Platform Abstraction Reference**
- [Quake source (id Software) — exemplary PAL design](https://github.com/id-Software/Quake)
- [libctru documentation](https://libctru.devkitpro.org/)

**Debugging on 3DS**
- `consoleInit()` + printf — primary debugging tool
- 3DS ARM fault dump — learn to read register state on crash
- Test on hardware frequently — Citra is more forgiving than real hardware

## Getting Started

> Spend one evening finding the right source project from the ps2sdk samples or ps2homebrew. Pick something that runs correctly in PCSX2 first.
>
> Your first task is not to make anything run — it's to get the project compiling for 3DS. Change the build system to target 3DS, stub every PS2-specific function with a no-op, and get a binary that links. That binary does nothing, but it produces the complete list of PS2 dependencies that need replacing. That list is your porting checklist.

## Career Connection

Platform abstraction layer design is exactly what firmware targeting multiple hardware boards requires — which is routine in defense electronics. The discipline of cleanly separating what software needs from how specific hardware provides it is a core embedded systems engineering skill.

## Notes & Caveats

The time range is wide because the PS2 homebrew chosen matters significantly. A simple 2D game might take 3 weeks; a 3D game with audio could take 3 months. Timebox aggressively — if stuck on a single subsystem for more than two weeks, stub it and move on. A complete end-to-end port with some stubs beats a half-finished attempt at perfection.

---

---

# Phase 4 · Project 4 · ADX Audio + MVD Video Subsystem
### Complete Media Playback for the Port

| Attribute | Value |
|---|---|
| Estimated Time | 4–6 weeks (part-time) |
| Difficulty | Intermediate |
| Status | 🔲 Not started |

## Overview

Build the complete media playback layer the port requires: streaming ADX audio through the 3DS DSP, and FMV video playback via the New 3DS/2DS hardware H.264 decoder (MVD). This project also serves as the forcing function for learning CIA packaging, since MVD access requires a properly installed title rather than a .3dsx homebrew app. The deliverables are two reusable engine components — an audio module and a video module — plus the build-time pipeline tools that prepare Persona 3's media assets for 3DS consumption.

## Background & Motivation

Persona 3 FES uses CRI Middleware's ADX format for nearly all audio (BGM, voice, SFX) and PlayStation Stream (PSS) container format for its FMV cutscenes — one of the key differences between FES and Portable, which replaced all FMVs with still images because the PSP couldn't handle them.

ADX is a streaming ADPCM format decoded on the fly from disc. The DSP background makes this the most tractable part of the project — ADPCM with CRI's predictor coefficients is simpler than the JANUS modulator stack. The 3DS DSP handles mixing via ndsp, and the main challenge is getting a continuous latency-stable stream without underruns.

MVD (the hardware video decoder on New 3DS/2DS) decodes H.264. PSS files from Persona 3 wrap MPEG-2 video, so a transcoding step is required: a build-time tool extracts video from PSS containers and transcodes to H.264, which the runtime then feeds to MVD. MVD expects complete access units (full frames) rather than arbitrary NAL unit chunks — this is the key constraint that caused issues in the Rokenbok live streaming attempt, but is a non-issue for file playback where you control the entire bitstream and can buffer complete frames before feeding them.

CIA packaging is a prerequisite for MVD access — the decoder requires a properly installed title with the right permissions. This project is the natural place to learn CIA builds since you'll need them for the final port distribution anyway.

## Learning Objectives

By the end of this project you will be able to:

- Implement an ADX frame decoder correctly (ADPCM with CRI predictor coefficients)
- Stream decoded PCM audio to the 3DS DSP via ndsp without underruns or artifacts
- Handle ADX loop metadata correctly for seamless BGM looping
- Write a build-time PSS extractor and H.264 transcoder for Persona 3 FES cutscenes
- Feed H.264 access units (complete frames) to MVD correctly and display decoded frames via citro3d
- Synchronize audio and video streams across a multi-minute cutscene without drift
- Package a .cia with correct permissions for MVD access
- Design clean audio and video APIs usable from game code

## Required Deliverables

> **All of the following must be present to consider this project complete:**
> 1. A standalone .cia ADX player running on real hardware: loads an ADX file from SD, streams correctly with loop support
> 2. A standalone .cia video player running on real hardware: plays a transcoded Persona 3 FES cutscene with synchronized audio
> 3. A build-time PSS extraction and H.264 transcoding tool in the asset pipeline
> 4. A reusable audio module (header + .c) exposing a clean API: init, play, stop, set volume, loop control
> 5. A reusable video module (header + .c) exposing a clean API: open, play, pause, stop, frame callback
> 6. Correct playback of at least two Persona 3 FES BGM tracks, one voice line, and one FMV cutscene
> 7. A technical write-up covering: ADX format and decoder implementation, MVD access unit requirements and why streaming differs from file playback, CIA packaging for MVD access

## Resources

**ADX Format & Audio**
- [VGMStream ADX decoder (reference implementation)](https://github.com/vgmstream/vgmstream/blob/master/src/coding/adx_decoder.c)
- [VGMStream full source](https://github.com/vgmstream/vgmstream)
- [ndsp documentation (libctru)](https://libctru.devkitpro.org/ndsp_8h.html)
- [3ds-examples audio sample](https://github.com/devkitPro/3ds-examples/tree/master/audio)
- [vgmstream-cli (PSS/ADX extraction)](https://github.com/vgmstream/vgmstream)

**MVD Video**
- [3DBrew MVD documentation](https://www.3dbrew.org/wiki/MVD_Services)
- [libctru MVD API](https://libctru.devkitpro.org/mvd_8h.html)
  - MVD requires complete H.264 access units — buffer all NAL units belonging to one frame before calling `mvdstdConvertImage`
  - Use FFmpeg for PSS → H.264 transcoding in the build-time pipeline
  - Double-buffer between decode and display to prevent frame tearing

**CIA Packaging**
- [makerom (CIA builder)](https://github.com/jakcron/Project_CTR)
- [3DS CIA building guide (3DBrew)](https://www.3dbrew.org/wiki/CIA)
  - MVD access requires the `mvd` service permission in the CIA's NCCH access control — this is the specific permission that .3dsx cannot grant

**PC Validation**
- [Python sounddevice (for validating ADX decoder on PC before 3DS)](https://python-sounddevice.readthedocs.io/)

## Getting Started

> Start with the ADX decoder on PC. Read VGMStream's adx_decoder.c in full, then write a Python script that decodes an ADX file to raw PCM and plays it via sounddevice. Validate the decoder is correct on PC with good tooling before touching 3DS. Once it's correct, porting to 3DS is just plumbing the PCM output into ndsp.
>
> For MVD, start by getting any H.264 file playing before worrying about Persona 3 assets. The 3DBrew MVD page and libctru headers are the primary references. The key insight from the Rokenbok experience: MVD requires complete access units. Parse the H.264 bytestream to find access unit boundaries (access unit delimiter NAL units, or new primary coded picture detection), buffer everything belonging to one frame, then feed the complete frame to MVD. File playback gives you full control over the bitstream so this is straightforward.
>
> Set up CIA packaging early in this project — don't leave it until the end. Get a minimal CIA that does nothing except initialize MVD successfully as your first CIA milestone, then build the video player on top of that foundation.

## Career Connection

ADPCM, streaming media pipelines, and hardware decoder integration are directly applicable to communications and sensor systems work. The pattern of validating on PC first then porting to embedded target is how signal processing algorithms should always be developed — build the math correctly in a high-observability environment before committing to bare metal.

## Notes & Caveats

The MVD access unit requirement is the single most important technical constraint in this project. It's the reason live streaming H.264 to MVD is difficult (you may not have a complete frame buffered when you need to decode) while file playback is straightforward (you have the entire bitstream and can find frame boundaries before feeding anything). The write-up should explain this distinction clearly — it's useful knowledge for anyone else attempting MVD integration.

---

---

# Phase 5 · Project 5 · Persona 3 Model & Map Viewer on 3DS
### Real Atlus Assets on Real Nintendo Hardware

| Attribute | Value |
|---|---|
| Estimated Time | 3–6 weeks (part-time) |
| Difficulty | Advanced |
| Status | 🔲 Not started |

## Overview

Build a 3DS homebrew application that loads and renders actual Persona 3 FES assets — a character model or a Tartarus room — on the New 2DS XL using citro3d. This is the first moment the port stops being theoretical. No gameplay or animation required — the deliverable is static geometry with correct textures rendering correctly on hardware, with a simple orbit camera for inspection.

## Background & Motivation

This project connects citro3d knowledge from Project 1 and Atlus format knowledge from Project 2 into something that looks unmistakably like Persona 3 running on a 3DS. Achieving this has real psychological value — it makes the port feel possible in a way no amount of planning does.

The practical challenges are format-specific. Atlus's GMD/RMD model format stores geometry, bone hierarchies, and material data in a proprietary layout. You'll write a loader that converts this into citro3d vertex buffers. Textures need conversion to Morton-tiled PICA200 format. And real constraints will surface immediately: some Persona 3 environments have polygon counts or texture memory requirements that will need LOD or streaming solutions in the final port. Discovering these constraints now, on a simple viewer, is much better than discovering them mid-port.

The model viewer is also a diagnostic tool that will be used throughout the rest of the project. Any time there's doubt about whether a model loaded correctly, the viewer provides a fast visual check.

## Learning Objectives

By the end of this project you will be able to:

- Parse Atlus GMD/RMD model format and extract vertex data, UV coordinates, and material references
- Convert Atlus texture formats to PICA200-compatible Morton-tiled layout
- Render at least one Persona 3 FES character model and one environment mesh on 3DS hardware
- Implement a simple orbit camera controlled by the circle pad
- Profile memory usage and draw call count against 3DS hardware limits
- Document any geometry or texture constraints discovered

## Required Deliverables

> **All of the following must be present to consider this project complete:**
> 1. A .3dsx model viewer running on real hardware loading Persona 3 FES models from SD card
> 2. Correct textures applied (not untextured geometry)
> 3. Orbit camera controlled by circle pad
> 4. Technical notes on the GMD/RMD format: structure, any undocumented fields, and how they were handled
> 5. Performance measurements: polygon count of the most complex scene rendered and its frame time

## Resources

**Atlus Model Format**
- [GFD-Studio source (model format reference)](https://github.com/tge-was-taken/GFD-Studio)
- [Amicitia](https://github.com/tge-was-taken/Amicitia)
- [Blender import plugin for Atlus formats](https://github.com/SutandoTsukai181/gfd-blender-plugin)
  - Use GFD-Studio's model loading code as the format reference — open source C# with clear structure

**citro3d & Texture Conversion**
- [citro3d source and examples](https://github.com/devkitPro/citro3d)
- [PICA200 texture format documentation](https://www.3dbrew.org/wiki/GPU/Texture_Formats)
- [tex3ds](https://github.com/devkitPro/tex3ds)

## Getting Started

> Extract a character model using GFD-Studio and import it into Blender first. Verify it looks correct there before writing any 3DS code. Then open GFD-Studio's source and find the model loading code — read how it parses vertex buffers and triangle lists. The 3DS loader is a C translation of that logic, outputting citro3d VBOs instead of whatever GFD-Studio outputs.
>
> Start with a character model, not an environment. Characters are simpler: one or a few meshes, manageable polygon count, well-understood texture layout. Once a character renders correctly, environments are the same problem at larger scale.

## Career Connection

Writing a binary format parser from a reference implementation to a target implementation is a fundamental skill in defense systems work — proprietary data formats from radar returns, sensor buses, and legacy communication protocols appear constantly. The methodology is identical: read the reference, understand the structure, reimplement for your target.

---

---

# Phase 6 · Project 6 · Persona 3 Combat Prototype on 3DS
### The Mechanical Core, Running on Hardware

| Attribute | Value |
|---|---|
| Estimated Time | 2–4 months (part-time) |
| Difficulty | Advanced |
| Status | 🔲 Not started |

## Overview

Build a functional turn-based combat demo using real Persona 3 assets and mechanics running on the New 2DS XL. One party member, a handful of Shadow enemies, working menus, real damage formulas, Persona skills, and status effects. No story, no dungeon, no calendar — just the battle loop end to end. This proves the hardest gameplay system works and establishes the architecture the rest of the port runs on.

## Background & Motivation

Combat is the mechanical core of Persona 3. From an engineering standpoint it's also the most complex and interdependent subsystem: party state, enemy AI, the Persona/skill system, affinities and resistances, status effects, the One More system, and turn-order logic all interact simultaneously.

This project requires three things in parallel: reverse engineering game data (damage formulas, skill parameters, enemy stats — using the PC port as a readable reference for shared systems), implementing game logic in C, and building enough UI to make it usable. The UI work is significant — Persona 3's battle menus are multi-layer, state-driven, and feed into the optional bottom screen UI stretch goal later.

The interpreter experience from Initiate Protocol is directly applicable here. A turn-based combat loop is a state machine: Wait → Choose Action → Execute Action → Check Conditions → Next Turn. The conditional branching and jump logic already built maps cleanly onto this architecture.

## Learning Objectives

By the end of this project you will be able to:

- Implement Persona 3's core damage formula and affinity/resistance system correctly
- Build a turn-order system supporting the One More mechanic
- Implement at least 10 Persona skills (physical, magical, healing, status) with correct behavior
- Build a state-machine-driven battle UI: command menu, target selection, animation states
- Implement basic enemy AI with skill usage based on affinity targeting
- Integrate real Persona 3 FES assets: character sprites, enemy models, UI elements, battle BGM via the audio module from Project 4
- Achieve stable 30fps minimum during combat on real hardware

## Required Deliverables

> **All of the following must be present to consider this project complete:**
> 1. A .3dsx combat demo on real hardware: enter a battle, select commands, win or lose, return to a title screen
> 2. Correct damage formula implementation (verifiable against known values from the original game)
> 3. Working One More system
> 4. At least one skill of each category: physical, magical, healing, status effect
> 5. Real Persona 3 FES assets used throughout
> 6. A game state architecture document: diagram or written description of the battle state machine, structured for reuse in the full port

## Resources

**Persona 3 Mechanics**
- [Persona 3 mechanics wiki](https://megamitensei.fandom.com/wiki/Persona_3)
- [Persona 3 FES data tables (GameFAQs)](https://gamefaqs.gamespot.com/ps2/937269-shin-megami-tensei-persona-3-fes)
  - Atlus Modding Discord has documented damage formulas in detail
  - Use Persona 3 Portable PC port + Ghidra for readable disassembly of shared combat systems

**Architecture Reference**
- [Game Programming Patterns — State and Command chapters (free online)](https://gameprogrammingpatterns.com/)
  - Read the State and Command chapters before architecting the battle system

## Getting Started

> Do not start with the UI. Start with a headless combat simulator: a C program running in the terminal, taking text input, outputting battle results as text. Get damage formulas right, turn order right, One More right. Validate against known values from the original game. Only once the logic is correct do you add rendering.
>
> This separation between game logic and presentation is one you'll maintain throughout the full port. The battle logic should have zero citro3d calls in it — it operates entirely on data structures that a renderer reads and displays.

## Career Connection

State machine design, data-driven architecture, and separating logic from presentation are foundational software engineering skills. The specific pattern of headless simulation first, visualization second is how signal processing algorithms should be prototyped — validate the math on test data before integrating into a real-time system.

## Notes & Caveats

This is the project most likely to expand in scope. Timebox to the core mechanic — correct basic combat loop is worth more than a half-finished attempt at every edge case. Leave Armageddon, shuffle time, and Tartarus trap tiles for the full port.

---

---

# Capstone: Persona 3D
### Full Port of Persona 3 FES to the New Nintendo 2DS XL

## Porting Philosophy

This is not a decompilation project. The goal is not to recover Persona 3's original source code — it's to write a new engine that targets 3DS hardware and implement all of Persona 3's systems inside it so the output is faithful to the original. The PS2 binary is the specification. PCSX2 is the test oracle. The Atlus community tooling provides the data. The port provides the engine.

The Atlus format tools handle the data extraction problem — scripts, models, textures, animations, and audio are extracted from the ISO in usable form. The engine handles the playback problem — it takes those same assets as input and produces the correct output on 3DS hardware through citro3d, ndsp, and the MVD rather than the PS2's GS, SPU2, and disc streaming hardware.

## Subsystem Breakdown

| Subsystem | Description & Project Dependencies |
|---|---|
| Asset Pipeline | Extract, convert, and package all game data for 3DS. Built on Project 2 tooling. |
| Rendering Engine | Characters, environments, UI, and effects via citro3d/PICA200. Built on Projects 1, 5. |
| Shader Library | Standard opaque, cel/toon (characters), unlit (UI), transparent, metallic, particle/additive. Each is a separate PICA200 shader tuned for its surface type. |
| Script / Event VM | Reimplementation of Atlus's BMD bytecode interpreter. Leverages existing interpreter experience; refined in Project 2. |
| Audio Engine | Streaming ADX/BGM, SFX, and voice through the 3DS DSP. Built on Project 4. |
| Video Playback | FMV cutscenes via MVD hardware decoder with PSS→H.264 transcoding pipeline. Built on Project 4. |
| Field / Dungeon Engine | Overworld navigation, Tartarus crawling, collision, and camera. Architecture from Project 3. |
| Combat System | Turn logic, Persona/skill system, enemy AI, status effects, One More. Built on Project 6. |
| Calendar & Social System | Date tracking, Social Links, stat management, flag state. Architecture from Project 6. |
| Save System | 3DS-native SD saves via FSUSER. Built on libctru familiarity from Project 1. |
| Input & UI Layer | PS2 button remapping and 3DS-native menus. Top screen for gameplay, standard overlay UI by default. |

## Integration Phases

| Phase | Goal |
|---|---|
| Integration 1 | Asset pipeline producing valid 3DS-ready data for all game content |
| Integration 2 | A single Gekkoukan High room renders with correct textures and collision |
| Integration 3 | Player can walk through a scene and trigger a scripted event/dialog |
| Integration 4 | Full Tartarus floor navigable with random encounters triggering correctly |
| Integration 5 | Full combat loop: enter encounter → fight → return to dungeon |
| Integration 6 | Calendar, Social Links, and day/night cycle running correctly |
| Integration 7 | Full game playable from title screen to credits (rough, bugs expected) |
| Integration 8 | Audio complete, video cutscenes playing via MVD, saves working, performance acceptable |
| Release | Polished, distributable patch with build instructions. No assets or ROM included. |

## Stretch Goals (Post-Stable-Port)

These are explicitly post-completion additions — none of them are pursued until the base port is fully playable.

> **Stereoscopic 3D**
>
> citro3d supports stereoscopic rendering natively via `Mtx_PerspStereoTilt`. The core change is rendering the scene twice with the camera offset laterally by ±IOD/2, reading the 3D slider via `osGet3DSliderState()` to scale the effect and gracefully fall back to flat rendering at zero. Performance cost is roughly halving the rendering budget — manageable for PS2-era geometry. To be added once real New 3DS hardware is available for testing.

> **Bottom Screen UI**
>
> An optional setting (off by default) that moves battle menus to the lower touchscreen with full touch support via `hidTouchRead()`. Requires citro2d for text rendering, a hit-testing layer for touch input, and clean separation between battle state and UI rendering — which the Project 6 architecture provides. Default behavior remains the original overlay layout so muscle memory is preserved.

> **Direct Party Control**
>
> An optional setting replicating Persona 3 Portable's direct party control. The battle state machine already handles the protagonist's turn — extending it to iterate over all party members before resolving a round is a relatively contained addition. The AI tactics system remains available as an alternative, matching P3P's implementation. Estimated at 1–2 weeks of work on top of a complete battle system.

## Distribution Model

To avoid legal issues, the port is distributed as a build system and patch tool only — never as pre-built binaries or extracted assets. Users provide a legally dumped copy of Persona 3 FES. The build tools extract assets from that dump, convert them to 3DS-ready formats, and assemble the final .cia on the user's own machine. No Atlus content is ever hosted or distributed directly.

> **Distributed publicly:**
> - Engine source code (rendering pipeline, audio system, script VM, etc.)
> - Asset conversion tools (PSS extractor, model converter, texture pipeline, etc.)
> - Build script that automates extraction → conversion → assembly
> - Documentation and build instructions
>
> **Never distributed:**
> - Extracted assets of any kind
> - The ROM or any portion of it
> - Pre-built .3dsx or .cia files

## Community Resources

- [Amicitia (Atlus format tools)](https://github.com/tge-was-taken/Amicitia)
- [AtlusScriptToolchain (BMD script format)](https://github.com/tge-was-taken/Atlus-Script-Tools)
- [GFD-Studio (model viewer/editor)](https://github.com/tge-was-taken/GFD-Studio)
- [PersonaEditor](https://github.com/Secre-C/PersonaEditor)
- [PCSX2 (PS2 emulator — hardware reference)](https://github.com/PCSX2/pcsx2)
- [Citra (3DS emulator — hardware reference)](https://github.com/citra-emu/citra)
- [Atlus Modding Discord](https://discord.gg/naoto)
- [Persona 3 Portable PC port (Steam — game logic reference)](https://store.steampowered.com/app/1557840/)

---

---

# Appendix: Quick Reference

## 3DS Development
- [devkitPro](https://devkitpro.org)
- [libctru documentation](https://libctru.devkitpro.org)
- [citro3d](https://github.com/devkitPro/citro3d)
- [3ds-examples](https://github.com/devkitPro/3ds-examples)
- [picasso (PICA200 shader assembler)](https://github.com/devkitPro/picasso)
- [tex3ds](https://github.com/devkitPro/tex3ds)
- [makerom (CIA builder)](https://github.com/jakcron/Project_CTR)
- [3DBrew wiki](https://www.3dbrew.org/wiki/Main_Page)
- [3DBrew MVD documentation](https://www.3dbrew.org/wiki/MVD_Services)

## PS2 Development & Emulation
- [ps2dev SDK](https://github.com/ps2dev/ps2sdk)
- [PCSX2](https://github.com/PCSX2/pcsx2)
- [ps2tek](https://psi-rockin.github.io/ps2tek/)
- [ps2homebrew collection](https://github.com/ps2homebrew)

## Atlus / Persona 3 Tooling
- [Amicitia](https://github.com/tge-was-taken/Amicitia)
- [AtlusScriptToolchain](https://github.com/tge-was-taken/AtlusScriptToolchain)
- [GFD-Studio](https://github.com/tge-was-taken/GFD-Studio)
- [PersonaEditor](https://github.com/Secre-C/PersonaEditor)
- [VGMStream (ADX audio)](https://github.com/vgmstream/vgmstream)
- [Atlus Modding Discord](https://discord.gg/naoto)

## Game Logic Reference
- [Persona 3 Portable PC port (Steam)](https://store.steampowered.com/app/1557840/)

## General References
- [Game Programming Patterns (free)](https://gameprogrammingpatterns.com/)
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)
- [Quake source (PAL design reference)](https://github.com/id-Software/Quake)
- [Python sounddevice](https://python-sounddevice.readthedocs.io/)

---

*Good luck. The name demands it exist.*
