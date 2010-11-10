/** 
* @file guitypes.h
* @brief types used within this system
* @author ken
* @date 2006-03-22
*/

#ifndef __GUI_TYPES_20060322_H__
#define __GUI_TYPES_20060322_H__


//============================================================================//
// include
//============================================================================//
#include <math.h>



//============================================================================//
// type
//============================================================================// 
namespace guiex
{
	typedef short			int16;
	typedef int				int32;
	typedef char			int8;


	typedef unsigned short	uint16;
	typedef unsigned int	uint32;
	typedef unsigned char	uint8;


	typedef float			real;

#if defined(WIN32)
	typedef unsigned __int64    uint64;
	typedef		  __int64		int64;
#	define	PRINTF_INT64		"%I64d"
#	define	PRINTF_UINT64		"%I64u"

#else
	typedef unsigned long long  uint64;
	typedef		  long long		int64;
#	define	PRINTF_INT64		"%lld"
#	define	PRINTF_UINT64		"%llu"

#endif
}//guiex


//============================================================================//
// macro
//============================================================================// 


#define GUI_INVALID	0xFFFFFFFF


// Efficiently returns the least power of two >= X...
# define GUI_POW(X) (((X) == 0)?1:(X-=1,X|=X>>1,X|=X>>2,X|=X>>4,X|=X>>8,X|=X>>16,(++X)))
# define GUI_EVEN(NUM) (((NUM) & 1) == 0)
# define GUI_ODD(NUM) (((NUM) & 1) == 1)
# define GUI_BIT_ENABLED(WORD, BIT) (((WORD) & (BIT)) != 0)
# define GUI_BIT_DISABLED(WORD, BIT) (((WORD) & (BIT)) == 0)
# define GUI_BIT_CMP_MASK(WORD, BIT, MASK) (((WORD) & (BIT)) == MASK)
# define GUI_SET_BITS(WORD, BITS) (WORD |= (BITS))
# define GUI_CLR_BITS(WORD, BITS) (WORD &= ~(BITS))




/**
* @brief value of real delta, used for compare value of  real
*/
#define GUI_REALDELTA	(1e-08)

/**
* @brief compare real value
*/
#define GUI_REAL_EQUAL( expected, actual )	\
	(fabs( (expected) - (actual) ) < (GUI_REALDELTA))


#define GUI_FLOAT2UINT_ROUND(value)	static_cast<uint32>((value)+0.5f)


#if defined(WIN32)
// A quick define to overcome different names for the same function
#	define snprintf _snprintf
#   define vsnprintf vsnprintf
#endif

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

//============================================================================//
// enum
//============================================================================// 

namespace guiex
{
	enum ETextAlignmentHorz
	{
		eTextAlignment_Horz_Center = 0x00,
		eTextAlignment_Horz_Left  = 0x01,
		eTextAlignment_Horz_Right = 0x02,
	};
	enum ETextAlignmentVert
	{
		eTextAlignment_Vert_Center = 0x00,
		eTextAlignment_Vert_Up = 0x10,
		eTextAlignment_Vert_Down = 0x20,
	};

	const uint8 GUI_TA_CENTER = eTextAlignment_Horz_Center | eTextAlignment_Vert_Center;
	const uint8 GUI_TA_HORIZON_MASK = 0x0F;
	const uint8 GUI_TA_VERTICAL_MASK = 0xF0;

	/**
	* @brief image operation
	*/
	enum EImageOperation
	{
		IMAGE_NONE,				///!< none operation
		IMAGE_ROTATE90CW,		///!< rotate image for 90 CW
		IMAGE_ROTATE90CCW,		///!< rotate image for 90 CCW
		IMAGE_FLIPHORIZON,		///!< flip image horizon
		IMAGE_FLIPVERTICAL,		///!< flip image vertical
	};

	enum EScreenValue
	{
		eScreenValue_Pixel = 0,
		eScreenValue_Percentage,
	};

	/**
	* @brief pixel format
	*/
	enum EGuiPixelFormat
	{
		/// 2 byte pixel format, 1 byte luminance, 1 byte alpha
		GUI_PF_LA_16 = 0,

		/// 3 byte pixel format
		GUI_PF_RGB_24,

		/// 4 byte pixel format
		GUI_PF_RGBA_32,

		/// 4 byte pixel format
		GUI_PF_ARGB_32,
	};

}//namespace guiex

#endif //__GUI_TYPES_20060322_H__

