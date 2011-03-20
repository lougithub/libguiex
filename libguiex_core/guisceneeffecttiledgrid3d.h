/** 
 * @file guisceneeffecttiledgrid3d.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-16
 */


#ifndef __KEN_GUISCENEEFFECTTILEDGRID3D_20110316_H__
#define	__KEN_GUISCENEEFFECTTILEDGRID3D_20110316_H__


//============================================================================//
// include
//============================================================================//
#include "guisceneeffectgridbase.h"

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
	class GUIEXPORT CGUISceneEffectTiledGrid3D : public CGUISceneEffectGridBase
	{
	public:
		CGUISceneEffectTiledGrid3D( const CGUIIntSize& rSceneSize, const CGUIIntSize& rGridSize );

		virtual int32 Initialize( );
		virtual void Release( );

	protected:
		virtual ~CGUISceneEffectTiledGrid3D();
		virtual void ProcessCaptureTexture( IGUIInterfaceRender* pRender );

	protected:
		SR_T2F	*m_pTexCoordinates;
		SR_V3F	*m_pVertices;
		SR_V3F	*m_pOriginalVertices;
		uint16	*m_pIndices;
	};
}

#endif //__KEN_GUISCENEEFFECTTILEDGRID3D_20110316_H__
