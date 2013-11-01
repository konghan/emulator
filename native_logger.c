/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#include "native_logger.h"

#include "emulator.h"

int native_logger_output(const char *log, int size){
	return logger_output(log, size);
}

int native_logger_init(){
	return 0;
}

int native_logger_fini(){
	return 0;
}


