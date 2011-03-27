/** 
* @file guiastiledgrid3d.cpp
* @brief
* @author ken
* @date 2011-03-16
*/

//============================================================================//
// include
//============================================================================// 
#include "guiastiledgrid3d.h"
#include "guisceneeffecttiledgrid3d.h"
#include "guiexception.h"
#include "guimath.h"

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//*****************************************************************************
	//	CGUIAsTiledGrid3D
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAsTiledGrid3D::CGUIAsTiledGrid3D( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsGridBase( rAsType, rAsName, rSceneName )
	{

	}
	//------------------------------------------------------------------------------
	CGUIAsTiledGrid3D::~CGUIAsTiledGrid3D( )
	{

	}
	//------------------------------------------------------------------------------
	CGUISceneEffectGridBase* CGUIAsTiledGrid3D::GenerateGrid( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize )
	{
		CGUISceneEffectGridBase* pGrid = new CGUISceneEffectTiledGrid3D( rSceneSize, rGridSize );
		pGrid->RefRetain();
		if( 0 != pGrid->Initialize() )
		{
			pGrid->RefRelease();
			throw CGUIException( "[CGUIAsTiledGrid3D::GenerateGrid]: failed to init effect grid3d!" );
			return NULL;
		}
		return pGrid;
	}
	//------------------------------------------------------------------------------
	CGUISceneEffectTiledGrid3D* CGUIAsTiledGrid3D::GetTiledGrid3D()
	{
		GUI_ASSERT( m_pEffectGrid, "[CGUIAsTiledGrid3D::GetTiledGrid3D]: no grid." );
		return static_cast<CGUISceneEffectTiledGrid3D*>( m_pEffectGrid );
	}
	//------------------------------------------------------------------------------
	/** 
	* returns the vertex than belongs to certain position in the grid 
	*/
	const SR_V3F_Quad& CGUIAsTiledGrid3D::GetTile( uint32 uX, uint32 uY )
	{
		return GetTiledGrid3D()->GetTile( uX, uY );
	}
	//------------------------------------------------------------------------------
	/** 
	* returns the non-transformed vertex than belongs to certain position in the grid 
	*/
	const SR_V3F_Quad& CGUIAsTiledGrid3D::GetOriginalTile( uint32 uX, uint32 uY )
	{
		return GetTiledGrid3D()->GetOriginalTile( uX, uY );
	}
	//------------------------------------------------------------------------------
	/**
	* sets a new vertex to a certain position of the grid 
	*/
	void CGUIAsTiledGrid3D::SetTile( uint32 uX, uint32 uY, const SR_V3F_Quad& rTile )
	{
		GetTiledGrid3D()->SetTile( uX, uY, rTile );
	}
	//------------------------------------------------------------------------------
	void CGUIAsTiledGrid3D::TurnOnTile( uint32 uX, uint32 uY )
	{
		SetTile( uX, uY, GetOriginalTile( uX, uY ));
	}
	//------------------------------------------------------------------------------
	void CGUIAsTiledGrid3D::TurnOffTile( uint32 uX, uint32 uY )
	{
		SR_V3F_Quad coords;
		memset( &coords, 0, sizeof( SR_V3F_Quad ) );
		SetTile( uX, uY, coords );
	}	
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsShakyTiles3D
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAsShakyTiles3D::CGUIAsShakyTiles3D( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsTiledGrid3D( "CGUIAsShakyTiles3D", rAsName, rSceneName )
		,m_nRandrange( 10 )
		,m_bShakeZ( false )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsShakyTiles3D::SetShakyTiles3DInfo( int32 range, bool bShakeZ )
	{
		m_nRandrange = range;
		m_bShakeZ = bShakeZ;
	}
	//------------------------------------------------------------------------------
	void CGUIAsShakyTiles3D::OnUpdate( )
	{
		CGUIAsTiledGrid3D::OnUpdate( );

		for( uint32 i = 0; i < m_aGridSize.GetWidth(); i++ )
		{
			for( uint32 j = 0; j < m_aGridSize.GetHeight(); j++ )
			{
				SR_V3F_Quad coords = GetOriginalTile(i,j);

				// X
				coords.bl.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.br.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tl.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tr.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;

				// Y
				coords.bl.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.br.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tl.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tr.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;

				if( m_bShakeZ ) 
				{
					coords.bl.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.br.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.tl.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.tr.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				}

				SetTile( i, j, coords );
			}
		}
	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsShatteredTiles3D
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsShatteredTiles3D::CGUIAsShatteredTiles3D( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsTiledGrid3D( "CGUIAsShatteredTiles3D", rAsName, rSceneName )
		,m_nRandrange( 10 )
		,m_bShatterZ( false )
		,m_bOnce( false )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIAsShatteredTiles3D::Reset( )
	{
		CGUIAsTiledGrid3D::Reset();

		m_bOnce = false;
	}
	//------------------------------------------------------------------------------
	void CGUIAsShatteredTiles3D::SetShatteredTiles3DInfo( int32 range, bool shatterZ )
	{
		m_nRandrange = range;
		m_bShatterZ = shatterZ;
	}
	//------------------------------------------------------------------------------
	void CGUIAsShatteredTiles3D::OnUpdate( )
	{
		CGUIAsTiledGrid3D::OnUpdate( );

		if( m_bOnce )
		{
			return;
		}
		m_bOnce = true;

		for( uint32 i = 0; i < m_aGridSize.GetWidth(); i++ )
		{
			for( uint32 j = 0; j < m_aGridSize.GetHeight(); j++ )
			{
				SR_V3F_Quad coords = GetOriginalTile(i,j);

				// X
				coords.bl.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.br.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tl.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tr.x += ( rand() % (m_nRandrange*2) ) - m_nRandrange;

				// Y
				coords.bl.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.br.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tl.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				coords.tr.y += ( rand() % (m_nRandrange*2) ) - m_nRandrange;

				if( m_bShatterZ )
				{
					coords.bl.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.br.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;				
					coords.tl.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
					coords.tr.z += ( rand() % (m_nRandrange*2) ) - m_nRandrange;
				}

				SetTile( i, j, coords );
			}
		}
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsShuffleTiles
	//*****************************************************************************

	//------------------------------------------------------------------------------
	CGUIAsShuffleTiles::CGUIAsShuffleTiles( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsTiledGrid3D( "CGUIAsShuffleTiles", rAsName, rSceneName )
		,m_nSeed( 10 )
		,m_nTilesCount( 0 )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsShuffleTiles::Reset( )
	{
		CGUIAsTiledGrid3D::Reset();

		m_vTilesOrder.clear();
		m_vTilesDelta.clear();
	}
	//------------------------------------------------------------------------------
	void CGUIAsShuffleTiles::SetShuffleTilesInfo( int32 seed )
	{
		m_nSeed = seed;
	}
	//------------------------------------------------------------------------------
	CGUIIntSize CGUIAsShuffleTiles::GetDelta( uint32 uX, uint32 uY )
	{
		CGUIIntSize	pos;

		int idx = uX * m_aGridSize.GetHeight() + uY;
		pos.m_uWidth = m_vTilesOrder[idx] / (int)m_aGridSize.GetHeight();
		pos.m_uHeight = m_vTilesOrder[idx] % (int)m_aGridSize.GetHeight();

		return CGUIIntSize(pos.m_uWidth - uX, pos.m_uHeight - uY);
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsShuffleTiles::OnInitGrid( )
	{
		if( 0 != CGUIAsTiledGrid3D::OnInitGrid() )
		{
			return -1;
		}

		if ( m_nSeed != -1 )
		{
			srand( m_nSeed );
		}

		m_nTilesCount = m_aGridSize.GetWidth() * m_aGridSize.GetHeight();
		m_vTilesOrder.resize(m_nTilesCount);

		for( uint32 i = 0; i < m_nTilesCount; i++ )
		{
			m_vTilesOrder[i] = i;
		}

		//shuffle
		for( int32 i = m_nTilesCount - 1; i >= 0; i-- )
		{
			int j = rand() % (i+1);
			std::swap( m_vTilesOrder[i], m_vTilesOrder[j] );
		}

		m_vTilesDelta.resize( m_nTilesCount );
		for( uint32 i = 0; i < m_aGridSize.GetWidth(); i++ )
		{
			for( uint32 j = 0; j < m_aGridSize.GetHeight(); j++ )
			{
				uint32 nIdx = i*m_aGridSize.GetHeight() + j;
				m_vTilesDelta[nIdx] = GetDelta(i,j);
			}
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAsShuffleTiles::OnFiniGrid( )
	{
		m_vTilesOrder.clear();
		m_vTilesDelta.clear();

		CGUIAsTiledGrid3D::OnFiniGrid();
	}
	//------------------------------------------------------------------------------
	void CGUIAsShuffleTiles::OnUpdate( )
	{
		CGUIAsTiledGrid3D::OnUpdate( );

		real fPercent = GetPercent();
		const CGUIVector2& rStep = GetTiledGrid3D()->GetStep();

		for( uint32 i = 0; i < m_aGridSize.GetWidth(); i++ )
		{
			for( uint32 j = 0; j < m_aGridSize.GetHeight(); j++ )
			{
				uint32 nIdx = i*m_aGridSize.GetHeight() + j;
				CGUIIntSize& rDelta = m_vTilesDelta[nIdx];

				CGUIVector2 position( rDelta.GetWidth() * fPercent, rDelta.GetHeight()*fPercent );

				SR_V3F_Quad coords = GetOriginalTile(i,j);

				coords.bl.x += (int32)(position.x * rStep.x);
				coords.bl.y += (int32)(position.y * rStep.y);

				coords.br.x += (int32)(position.x * rStep.x);
				coords.br.y += (int32)(position.y * rStep.y);

				coords.tl.x += (int32)(position.x * rStep.x);
				coords.tl.y += (int32)(position.y * rStep.y);

				coords.tr.x += (int32)(position.x * rStep.x);
				coords.tr.y += (int32)(position.y * rStep.y);

				SetTile( i, j, coords );
			}
		}
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsFadeOutTRTiles
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsFadeOutTRTiles::CGUIAsFadeOutTRTiles( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName)
		:CGUIAsTiledGrid3D( rAsType, rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	CGUIAsFadeOutTRTiles::CGUIAsFadeOutTRTiles( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsTiledGrid3D( "CGUIAsFadeOutTRTiles", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	real CGUIAsFadeOutTRTiles::TestFunc( uint32 uX, uint32 uY, real fPercent )
	{
		CGUIVector2	n( m_aGridSize.GetWidth() * fPercent, m_aGridSize.GetHeight()*fPercent );
		if ( (n.x+n.y) == 0.0f )
		{
			return 1.0f;
		}

		return powf( (uX+uY) / (n.x+n.y), 6 );
	}
	//------------------------------------------------------------------------------
	void CGUIAsFadeOutTRTiles::TransformTile( uint32 uX, uint32 uY, real distance )
	{
		SR_V3F_Quad coords = GetOriginalTile(uX,uY);
		const CGUIVector2& step = GetTiledGrid3D()->GetStep();

		coords.bl.x += (step.x / 2) * (1.0f - distance);
		coords.bl.y -= (step.y / 2) * (1.0f - distance);

		coords.br.x -= (step.x / 2) * (1.0f - distance);
		coords.br.y -= (step.y / 2) * (1.0f - distance);

		coords.tl.x += (step.x / 2) * (1.0f - distance);
		coords.tl.y += (step.y / 2) * (1.0f - distance);

		coords.tr.x -= (step.x / 2) * (1.0f - distance);
		coords.tr.y += (step.y / 2) * (1.0f - distance);

		SetTile( uX, uY, coords );
	}
	//------------------------------------------------------------------------------
	void CGUIAsFadeOutTRTiles::OnUpdate( )
	{
		CGUIAsTiledGrid3D::OnUpdate( );

		real fPercent = GetPercent();
		for( uint32 i = 0; i < m_aGridSize.GetWidth(); i++ )
		{
			for( uint32 j = 0; j < m_aGridSize.GetHeight(); j++ )
			{
				float distance = TestFunc(i,j, fPercent );
				if ( distance == 0 )
				{
					TurnOffTile(i,j);
				}
				else if ( distance < 1 )
				{
					TransformTile(i,j,distance);
				}
				else
				{
					TurnOnTile(i,j);
				}
			}
		}
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsFadeOutBLTiles
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsFadeOutBLTiles::CGUIAsFadeOutBLTiles( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsFadeOutTRTiles( "CGUIAsFadeOutBLTiles", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	real CGUIAsFadeOutBLTiles::TestFunc( uint32 uX, uint32 uY, real fPercent )
	{
		fPercent = 1.0f - fPercent;
		CGUIVector2	n( m_aGridSize.GetWidth() * fPercent, m_aGridSize.GetHeight()*fPercent );
		if ( (uX+uY) == 0 )
		{
			return 1.0f;
		}

		return powf( (n.x+n.y) / (uX+uY), 6 );
	}
	//------------------------------------------------------------------------------




	//*****************************************************************************
	//	CGUIAsFadeOutUpTiles
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsFadeOutUpTiles::CGUIAsFadeOutUpTiles( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName)
	:CGUIAsFadeOutTRTiles( rAsType, rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	CGUIAsFadeOutUpTiles::CGUIAsFadeOutUpTiles( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsFadeOutTRTiles( "CGUIAsFadeOutUpTiles", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	real CGUIAsFadeOutUpTiles::TestFunc( uint32 uX, uint32 uY, real fPercent )
	{
		CGUIVector2	n( m_aGridSize.GetWidth() * fPercent, m_aGridSize.GetHeight()*fPercent );
		if ( n.y == 0.0f )
		{
			return 1.0f;
		}

		return powf( uY / n.y, 6 );
	}
	//------------------------------------------------------------------------------
	void CGUIAsFadeOutUpTiles::TransformTile( uint32 uX, uint32 uY, real distance )
	{
		SR_V3F_Quad coords = GetOriginalTile(uX,uY);
		const CGUIVector2& step = GetTiledGrid3D()->GetStep();

		coords.bl.y -= (step.y / 2) * (1.0f - distance);
		coords.br.y -= (step.y / 2) * (1.0f - distance);
		coords.tl.y += (step.y / 2) * (1.0f - distance);
		coords.tr.y += (step.y / 2) * (1.0f - distance);

		SetTile( uX, uY, coords );
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsFadeOutDownTiles
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsFadeOutDownTiles::CGUIAsFadeOutDownTiles( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsFadeOutUpTiles( "CGUIAsFadeOutDownTiles", rAsName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	real CGUIAsFadeOutDownTiles::TestFunc( uint32 uX, uint32 uY, real fPercent )
	{
		fPercent = 1.0f - fPercent;
		CGUIVector2	n( m_aGridSize.GetWidth() * fPercent, m_aGridSize.GetHeight()*fPercent );
		if ( uY == 0 )
		{
			return 1.0f;
		}

		return powf( n.y / uY, 6 );
	}
	//------------------------------------------------------------------------------



	//*****************************************************************************
	//	CGUIAsTurnOffTiles
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsTurnOffTiles::CGUIAsTurnOffTiles( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsTiledGrid3D( "CGUIAsTurnOffTiles", rAsName, rSceneName )
		,m_nSeed( 10 )
		,m_nTilesCount( 0 )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsTurnOffTiles::Reset( )
	{
		CGUIAsTiledGrid3D::Reset();

		m_vTilesOrder.clear();
	}
	//------------------------------------------------------------------------------
	void CGUIAsTurnOffTiles::SetTurnOffTilesInfo( int32 seed )
	{
		m_nSeed = seed;
	}
	//------------------------------------------------------------------------------
	int32 CGUIAsTurnOffTiles::OnInitGrid( )
	{
		if( 0 != CGUIAsTiledGrid3D::OnInitGrid() )
		{
			return -1;
		}

		if ( m_nSeed != -1 )
		{
			srand( m_nSeed );
		}

		m_nTilesCount = m_aGridSize.GetWidth() * m_aGridSize.GetHeight();
		m_vTilesOrder.resize(m_nTilesCount);

		for( uint32 i = 0; i < m_nTilesCount; i++ )
		{
			m_vTilesOrder[i] = i;
		}

		//shuffle
		for( int32 i = m_nTilesCount - 1; i >= 0; i-- )
		{
			int j = rand() % (i+1);
			std::swap( m_vTilesOrder[i], m_vTilesOrder[j] );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIAsTurnOffTiles::OnFiniGrid( )
	{
		m_vTilesOrder.clear();

		CGUIAsTiledGrid3D::OnFiniGrid();
	}
	//------------------------------------------------------------------------------
	void CGUIAsTurnOffTiles::OnUpdate( )
	{
		CGUIAsTiledGrid3D::OnUpdate( );

		real fPercent = GetPercent();
		uint32 l = (uint32)( fPercent * (real)m_nTilesCount );
		for( uint32 i = 0; i < m_nTilesCount; i++ )
		{
			uint32 t = m_vTilesOrder[i];
			uint32 uX = t / m_aGridSize.GetHeight();
			uint32 uY = t % m_aGridSize.GetHeight();

			if ( i < l )
			{
				TurnOffTile( uX, uY );
			}
			else
			{
				TurnOnTile( uX, uY );
			}
		}
	}
	//------------------------------------------------------------------------------




	//*****************************************************************************
	//	CGUIAsWavesTiles3D
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsWavesTiles3D::CGUIAsWavesTiles3D( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsTiledGrid3D( "CGUIAsWavesTiles3D", rAsName, rSceneName )
		,m_nWaves( 1 )
		,m_fAmplitude( 3.0f )
		,m_fAmplitudeRate( 1.0f )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsWavesTiles3D::SetWavesInfo( int32 nWaves, real fAmplitude)
	{
		m_nWaves = nWaves;
		m_fAmplitude = fAmplitude;
		m_fAmplitudeRate = 1.0f;
	}
	//------------------------------------------------------------------------------
	void CGUIAsWavesTiles3D::OnUpdate()
	{
		CGUIAsTiledGrid3D::OnUpdate( );

		real fPercent = GetPercent();
		for( uint32 i = 0; i < m_aGridSize.GetWidth(); i++ )
		{
			for( uint32 j = 0; j < m_aGridSize.GetHeight(); j++ )
			{
				SR_V3F_Quad coords = GetOriginalTile(i,j);

				coords.bl.z = (sinf(fPercent*CGUIMath::GUI_PI*m_nWaves*2 + (coords.bl.y+coords.bl.x) * 0.01f) * m_fAmplitude * m_fAmplitudeRate );
				coords.br.z	= coords.bl.z;
				coords.tl.z = coords.bl.z;
				coords.tr.z = coords.bl.z;

				SetTile( i, j, coords );
			}
		}
	}
	//------------------------------------------------------------------------------




	//*****************************************************************************
	//	CGUIAsJumpTiles3D
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsJumpTiles3D::CGUIAsJumpTiles3D( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsTiledGrid3D( "CGUIAsJumpTiles3D", rAsName, rSceneName )
		,m_nJumps( 1 )
		,m_fAmplitude( 3.0f )
		,m_fAmplitudeRate( 1.0f )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsJumpTiles3D::SetJumpTilesInfo( int32 nJumps, real fAmplitude)
	{
		m_nJumps = nJumps;
		m_fAmplitude = fAmplitude;
		m_fAmplitudeRate = 1.0f;
	}
	//------------------------------------------------------------------------------
	void CGUIAsJumpTiles3D::OnUpdate()
	{
		CGUIAsTiledGrid3D::OnUpdate( );

		real fPercent = GetPercent();
		float sinz =  (sinf(CGUIMath::GUI_PI*fPercent*m_nJumps*2) * m_fAmplitude * m_fAmplitudeRate );
		float sinz2 = (sinf(CGUIMath::GUI_PI*(fPercent*m_nJumps*2 + 1)) * m_fAmplitude * m_fAmplitudeRate );

		for( uint32 i = 0; i < m_aGridSize.GetWidth(); i++ )
		{
			for( uint32 j = 0; j < m_aGridSize.GetHeight(); j++ )
			{
				SR_V3F_Quad coords = GetOriginalTile(i,j);
				
				if ( ((i+j) % 2) == 0 )
				{
					coords.bl.z += sinz;
					coords.br.z += sinz;
					coords.tl.z += sinz;
					coords.tr.z += sinz;
				}
				else
				{
					coords.bl.z += sinz2;
					coords.br.z += sinz2;
					coords.tl.z += sinz2;
					coords.tr.z += sinz2;
				}

				SetTile( i, j, coords );
			}
		}
	}


	//*****************************************************************************
	//	CGUIAsSplitRows
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsSplitRows::CGUIAsSplitRows( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsTiledGrid3D( "CGUIAsSplitRows", rAsName, rSceneName )
		,m_nRows( 1 )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsSplitRows::SetSplitRowsInfo( uint32 nRows )
	{
		m_nRows = nRows;
	}
	//------------------------------------------------------------------------------
	void CGUIAsSplitRows::OnUpdate()
	{
		CGUIAsTiledGrid3D::OnUpdate( );

		uint32 nSceneWidth = GetTiledGrid3D()->GetSceneWidth();
		real fPercent = GetPercent();
		for( uint32 i = 0; i < m_aGridSize.GetWidth(); i++ )
		{
			for( uint32 j = 0; j < m_aGridSize.GetHeight(); j++ )
			{
				SR_V3F_Quad coords = GetOriginalTile(i,j);
				real direction = 1;
				if ( (j % 2 ) == 0 )
				{
					direction = -1;
				}	

				coords.bl.x += direction * nSceneWidth * fPercent;
				coords.br.x += direction * nSceneWidth * fPercent;
				coords.tl.x += direction * nSceneWidth * fPercent;
				coords.tr.x += direction * nSceneWidth * fPercent;

				SetTile( i, j, coords );
			}
		}

	}
	//------------------------------------------------------------------------------


	//*****************************************************************************
	//	CGUIAsSplitCols
	//*****************************************************************************
	//------------------------------------------------------------------------------
	CGUIAsSplitCols::CGUIAsSplitCols( const CGUIString& rAsName, const CGUIString& rSceneName )
		:CGUIAsTiledGrid3D( "CGUIAsSplitCols", rAsName, rSceneName )
		,m_nCols( 1 )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIAsSplitCols::SetSplitColsInfo( uint32 nRows )
	{
		m_nCols = nRows;
	}
	//------------------------------------------------------------------------------
	void CGUIAsSplitCols::OnUpdate()
	{
		CGUIAsTiledGrid3D::OnUpdate( );

		uint32 nSceneHeight = GetTiledGrid3D()->GetSceneHeight();
		real fPercent = GetPercent();
		for( uint32 i = 0; i < m_aGridSize.GetWidth(); i++ )
		{
			for( uint32 j = 0; j < m_aGridSize.GetHeight(); j++ )
			{
				SR_V3F_Quad coords = GetOriginalTile(i,j);
				real direction = 1;
				if ( (i % 2 ) == 0 )
				{
					direction = -1;
				}
				
				coords.bl.y += direction * nSceneHeight * fPercent;
				coords.br.y += direction * nSceneHeight * fPercent;
				coords.tl.y += direction * nSceneHeight * fPercent;
				coords.tr.y += direction * nSceneHeight * fPercent;

				SetTile( i,j, coords );
			}
		}

	}
	//------------------------------------------------------------------------------

}