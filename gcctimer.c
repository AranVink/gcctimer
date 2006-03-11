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

#include <pc.h>
#include <dpmi.h>
#include <go32.h>

#include "gcctimer.h"

/* INT 8 is the timer interrupt vector */
#define TIMER_INT	8

/* CLD clears the direction flag for string operations */
#define cld()		__asm__ ("cld");

/* Save the FPU state on the stack */
#define savefpu()	__asm__ ("sub %esp, 200\n\t" \
				 "fsave (%esp)");

/* Reset the FPU */
#define clearfpu()	__asm__ ("finit");

/* Restore the FPU state from the stack */
#define restorefpu()	__asm__ ("frstor (%esp)\n\t" \
				 "add %esp, 200");

/* Stores the addresses of the old and new interrupt handlers */
static _go32_dpmi_seginfo	oldhandler, myhandler;
/* Stores the "real" interrupt handler to execute */
static TimerHandler		real_handler;
/* Stores the current frequency of the timer, as sent to the hardware */
static unsigned short		timer_rate;

static void timer_handler(void)
{
  static unsigned long	intcount = 0;
  static int		overload = 0;

  /* Save & reset FPU state */
/*   savefpu(); */
/*   clearfpu(); */

  /* Not sure what this is for */
/*   loades(); */

  /* Call the old handler at the original rate of 18.2Hz */
  intcount += timer_rate;
  if(intcount & 0xffff0000) {
    intcount &= 0xffff;
    // TODO: call the old handler here
    /* Clear the direction flag -- it's good to be safe */
    cld();
  }

  /* Hardware interrupt served -- bottom end coming up */
  outportb(0x20, 0x20);

  /* Critical section: Call real interrupt handler */
  if(overload == 0) {
    overload++;
/*     if(!indosCheck()) */
/*       stackcall(real_handler); */
    real_handler();
    overload--;
  }

/*   restorefpu(); */
}

int timer_init(TimerHandler handler)
{
  /* Save old handler */
  _go32_dpmi_get_protected_mode_interrupt_vector(TIMER_INT, &oldhandler);

  /* Setup our handler */
  real_handler = handler;
  myhandler.pm_selector = _go32_my_cs();
  myhandler.pm_offset = (unsigned long)timer_handler;

  /* Allocate an IRET wrapper */
  if(_go32_dpmi_allocate_iret_wrapper(&myhandler))
    return 0;

  /* Set our handler to be the new interrupt handler */
  return _go32_dpmi_set_protected_mode_interrupt_vector(TIMER_INT, &myhandler);
}

void timer_deinit(void)
{
  /* Reset timer frequency to 18.2Hz */
  timer_setrate(0);

  /* Reset handler to old one */
  _go32_dpmi_set_protected_mode_interrupt_vector(TIMER_INT, &oldhandler);

  /* Free the IRET wrapper */
  _go32_dpmi_free_iret_wrapper(&myhandler);
}

void timer_setrate(unsigned short rate)
{
  timer_rate = rate;

  /* Set rate in hardware */
  outportb(0x43, 0x34);
  outportb(0x40, rate);
  outportb(0x40, rate >> 8);
}
