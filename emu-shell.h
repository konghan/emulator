/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#ifndef __EMU_SHELL_H__
#define __EMU_SHELL_H__

#include <cairo.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct emu_shell{
	int		es_id;

	int		es_width;
	int		es_height;

	// screen for watch screen output.
	int		es_screen_x;
	int		es_screen_y;
	int		es_screen_width;
	int		es_screen_height;

	char	es_file[1024];
}emu_shell_t;

emu_shell_t *emu_shell_getinfo(int id);

void emu_shell_draw(cairo_t *cr, cairo_surface_t *surface, int width, int height);

#ifdef __cplusplus
}
#endif

#endif //__EMU_SHELL_H__
