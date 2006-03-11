/*
 * DJGPP Timer Library
 * Copyright (c) 2006 Simon Peter <dn.tlp@gmx.net>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef GCCTIMER_H
#define GCCTIMER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*TimerHandler)(void);

int timer_init(TimerHandler handler);
void timer_deinit(void);
void timer_setrate(unsigned short rate);

#ifdef __cplusplus
}
#endif

#endif
