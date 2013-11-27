/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#include "natv_surface.h"

#include "emu-shell.h"

#include "swapi_surface.h"
#include "swapi_sys_thread.h"

typedef struct natv_surface{
	swapi_spinlock_t	ng_lock;
	int					ng_init;
	
	cairo_surface_t		*ng_surface;
	cairo_t				*ng_context;
	int					ng_width;
	int					ng_height;
}natv_surface_t;

static natv_surface_t __gs_natv_surface = {};

static inline natv_surface_t *get_ngctx(){
	return &__gs_natv_surface;
}

int natv_surface_getinfo(natv_surface_info_t *info){
	emu_shell_t		*es;

	ASSERT(info != NULL);

	es = emu_shell_getinfo(0);

	info->nsi_width		= es->es_screen_width;
	info->nsi_height	= es->es_screen_height;

	// CAIRO_FORMAT_ARGB32, CAIRO_FORMAT_A8, CAIRO_FORMAT_RGB16_565
	// CAIRO_FORMAT_RGB30
	info->nsi_type = CAIRO_FORMAT_RGB24;

	return 0;
}

static void natv_surface_drawlogo(cairo_t *cr, cairo_surface_t *surface){
	cairo_set_source_rgba(cr, 0.2, 0.2, 0.2, 1);
	cairo_rectangle(cr, 0, 0, 128, 128);
	cairo_fill(cr);

	cairo_set_source_rgba(cr, 0, 0, 0, 0.4);
	cairo_rectangle(cr, 0, 0, 32, 32);
	cairo_fill(cr);

	cairo_set_source_rgba(cr, 1, 0, 0, 0.4);
	cairo_rectangle(cr, 32, 32, 32, 32);
	cairo_fill(cr);

	cairo_set_source_rgba(cr, 0, 1, 0, 0.4);
	cairo_rectangle(cr, 64, 64, 32, 32);
	cairo_fill(cr);

	cairo_set_source_rgba(cr, 0, 0, 1, 0.4);
	cairo_rectangle(cr, 96, 96, 32, 32);
	cairo_fill(cr);
}


int natv_surface_module_init(){
	natv_surface_t		*ng = get_ngctx();
	natv_surface_info_t	info;

	natv_surface_getinfo(&info);
	
	ng->ng_surface = cairo_image_surface_create(info.nsi_type,
			info.nsi_width, info.nsi_height);

	if(ng->ng_surface == NULL){
		return -1;
	}

	ng->ng_context = cairo_create(ng->ng_surface);
	if(ng->ng_context == NULL){
		cairo_surface_destroy(ng->ng_surface);
		return -1;
	}

	natv_surface_drawlogo(ng->ng_context, ng->ng_surface);

	swapi_spin_init(&ng->ng_lock);
	ng->ng_init = 1;

	return 0;
}

int natv_surface_module_fini(){
	natv_surface_t *ng = get_ngctx();

	if(ng->ng_init == 0){
		return -1;
	}

	ng->ng_init = 0;
	swapi_spin_fini(&ng->ng_lock);

	cairo_destroy(ng->ng_context);
	cairo_surface_destroy(ng->ng_surface);

	return 0;
}

int natv_surface_draw(swapi_surface_t *sf, int x, int y, int width, int height){
	natv_surface_t *ng = get_ngctx();

	swapi_spin_lock(&ng->ng_lock);

	cairo_set_source_surface(ng->ng_context, sf->ss_sf, x, y);
	cairo_paint(ng->ng_context);
	
	swapi_spin_unlock(&ng->ng_lock);
	
	natv_surface_renter_to_device();

	return 0;
}

cairo_surface_t *_natv_surface_get(){
	natv_surface_t *ng = get_ngctx();
	return ng->ng_surface;
}
