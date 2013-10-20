/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#include "native_graphic.h"

#include "emu-shell.h"

#include "swapi_sys_thread.h"

typedef struct native_graphic{
	swapi_spinlock_t	ng_lock;
	int					ng_init;
	
	cairo_surface_t		*ng_surface;
	cairo_t				*ng_context;
	int					ng_width;
	int					ng_height;
}native_graphic_t;

static native_graphic_t __gs_native_graphic = {};

static inline native_graphic_t *get_ngctx(){
	return &__gs_native_graphic;
}

int native_graphic_getinfo(native_graphic_info_t *info){
	emu_shell_t		*es;

	ASSERT(info != NULL);

	es = emu_shell_getinfo(0);

	info->ngi_width		= es->es_screen_width;
	info->ngi_height	= es->es_screen_height;

	// CAIRO_FORMAT_ARGB32, CAIRO_FORMAT_A8, CAIRO_FORMAT_RGB16_565
	// CAIRO_FORMAT_RGB30
	info->ngi_rgbtype = CAIRO_FORMAT_RGB24;

	return 0;
}

static void native_graphic_drawlogo(cairo_t *cr, cairo_surface_t *surface){

	cairo_set_source_rgb(cr, 128, 128, 128);
	cairo_rectangle(cr, 0, 0, 128, 128);
	cairo_fill(cr);
}

int native_graphic_init(){
	native_graphic_t		*ng = get_ngctx();
	native_graphic_info_t	info;

	native_graphic_getinfo(&info);
	
	ng->ng_surface = cairo_image_surface_create(info.ngi_rgbtype,
			info.ngi_width, info.ngi_height);

	if(ng->ng_surface == NULL){
		return -1;
	}

	ng->ng_context = cairo_create(ng->ng_surface);
	if(ng->ng_context == NULL){
		cairo_surface_destroy(ng->ng_surface);
		return -1;
	}

	native_graphic_drawlogo(ng->ng_context, ng->ng_surface);

	swapi_spin_init(&ng->ng_lock);
	ng->ng_init = 1;

	return 0;
}

int native_graphic_fini(){
	native_graphic_t *ng = get_ngctx();

	if(ng->ng_init == 0){
		return -1;
	}

	ng->ng_init = 0;
	swapi_spin_fini(&ng->ng_lock);

	cairo_destroy(ng->ng_context);
	cairo_surface_destroy(ng->ng_surface);

	return 0;
}

cairo_surface_t *native_graphic_get(){
	native_graphic_t *ng = get_ngctx();

	return ng->ng_surface;
}

extern int native_flush_device(int x, int y, int width, int height);

int native_graphic_draw(cairo_surface_t *surface, int x, int y, int width, int height){
	native_graphic_t *ng = get_ngctx();

	swapi_spin_lock(&ng->ng_lock);

	cairo_set_source_surface(ng->ng_context, surface, x, y);
	cairo_paint(ng->ng_context);
	
	swapi_spin_unlock(&ng->ng_lock);

	native_flush_device(x, y, width, height);

	return 0;
}

