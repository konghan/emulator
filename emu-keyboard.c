/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the GNU v3 license, see the LICENSE file.
 */
#include "emu-keyboard.h"

#include "swapi_message.h"
#include "swapi_loop.h"

int emu_keyboard_post(int keyid){
	swapi_message_t		msg;

	msg.sm_type = kSWAPI_MSGTYPE_KEYBOARD;
	msg.sm_size = 0;
	msg.sm_data = keyid;

	return swapi_loop_post(&msg);
}

