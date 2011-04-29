/** 
* @file guiutility.cpp
* @brief utility function for gui system
* @author ken
* @date 2006-06-01
*/


//============================================================================//
// include 
//============================================================================// 
#include "guiutility.h"
#include <string.h>
#if defined(GUIEX_PLATFORM_WIN32)
#include <windows.h>
#else
#include <ctype.h>
#endif
#include <stdio.h>
#include <locale.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	char * CGUIUtility::timestamp (char date_and_time[],int32 date_and_timelen,int32 return_pointer_to_first_digit)
	{
		if (date_and_timelen < 35)
		{
			return NULL;
		}

#if defined(GUIEX_PLATFORM_WIN32)
		// Emulate Unix.  Win32 does NOT support all the UNIX versions
		// below, so DO we need this ifdef.
		static const char *day_of_week_name[] =
		{
			("Sun"),
			("Mon"),
			("Tue"),
			("Wed"),
			("Thu"),
			("Fri"),
			("Sat")
		};

		static const char *month_name[] =
		{
			("Jan"),
			("Feb"),
			("Mar"),
			("Apr"),
			("May"),
			("Jun"),
			("Jul"),
			("Aug"),
			("Sep"),
			("Oct"),
			("Nov"),
			("Dec")
		};

		SYSTEMTIME local;
		::GetLocalTime (&local);

		sprintf (date_and_time,
			("%3s %3s %2d %04d %02d:%02d:%02d.%06d"),
			day_of_week_name[local.wDayOfWeek],
			month_name[local.wMonth - 1],
			(int32) local.wDay,
			(int32) local.wYear,
			(int32) local.wHour,
			(int32) local.wMinute,
			(int32) local.wSecond,
			(int32) (local.wMilliseconds * 1000));
		return &date_and_time[15 + (return_pointer_to_first_digit != 0)];
#elif defined(GUIEX_PLATFORM_LINUX)
		long now;
		char timebuf[40];
		now=(long)time((time_t *)0);
		ctime_r (&now,timebuf);

		// date_and_timelen > sizeof timebuf!
		strncpy (date_and_time,timebuf,date_and_timelen);
		char yeartmp[5];
		strncpy (yeartmp,&date_and_time[20],5);
		char timetmp[9];
		strncpy (timetmp,&date_and_time[11],9);
		sprintf (&date_and_time[11], ("%s %s.%06ld"),yeartmp,timetmp,now );
		date_and_time[33] = '\0';
		return &date_and_time[15 + (return_pointer_to_first_digit != 0)];
#elif defined( GUIEX_PLATFORM_MAC)
		long now;
		char timebuf[40];
		now=(long)time((time_t *)0);
		ctime_r (&now,timebuf);
		
		// date_and_timelen > sizeof timebuf!
		strncpy (date_and_time,timebuf,date_and_timelen);
		char yeartmp[5];
		strncpy (yeartmp,&date_and_time[20],5);
		char timetmp[9];
		strncpy (timetmp,&date_and_time[11],9);
		sprintf (&date_and_time[11], ("%s %s.%06ld"),yeartmp,timetmp,now );
		date_and_time[33] = '\0';
		return &date_and_time[15 + (return_pointer_to_first_digit != 0)];
#else
#	error "unknown platform"
#endif /* WIN32 */
		//------------------------------------------------------------------------------
	}
	//------------------------------------------------------------------------------
	size_t CGUIUtility::format_hexdump (const char *buffer,size_t size,char *obuf,size_t obuf_sz)
	{
		unsigned char c;
		char textver[16 + 1];

		// We can fit 16 bytes output in text mode per line, 4 chars per byte.
		size_t maxlen = (obuf_sz / 68) * 16;

		if (size > maxlen)
		{
			size = maxlen;
		}

		size_t i;

		size_t lines = size / 16;
		for (i = 0; i < lines; i++)
		{
			size_t j;

			for (j = 0 ; j < 16; j++)
			{
				c = (unsigned char) buffer[(i << 4) + j];    // or, buffer[i*16+j]
				sprintf (obuf,"%02x ",c);
				obuf += 3;
				if (j == 7)
				{
					sprintf (obuf," ");
					obuf++;
				}
				textver[j] = isprint (c) ? c : '.';
			}

			textver[j] = 0;

			sprintf (obuf,"  %s\n",textver);

			while (*obuf != '\0')
			{
				obuf++;
			}
		}

		if (size % 16)
		{
			for (i = 0 ; i < size % 16; i++)
			{
				c = (unsigned char) buffer[size - size % 16 + i];
				sprintf (obuf,"%02x ",c);
				obuf += 3;
				if (i == 7)
				{
					sprintf (obuf," ");
					obuf++;
				}
				textver[i] = isprint (c) ? c : '.';
			}

			for (i = size % 16; i < 16; i++)
			{
				sprintf (obuf,"   ");
				obuf += 3;
				if (i == 7)
				{
					sprintf (obuf," ");
					obuf++;
				}
				textver[i] = ' ';
			}

			textver[i] = 0;
			sprintf (obuf,"  %s\n",textver);
		}
		return size;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIUtility::Log2 (uint32 num)
	{
		unsigned long log = 0;

		for (; num > 0; ++log)
			num >>= 1;

		return log;
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIUtility::GenerateWidgetName()
	{
		static int nNameCount = 0;
		char buf[32];
		snprintf( buf,32, "widgetname_%d", nNameCount++ );
		return buf;
	}
	//------------------------------------------------------------------------------

}//namespace guiex