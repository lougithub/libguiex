/** 
 * @file guicanvaslayer.cpp
 * @brief canvas layer used in system
 * @author ken
 * @date 2010-12-22
 */

//============================================================================//
// include 
//============================================================================// 
#include "guiuicanvaslayer.h"
#include "guiwidget.h"
#include "guiwidgetmanager.h"
#include "guiexception.h"
#include "guisystem.h"
#include "guilogmsgmanager.h"

#include <algorithm>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIUICanvasLayer::CGUIUICanvasLayer( const char* szLayerName )
		:CGUICanvasLayer( szLayerName, true )
		,m_pPopupWidget(NULL)

	{
		SetSizeType(eScreenValue_Percentage);
		SetSize( 1.0f, 1.0f );
	}
	//------------------------------------------------------------------------------
	CGUIUICanvasLayer::~CGUIUICanvasLayer()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::Initialize( )
	{
		CGUICanvasLayer::Initialize();
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::Finalize( )
	{
		//close all popup widget
		while( GetCurrentPopupWidget())
		{
			ClosePopupWidget(GetCurrentPopupWidget());
		}

		//close all modal dialog
		while(GetTopestDialog())
		{
			CloseDialog(GetTopestDialog());
		}

		//close all page
		while( !m_arrayOpenedPage.empty())
		{
			CloseUIPage(*m_arrayOpenedPage.begin());
		}

		//clear page in garbage
		RefreshGarbage();

		CGUICanvasLayer::Finalize();
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::DestroySelf( )
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::Update( real fDeltaTime )
	{
		CGUICanvasLayer::Update( fDeltaTime );

		//update dlg
		for(TArrayWidget::iterator itor = m_arrayOpenedDlg.begin();
			itor != m_arrayOpenedDlg.end();
			++itor)
		{
			(*itor)->Update( fDeltaTime );
		}

		//update popup widget
		if( m_pPopupWidget )
		{
			m_pPopupWidget->Update( fDeltaTime );
		}		

		RefreshGarbage();
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::Render( IGUIInterfaceRender* pRender )
	{
		CGUICanvasLayer::Render( pRender );

		//render dlg
		for(TArrayWidget::iterator itor = m_arrayOpenedDlg.begin();
			itor != m_arrayOpenedDlg.end();
			++itor)
		{
			(*itor)->Render(pRender);
		}

		//render popup widget
		if( m_pPopupWidget )
		{
			m_pPopupWidget->Render(pRender);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::RenderExtraInfo( IGUIInterfaceRender* pRender )
	{
		CGUICanvasLayer::RenderExtraInfo( pRender );

		//render dlg
		for(TArrayWidget::iterator itor = m_arrayOpenedDlg.begin();
			itor != m_arrayOpenedDlg.end();
			++itor)
		{
			(*itor)->RenderExtraInfo(pRender);
		}
		//render extra info
		if( m_pPopupWidget )
		{
			m_pPopupWidget->RenderExtraInfo(pRender);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::Refresh( )
	{
		CGUICanvasLayer::Refresh( );

		//update ui dlg
		for(TArrayWidget::iterator itor = m_arrayOpenedDlg.begin();
			itor != m_arrayOpenedDlg.end();
			++itor)
		{
			(*itor)->Refresh();
		}

		//update ui popup widget
		if( m_pPopupWidget )
		{
			m_pPopupWidget->Refresh();
		}
	}
	//------------------------------------------------------------------------------
	CGUIWidget* CGUIUICanvasLayer::GetWidgetUnderPoint( const CGUIVector2& rPos )
	{
		//capture input
		CGUIWidget* pRootWidget = GetCurrentRootWidget();
		if( pRootWidget )
		{
			return pRootWidget->GetWidgetAtPoint(rPos);
		}
		else
		{
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::OpenDialog(CGUIWidget* pDlg)
	{
		GUI_ASSERT( pDlg, "invalid parameter" );
		GUI_ASSERT( pDlg->GetParent() == false, "the modal dialog shouldn't have a parent");

		if( !CGUIWidgetManager::Instance()->HasPage( pDlg ))
		{
			throw CGUIException( "[CGUIUICanvasLayer::OpenDialog]: the dialog <%s:%s> isn't a page", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str());
		}

		pDlg->SetParent( NULL );

		TArrayWidget::iterator itor = std::find(m_arrayOpenedDlg.begin(), m_arrayOpenedDlg.end(), pDlg );
		if( (*itor) == pDlg)
		{
			throw CGUIException( "[CGUIUICanvasLayer::OpenDialog]: failed to open dialog<%s:%s>, it has opened!", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str());
		}

		m_arrayOpenedDlg.push_back(pDlg);
		pDlg->Open();

		GUI_TRACE( GUI_FORMAT("OpenDialog <%s : %s> \n", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str()));
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::CloseDialog(CGUIWidget* pDlg)
	{
		GUI_ASSERT(pDlg, "invalid parameter");

		TArrayWidget::iterator itor = std::find(m_arrayOpenedDlg.begin(), m_arrayOpenedDlg.end(), pDlg );
		if( (*itor) == pDlg)
		{		
			m_arrayOpenedDlg.erase(itor);
			GSystem->UngisterGlobalKeyByRoot(pDlg);
			pDlg->Close();

			if( CGUIWidgetManager::Instance()->HasDynamicPage( pDlg ) )
			{
				AddToDynamicGarbage( pDlg );
			}
			return;
		}

		throw CGUIException( "[CGUIUICanvasLayer::CloseDialog]: failed to close dialog <%s : %s>.", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str());
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIUICanvasLayer::GetTopestDialog( ) const 
	{
		if(m_arrayOpenedDlg.empty())
		{
			return NULL;
		}
		else
		{
			return m_arrayOpenedDlg.back();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::CloseByAutoSelect( CGUIWidget* pWidget )
	{
		GUI_ASSERT( pWidget, "invalid parameter" );

		//is page
		TArrayWidget::iterator itorPage = std::find(m_arrayOpenedPage.begin(), m_arrayOpenedPage.end(), pWidget );
		if( itorPage != m_arrayOpenedPage.end())
		{
			CloseUIPage( pWidget );
			return;
		}

		//is dialog
		TArrayWidget::iterator itorDlg = std::find(m_arrayOpenedDlg.begin(), m_arrayOpenedDlg.end(), pWidget );
		if( itorDlg != m_arrayOpenedDlg.end())
		{		
			CloseDialog( pWidget );
			return;
		}

		//is popup widget
		if( pWidget == m_pPopupWidget )
		{		
			ClosePopupWidget( pWidget );
			return;
		}
	}
	//------------------------------------------------------------------------------
	CGUIWidget* CGUIUICanvasLayer::GetCurrentPopupWidget( ) const
	{
		return m_pPopupWidget;
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::ClosePopupWidget(CGUIWidget* pWidget)
	{
		GUI_ASSERT(pWidget, "invalid parameter");

		if( m_pPopupWidget = pWidget )
		{
			pWidget->Close();
			m_pPopupWidget = NULL;
			GUI_TRACE( GUI_FORMAT("ClosePopupWidget <%s> \n", pWidget->GetName().c_str()));
			return;
		}
		else
		{
			GUI_FORCE_ASSERT(GUI_FORMAT("failed to close popup widget <%s>", pWidget->GetName().c_str()));
		}
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::OpenPopupWidget(CGUIWidget* pWidget)
	{
		GUI_ASSERT(pWidget, "invalid parameter");

		if( m_pPopupWidget )
		{
			GUI_FORCE_ASSERT(GUI_FORMAT("failed to open popup widget, a popup widget has opened! <%s>", m_pPopupWidget->GetName().c_str()));
		}

		pWidget->Open();
		m_pPopupWidget = pWidget;

		GUI_TRACE( GUI_FORMAT("OpenPopupWidget <%s> \n", pWidget->GetName().c_str()));
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::OpenUIPage(CGUIWidget* pPage)
	{
		GUI_ASSERT( pPage, "invalid parameter" );

		if( CGUIWidgetManager::Instance()->HasPage( pPage ) == false )
		{
			throw CGUIException( "[CGUIUICanvasLayer::OpenUIPage]: the widget <%s> isn't a page!", pPage->GetName().c_str());
		}

		pPage->SetParent( this );
		pPage->Open();
		pPage->Refresh();

		m_arrayOpenedPage.push_back(pPage);
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::CloseUIPage(CGUIWidget* pPage)
	{
		GUI_ASSERT( pPage, "invalid parameter" );

		if( CGUIWidgetManager::Instance()->HasPage( pPage ) == false )
		{
			throw CGUIException( "[CGUIUICanvasLayer::CloseUIPage]: the widget <%s> isn't a page!", pPage->GetName().c_str());
		}

		if( pPage->IsOpen() == false )
		{
			throw CGUIException( "[CGUIUICanvasLayer::CloseUIPage]: the page <%s> has closed!", pPage->GetName().c_str());
		}

		TArrayWidget::iterator itor = std::find(m_arrayOpenedPage.begin(), m_arrayOpenedPage.end(), pPage );
		if( itor == m_arrayOpenedPage.end())
		{
			throw CGUIException( "[CGUIUICanvasLayer::CloseUIPage]: can't find page in opend page <%s> list!", pPage->GetName().c_str());
		}
		m_arrayOpenedPage.erase( itor );
		pPage->Close();
		pPage->SetParent(NULL);

		if( CGUIWidgetManager::Instance()->HasDynamicPage( pPage ) )
		{
			AddToDynamicGarbage( pPage );
		}
		return;
	}
	//------------------------------------------------------------------------------
	///< get opened page num, which should be opened by method OpenUIPage()
	uint32 CGUIUICanvasLayer::GetOpenedPageNum() const
	{
		return m_arrayOpenedPage.size();
	}
	//------------------------------------------------------------------------------
	///< get opened page by index
	CGUIWidget*	CGUIUICanvasLayer::GetOpenedPageByIndex( uint32 nIdx )
	{
		if( nIdx >= m_arrayOpenedPage.size())
		{
			throw CGUIException( "[CGUIUICanvasLayer::GetOpenedPageByIndex]: the given index <%d> is overflow, total opened page size is <%d>",
				nIdx, m_arrayOpenedPage.size());
		}

		return m_arrayOpenedPage[nIdx];
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::AddToGarbage( CGUIWidget* pWidget )
	{
		m_vecPageGarbage.push_back(pWidget);
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::AddToDynamicGarbage( CGUIWidget* pWidget )
	{
		m_vecDynamicPageGarbage.push_back(pWidget);
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::RefreshGarbage( )
	{
		for( TArrayWidget::iterator itor = m_vecPageGarbage.begin();
			itor != m_vecPageGarbage.end();
			++itor )
		{
			CGUIWidgetManager::Instance()->ReleasePage( *itor );
		}
		m_vecPageGarbage.clear();

		for( TArrayWidget::iterator itor = m_vecDynamicPageGarbage.begin();
			itor != m_vecDynamicPageGarbage.end();
			++itor )
		{
			CGUIWidgetManager::Instance()->DestroyDynamicPage( *itor );
		}
		m_vecDynamicPageGarbage.clear();
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIUICanvasLayer::GetCurrentRootWidget( )
	{
		CGUIWidget* pDlgRoot = NULL;
		if( pDlgRoot = GetCurrentPopupWidget())
		{
			return pDlgRoot;
		}
		else if( pDlgRoot = GetTopestDialog())
		{
			return pDlgRoot;
		}
		else
		{
			return this;
		}
	}
	//------------------------------------------------------------------------------
}
