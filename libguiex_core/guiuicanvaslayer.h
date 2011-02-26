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
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIUICanvasLayer : public CGUICanvasLayer
	{
	public:
		CGUIUICanvasLayer( const char* szLayerName );
		virtual ~CGUIUICanvasLayer();

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

		void AddToGarbage( CGUIWidget* pWidget );
		void AddToDynamicGarbage( CGUIWidget* pWidget );
		void RefreshGarbage( );

		CGUIWidget*	GetCurrentRootWidget( ) const;

	protected:
		void GenerateRootWidget();
		void DestroyRootWidget();

	private:
		typedef std::vector<CGUIWidget*>	TArrayWidget;	
		TArrayWidget m_arrayOpenedPage;
		TArrayWidget m_vecPageGarbage;
		TArrayWidget m_vecDynamicPageGarbage;
		TArrayWidget m_arrayOpenedDlg; //widget modal dialog
		CGUIWidget*	m_pPopupWidget; //popup widget
		CGUIWidget* m_pWgtRoot; ///widget root
	};

}//namespace guiex

#endif //__GUI_UICANVASLAYER_20110226_H__
