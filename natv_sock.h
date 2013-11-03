/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the GNU v3 license, see the LICENSE file.
 */

#ifndef __NATV_SOCK_H__
#define __NATV_SOCK_H__

struct swap_id;

int natv_sock_send(struct swap_id *sid, size_t size, void *data);
int natv_sock_send_iov(struct swap_id *sid, int num, struct iov *ios);

int natv_sock_module_init();
int natv_sock_module_fini();

#endif // __NATV_SOCK_H__

