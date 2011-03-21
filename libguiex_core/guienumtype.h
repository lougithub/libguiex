/** 
* @file guienumtypes.h
* @brief types used within this system
* @author ken
* @date 2010-11-22
*/

#ifndef __GUI_ENUMTYPES_20101122_H__
#define __GUI_ENUMTYPES_20101122_H__


//============================================================================//
// include
//============================================================================//
#include "guitypes.h"

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
	* @brief image orientation
	*/
	enum EImageOrientation
	{
		eImageOrientation_Normal,		///!< none operation
		eImageOrientation_90CW,			///!< rotate image for 90 CW
		eImageOrientation_90CCW,		///!< rotate image for 90 CCW
		eImageOrientation_FlipHorizon,	///!< flip image horizon
		eImageOrientation_FlipVertical,	///!< flip image vertical
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
		GUI_PF_LUMINANCE_ALPHA_16 = 0,
		
		//1 byte pixel format
		GUI_PF_LUMINANCE_8,

		/// 3 byte pixel format
		GUI_PF_RGB_24,

		/// 4 byte pixel format
		GUI_PF_RGBA_32,

		/// 4 byte pixel format
		GUI_PF_ARGB_32,
	};

	enum EInterpolationType
	{
		eInterpolationType_Linear = 0,
		eInterpolationType_EaseIn,
		eInterpolationType_EaseInOut,
	};


	enum EParticle2DSystemMode
	{
		eParticle2DSystemMode_Gravity = 0,
		eParticle2DSystemMode_Radius,
	};

	enum EBlendFunc
	{
		eBlendFunc_ZERO = 0,
		eBlendFunc_ONE,
		eBlendFunc_SRC_COLOR,
		eBlendFunc_ONE_MINUS_SRC_COLOR,
		eBlendFunc_SRC_ALPHA,
		eBlendFunc_ONE_MINUS_SRC_ALPHA,
		eBlendFunc_DST_ALPHA,
		eBlendFunc_ONE_MINUS_DST_ALPHA,
	};

	enum ERenderBuffer
	{
		eRenderBuffer_COLOR_BIT = 0x00000001,
		eRenderBuffer_DEPTH_BIT = 0x00000002,
		eRenderBuffer_STENCIL_BIT = 0x00000004,
	};

	enum EMatrixMode
	{
		eMatrixMode_PROJECTION = 0,
		eMatrixMode_MODELVIEW,
	};

}//namespace guiex

#endif //__GUI_ENUMTYPES_20101122_H__

