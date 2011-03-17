/** 
* @file guiasgrid3d.cpp
* @brief
* @author ken
* @date 2011-03-16
*/

//============================================================================//
// include
//============================================================================// 
#include "guiasgrid3d.h"
#include "guisceneeffectgrid3d.h"
#include "guiexception.h"

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//*****************************************************************************
	//	CGUIAsGrid3D
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAsGrid3D::CGUIAsGrid3D( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsGridBase( rAsType,rAsName, rSceneName )
	{

	}
	//------------------------------------------------------------------------------
	CGUIAsGrid3D::~CGUIAsGrid3D( )
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUIAsGrid3D::ProcessProperty( const CGUIProperty& rProperty )
	{
		int32 ret = CGUIAsGridBase::ProcessProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsGrid3D::GenerateProperty( CGUIProperty& rProperty )
	{
		int32 ret = CGUIAsGridBase::GenerateProperty( rProperty );
		if( ret != 0 )
		{
			return ret;
		}



		return 0;
	}
	//------------------------------------------------------------------------------
	CGUISceneEffectGridBase* CGUIAsGrid3D::GenerateGrid( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize )
	{
		CGUISceneEffectGridBase* pGrid = new CGUISceneEffectGrid3D( rSceneSize, rGridSize );
		pGrid->RefRetain();
		if( 0 != pGrid->Initialize() )
		{
			pGrid->RefRelease();
			throw CGUIException( "[CGUIAsGrid3D::GenerateGrid]: failed to init effect grid3d!" );
			return NULL;
		}
		return pGrid;
	}
	//------------------------------------------------------------------------------
	CGUISceneEffectGrid3D* CGUIAsGrid3D::GetGrid3D()
	{
		GUI_ASSERT( m_pEffectGrid, "[CGUIAsGrid3D::GetVertex]: no grid." );
		return static_cast<CGUISceneEffectGrid3D*>( m_pEffectGrid );
	}
	//------------------------------------------------------------------------------
	/** 
	 * returns the vertex than belongs to certain position in the grid 
	 */
	const SR_V3F& CGUIAsGrid3D::GetVertex( const CGUIIntSize& rPos )
	{
		return GetGrid3D()->GetVertex( rPos );
	}
	//------------------------------------------------------------------------------
	/** 
	 * returns the non-transformed vertex than belongs to certain position in the grid 
	 */
	const SR_V3F& CGUIAsGrid3D::GetOriginalVertex( const CGUIIntSize& rPos )
	{
		return GetGrid3D()->GetOriginalVertex( rPos );
	}
	//------------------------------------------------------------------------------
	/**
	 * sets a new vertex to a certain position of the grid 
	 */
	void CGUIAsGrid3D::SetVertex( const CGUIIntSize& rPos, const SR_V3F& rVertex )
	{
		GetGrid3D()->SetVertex( rPos, rVertex );
	}
	//------------------------------------------------------------------------------
}

