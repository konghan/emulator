/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#include "emu-shell.h"

static emu_shell_t __gs_shell = {
	.es_id				= 0,
	.es_width			= 260,
	.es_height			= 450,
	
	.es_screen_x		= 56,
	.es_screen_y		= 160,
	.es_screen_width	= 128,
	.es_screen_height	= 128,
	.es_file			= "../res/smatch-shell.png",
};

emu_shell_t *emu_shell_getinfo(int id){
	return &__gs_shell;
}

#define M_PI  3.1415926

void emu_shell_draw(cairo_t *cr, cairo_surface_t *surface, int w, int h){

	emu_shell_t *es = emu_shell_getinfo(0);

	double x	= es->es_screen_x - 5,
		   y    = es->es_screen_y - 5,
		   width   = 137.0,
		   height        = 137.0,
		   aspect        = 1.0,     /* aspect ratio */
		   corner_radius = 3.0;   /* and corner curvature radius */

	double radius = corner_radius / aspect;
	double degrees = M_PI / 180.0;

	// draw backgroud
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_rectangle(cr, 0, 0, w, h);
	cairo_fill(cr);
	
	// draw shell
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width (cr, 10.0);

	cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
	cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);

	cairo_close_path (cr);
	cairo_fill_preserve (cr);
	cairo_stroke (cr);
}

