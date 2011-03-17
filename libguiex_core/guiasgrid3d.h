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

	public:
		virtual int32 ProcessProperty( const CGUIProperty& rProperty );
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

	protected:
		virtual CGUISceneEffectGridBase* GenerateGrid( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize );

		CGUISceneEffectGrid3D* GetGrid3D();

		const SR_V3F& GetVertex( const CGUIIntSize& rPos );
		const SR_V3F& GetOriginalVertex( const CGUIIntSize& rPos );
		void SetVertex( const CGUIIntSize& rPos, const SR_V3F& rVertex );

	protected:
	};

}//namespace guiex


#endif //__GUI_ASGRID3D_20110316_H__



