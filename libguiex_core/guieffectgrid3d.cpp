/** 
 * @file guieffectgrid3d.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-15
 */


//============================================================================//
// include
//============================================================================//
#include "guieffectgrid3d.h"
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
	CGUIEffectGrid3D::CGUIEffectGrid3D( )
	{

	}
	//------------------------------------------------------------------------------
	CGUIEffectGrid3D::~CGUIEffectGrid3D()
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUIEffectGrid3D::Initialize( uint32 uTextureWidth, uint32 uTextureHeight )
	{
		if( CGUISceneCapture::Initialize( uTextureWidth, uTextureHeight ) != 0 )
		{
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIEffectGrid3D::Release( )
	{

		CGUISceneCapture::Release();
	}
	//------------------------------------------------------------------------------
	void CGUIEffectGrid3D::ProcessCaptureTexture( IGUIInterfaceRender* pRender )
	{
		CGUISceneCapture::ProcessCaptureTexture( pRender );

	}
	//------------------------------------------------------------------------------
}//namespace guiex
