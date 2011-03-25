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
		CGUISceneEffectTiledGrid3D( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize );

		virtual int32 Initialize( );
		virtual void Release( );

		void Reset( );

		const SR_V3F_Quad& GetTile( uint32 uX, uint32 uY );
		const SR_V3F_Quad& GetOriginalTile( uint32 uX, uint32 uY );
		void SetTile( uint32 uX, uint32 uY, const SR_V3F_Quad& rTile );

	protected:
		virtual ~CGUISceneEffectTiledGrid3D();
		virtual void ProcessCaptureTexture( IGUIInterfaceRender* pRender );

	protected:
		SR_T2F	*m_pTexCoordinates;
		SR_V3F_Quad	*m_pVertices;
		SR_V3F_Quad	*m_pOriginalVertices;
		uint16	*m_pIndices;
	};
}

#endif //__KEN_GUISCENEEFFECTTILEDGRID3D_20110316_H__
