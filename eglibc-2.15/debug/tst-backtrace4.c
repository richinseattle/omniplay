/* Copyright (C) 2011 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by CodeSourcery.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <execinfo.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

static int do_test (void);
#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"

/* Set to a non-zero value if the test fails.  */
volatile int ret;

/* Accesses to X are used to prevent optimization.  */
volatile int x;

/* Called if the test fails.  */
#define FAIL() \
  do { printf ("Failure on line %d\n", __LINE__); ret = 1; } while (0)

/* The backtrace should include at least handle_signal, a signal
   trampoline, 3 * fn, and do_test.  */
#define NUM_FUNCTIONS 6

/* Use this attribute to prevent inlining, so that all expected frames
   are present.  */
#define NO_INLINE __attribute__((noinline))

volatile int sig_handled = 0;

void
handle_signal (int signum)
{
  void *addresses[NUM_FUNCTIONS];
  char **symbols;
  int n;
  int i;

  sig_handled = 1;

  /* Get the backtrace addresses.  */
  n = backtrace (addresses, sizeof (addresses) / sizeof (addresses[0]));
  printf ("Obtained backtrace with %d functions\n", n);
  /*  Check that there are at least six functions.  */
  if (n < NUM_FUNCTIONS)
    {
      FAIL ();
      return;
    }
  /* Convert them to symbols.  */
  symbols = backtrace_symbols (addresses, n);
  /* Check that symbols were obtained.  */
  if (symbols == NULL)
    {
      FAIL ();
      return;
    }
  for (i = 0; i < n; ++i)
    printf ("Function %d: %s\n", i, symbols[i]);
  /* Check that the function names obtained are accurate.  */
  if (strstr (symbols[0], "handle_signal") == NULL)
    {
      FAIL ();
      return;
    }
  /* Do not check name for signal trampoline.  */
  for (i = 2; i < n - 1; i++)
    if (strstr (symbols[i], "fn") == NULL)
      {
	FAIL ();
	return;
      }
  /* Symbol names are not available for static functions, so we do not
     check do_test.  */
}

NO_INLINE int
fn (int c)
{
  pid_t parent_pid, child_pid;

  if (c > 0)
    {
      fn (c - 1);
      return x;
    }

  signal (SIGUSR1, handle_signal);
  parent_pid = getpid ();

  child_pid = fork ();
  if (child_pid == (pid_t) -1)
    abort ();
  else if (child_pid == 0)
    {
      sleep (1);
      kill (parent_pid, SIGUSR1);
      _exit (0);
    }

  /* In the parent.  */
  while (sig_handled == 0)
    ;

  return 0;
}

NO_INLINE static int
do_test (void)
{
  fn (2);
  return ret;
}
