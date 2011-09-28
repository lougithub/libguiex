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
#pragma pack( push, 1 )
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


	struct SVertexFormat_V2F
	{
		real x;
		real y;
	};

	struct SVertexFormat_V3F
	{
		real x;
		real y;
		real z;
	};

	struct SVertexFormat_T2F
	{
		real u;
		real v;
	};

	struct SVertexFormat_C4F 
	{
		real r;
		real g;
		real b;
		real a;
	};

	union SVertexFormat_C4UB
	{
		struct
		{
			uint8 a,b,g,r;
		};
		uint32 abgr; 
	};

	struct SVertexFormat_V3F_T2F_C4UB
	{
		SVertexFormat_V3F vertices;
		SVertexFormat_T2F texCoords;
		SVertexFormat_C4UB color;
	};

	struct SVertexFormat_T2F_C4UB
	{
		SVertexFormat_T2F texCoords;
		SVertexFormat_C4UB color;
	};

	struct SVertexFormat_V3F_C4UB
	{
		SVertexFormat_V3F vertices;
		SVertexFormat_C4UB color;
	};

	struct SVertexFormat_V3F_T2F
	{
		SVertexFormat_V3F vertices;
		SVertexFormat_T2F texCoords;
	};

	struct SVertexFormat_V2F_T2F_C4UB
	{
		SVertexFormat_V2F vertices;
		SVertexFormat_T2F texCoords;
		SVertexFormat_C4UB colors;
	};

	enum EQuadPosition
	{
		eQuad_TopLeft = 0,
		eQuad_BottomLeft,
		eQuad_TopRight,
		eQuad_BottomRight,
	};

	struct SVertexFormat_V2F_T2F_C4UB_Quad
	{
		SVertexFormat_V2F_T2F_C4UB vertices[4]; //tl, bl, tr, br
	};

	struct SVertexFormat_V3F_Quad
	{
		SVertexFormat_V3F vertices[4]; //tl, bl, tr, br
	};

	struct SVertexFormat_V3F_T2F_Quad
	{
		SVertexFormat_V3F_T2F vertices[4]; //tl, bl, tr, br
	};
}
#pragma pack( pop )


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	
}


#endif //__GUI_RENDERTYPE_20110114_H__
