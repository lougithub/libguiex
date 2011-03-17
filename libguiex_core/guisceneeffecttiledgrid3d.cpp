/** 
 * @file guisceneeffecttiledgrid3d.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-16
 */


//============================================================================//
// include
//============================================================================//
#include "guisceneeffecttiledgrid3d.h"
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
	CGUISceneEffectTiledGrid3D::CGUISceneEffectTiledGrid3D( const CGUIIntSize& rSceneSize, const CGUIIntSize& rGridSize )
		:CGUISceneEffectGridBase( rSceneSize, rGridSize )
	{

	}
	//------------------------------------------------------------------------------
	CGUISceneEffectTiledGrid3D::~CGUISceneEffectTiledGrid3D()
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUISceneEffectTiledGrid3D::Initialize( )
	{
		if( CGUISceneEffectGridBase::Initialize( ) != 0 )
		{
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffectTiledGrid3D::Release( )
	{

		CGUISceneEffectGridBase::Release();
	}
	//------------------------------------------------------------------------------
	void CGUISceneEffectTiledGrid3D::ProcessCaptureTexture( IGUIInterfaceRender* pRender )
	{
		CGUISceneEffectGridBase::ProcessCaptureTexture( pRender );

	}
	//------------------------------------------------------------------------------
}//namespace guiex
