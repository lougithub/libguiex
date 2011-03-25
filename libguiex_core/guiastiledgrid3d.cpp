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
			int v = m_vTilesOrder[i];
			m_vTilesOrder[i] = m_vTilesOrder[j];
			m_vTilesOrder[j] = v;
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

		real fPercent = GetElapsedTime() / GetTotalTime();
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
	void CGUIAsFadeOutTRTiles::TurnOnTile( uint32 uX, uint32 uY )
	{
		SetTile( uX, uY, GetOriginalTile( uX, uY ));
	}
	//------------------------------------------------------------------------------
	void CGUIAsFadeOutTRTiles::TurnOffTile( uint32 uX, uint32 uY )
	{
		SR_V3F_Quad coords;
		memset( &coords, 0, sizeof( SR_V3F_Quad ) );
		SetTile( uX, uY, coords );
	}
	//------------------------------------------------------------------------------
	void CGUIAsFadeOutTRTiles::TransformTile( uint32 uX, uint32 uY, real distance )
	{
		SR_V3F_Quad coords = GetOriginalTile(uX,uY);
		const CGUIVector2& step = GetTiledGrid3D()->GetStep();

		coords.bl.x += (step.x / 2) * (1.0f - distance);
		coords.bl.y += (step.y / 2) * (1.0f - distance);

		coords.br.x -= (step.x / 2) * (1.0f - distance);
		coords.br.y += (step.y / 2) * (1.0f - distance);

		coords.tl.x += (step.x / 2) * (1.0f - distance);
		coords.tl.y -= (step.y / 2) * (1.0f - distance);

		coords.tr.x -= (step.x / 2) * (1.0f - distance);
		coords.tr.y -= (step.y / 2) * (1.0f - distance);

		SetTile( uX, uY, coords );
	}
	//------------------------------------------------------------------------------
	void CGUIAsFadeOutTRTiles::OnUpdate( )
	{
		CGUIAsTiledGrid3D::OnUpdate( );

		real fPercent = GetElapsedTime() / GetTotalTime();
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

	///**
	//* Fades out the tiles in a Top-Right direction
	//*/
	//class GUIEXPORT CGUIAsFadeOutTRTiles : public CGUIAsTiledGrid3D
	//{

	//protected:
	//	CGUIAsFadeOutTRTiles( const CGUIString& rAsName, const CGUIString& rSceneName );
	//	virtual void OnUpdate( );

	//protected:
	//	GUI_AS_GENERATOR_DECLARE( CGUIAsFadeOutTRTiles);

	//private:
	//};


	////*****************************************************************************
	////	CGUIAsFadeOutBLTiles
	////*****************************************************************************

	///**
	//*  Fades out the tiles in a Bottom-Left direction
	//*/
	//class GUIEXPORT CGUIAsFadeOutBLTiles : public CGUIAsFadeOutTRTiles
	//{

	//protected:
	//	CGUIAsFadeOutBLTiles( const CGUIString& rAsName, const CGUIString& rSceneName );
	//	virtual void OnUpdate( );

	//protected:
	//	GUI_AS_GENERATOR_DECLARE( CGUIAsFadeOutBLTiles);

	//private:
	//};


	////*****************************************************************************
	////	CGUIAsFadeOutUpTiles
	////*****************************************************************************
	///**
	//*   Fades out the tiles in upwards direction
	//*/
	//class GUIEXPORT CGUIAsFadeOutUpTiles : public CGUIAsFadeOutTRTiles
	//{

	//protected:
	//	CGUIAsFadeOutUpTiles( const CGUIString& rAsName, const CGUIString& rSceneName );
	//	virtual void OnUpdate( );

	//protected:
	//	GUI_AS_GENERATOR_DECLARE( CGUIAsFadeOutUpTiles);

	//private:
	//};


	////*****************************************************************************
	////	CGUIAsFadeOutDownTiles
	////*****************************************************************************
	///**
	//*	Fades out the tiles in downwards direction
	//*/
	//class GUIEXPORT CGUIAsFadeOutDownTiles : public CGUIAsFadeOutUpTiles
	//{

	//protected:
	//	CGUIAsFadeOutDownTiles( const CGUIString& rAsName, const CGUIString& rSceneName );
	//	virtual void OnUpdate( );

	//protected:
	//	GUI_AS_GENERATOR_DECLARE( CGUIAsFadeOutDownTiles);

	//private:
	//};


	////*****************************************************************************
	////	CGUIAsTurnOffTiles
	////*****************************************************************************
	///**
	//*	 Turn off the tiles in random order
	//*/	
	//class GUIEXPORT CGUIAsTurnOffTiles : public CGUIAsTiledGrid3D
	//{

	//protected:
	//	CGUIAsTurnOffTiles( const CGUIString& rAsName, const CGUIString& rSceneName );
	//	virtual void OnUpdate( );

	//protected:
	//	GUI_AS_GENERATOR_DECLARE( CGUIAsTurnOffTiles);

	//private:
	//	int32	m_nSeed;
	//	int32 m_nTilesCount;
	//	int32 *m_pTilesOrder;
	//};

	////*****************************************************************************
	////	CGUIAsWavesTiles3D
	////*****************************************************************************
	//class GUIEXPORT CGUIAsWavesTiles3D : public CGUIAsTiledGrid3D
	//{

	//protected:
	//	CGUIAsWavesTiles3D( const CGUIString& rAsName, const CGUIString& rSceneName );
	//	virtual void OnUpdate( );

	//protected:
	//	GUI_AS_GENERATOR_DECLARE( CGUIAsWavesTiles3D);

	//private:
	//	int32 waves;
	//	real amplitude;
	//	real amplitudeRate;
	//};



	////*****************************************************************************
	////	CGUIAsJumpTiles3D
	////*****************************************************************************
	///**
	//*	A sin function is executed to move the tiles across the Z axis
	//*/	
	//class GUIEXPORT CGUIAsJumpTiles3D : public CGUIAsTiledGrid3D
	//{

	//protected:
	//	CGUIAsJumpTiles3D( const CGUIString& rAsName, const CGUIString& rSceneName );
	//	virtual void OnUpdate( );

	//protected:
	//	GUI_AS_GENERATOR_DECLARE( CGUIAsJumpTiles3D);

	//private:
	//	int32 jumps;
	//	real amplitude;
	//	real amplitudeRate;
	//};


	////*****************************************************************************
	////	CGUIAsSplitRows
	////*****************************************************************************
	//class GUIEXPORT CGUIAsSplitRows : public CGUIAsTiledGrid3D
	//{

	//protected:
	//	CGUIAsSplitRows( const CGUIString& rAsName, const CGUIString& rSceneName );
	//	virtual void OnUpdate( );

	//protected:
	//	GUI_AS_GENERATOR_DECLARE( CGUIAsSplitRows);

	//private:
	//	int32 rows;
	//	CGUIIntSize winSize;
	//};



	////*****************************************************************************
	////	CGUIAsSplitCols
	////*****************************************************************************
	//class GUIEXPORT CGUIAsSplitCols : public CGUIAsTiledGrid3D
	//{

	//protected:
	//	CGUIAsSplitCols( const CGUIString& rAsName, const CGUIString& rSceneName );
	//	virtual void OnUpdate( );

	//protected:
	//	GUI_AS_GENERATOR_DECLARE( CGUIAsSplitCols);

	//private:
	//	int32 cols;
	//	CGUIIntSize	winSize;
	//};
}