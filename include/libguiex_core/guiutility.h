/** 
 * @file guiutility.h
 * @brief utility function for gui system
 * @author ken
 * @date 2006-06-01
 */


#ifndef __GUI_UTILITY_H_20060601__
#define __GUI_UTILITY_H_20060601__


//============================================================================//
// include
//============================================================================// 
#include "guibase.h"




namespace guiex
{
//============================================================================//
// class
//============================================================================// 
/**
 * @class CGUIUtility
 * @brief some useful function for guiex system
 */
class GUIEXPORT CGUIUtility
{
public:
	//static char * Strsncpy (char *dst, const char *src, size_t maxlen);

	static char * timestamp (char date_and_time[],
								  int date_and_timelen,
								  int return_pointer_to_first_digit=0);

	static uint32 Log2 (uint32 num);

	static size_t format_hexdump (const char *buffer,size_t size,char *obuf,size_t obuf_sz);

};


}//namespace guiex

#endif //__GUI_UTILITY_H_20060601__
