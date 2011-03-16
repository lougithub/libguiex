/** 
 * @file guieffecttiledgrid3d.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-16
 */


//============================================================================//
// include
//============================================================================//
#include "guieffecttiledgrid3d.h"
#include "guiexception.h"
#include "guiinterfacerender.h"
#include "guitexture.h"
#include "guitexturemanager.h"


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIEffectTiledGrid3D::CGUIEffectTiledGrid3D( )
	{

	}
	//------------------------------------------------------------------------------
	CGUIEffectTiledGrid3D::~CGUIEffectTiledGrid3D()
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUIEffectTiledGrid3D::Initialize( uint32 uTextureWidth, uint32 uTextureHeight )
	{
		if( CGUISceneCapture::Initialize( uTextureWidth, uTextureHeight ) != 0 )
		{
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIEffectTiledGrid3D::Release( )
	{

		CGUISceneCapture::Release();
	}
	//------------------------------------------------------------------------------
	void CGUIEffectTiledGrid3D::ProcessCaptureTexture( IGUIInterfaceRender* pRender )
	{
		CGUISceneCapture::ProcessCaptureTexture( pRender );

	}
	//------------------------------------------------------------------------------
}//namespace guiex
