
#include "stdafx.h"
#include <tolc_datetime.h>


TOLEXTC_BEGIN


/* global */

TOLCDECL Bool TOLAPI Year_IsLeap(Short year)
{
	return (year%4==0 && year%100!=0) || (year%400==0);
}

TOLCDECL Tiny TOLAPI Month_LastDay(Short year, Tiny month)
{
	switch(month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		case 2:
			return Year_IsLeap(year)?29:28;
	}

	return 0;
}


/* date */

TOLCDECL Bool TOLAPI Date_Test(PCT_DATE pDate)
{
	return pDate && pDate->year>0 &&
		pDate->month>0 && pDate->month<=12 &&
		pDate->day>0 && pDate->day<=Month_LastDay(pDate->year, pDate->month);
}

TOLCDECL void TOLAPI Date_Normalize(PT_DATE pDate)
{
	if(!pDate)
		return;

	if(pDate->month>0)
	{
		while(pDate->month>12)
		{
			pDate->month-=12;
			pDate->year++;
		}
	}
	else
	{
		while(pDate->month<1)
		{
			pDate->month+=12;
			pDate->year--;
		}
	}

	if(pDate->day>0)
	{
		Tiny lastday=Month_LastDay(pDate->year, pDate->month);

		while(pDate->day>lastday)
		{
			pDate->day-=lastday;

			pDate->month++;
			if(pDate->month>12)
			{
				pDate->month-=12;
				pDate->year++;
			}

			lastday=Month_LastDay(pDate->year, pDate->month);
		}
	}
	else
	{
		while(pDate->day<1)
		{
			pDate->month--;
			if(pDate->month<1)
			{
				pDate->month+=12;
				pDate->year--;
			}

			pDate->day+=Month_LastDay(pDate->year, pDate->month);
		}
	}
}

TOLCDECL void TOLAPI Date_Set(PT_DATE pDate, Short year, Tiny month, Tiny day)
{
	if(pDate)
	{
		pDate->year=year;
		pDate->month=month;
		pDate->day=day;
	}
}

TOLCDECL void TOLAPI Date_SetDate(PT_DATE pDateDest, PCT_DATE pDateSrc)
{
	if(pDateDest && pDateSrc)
	{
		pDateDest->year=pDateSrc->year;
		pDateDest->month=pDateSrc->month;
		pDateDest->day=pDateSrc->day;
	}
}

TOLCDECL void TOLAPI Date_Add(PT_DATE pDate, Short year, Tiny month, Tiny day)
{
	if(pDate)
	{
		pDate->year+=year;
		pDate->month+=month;
		pDate->day+=day;
		Date_Normalize(pDate);
	}

}

TOLCDECL void TOLAPI Date_AddDate(PT_DATE pDateDest, PCT_DATE pDateSrc)
{
	if(pDateDest && pDateSrc)
	{
		pDateDest->year=pDateSrc->year;
		pDateDest->month=pDateSrc->month;
		pDateDest->day=pDateSrc->day;
		Date_Normalize(pDateDest);
	}
}

TOLCDECL int TOLAPI Date_Compare(PCT_DATE pDate1, PCT_DATE pDate2)
{
	if(pDate1!=NULL && pDate2!=NULL)
	{
		if(pDate1->year>pDate2->year)
			return T_DATETIME_YEAR;
		else if(pDate1->year<pDate2->year)
			return -T_DATETIME_YEAR;
		else if(pDate1->month>pDate2->month)
			return T_DATETIME_MONTH;
		else if(pDate1->month<pDate2->month)
			return -T_DATETIME_MONTH;
		else if(pDate1->day>pDate2->day)
			return T_DATETIME_DAY;
		else if(pDate1->day<pDate2->day)
			return -T_DATETIME_DAY;
	}
	else if(pDate1==NULL)
		return T_DATETIME_NULL;
	else if(pDate2==NULL)
		return -T_DATETIME_NULL;

	return T_DATETIME_EQUAL;
}


/* time */

TOLCDECL Bool TOLAPI Time_Test(PCT_TIME pTime)
{
	return pTime &&
		pTime->hour>=0 && pTime->hour<24 &&
		pTime->minute>=0 && pTime->minute<60 &&
		pTime->second>=0 && pTime->second<60 &&
		pTime->millisecond>=0 && pTime->millisecond<1000;
}

