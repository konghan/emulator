/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#include "natv_logger.h"

#include "swapi_queue.h"
#include "swapi_sys_thread.h"
#include "swapi_message.h"

#include "emulator.h"

#define knatv_logger_LOGSIZE		128
#define knatv_logger_PENDINGS		32

typedef struct natv_logger{
	int					nl_init;
	FILE				*nl_fd;

	swapi_queue_t		*nl_queue;
	swapi_thread_t		nl_thread;
}natv_logger_t;

static natv_logger_t __gs_log = {};

static inline natv_logger_t *get_log(){
	return &__gs_log;
}

int natv_logger_output(const char *log, int size){
	natv_logger_t		*nl =  get_log();
	swapi_message_t		msg;

	if(nl->nl_init == 0){
		return -1;
	}

	msg.sm_ptr = malloc(knatv_logger_LOGSIZE);
	if(msg.sm_ptr == NULL){
		return -1;
	}
	memset(msg.sm_ptr, 0, knatv_logger_LOGSIZE);

	if(size >= (knatv_logger_LOGSIZE-1))
		size = knatv_logger_LOGSIZE - 1;

	strncpy((char *)msg.sm_ptr, log, size);
	msg.sm_size = size;

	return swapi_queue_post(nl->nl_queue, &msg);
}

static int logger_routine(void *p){
	natv_logger_t		*nl = (natv_logger_t *)p;
	swapi_message_t		msg;
	
	while(1){
		swapi_queue_wait(nl->nl_queue, &msg);

		logger_output((char*)msg.sm_ptr, msg.sm_size);
		if(nl->nl_fd != NULL){
			fwrite((void*)msg.sm_ptr, msg.sm_size, 1, nl->nl_fd);
			fflush(nl->nl_fd);

			printf("%s\n", (char*)msg.sm_ptr);
		}

		free(msg.sm_ptr);
	}

	return 0;
}

int natv_logger_init(){
	natv_logger_t *nl = get_log();

	if(swapi_queue_create(sizeof(swapi_message_t), knatv_logger_PENDINGS,
				&nl->nl_queue) != 0){
		return -1;
	}

	if(swapi_thread_create(&nl->nl_thread, logger_routine, nl) != 0){
		return -1;
	}

	nl->nl_fd = fopen("natv.log", "w");
	
	nl->nl_init = 1;

	return 0;
}

int natv_logger_fini(){

	// FIXME: destroy logger

	return 0;
}


