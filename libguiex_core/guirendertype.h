/** 
* @file guirendertype.h
* @brief 
* @author ken
* @date 2011-01-14
*/


#ifndef __GUI_RENDERTYPE_20110114_H__
#define __GUI_RENDERTYPE_20110114_H__

//============================================================================//
// include
//============================================================================// 
#include "guitypes.h"
#include "guienumtype.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{

	struct SGUIBlendFunc
	{
		//! source blend function
		EBlendFunc src;
		//! destination blend function
		EBlendFunc dst;

		SGUIBlendFunc()
			:src( eBlendFunc_ONE )
			,dst( eBlendFunc_SRC_ALPHA )
		{
		}
	} ;


	struct SR_V2F
	{
		real x;
		real y;
	};

	struct SR_V3F
	{
		real x;
		real y;
		real z;
	};

	struct SR_T2F
	{
		real u;
		real v;
	};

	struct SR_C4F 
	{
		real r;
		real g;
		real b;
		real a;
	};


	struct SR_V2F_C4F_T2F
	{
		SR_V2F vertices;
		SR_C4F colors;
		SR_T2F texCoords;
	};

	struct SR_V2F_C4F_T2F_Quad
	{
		//! bottom left
		SR_V2F_C4F_T2F	bl;
		//! bottom right
		SR_V2F_C4F_T2F	br;
		//! top left
		SR_V2F_C4F_T2F	tl;
		//! top right
		SR_V2F_C4F_T2F	tr;
	};


	extern void ConvGUIColor_2_C4f( const class CGUIColor& rSrc, SR_C4F& rDst );
}

#endif //__GUI_RENDERTYPE_20110114_H__