TOLCDECL void TOLAPI Time_Normalize(PT_TIME pTime)
{
	if(!pTime)
		return;

	if(pTime->millisecond>0)
	{
		pTime->second+=pTime->millisecond/1000;
		pTime->millisecond%=1000;
	}
	else if(pTime->millisecond<0)
	{
		pTime->second+=(pTime->millisecond/1000-1);
		pTime->millisecond%=-1000;
	}

	if(pTime->second>0)
	{
		pTime->minute+=pTime->second/60;
		pTime->second%=60;
	}
	else if(pTime->second<0)
	{
		pTime->minute+=(pTime->second/60-1);
		pTime->second%=-60;
	}

	if(pTime->minute>0)
	{
		pTime->hour+=pTime->minute/60;
		pTime->minute%=60;
	}
	else if(pTime->minute<0)
	{
		pTime->hour+=(pTime->minute/60-1);
		pTime->minute%=-60;
	}
}

TOLCDECL void TOLAPI Time_Set(PT_TIME pTime, Tiny hour, Tiny minute, Tiny second, Short millisecond)
{
	if(pTime)
	{
		pTime->hour=hour;
		pTime->minute=minute;
		pTime->second=second;
		pTime->millisecond=millisecond;
	}
}

TOLCDECL void TOLAPI Time_SetTime(PT_TIME pTime, PCT_TIME pTimeSrc)
{
	if(pTime && pTimeSrc)
	{
		pTime->hour=pTimeSrc->hour;
		pTime->minute=pTimeSrc->minute;
		pTime->second=pTimeSrc->second;
		pTime->millisecond=pTimeSrc->millisecond;
	}
}

TOLCDECL void TOLAPI Time_Add(PT_TIME pTime, Tiny hour, Tiny minute, Tiny second, Short millisecond)
{
	if(pTime)
	{
		pTime->hour+=hour;
		pTime->minute+=minute;
		pTime->second+=second;
		pTime->millisecond+=millisecond;
		Time_Normalize(pTime);
	}
}

TOLCDECL void TOLAPI Time_AddTime(PT_TIME pTimeDest, PCT_TIME pTimeSrc)
{
	if(pTimeDest && pTimeSrc)
	{
		pTimeDest->hour=pTimeSrc->hour;
		pTimeDest->minute=pTimeSrc->minute;
		pTimeDest->second=pTimeSrc->second;
		pTimeDest->millisecond=pTimeSrc->millisecond;
		Time_Normalize(pTimeDest);
	}
}

TOLCDECL int TOLAPI Time_Compare(PCT_TIME pTime1, PCT_TIME pTime2)
{
	if(pTime1!=NULL && pTime2!=NULL)
	{
		if(pTime1->hour>pTime2->hour)
			return T_DATETIME_HOUR;
		else if(pTime1->hour<pTime2->hour)
			return -T_DATETIME_HOUR;
		else if(pTime1->minute>pTime2->minute)
			return T_DATETIME_MINUTE;
		else if(pTime1->minute<pTime2->minute)
			return -T_DATETIME_MINUTE;
		else if(pTime1->second>pTime2->second)
			return T_DATETIME_SECOND;
		else if(pTime1->second<pTime2->second)
			return -T_DATETIME_SECOND;
		else if(pTime1->millisecond>pTime2->millisecond)
			return T_DATETIME_MILLISECOND;
		else if(pTime1->millisecond<pTime2->millisecond)
			return -T_DATETIME_MILLISECOND;
	}
	else if(pTime1==NULL)
		return T_DATETIME_NULL;
	else if(pTime2==NULL)
		return -T_DATETIME_NULL;

	return T_DATETIME_EQUAL;
}


/* datetime */

TOLCDECL Bool TOLAPI DateTime_Test(PCT_DATETIME pDateTime)
{
	return pDateTime &&
		Date_Test(&pDateTime->date) &&
		Time_Test(&pDateTime->time);
}

void TOLAPI DateTime_AdjustHour(PTiny pHour, PTiny pDay)
{
	if(*pHour>0)
	{
		while(*pHour>60)
		{
			*pHour-=60;
			*pDay++;
		}
	}
	else
	{
		while(*pHour<1)
		{
			*pHour+=60;
			*pDay--;
		}
	}
}

TOLCDECL void TOLAPI DateTime_Normalize(PT_DATETIME pDateTime)
{
}

TOLCDECL void TOLAPI DateTime_Set(PT_DATETIME pDateTime, Short year, Tiny month, Tiny day, Tiny hour, Tiny minute, Tiny second, Short millisecond)
{
	if(pDateTime)
	{
		Time_Set(&pDateTime->time, hour, minute, second, millisecond);
		Date_Set(&pDateTime->date, year, month, day);
	}
}

TOLCDECL void TOLAPI DateTime_SetDateTime(PT_DATETIME pDateTimeDest, PCT_DATETIME pDateTimeSrc)
{
	if(pDateTimeDest && pDateTimeSrc)
	{
		Time_SetTime(&pDateTimeDest->time, &pDateTimeSrc->time);
		Date_SetDate(&pDateTimeDest->date, &pDateTimeSrc->date);
	}
}

