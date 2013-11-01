/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#include "emulator.h"

#include "swapi.h"


int emulator_init(){	
	return swapi_init();
}

int emulator_fini(){
	return swapi_fini();
}

