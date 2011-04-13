/** 
 * @file guiuicanvaslayer.h
 * @brief ui canvas layer used in system
 * @author ken
 * @date 2011-02-26
 */

#ifndef __GUI_UICANVASLAYER_20110226_H__
#define __GUI_UICANVASLAYER_20110226_H__

//============================================================================//
// include
//============================================================================// 
#include "guicanvaslayer.h"
#include <vector>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUICamera;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIUICanvasLayer : public CGUICanvasLayer
	{
	public:
		CGUIUICanvasLayer( const char* szLayerName );
		virtual ~CGUIUICanvasLayer();

		virtual void Initialize( );
		virtual void Finalize( );

		virtual void Update( real fDeltaTime );
		virtual void Render( class IGUIInterfaceRender* pRender );
		virtual void RenderExtraInfo( IGUIInterfaceRender* pRender );
		virtual void Refresh( );
		virtual CGUIWidget* GetWidgetUnderPoint( const CGUIVector2& rPos );
		virtual void DestroySelf( );

		void OpenDialog(CGUIWidget* pDlg);
		void CloseDialog(CGUIWidget* pDlg);
		CGUIWidget*	GetTopestDialog( ) const;

		void OpenPopupWidget(CGUIWidget* pWidget);
		CGUIWidget* GetCurrentPopupWidget( ) const;
		void ClosePopupWidget(CGUIWidget* pWidget);

		void CloseByAutoSelect( CGUIWidget* pWidget );

		void OpenUIPage(CGUIWidget* pPage);
		void CloseUIPage(CGUIWidget* pPage);
		CGUIWidget*	GetOpenedPageByIndex( uint32 nIdx );
		uint32 GetOpenedPageNum() const;

		CGUIWidget*	GetCurrentRootWidget( );

		const CGUICamera* GetCamera() const;
		CGUICamera* GetCamera();

	private:
		typedef std::vector<CGUIWidget*>	TArrayWidget;	
		TArrayWidget m_arrayOpenedPage;
		TArrayWidget m_arrayOpenedDlg; //widget modal dialog
		CGUIWidget*	m_pPopupWidget; //popup widget

		CGUICamera* m_pDefaultUICamera;
	};

}//namespace guiex

#endif //__GUI_UICANVASLAYER_20110226_H__
