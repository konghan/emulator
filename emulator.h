/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#include <cairo.h>

#ifdef __cplusplus
extern "C" {
#endif

int emulator_init();
int emualtor_fini();

int logger_output(const char *log, int size);

#ifdef __cplusplus
}
#endif

#endif //__EMULATOR_H__
