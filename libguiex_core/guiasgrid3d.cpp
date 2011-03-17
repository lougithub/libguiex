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
#include "guimath.h"

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




	//*****************************************************************************
	//	CGUIAsGrid3D
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAsPageTurn3D::CGUIAsPageTurn3D( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsGrid3D( "CGUIAsPageTurn3D", rAsName, rSceneName )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsPageTurn3D::OnUpdate( )
	{
		CGUIAsGrid3D::OnUpdate( );

		if( !IsGridInit() )
		{
			return;
		}

		real fPercent = GetElapsedTime() / GetTotalTime();
		real tt = GUIMax( 0.0f, fPercent - 0.25f );
		real deltaAy = ( tt * tt * 500);
		real ay = -100 - deltaAy;

		real deltaTheta = - CGUIMath::GUI_HALF_PI * sqrtf( fPercent) ;
		real theta = CGUIMath::GUI_HALF_PI +deltaTheta;

		real sinTheta = sinf(theta);
		real cosTheta = cosf(theta);

		for( uint32 i = 0; i <= m_aGridSize.m_uWidth; i++ )
		{
			for( uint32 j = 0; j <= m_aGridSize.m_uHeight; j++ )
			{
				// Get original vertex
				SR_V3F p = GetOriginalVertex( CGUIIntSize(i,j));

				real R = sqrtf(p.x*p.x + (p.y - ay) * (p.y - ay));
				real r = R * sinTheta;
				real alpha = asinf( p.x / R );
				real beta = alpha / sinTheta;
				real cosBeta = cosf( beta );

				// If beta > PI then we've wrapped around the cone
				// Reduce the radius to stop these points interfering with others
				if( beta <= CGUIMath::GUI_PI)
				{
					p.x = ( r * sinf(beta));
				}
				else
				{
					// Force X = 0 to stop wrapped
					// points
					p.x = 0;
				}

				p.y = ( R + ay - ( r*(1 - cosBeta)*sinTheta));

				// We scale z here to avoid the animation being
				// too much bigger than the screen due to perspectve transform
				p.z = (r * ( 1 - cosBeta ) * cosTheta) / 7; // "100" didn't work for

				// Stop z coord from dropping beneath underlying page in a transition
				// issue #751				
				if( p.z<0.5f )
				{
					p.z = 0.5f;
				}

				// Set new coords
				SetVertex( CGUIIntSize(i,j), p );
			}
		}
	}
	//------------------------------------------------------------------------------

} //namespace guiex

