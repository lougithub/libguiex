/** 
* @file guiastiledgrid3d.h
* @brief
* @author ken
* @date 2011-03-16
*/

#ifndef __GUI_ASTILEDGRID3D_20110316_H__
#define __GUI_ASTILEDGRID3D_20110316_H__

//============================================================================//
// include
//============================================================================// 
#include "guiasgridbase.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUISceneEffectTiledGrid3D;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	//*****************************************************************************
	//	CGUIAsTiledGrid3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsTiledGrid3D : public CGUIAsGridBase
	{
	protected:
		CGUIAsTiledGrid3D( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName);
		virtual ~CGUIAsTiledGrid3D( );

	protected:
		virtual CGUISceneEffectGridBase* GenerateGrid( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize );

		CGUISceneEffectTiledGrid3D* GetTiledGrid3D();

		const SR_V3F_Quad& GetTile( uint32 uX, uint32 uY );
		const SR_V3F_Quad& GetOriginalTile( uint32 uX, uint32 uY );
		void SetTile( uint32 uX, uint32 uY, const SR_V3F_Quad& rTile );

		void TurnOnTile( uint32 uX, uint32 uY );
		void TurnOffTile( uint32 uX, uint32 uY );
	};


	//*****************************************************************************
	//	CGUIAsShakyTiles3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsShakyTiles3D : public CGUIAsTiledGrid3D
	{
	public:
		void SetShakyTiles3DInfo( int32 range, bool bShakeZ );

	protected:
		CGUIAsShakyTiles3D( const CGUIString& rAsName, const CGUIString& rSceneName );

	protected:
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsShakyTiles3D);

	private:
		int32 m_nRandrange;
		bool m_bShakeZ;
	};


	//*****************************************************************************
	//	CGUIAsShatteredTiles3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsShatteredTiles3D : public CGUIAsTiledGrid3D
	{
	public:
		void SetShatteredTiles3DInfo( int32 range, bool shatterZ );
		virtual void Reset( );

	protected:
		CGUIAsShatteredTiles3D( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsShatteredTiles3D);

	private:
		int32 m_nRandrange;
		bool m_bOnce;
		bool m_bShatterZ;
	};


	//*****************************************************************************
	//	CGUIAsShuffleTiles
	//*****************************************************************************
	/**
	*  Shuffle the tiles in random order
	*/
	class GUIEXPORT CGUIAsShuffleTiles : public CGUIAsTiledGrid3D
	{
	public:
		void SetShuffleTilesInfo( int32 seed );
		virtual void Reset( );

	protected:
		CGUIAsShuffleTiles( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

		virtual int32 OnInitGrid();
		virtual void OnFiniGrid();

		CGUIIntSize GetDelta( uint32 uX, uint32 uY );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsShuffleTiles);

	private:
		int32 m_nSeed;
		uint32  m_nTilesCount;
		std::vector<uint32> m_vTilesOrder;
		std::vector<CGUIIntSize> m_vTilesDelta;
	};


	//*****************************************************************************
	//	CGUIAsFadeOutTRTiles
	//*****************************************************************************
	/**
	* Fades out the tiles in a Top-Right direction
	*/
	class GUIEXPORT CGUIAsFadeOutTRTiles : public CGUIAsTiledGrid3D
	{

	protected:
		CGUIAsFadeOutTRTiles( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName);
		CGUIAsFadeOutTRTiles( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

		virtual real TestFunc( uint32 uX, uint32 uY, real fPercent );
		virtual void TransformTile( uint32 uX, uint32 uY, real distance );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsFadeOutTRTiles);
	};


	//*****************************************************************************
	//	CGUIAsFadeOutBLTiles
	//*****************************************************************************

	/**
	*  Fades out the tiles in a Bottom-Left direction
	*/
	class GUIEXPORT CGUIAsFadeOutBLTiles : public CGUIAsFadeOutTRTiles
	{
	protected:
		CGUIAsFadeOutBLTiles( const CGUIString& rAsName, const CGUIString& rSceneName );

		virtual real TestFunc( uint32 uX, uint32 uY, real fPercent );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsFadeOutBLTiles);

	private:
	};


	//*****************************************************************************
	//	CGUIAsFadeOutUpTiles
	//*****************************************************************************
	/**
	*   Fades out the tiles in upwards direction
	*/
	class GUIEXPORT CGUIAsFadeOutUpTiles : public CGUIAsFadeOutTRTiles
	{

	protected:
		CGUIAsFadeOutUpTiles( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName);
		CGUIAsFadeOutUpTiles( const CGUIString& rAsName, const CGUIString& rSceneName );
	
		virtual real TestFunc( uint32 uX, uint32 uY, real fPercent );
		virtual void TransformTile( uint32 uX, uint32 uY, real distance );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsFadeOutUpTiles);

	private:
	};


	//*****************************************************************************
	//	CGUIAsFadeOutDownTiles
	//*****************************************************************************
	/**
	*	Fades out the tiles in downwards direction
	*/
	class GUIEXPORT CGUIAsFadeOutDownTiles : public CGUIAsFadeOutUpTiles
	{
	protected:
		CGUIAsFadeOutDownTiles( const CGUIString& rAsName, const CGUIString& rSceneName );
		
		virtual real TestFunc( uint32 uX, uint32 uY, real fPercent );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsFadeOutDownTiles);

	private:
	};


	//*****************************************************************************
	//	CGUIAsTurnOffTiles
	//*****************************************************************************
	/**
	*	 Turn off the tiles in random order
	*/	
	class GUIEXPORT CGUIAsTurnOffTiles : public CGUIAsTiledGrid3D
	{
	public:
		void SetTurnOffTilesInfo( int32 seed );
		virtual void Reset( );

	protected:
		CGUIAsTurnOffTiles( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

		virtual int32 OnInitGrid();
		virtual void OnFiniGrid();

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsTurnOffTiles);

	private:
		int32 m_nSeed;
		uint32 m_nTilesCount;
		std::vector<uint32> m_vTilesOrder;
	};

	//*****************************************************************************
	//	CGUIAsWavesTiles3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsWavesTiles3D : public CGUIAsTiledGrid3D
	{
	public:
		void SetWavesInfo( int32 nWaves, real fAmplitude);

	protected:
		CGUIAsWavesTiles3D( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsWavesTiles3D);

	private:
		int32 m_nWaves;
		real m_fAmplitude;
		real m_fAmplitudeRate;
	};



	//*****************************************************************************
	//	CGUIAsJumpTiles3D
	//*****************************************************************************
	/**
	*	A sin function is executed to move the tiles across the Z axis
	*/	
	class GUIEXPORT CGUIAsJumpTiles3D : public CGUIAsTiledGrid3D
	{
	public:
		void SetJumpTilesInfo( int32 nJumps, real fAmplitude);

	protected:
		CGUIAsJumpTiles3D( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsJumpTiles3D);

	private:
		int32 m_nJumps;
		real m_fAmplitude;
		real m_fAmplitudeRate;
	};


	//*****************************************************************************
	//	CGUIAsSplitRows
	//*****************************************************************************
	//TODO: fix it later
	class GUIEXPORT CGUIAsSplitRows : public CGUIAsTiledGrid3D
	{
	public:
		void SetSplitRowsInfo( uint32 nRows );

	protected:
		CGUIAsSplitRows( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsSplitRows);

	private:
		uint32 m_nRows;
	};



	//*****************************************************************************
	//	CGUIAsSplitCols
	//*****************************************************************************
	class GUIEXPORT CGUIAsSplitCols : public CGUIAsTiledGrid3D
	{
	public:
		void SetSplitColsInfo( uint32 nCols );

	protected:
		CGUIAsSplitCols( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsSplitCols);

	private:
		int32 m_nCols;
	};

}//namespace guiex


#endif //__GUI_ASTILEDGRID3D_20110316_H__



