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
		eTextAlignment_Horz_Center = 0,
		eTextAlignment_Horz_Left,
		eTextAlignment_Horz_Right,
	};
	enum ETextAlignmentVert
	{
		eTextAlignment_Vert_Center = 0,
		eTextAlignment_Vert_Up,
		eTextAlignment_Vert_Down,
	};

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
		GUI_PF_UNKNOEWN = 0,

		/// 2 byte pixel format, 1 byte luminance, 1 byte alpha
		GUI_PF_LUMINANCE_ALPHA_16,
		
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

	enum ETMXOrientation
	{
		eTMXOrientationOrtho = 0,
		eTMXOrientationHex,
		eTMXOrientationIso,
	};

	enum EBlendFunc
	{
		eBlendFunc_ZERO = 0,
		eBlendFunc_ONE,
		eBlendFunc_SRC_COLOR,
		eBlendFunc_DST_COLOR,
		eBlendFunc_ONE_MINUS_SRC_COLOR,
		eBlendFunc_ONE_MINUS_DST_COLOR,
		eBlendFunc_SRC_ALPHA,
		eBlendFunc_ONE_MINUS_SRC_ALPHA,
		eBlendFunc_DST_ALPHA,
		eBlendFunc_ONE_MINUS_DST_ALPHA,
		eBlendFunc_SRC_ALPHA_SATURATE,
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

	enum EScreenOrientation
	{
		eScreenOrientation_Portrait = 0,
		eScreenOrientation_PortraitUpsideDown,
		eScreenOrientation_LandscapeLeft,
		eScreenOrientation_LandscapeRight,

		//eScreenOrientation_FaceUp,
		//eScreenOrientation_FaceDown,
	};

	//enum EBufferMode
	//{
	//	eBufferMode_Back = 0,
	//	eBufferMode_Front,

	//};

}//namespace guiex

#endif //__GUI_ENUMTYPES_20101122_H__

