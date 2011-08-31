/** 
 * @file guicanvaslayer.h
 * @brief canvas layer used in system
 * @author ken
 * @date 2010-12-22
 */

#ifndef __GUI_CANVASLAYER_20101222_H__
#define __GUI_CANVASLAYER_20101222_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiwidget.h"
#include "guisystem.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIVector2;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUICanvasLayer : public CGUIWidget
	{
	public:
		CGUICanvasLayer( const char* szLayerName, bool bTopMost = false, const CGUISize& rCanvasSize = GSystem->GetScreenSize());
		virtual ~CGUICanvasLayer();

		virtual void Initialize( );
		virtual void Finalize( );
		virtual void Update( real fDeltaTime );
		virtual void Render( class IGUIInterfaceRender* pRender );
		virtual void RenderExtraInfo( IGUIInterfaceRender* pRender );
		virtual void Refresh( );
		virtual bool IsInputConsumed() const;

		virtual CGUIWidget* GetWidgetUnderPoint( const CGUIVector2& rPos );

		virtual void DestroySelf( ) = 0;

		const CGUIString& GetLayerName( ) const;
		bool IsTopMost( ) const;

	protected:
		friend class CGUICanvasLayerManager;
		virtual void BeginRender( class IGUIInterfaceRender* pRender );
		virtual void EndRender( class IGUIInterfaceRender* pRender );

	private:
		CGUIString m_strLayerName;
		bool m_bTopMost;
	};

}//namespace guiex

#endif //__GUI_CANVASLAYER_20101222_H__