TOLCDECL void TOLAPI DateTime_Add(PT_DATETIME pDateTime, Short year, Tiny month, Tiny day, Tiny hour, Tiny minute, Tiny second, Short millisecond)
{
	if(pDateTime)
	{
		Time_Add(&pDateTime->time, hour, minute, second, millisecond);
		DateTime_AdjustHour(&pDateTime->time.hour, &pDateTime->date.day);
		Date_Add(&pDateTime->date, year, month, day);
	}
}

TOLCDECL void TOLAPI DateTime_AddDateTime(PT_DATETIME pDateTimeDest, PCT_DATETIME pDateTimeSrc)
{
	if(pDateTimeDest && pDateTimeSrc)
	{
		Time_AddTime(&pDateTimeDest->time, &pDateTimeSrc->time);
		DateTime_AdjustHour(&pDateTimeDest->time.hour, &pDateTimeDest->date.day);
		Date_AddDate(&pDateTimeDest->date, &pDateTimeSrc->date);
	}
}

TOLCDECL int TOLAPI DateTime_Compare(PCT_DATETIME pDateTime1, PCT_DATETIME pDateTime2)
{
	if(pDateTime1!=NULL && pDateTime2!=NULL)
	{
		int ret=Date_Compare(&pDateTime1->date, &pDateTime2->date);
		if(ret==T_DATETIME_EQUAL)
			ret=Time_Compare(&pDateTime1->time, &pDateTime2->time);

		return ret;
	}
	else if(pDateTime1==NULL)
		return T_DATETIME_NULL;
	else if(pDateTime2==NULL)
		return -T_DATETIME_NULL;

	return T_DATETIME_EQUAL;
}




/* LPSYSTEMTIME */

TOLCDECL void TOLAPI Date_SetSystem(PT_DATE pDate)
{
	if(pDate)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		Date_FromSysTime(pDate, &st);
	}
}

TOLCDECL void TOLAPI Date_FromSysTime(PT_DATE pDate, LPSYSTEMTIME pSysTime)
{
	if(pDate && pSysTime)
	{
		pDate->year=pSysTime->wYear;
		pDate->month=(Tiny)pSysTime->wMonth;
		pDate->day=(Tiny)pSysTime->wDay;
	}
}

TOLCDECL void TOLAPI Date_ToSysTime(PT_DATE pDate, LPSYSTEMTIME pSysTime)
{
	if(pDate && pSysTime)
	{
		pSysTime->wYear=pDate->year;
		pSysTime->wMonth=pDate->month;
		pSysTime->wDay=pDate->day;
	}
}

TOLCDECL void TOLAPI Time_SetSystem(PT_TIME pTime)
{
	if(pTime)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		Time_FromSysTime(pTime, &st);
	}
}

TOLCDECL void TOLAPI Time_FromSysTime(PT_TIME pTime, LPSYSTEMTIME pSysTime)
{
	if(pTime && pSysTime)
	{
		pTime->hour=(Tiny)pSysTime->wHour;
		pTime->minute=(Tiny)pSysTime->wMinute;
		pTime->second=(Tiny)pSysTime->wSecond;
		pTime->millisecond=pSysTime->wMilliseconds;
	}
}

TOLCDECL void TOLAPI Time_ToSysTime(PT_TIME pTime, LPSYSTEMTIME pSysTime)
{
	if(pTime && pSysTime)
	{
		pSysTime->wHour=pTime->hour;
		pSysTime->wMinute=pTime->minute;
		pSysTime->wSecond=pTime->second;
		pSysTime->wMilliseconds=pTime->millisecond;
	}
}
TOLCDECL void TOLAPI DateTime_SetSystem(PT_DATETIME pDateTime)
{
	if(pDateTime)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		DateTime_FromSysTime(pDateTime, &st);
	}
}

TOLCDECL void TOLAPI DateTime_FromSysTime(PT_DATETIME pDateTime, LPSYSTEMTIME pSysTime)
{
	if(pDateTime && pSysTime)
	{
		Date_FromSysTime(&pDateTime->date, pSysTime);
		Time_FromSysTime(&pDateTime->time, pSysTime);
	}
}

TOLCDECL void TOLAPI DateTime_ToSysTime(PT_DATETIME pDateTime, LPSYSTEMTIME pSysTime)
{
	if(pDateTime && pSysTime)
	{
		Date_ToSysTime(&pDateTime->date, pSysTime);
		Time_ToSysTime(&pDateTime->time, pSysTime);
	}
}


TOLEXTC_END
