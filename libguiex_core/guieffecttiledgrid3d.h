/** 
 * @file guieffecttiledgrid3d.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-16
 */


#ifndef __KEN_GUIEFFECTTILEDGRID3D_20110316_H__
#define	__KEN_GUIEFFECTTILEDGRID3D_20110316_H__


//============================================================================//
// include
//============================================================================//
#include "guieffectgridbase.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIInterfaceRender;
}

//============================================================================//
// class
//============================================================================//
namespace guiex
{
	class GUIEXPORT CGUIEffectTiledGrid3D : public CGUIEffectGridBase
	{
	public:
		CGUIEffectTiledGrid3D( const CGUIIntSize& rSceneSize, const CGUIIntSize& rGridSize );

		virtual int32 Initialize( );
		virtual void Release( );

	protected:
		virtual ~CGUIEffectTiledGrid3D();
		virtual void ProcessCaptureTexture( IGUIInterfaceRender* pRender );

	protected:
	};
}

#endif //__KEN_GUIEFFECTTILEDGRID3D_20110316_H__
