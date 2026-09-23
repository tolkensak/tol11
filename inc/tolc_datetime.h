
#ifndef _INC_TOLC_DATET_TIME_H
#define _INC_TOLC_DATET_TIME_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


/* common functions */

TOLCDECL Bool TOLAPI Year_IsLeap(Short year);
TOLCDECL Tiny TOLAPI Month_LastDay(Short year, Tiny month);


/* return values of functions compare */

#define T_DATETIME_EQUAL       0
#define T_DATETIME_NULL        1
#define T_DATETIME_YEAR        2
#define T_DATETIME_MONTH       3
#define T_DATETIME_DAY         4
#define T_DATETIME_HOUR        5
#define T_DATETIME_MINUTE      6
#define T_DATETIME_SECOND      7
#define T_DATETIME_MILLISECOND 8


/* T_DATE */

typedef struct tagDate {
	Short year;
	Tiny month;
	Tiny day;
} T_DATE, *PT_DATE;

typedef const T_DATE *PCT_DATE;

TOLCDECL Bool TOLAPI Date_Test(PCT_DATE pDate);
TOLCDECL void TOLAPI Date_Normalize(PT_DATE pDate);

TOLCDECL void TOLAPI Date_Set(PT_DATE pDate, Short year, Tiny month, Tiny day);
TOLCDECL void TOLAPI Date_SetDate(PT_DATE pDateDest, PCT_DATE pDateSrc);
TOLCDECL void TOLAPI Date_Add(PT_DATE pDate, Short year, Tiny month, Tiny day);
TOLCDECL void TOLAPI Date_AddDate(PT_DATE pDateDest, PCT_DATE pDateSrc);

TOLCDECL int TOLAPI Date_Compare(PCT_DATE pDate1, PCT_DATE pDate2);


/* T_TIME */

typedef struct tagTime {
	Tiny hour;
	Tiny minute;
	Tiny second;
	Short millisecond;
} T_TIME, *PT_TIME;

typedef const T_TIME *PCT_TIME;

TOLCDECL Bool TOLAPI Time_Test(PCT_TIME pTime);
TOLCDECL void TOLAPI Time_Normalize(PT_TIME pTime);

TOLCDECL void TOLAPI Time_Set(PT_TIME pTime, Tiny hour, Tiny minute, Tiny second, Short millisecond);
TOLCDECL void TOLAPI Time_SetTime(PT_TIME pTimeDest, PCT_TIME pTimeSrc);
TOLCDECL void TOLAPI Time_Add(PT_TIME pTime, Tiny hour, Tiny minute, Tiny second, Short millisecond);
TOLCDECL void TOLAPI Time_AddTime(PT_TIME pTimeDest, PCT_TIME pTimeSrc);

TOLCDECL int TOLAPI Time_Compare(PCT_TIME pTime1, PCT_TIME pTime2);


/* T_DATETIME */

typedef struct tagDateTime {
	T_DATE date;
	T_TIME time;
} T_DATETIME, *PT_DATETIME;

typedef const T_DATETIME *PCT_DATETIME;

TOLCDECL Bool TOLAPI DateTime_Test(PCT_DATETIME pDateTime);
TOLCDECL void TOLAPI DateTime_Normalize(PT_DATETIME pDateTime);

TOLCDECL void TOLAPI DateTime_Set(PT_DATETIME pDateTime, Short year, Tiny month, Tiny day, Tiny hour, Tiny minute, Tiny second, Short millisecond);
TOLCDECL void TOLAPI DateTime_SetDateTime(PT_DATETIME pDateTimeDest, PCT_DATETIME pDateTimeSrc);
TOLCDECL void TOLAPI DateTime_Add(PT_DATETIME pDateTime, Short year, Tiny month, Tiny day, Tiny hour, Tiny minute, Tiny second, Short millisecond);
TOLCDECL void TOLAPI DateTime_AddDateTime(PT_DATETIME pDateTimeDest, PCT_DATETIME pDateTimeSrc);

TOLCDECL int TOLAPI DateTime_Compare(PCT_DATETIME pDateTime1, PCT_DATETIME pDateTime2);



/* LPSYSTEMTIME */

TOLCDECL void TOLAPI Date_SetSystem(PT_DATE pDate);
TOLCDECL void TOLAPI Date_FromSysTime(PT_DATE pDate, LPSYSTEMTIME pSysTime);
TOLCDECL void TOLAPI Date_ToSysTime(PT_DATE pDate, LPSYSTEMTIME pSysTime);

TOLCDECL void TOLAPI Time_SetSystem(PT_TIME pTime);
TOLCDECL void TOLAPI Time_FromSysTime(PT_TIME pTime, LPSYSTEMTIME pSysTime);
TOLCDECL void TOLAPI Time_ToSysTime(PT_TIME pTime, LPSYSTEMTIME pSysTime);

TOLCDECL void TOLAPI DateTime_SetSystem(PT_DATETIME pDateTime);
TOLCDECL void TOLAPI DateTime_FromSysTime(PT_DATETIME pTime, LPSYSTEMTIME pSysTime);
TOLCDECL void TOLAPI DateTime_ToSysTime(PT_DATETIME pDateTime, LPSYSTEMTIME pSysTime);


TOLEXTC_END

#endif /* _INC_TOLC_DATET_TIME_H */
