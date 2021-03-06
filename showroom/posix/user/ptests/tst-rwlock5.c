/* Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.

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

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <rtai_posix.h>

static pthread_mutex_t m; // = PTHREAD_MUTEX_INITIALIZER;
static pthread_rwlock_t r;


static void *
tf (void *arg)
{
  pthread_setschedparam_np(0, SCHED_FIFO, 0, 0xF, PTHREAD_HARD_REAL_TIME_NP);
  if (pthread_rwlock_wrlock (&r) == 0)
    {
      puts ("child: rwlock_wrlock succeeded");
      exit (1);
    }

  puts ("child: rwlock_wrlock returned");

  exit (1);
}


static int
do_test (void)
{
  pthread_t th;

  if (pthread_rwlock_init (&r, NULL) != 0)
    {
      puts ("rwlock_init failed");
      return 1;
    }

  if (pthread_rwlock_wrlock (&r) != 0)
    {
      puts ("rwlock_wrlock failed");
      return 1;
    }

  if (pthread_mutex_lock (&m) != 0)
    {
      puts ("mutex_lock failed");
      return 1;
    }

  /* Set an alarm for 1 second.  The wrapper will expect this.  */
  alarm (1);

  if (pthread_create (&th, NULL, tf, NULL) != 0)
    {
      puts ("create failed");
      return 1;
    }

  /* This call should never return.  */
  pthread_mutex_lock (&m);

  puts ("2nd mutex_lock returned");
  return 1;
}

#define EXPECTED_SIGNAL SIGALRM
int main(void)
{
	pthread_setschedparam_np(0, SCHED_FIFO, 0, 0xF, PTHREAD_HARD_REAL_TIME_NP);
	start_rt_timer(0);
	pthread_mutex_init(&m, NULL);
	do_test();
	return 0;
}
