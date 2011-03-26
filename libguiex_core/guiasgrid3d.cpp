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
		GUI_ASSERT( m_pEffectGrid, "[CGUIAsGrid3D::GetGrid3D]: no grid." );
		return static_cast<CGUISceneEffectGrid3D*>( m_pEffectGrid );
	}
	//------------------------------------------------------------------------------
	/** 
	 * returns the vertex than belongs to certain position in the grid 
	 */
	const SR_V3F& CGUIAsGrid3D::GetVertex( uint32 uX, uint32 uY )
	{
		return GetGrid3D()->GetVertex( uX, uY );
	}
	//------------------------------------------------------------------------------
	/** 
	 * returns the non-transformed vertex than belongs to certain position in the grid 
	 */
	const SR_V3F& CGUIAsGrid3D::GetOriginalVertex( uint32 uX, uint32 uY )
	{
		return GetGrid3D()->GetOriginalVertex( uX, uY );
	}
	//------------------------------------------------------------------------------
	/**
	 * sets a new vertex to a certain position of the grid 
	 */
	void CGUIAsGrid3D::SetVertex( uint32 uX, uint32 uY, const SR_V3F& rVertex )
	{
		GetGrid3D()->SetVertex( uX, uY, rVertex );
	}
	//------------------------------------------------------------------------------




	//*****************************************************************************
	//	CGUIAsPageTurn3D
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

		real fPercent = GetPercent();
		real tt = GUIMax( 0.0f, fPercent - 0.25f );
		real deltaAy = ( tt * tt * 500);
		real ay = -100 - deltaAy;

		real deltaTheta = - CGUIMath::GUI_HALF_PI * sqrtf( fPercent) ;
		real theta = CGUIMath::GUI_HALF_PI +deltaTheta;

		real sinTheta = sinf(theta);
		real cosTheta = cosf(theta);

		for( uint32 i = 0; i <= m_aGridSize.GetWidth(); i++ )
		{
			for( uint32 j = 0; j <= m_aGridSize.GetHeight(); j++ )
			{
				// Get original vertex
				SR_V3F p = GetOriginalVertex( i,j);

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
				SetVertex( i,j, p );
			}
		}
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsWaves3D
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWaves3D::CGUIAsWaves3D( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsGrid3D( "CGUIAsWaves3D", rAsName, rSceneName )
		,m_nWaves( 1 )
		,m_fAmplitude( 3.0f )
		,m_fAmplitudeRate( 1.0f )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsWaves3D::SetWavesInfo( int32 nWaves, real fAmplitude)
	{
		m_nWaves = nWaves;
		m_fAmplitude = fAmplitude;
		m_fAmplitudeRate = 1.0f;
	}
	//------------------------------------------------------------------------------
	void CGUIAsWaves3D::OnUpdate( )
	{
		CGUIAsGrid3D::OnUpdate( );
		real fPercent = GetPercent();

		for( uint32 i = 0; i < (m_aGridSize.GetWidth()+1); i++ )
		{
			for( uint32 j = 0; j < (m_aGridSize.GetHeight()+1); j++ )
			{
				SR_V3F v = GetOriginalVertex( i,j);
				v.z += (sinf(CGUIMath::GUI_PI * fPercent * m_nWaves*2 + (v.y+v.x) * .01f) * m_fAmplitude * m_fAmplitudeRate);
				SetVertex( i,j, v );
			}
		}
	}
	//------------------------------------------------------------------------------


	


	//*****************************************************************************
	//	CGUIAsShaky3D
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsShaky3D::CGUIAsShaky3D( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsGrid3D( "CGUIAsShaky3D", rAsName, rSceneName )
		,m_bShakeZ( false )
		,m_nRandrange( 4 )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsShaky3D::SetShakyInfo( int32 nRange, bool bShakeZ )
	{
		m_nRandrange = nRange;
		m_bShakeZ = bShakeZ;
	}
	//------------------------------------------------------------------------------
	void CGUIAsShaky3D::OnUpdate( )
	{
		CGUIAsGrid3D::OnUpdate( );

		for( uint32 i = 0; i < (m_aGridSize.GetWidth()+1); i++ )
		{
			for( uint32 j = 0; j < (m_aGridSize.GetHeight()+1); j++ )
			{
				SR_V3F v = GetOriginalVertex( i, j );
				v.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				v.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				if( m_bShakeZ )
				{
					v.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				}

				SetVertex( i,j, v);
			}
		}

	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsFlipX3D
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsFlipX3D::CGUIAsFlipX3D( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsGrid3D( rAsType, rAsName, rSceneName )
	{

	}
	//------------------------------------------------------------------------------
	CGUIAsFlipX3D::CGUIAsFlipX3D( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsGrid3D( "CGUIAsFlipX3D", rAsName, rSceneName )
	{

	}
	//------------------------------------------------------------------------------
	int32 CGUIAsFlipX3D::OnInitGrid()
	{
		if( 0 != CGUIAsGrid3D::OnInitGrid() )
		{
			return -1;
		}
		
		if( m_aGridSize.GetHeight() !=1 || m_aGridSize.GetWidth() != 1 )
		{
			throw CGUIException("CGUIAsFlipX3D::OnInitGrid: grid size should be (1,1)");
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAsFlipX3D::OnUpdate( )
	{
		CGUIAsGrid3D::OnUpdate( );

		real angle = CGUIMath::GUI_PI * GetPercent(); // 180 degrees
		real mz = sinf( angle );
		angle = angle / 2.0f;     // x calculates degrees from 0 to 90
		real mx = cosf( angle );

		SR_V3F v0 = GetOriginalVertex( 1,1);
		SR_V3F v1 = GetOriginalVertex( 0,0);

		real x0 = v0.x;
		real x1 = v1.x;
		real x;
		CGUIIntSize	a, b, c, d;

		if ( x0 > x1 )
		{
			// Normal Grid
			a.SetValue(0,0);
			b.SetValue(0,1);
			c.SetValue(1,0);
			d.SetValue(1,1);
			x = x0;
		}
		else
		{
			// Reversed Grid
			c.SetValue(0,0);
			d.SetValue(0,1);
			a.SetValue(1,0);
			b.SetValue(1,1);
			x = x1;
		}

		SR_V3F diff;
		diff.x = ( x - x * mx );
		diff.z = fabsf( floorf( (x * mz) / 4.0f ) );

		SR_V3F v;

		// bottom-left
		v = GetOriginalVertex( a.GetWidth(), a.GetHeight());
		v.x = diff.x;
		v.z += diff.z;
		SetVertex(a.GetWidth(), a.GetHeight(), v);

		// upper-left
		v = GetOriginalVertex( b.GetWidth(), b.GetHeight());
		v.x = diff.x;
		v.z += diff.z;
		SetVertex(b.GetWidth(), b.GetHeight(), v);

		// bottom-right
		v = GetOriginalVertex( c.GetWidth(), c.GetHeight());
		v.x -= diff.x;
		v.z -= diff.z;
		SetVertex(c.GetWidth(), c.GetHeight(), v);

		// upper-right
		v = GetOriginalVertex( d.GetWidth(), d.GetHeight());
		v.x -= diff.x;
		v.z -= diff.z;
		SetVertex(d.GetWidth(), d.GetHeight(), v);
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsFlipY3D
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsFlipY3D::CGUIAsFlipY3D( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsFlipX3D( "CGUIAsFlipY3D", rAsName, rSceneName )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsFlipY3D::OnUpdate( )
	{
		CGUIAsFlipX3D::OnUpdate( );

		real angle = CGUIMath::GUI_PI * GetPercent(); // 180 degrees
		real mz = sinf( angle );
		angle = angle / 2.0f;     // x calculates degrees from 0 to 90
		real my = cosf( angle );

		SR_V3F v0 = GetOriginalVertex( 1,1);
		SR_V3F v1 = GetOriginalVertex( 0,0);

		real y0 = v0.y;
		real y1 = v1.y;
		real y;
		CGUIIntSize	a, b, c, d;

		if ( y0 > y1 )
		{
			// Normal Grid
			a.SetValue(0,0);
			b.SetValue(0,1);
			c.SetValue(1,0);
			d.SetValue(1,1);
			y = y0;
		}
		else
		{
			// Reversed Grid
			c.SetValue(0,0);
			d.SetValue(0,1);
			a.SetValue(1,0);
			b.SetValue(1,1);
			y = y1;
		}

		SR_V3F diff;
		diff.y = y - y * my;
		diff.z = fabsf( floorf( (y * mz) / 4.0f ) );

		SR_V3F v;

		// bottom-left
		v = GetOriginalVertex( a.GetWidth(), a.GetHeight());
		v.y = diff.y;
		v.z += diff.z;
		SetVertex(a.GetWidth(), a.GetHeight(), v);

		// upper-left
		v = GetOriginalVertex( b.GetWidth(), b.GetHeight());
		v.y -= diff.y;
		v.z -= diff.z;
		SetVertex(b.GetWidth(), b.GetHeight(), v);

		// bottom-right
		v = GetOriginalVertex( c.GetWidth(), c.GetHeight());
		v.y = diff.y;
		v.z += diff.z;
		SetVertex(c.GetWidth(), c.GetHeight(), v);

		// upper-right
		v = GetOriginalVertex( d.GetWidth(), d.GetHeight());
		v.y -= diff.y;
		v.z -= diff.z;
		SetVertex(d.GetWidth(), d.GetHeight(), v);
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsLens3D
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsLens3D::CGUIAsLens3D( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsGrid3D( "CGUIAsLens3D", rAsName, rSceneName )
		,m_fRadius( 10.0f )
		,m_fLensEffect( 0.7f )
		,m_bDirty( true )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsLens3D::SetLens3DInfo( const CGUIVector2& pos, real radius )
	{
		SetPosition( pos );
		m_fRadius = radius;
		m_fLensEffect = 0.7f;
	}
	//------------------------------------------------------------------------------
	void CGUIAsLens3D::SetPosition( const CGUIVector2& pos )
	{
		if( pos != m_aPosition )
		{
			m_aPosition = pos;
			m_aPositionInPixels.x = pos.x;
			m_aPositionInPixels.y = pos.y;

			m_bDirty = true;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAsLens3D::OnUpdate( )
	{
		CGUIAsGrid3D::OnUpdate( );

		if ( !m_bDirty )
		{
			return;
		}
		m_bDirty = false;
		for( uint32 i = 0; i < (m_aGridSize.GetWidth()+1); i++ )
		{
			for( uint32 j = 0; j < (m_aGridSize.GetHeight()+1); j++ )
			{
				SR_V3F v = GetOriginalVertex( i,j );
				CGUIVector2 vect = m_aPositionInPixels - CGUIVector2(v.x,v.y);
				real r = vect.Length();

				if ( r < m_fRadius )
				{
					r = m_fRadius - r;
					real pre_log = r / m_fRadius;
					if ( pre_log == 0 ) pre_log = 0.001f;
					float l = logf(pre_log) * m_fLensEffect;
					float new_r = expf( l ) * m_fRadius;

					if ( vect.Length() > 0 )
					{
						vect.Normalise();
						CGUIVector2 new_vect = vect * new_r;
						v.z += new_vect.Length() * m_fLensEffect;
					}
				}

				SetVertex(i,j, v);
			}
		}
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsRipple3D
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsRipple3D::CGUIAsRipple3D( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsGrid3D( "CGUIAsRipple3D", rAsName, rSceneName )
		,m_fRadius(10.0f)
		,m_nWaves(1)
		,m_fAmplitude( 3.0f)
		,m_fAmplitudeRate(1.0f)
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsRipple3D::SetRipple3DInfo( const CGUIVector2& pos, real radius, uint32 waves, real amplitude )
	{
		SetPosition( pos );
		m_fRadius = radius;
		m_nWaves = waves;
		m_fAmplitude = amplitude;
		m_fAmplitudeRate = 1.0f;
	}
	//------------------------------------------------------------------------------
	void CGUIAsRipple3D::SetPosition(const CGUIVector2& pos)
	{
		m_aPosition = pos;
		m_aPositionInPixels.x = pos.x;
		m_aPositionInPixels.y = pos.y;
	}
	//------------------------------------------------------------------------------
	void CGUIAsRipple3D::OnUpdate( )
	{
		CGUIAsGrid3D::OnUpdate( );

		real fPercent = GetPercent();
		for( uint32 i = 0; i < (m_aGridSize.GetWidth()+1); i++ )
		{
			for( uint32 j = 0; j < (m_aGridSize.GetHeight()+1); j++ )
			{
				SR_V3F v = GetOriginalVertex( i,j );
				CGUIVector2 vect = m_aPositionInPixels - CGUIVector2(v.x,v.y);
				real r = vect.Length();

				if ( r < m_fRadius )
				{
					r = m_fRadius - r;
					real rate = powf( r / m_fRadius, 2);
					v.z += (sinf( fPercent*CGUIMath::GUI_PI*m_nWaves*2 + r * 0.1f) * m_fAmplitude * m_fAmplitudeRate * rate );
				}

				SetVertex(i,j, v);
			}
		}
	}
	//------------------------------------------------------------------------------

	//*****************************************************************************
	//	CGUIAsLiquid
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsLiquid::CGUIAsLiquid( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsGrid3D( "CGUIAsLiquid", rAsName, rSceneName )
		,m_nWaves(1)
		,m_fAmplitude( 3.0f)
		,m_fAmplitudeRate(1.0f)
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsLiquid::SetLiquidInfo( uint32 waves, real amplitude )
	{
		m_nWaves = waves;
		m_fAmplitude = amplitude;
		m_fAmplitudeRate = 1.0f;
	}
	//------------------------------------------------------------------------------
	void CGUIAsLiquid::OnUpdate( )
	{
		CGUIAsGrid3D::OnUpdate( );

		real fPercent = GetPercent();
		for( uint32 i = 0; i < (m_aGridSize.GetWidth()+1); i++ )
		{
			for( uint32 j = 0; j < (m_aGridSize.GetHeight()+1); j++ )
			{
				SR_V3F v = GetOriginalVertex( i,j );

				v.x = (v.x + (sinf(fPercent*CGUIMath::GUI_PI*m_nWaves*2 + v.x * .01f) * m_fAmplitude * m_fAmplitudeRate));
				v.y = (v.y + (sinf(fPercent*CGUIMath::GUI_PI*m_nWaves*2 + v.y * .01f) * m_fAmplitude * m_fAmplitudeRate));

				SetVertex(i,j, v);
			}
		}
	}
	//------------------------------------------------------------------------------




	//*****************************************************************************
	//	CGUIAsWaves
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWaves::CGUIAsWaves( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsGrid3D( "CGUIAsWaves", rAsName, rSceneName )
		,m_nWaves(1)
		,m_fAmplitude( 3.0f)
		,m_fAmplitudeRate(1.0f)
		,m_bVertical(true)
		,m_bHorizontal(true)
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsWaves::SetWavesInfo( uint32 waves, real amplitude, bool horizontal, bool vertical )
	{
		m_nWaves = waves;
		m_fAmplitude = amplitude;
		m_fAmplitudeRate = 1.0f;
		m_bVertical = vertical;
		m_bHorizontal = horizontal;
	}
	//------------------------------------------------------------------------------
	void CGUIAsWaves::OnUpdate( )
	{
		CGUIAsGrid3D::OnUpdate( );

		real fPercent = GetPercent();
		for( uint32 i = 0; i < (m_aGridSize.GetWidth()+1); i++ )
		{
			for( uint32 j = 0; j < (m_aGridSize.GetHeight()+1); j++ )
			{
				SR_V3F v = GetOriginalVertex( i,j );

				if ( m_bVertical )
				{
					v.x = (v.x + (sinf(fPercent*CGUIMath::GUI_PI*m_nWaves*2 + v.y * .01f) * m_fAmplitude * m_fAmplitudeRate));
				}

				if ( m_bHorizontal )
				{
					v.y = (v.y + (sinf(fPercent*CGUIMath::GUI_PI*m_nWaves*2 + v.x * .01f) * m_fAmplitude * m_fAmplitudeRate));
				}

				SetVertex(i,j, v);
			}
		}
	}
	//------------------------------------------------------------------------------


	
	//*****************************************************************************
	//	CGUIAsTwirl
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsTwirl::CGUIAsTwirl( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsGrid3D( "CGUIAsTwirl", rAsName, rSceneName )
		,m_nTwirls(1)
		,m_fAmplitude( 3.0f)
		,m_fAmplitudeRate(1.0f)
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsTwirl::SetTwirlInfo( const CGUIVector2& pos, uint32 twirls, real amplitude )
	{
		SetPosition( pos );
		m_nTwirls = twirls;
		m_fAmplitude = amplitude;
		m_fAmplitudeRate = 1.0f;

	}
	//------------------------------------------------------------------------------
	void CGUIAsTwirl::SetPosition( const CGUIVector2& pos )
	{
		m_aPosition = pos;
		m_aPositionInPixels.x = pos.x;
		m_aPositionInPixels.y = pos.y;
	}
	//------------------------------------------------------------------------------
	void CGUIAsTwirl::OnUpdate( )
	{
		CGUIAsGrid3D::OnUpdate( );

		real fPercent = GetPercent();
		CGUIVector2	c = m_aPositionInPixels;
		for( uint32 i = 0; i < (m_aGridSize.GetWidth()+1); i++ )
		{
			for( uint32 j = 0; j < (m_aGridSize.GetHeight()+1); j++ )
			{
				SR_V3F v = GetOriginalVertex( i,j );

				CGUIVector2	avg(i-(m_aGridSize.GetWidth()/2.0f), j-(m_aGridSize.GetHeight()/2.0f));
				real r = avg.Length();

				real amp = 0.1f * m_fAmplitude * m_fAmplitudeRate;
				real a = r * cosf( CGUIMath::GUI_PI/2.0f + fPercent * CGUIMath::GUI_PI * m_nTwirls * 2 ) * amp;

				real cosA = cosf(a);
				real sinA = sinf(a);

				CGUIVector2	d( sinA * (v.y-c.y) + cosA * (v.x-c.x), cosA * (v.y-c.y) - sinA * (v.x-c.x) );

				v.x = c.x + d.x;
				v.y = c.y + d.y;

				SetVertex(i,j, v);
			}
		}
	}
	//------------------------------------------------------------------------------
} //namespace guiex

