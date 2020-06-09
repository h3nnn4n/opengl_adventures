#ifndef _CLICKCOLOR_RENDERING_H
#define _CLICKCOLOR_RENDERING_H

extern unsigned int color_id_framebuffer;
extern unsigned int color_id_texColorBuffer;

extern unsigned int normals_framebuffer;
extern unsigned int normals_texColorBuffer;

void clickcolor_render_pass();
void build_clickcolor_framebuffer();

void build_clickcolor_color_id_framebuffer();
void build_clickcolor_normals_framebuffer();

#endif
