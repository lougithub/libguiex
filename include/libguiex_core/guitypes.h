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



// used to do something regularly
#define ITEM_END
#define ITEM(item, next) ITEM_BEGIN_TOKEN item ITEM_END_TOKEN next
#define MACRO_LIST1(x1) ITEM(x1, ITEM_END)
#define MACRO_LIST2(x1,x2) ITEM(x2, MACRO_LIST1(x1))
#define MACRO_LIST3(x1,x2,x3) ITEM(x3, MACRO_LIST2(x1,x2))
#define MACRO_LIST4(x1,x2,x3,x4) ITEM(x4, MACRO_LIST3(x1,x2,x3))
#define MACRO_LIST5(x1,x2,x3,x4,x5) ITEM(x5, MACRO_LIST4(x1,x2,x3,x4))
#define MACRO_LIST6(x1,x2,x3,x4,x5,x6) ITEM(x6, MACRO_LIST5(x1,x2,x3,x4,x5))
#define MACRO_LIST7(x1,x2,x3,x4,x5,x6,x7) ITEM(x7, MACRO_LIST6(x1,x2,x3,x4,x5,x6))
#define MACRO_LIST8(x1,x2,x3,x4,x5,x6,x7,x8) ITEM(x8, MACRO_LIST7(x1,x2,x3,x4,x5,x6,x7))
#define MACRO_LIST9(x1,x2,x3,x4,x5,x6,x7,x8,x9) ITEM(x9, MACRO_LIST8(x1,x2,x3,x4,x5,x6,x7,x8))
#define MACRO_LIST10(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10) ITEM(x10, MACRO_LIST9(x1,x2,x3,x4,x5,x6,x7,x8,x9))
#define MACRO_LIST11(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11) ITEM(x11, MACRO_LIST10(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10))
#define MACRO_LIST12(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12) ITEM(x12, MACRO_LIST11(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11))
#define MACRO_LIST13(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13) ITEM(x13, MACRO_LIST12(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12))
#define MACRO_LIST14(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14) ITEM(x14, MACRO_LIST13(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13))
#define MACRO_LIST15(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15) ITEM(x15, MACRO_LIST14(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14))
#define MACRO_LIST16(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16) ITEM(x16, MACRO_LIST15(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15))
#define MACRO_LIST17(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17) ITEM(x17, MACRO_LIST16(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16))
#define MACRO_LIST18(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18) ITEM(x18, MACRO_LIST17(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17))
#define MACRO_LIST19(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19) ITEM(x19, MACRO_LIST18(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18))
#define MACRO_LIST20(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20) ITEM(x20, MACRO_LIST19(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19))
#define MACRO_LIST21(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20,x21) ITEM(x21, MACRO_LIST20(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20))
//usage
//#define ITEM_LIST MACRO_LIST3(A, B, C)



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
	/**
	* @brief align mode
	*/
	enum EAlignMode
	{
		eAbsolute,								///the position and size is fixed
		eRelative,								///the position and rsize is relative
		eRelativePos_AbsoluteSize,	///the position is relative but size is absolute

		__ALIGN_MODE_MAX__
	};

	const uint8 GUI_TA_H_LEFT  = 0x00;
	const uint8 GUI_TA_H_RIGHT = 0x01;
	const uint8 GUI_TA_H_CENTER = 0x02;
	const uint8 GUI_TA_V_UP = 0x00;
	const uint8 GUI_TA_V_DOWN = 0x10;
	const uint8 GUI_TA_V_CENTER = 0x20;

	const uint8 GUI_TA_CENTER = GUI_TA_H_CENTER | GUI_TA_V_CENTER;
	const uint8 GUI_TA_HORIZON_MASK = 0x0F;
	const uint8 GUI_TA_VERTICAL_MASK = 0xF0;

	/**
	* @brief image opration
	*/
	enum EImageOperation
	{
		IMAGE_NONE,				///!< none operation
		IMAGE_ROTATE90CW,		///!< rotate image for 90 CW
		IMAGE_ROTATE90CCW,		///!< rotate image for 90 CCW
		IMAGE_FLIPHORIZON,		///!< flip image horizon
		IMAGE_FLIPVERTICAL,		///!< flip image vertical
	};


	/**
	* @brief pixel format
	*/
	enum EGuiPixelFormat
	{
		/// 2 byte pixel format, 1 byte luminance, 1 byte alpha
		GUI_PF_LA_16 = 0,

		/// 4 byte pixel format
		GUI_PF_RGBA_32,

		/// 4 byte pixel format
		GUI_PF_ARGB_32,
	};

}//namespace guiex

#endif //__GUI_TYPES_20060322_H__

