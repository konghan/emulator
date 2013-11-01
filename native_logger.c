/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#include "native_logger.h"

#include "swapi_queue.h"
#include "swapi_sys_thread.h"
#include "swapi_message.h"

#include "emulator.h"

#define kNATIVE_LOGGER_LOGSIZE		128
#define kNATIVE_LOGGER_PENDINGS		32

typedef struct native_logger{
	int					nl_init;

	swapi_queue_t		*nl_queue;
	swapi_thread_t		nl_thread;
}native_logger_t;

static native_logger_t __gs_log = {};

static inline native_logger_t *get_log(){
	return &__gs_log;
}

int native_logger_output(const char *log, int size){
	native_logger_t		*nl =  get_log();
	swapi_message_t		msg;

	if(nl->nl_init == 0){
		return -1;
	}

	msg.sm_ptr = malloc(kNATIVE_LOGGER_LOGSIZE);
	if(msg.sm_ptr == NULL){
		return -1;
	}
	memset(msg.sm_ptr, 0, kNATIVE_LOGGER_LOGSIZE);

	if(size >= (kNATIVE_LOGGER_LOGSIZE-1))
		size = kNATIVE_LOGGER_LOGSIZE - 1;

	strncpy((char *)msg.sm_ptr, log, size);
	msg.sm_size = size;

	return swapi_queue_post(nl->nl_queue, &msg);
}

static int logger_routine(void *p){
	native_logger_t		*nl = (native_logger_t *)p;
	swapi_message_t		msg;
	
	while(1){
		swapi_queue_wait(nl->nl_queue, &msg);

		logger_output((char*)msg.sm_ptr, msg.sm_size);

		free(msg.sm_ptr);
	}

	return 0;
}

int native_logger_init(){
	native_logger_t *nl = get_log();

	if(swapi_queue_create(sizeof(swapi_message_t), kNATIVE_LOGGER_PENDINGS,
				&nl->nl_queue) != 0){
		return -1;
	}

	if(swapi_thread_create(&nl->nl_thread, logger_routine, nl) != 0){
		return -1;
	}
	
	nl->nl_init = 1;

	return 0;
}

int native_logger_fini(){

	// FIXME: destroy logger

	return 0;
}


