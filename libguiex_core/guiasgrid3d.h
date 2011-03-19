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
		CGUIAsGrid3D(const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName);
		virtual ~CGUIAsGrid3D( );

	protected:
		virtual CGUISceneEffectGridBase* GenerateGrid( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize );

		CGUISceneEffectGrid3D* GetGrid3D();

		const SR_V3F& GetVertex( const CGUIIntSize& rPos );
		const SR_V3F& GetOriginalVertex( const CGUIIntSize& rPos );
		void SetVertex( const CGUIIntSize& rPos, const SR_V3F& rVertex );

	protected:
	};



	//*****************************************************************************
	//	CGUIAsPageTurn3D
	//*****************************************************************************
	class GUIEXPORT CGUIAsPageTurn3D : public CGUIAsGrid3D
	{
	public:
		CGUIAsPageTurn3D( const CGUIString& rAsName, const CGUIString& rSceneName );

	protected:
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
		CGUIAsWaves3D( const CGUIString& rAsName, const CGUIString& rSceneName );

		void SetWavesInfo( int32 nWaves, real fAmplitude);

	protected:
		virtual void OnUpdate( );

	private:
		int32 m_nWaves;
		real m_fAmplitude;
		real m_fAmplitudeRate;

	protected:
		GUI_AS_GENERATOR_DECLARE( CGUIAsWaves3D);
	};

}//namespace guiex


#endif //__GUI_ASGRID3D_20110316_H__



