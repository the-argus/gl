#pragma once

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "opengl demo"

#define VERTEX_PASSTHROUGH "shaders/passthrough.vs"
#define FRAG_YELLOW "shaders/yellow.fs"
#define FRAG_TIMER "shaders/time.fs"
#define FRAG_COL_POS "shaders/poscolor.fs"
#define VERTEX_COL_POS "shaders/poscolor.vs"
#define VERTEX_OFFSET "shaders/offset.vs"
#define VERTEX_TEX_OFFSET "shaders/texture-offset.vs"
#define FRAG_TEX "shaders/texture.fs"
#define FRAG_FULL "shaders/full.fs"
#define VERTEX_FULL "shaders/full.vs"
#define FRAG_TIME_TEXTURED "shaders/time-textured.fs"

// 3d shaders!!! (they use da matrices for transform)
#define VERTEX_FULL_3D "shaders/full-3d.vs"
#define VERTEX_TEXTURE_3D "shaders/nocolor-3d.vs"
