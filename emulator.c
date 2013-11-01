/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#include "emulator.h"

#include "native_logger.h"

#include "swapi.h"


int emulator_init(){
	native_logger_init();

	return swapi_init();
}

int emulator_fini(){
	native_logger_fini();

	return swapi_fini();
}

