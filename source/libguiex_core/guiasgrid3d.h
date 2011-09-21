/** 
* @file guiasgrid3d.h
* @brief
* @author ken
* @date 2011-03-16
*/

#ifndef __GUI_ASGRID3D_20110316_H__
#define __GUI_ASGRID3D_20110316_H__

//============================================================================//
// include
//============================================================================// 
#include "guiasgridbase.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUISceneEffectGrid3D;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//*****************************************************************************
	//	CGUIAsGrid3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsGrid3D : public CGUIAsGridBase
	{
	protected:
		CGUIAsGrid3D( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName);
		virtual ~CGUIAsGrid3D( );

	protected:
		virtual CGUISceneEffectGridBase* GenerateGrid( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize );

		CGUISceneEffectGrid3D* GetGrid3D();

		const SVertexFormat_V3F& GetVertex( uint32 uX, uint32 uY );
		const SVertexFormat_V3F& GetOriginalVertex( uint32 uX, uint32 uY );
		void SetVertex( uint32 uX, uint32 uY, const SVertexFormat_V3F& rVertex );

	protected:
	};



	//*****************************************************************************
	//	CGUIAsPageTurn3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsPageTurn3D : public CGUIAsGrid3D
	{
	protected:
		CGUIAsPageTurn3D( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsPageTurn3D);
	};


	//*****************************************************************************
	//	CGUIAsWaves3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsWaves3D : public CGUIAsGrid3D
	{
	public:
		void SetWavesInfo( int32 nWaves, real fAmplitude);

	protected:
		CGUIAsWaves3D( const CGUIString& rAsName, const CGUIString& rSceneName );

		virtual void OnUpdate( );

	private:
		int32 m_nWaves;
		real m_fAmplitude;
		real m_fAmplitudeRate;

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsWaves3D);
	};


	//*****************************************************************************
	//	CGUIAsShaky3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsShaky3D : public CGUIAsGrid3D
	{
	public:
		void SetShakyInfo( int32 nRange, bool bShakeZ );

	protected:
		CGUIAsShaky3D( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	private:
		int32 m_nRandrange;
		bool m_bShakeZ;

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsShaky3D);
	};


	//*****************************************************************************
	//	CGUIAsFlipX3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsFlipX3D : public CGUIAsGrid3D
	{
	protected:
		CGUIAsFlipX3D( const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName );
		CGUIAsFlipX3D( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );
		virtual int32 OnInitGrid();

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsFlipX3D);
	};


	//*****************************************************************************
	//	CGUIAsFlipY3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsFlipY3D : public CGUIAsFlipX3D
	{
	protected:
		CGUIAsFlipY3D( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsFlipY3D);
	};


	//*****************************************************************************
	//	CGUIAsLens3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsLens3D : public CGUIAsGrid3D
	{
	public:
		void SetLens3DInfo( const CGUIVector2& pos, real radius );
		void SetPosition( const CGUIVector2& pos );

	protected:
		CGUIAsLens3D( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsLens3D);

	protected:
		CGUIVector2	m_aPosition;
		CGUIVector2 m_aPositionInPixels;
		real m_fRadius;
		real m_fLensEffect;
		bool m_bDirty;
	};



	//*****************************************************************************
	//	CGUIAsRipple3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsRipple3D : public CGUIAsGrid3D
	{
	public:
		void SetRipple3DInfo( const CGUIVector2& pos, real radius, uint32 waves, real amplitude );
		void SetPosition(const CGUIVector2& pos);

	protected:
		CGUIAsRipple3D( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsRipple3D);

	protected:
		CGUIVector2	m_aPosition;
		CGUIVector2	m_aPositionInPixels;
		real m_fRadius;
		int32 m_nWaves;
		real m_fAmplitude;
		real m_fAmplitudeRate;
	};




	//*****************************************************************************
	//	CGUIAsLiquid
	//*****************************************************************************
	class GUIEXPORT CGUIAsLiquid : public CGUIAsGrid3D
	{
	public:
		void SetLiquidInfo( uint32 waves, real amplitude );

	protected:
		CGUIAsLiquid( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsLiquid);

	protected:
		int32 m_nWaves;
		real m_fAmplitude;
		real m_fAmplitudeRate;

	};


	//*****************************************************************************
	//	CGUIAsWaves
	//*****************************************************************************
	class GUIEXPORT CGUIAsWaves : public CGUIAsGrid3D
	{
	public:
		void SetWavesInfo( uint32 waves, real amplitude, bool horizontal, bool vertical );

	protected:
		CGUIAsWaves( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsWaves);

	protected:
		int32 m_nWaves;
		real m_fAmplitude;
		real m_fAmplitudeRate;
		bool m_bVertical;
		bool m_bHorizontal;
	};


	//*****************************************************************************
	//	CGUIAsTwirl
	//*****************************************************************************
	class GUIEXPORT CGUIAsTwirl : public CGUIAsGrid3D
	{
	public:
		void SetTwirlInfo( const CGUIVector2& pos, uint32 twirls, real amplitude );
		void SetPosition(const CGUIVector2& pos);

	protected:
		CGUIAsTwirl( const CGUIString& rAsName, const CGUIString& rSceneName );
		virtual void OnUpdate( );

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsTwirl);

	protected:
		CGUIVector2	m_aPosition;
		CGUIVector2	m_aPositionInPixels;
		uint32 m_nTwirls;
		real m_fAmplitude;
		real m_fAmplitudeRate;
	};



}//namespace guiex


#endif //__GUI_ASGRID3D_20110316_H__



