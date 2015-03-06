/*
 * This file is Copyright (c) 2015 by the GPSD project
 * BSD terms apply: see the file COPYING in the distribution root for details.
 */

#ifndef GPSD_NTPSHM_H
#define GPSD_NTPSHM_H

#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NTPD_BASE	0x4e545030	/* "NTP0" */

/* 
 * How to read and write fields in an NTP shared segment.
 * This definition of shmTime is from ntpd source ntpd/refclock_shm.c
 *
 * The fields aren't documented there.  It appears the only use of
 * nsamples is internal to the (obsolete and deprecated) EES M201
 * receiver refclock. The precision field is nominally log(2) of the
 * source's jitter in seconds:
 * 	-1 is about 100mSec jitter
 *	-10 is about 1 mSec jitter (GR-601W or other USB with 1ms poll interval)
 *	-13 is about 100 uSec
 *	-20 is about 1 nSec (typical for serial PPS)
 */

struct shmTime
{
    int mode;	/* 0 - if valid set
		 *       use values,
		 *       clear valid
		 * 1 - if valid set
		 *       if count before and after read of values is equal,
		 *         use values
		 *       clear valid
		 */
    volatile int count;
    time_t clockTimeStampSec;
    int clockTimeStampUSec;
    time_t receiveTimeStampSec;
    int receiveTimeStampUSec;
    int leap;			/* not leapsecond offset, a notification code */
    int precision;		/* log(2) of source jitter */
    int nsamples;		/* not used */
    volatile int valid;
    unsigned        clockTimeStampNSec;     /* Unsigned ns timestamps */
    unsigned        receiveTimeStampNSec;   /* Unsigned ns timestamps */
    int             dummy[8];
};

#endif

/*
 * These types are internal to GPSD
 */
enum segstat_t {OK, NO_SEGMENT, NOT_READY, BAD_MODE, CLASH};

struct shm_stat_t {
    enum segstat_t status;
    struct timespec tvc, tvr, tvt;
    int precision;
    int leap;
};

struct shmTime /*@null@*/ *shm_get(int, bool, bool);
extern char *shm_name(const int);
enum segstat_t shm_query(/*@null@*/struct shmTime *, 
			 /*@out@*/struct shm_stat_t *,
			 const bool);

/* end */