/** 
 * @file guieffectgrid3d.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-16
 */


#ifndef __KEN_GUIEFFECTGRID3D_20110316_H__
#define	__KEN_GUIEFFECTGRID3D_20110316_H__


//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "guiscenecapture.h"

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
	class GUIEXPORT CGUIEffectGrid3D : public CGUISceneCapture
	{
	public:
		CGUIEffectGrid3D( );

		virtual int32 Initialize( uint32 uTextureWidth, uint32 uTextureHeight );
		virtual void Release( );

	protected:
		virtual ~CGUIEffectGrid3D();
		virtual void ProcessCaptureTexture( IGUIInterfaceRender* pRender );

	protected:
	};
}

#endif //__KEN_GUIEFFECTGRID3D_20110316_H__
