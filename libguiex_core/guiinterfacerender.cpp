/** 
* @file guiinterfacerender.cpp
* @brief interface for render module
* @author ken
* @date 2006-07-04
*/

//============================================================================//
// include
//============================================================================// 
#include "guiinterfacerender.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceRender::StaticGetModuleType()
	{ 
		return "IGUIRender";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceRender::IGUIInterfaceRender( const char* szModuleName )
		:IGUIInterface( StaticGetModuleType(), szModuleName )
	{

	}
	//------------------------------------------------------------------------------ 
	IGUIInterfaceRender::~IGUIInterfaceRender()
	{

	}
	//------------------------------------------------------------------------------
	void IGUIInterfaceRender::DrawRect(
		const CGUIRect& rDestRect, 
		real fLineWidth,
		real z,
		const CGUIColor& rColor )
	{
		DrawRect( rDestRect, fLineWidth, z, rColor,rColor,rColor,rColor );
	}
	//------------------------------------------------------------------------------
	void IGUIInterfaceRender::DrawTile(	
		const CGUIRect& rDestRect, 
		real z, 
		const CGUITexture* pTex, 
		const CGUIRect& rTextureRect,
		EImageOrientation eImageOrientation,
		const CGUIColor& rColor
		)
	{
		DrawTile( rDestRect, z, pTex, rTextureRect, eImageOrientation, rColor,rColor,rColor,rColor );
	}
	//------------------------------------------------------------------------------
}//namespace guiex


