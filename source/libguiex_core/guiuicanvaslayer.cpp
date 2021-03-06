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
#include "guiinterfacerender.h"

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

		if( !CGUIWidgetManager::Instance()->HasPage( pDlg ) &&
			!CGUIWidgetManager::Instance()->HasDynamicPage( pDlg ))
		{
			GUI_THROW( GUI_FORMAT( "[CGUIUICanvasLayer::OpenDialog]: the dialog <%s:%s> isn't a page", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str()));
		}

		pDlg->SetParent( NULL );

		TArrayWidget::iterator itor = std::find(m_arrayOpenedDlg.begin(), m_arrayOpenedDlg.end(), pDlg );
		if( itor != m_arrayOpenedDlg.end() )
		{
			GUI_THROW( GUI_FORMAT( "[CGUIUICanvasLayer::OpenDialog]: failed to open dialog<%s:%s>, it has opened!", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str()));
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
		if( itor != m_arrayOpenedDlg.end() )
		{		
			m_arrayOpenedDlg.erase(itor);
			pDlg->Close();
			return;
		}

		GUI_THROW( GUI_FORMAT( "[CGUIUICanvasLayer::CloseDialog]: failed to close dialog <%s : %s>.", pDlg->GetSceneName().c_str(), pDlg->GetName().c_str()));
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
	void CGUIUICanvasLayer::OpenUIPage(CGUIWidget* pPage)
	{
		GUI_ASSERT( pPage, "invalid parameter" );

		if( !CGUIWidgetManager::Instance()->HasPage( pPage ) &&
			!CGUIWidgetManager::Instance()->HasDynamicPage( pPage ))
		{
			GUI_THROW( GUI_FORMAT( "[CGUIUICanvasLayer::OpenUIPage]: the widget <%s> isn't a page!", pPage->GetName().c_str()));
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
			GUI_THROW( GUI_FORMAT( "[CGUIUICanvasLayer::CloseUIPage]: the widget <%s> isn't a page!", pPage->GetName().c_str()));
		}

		if( pPage->IsOpen() == false )
		{
			GUI_THROW( GUI_FORMAT( "[CGUIUICanvasLayer::CloseUIPage]: the page <%s> has closed!", pPage->GetName().c_str()));
		}

		TArrayWidget::iterator itor = std::find(m_arrayOpenedPage.begin(), m_arrayOpenedPage.end(), pPage );
		if( itor == m_arrayOpenedPage.end())
		{
			GUI_THROW( GUI_FORMAT( "[CGUIUICanvasLayer::CloseUIPage]: can't find page in opend page <%s> list!", pPage->GetName().c_str()));
		}
		m_arrayOpenedPage.erase( itor );
		pPage->Close();
		pPage->SetParent(NULL);
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
			GUI_THROW( GUI_FORMAT( 
				"[CGUIUICanvasLayer::GetOpenedPageByIndex]: the given index <%d> is overflow, total opened page size is <%d>",
				nIdx, m_arrayOpenedPage.size()));
		}

		return m_arrayOpenedPage[nIdx];
	}
	//------------------------------------------------------------------------------
	bool CGUIUICanvasLayer::IsInputConsumed() const
	{
		if( m_pPopupWidget || GetTopestDialog() )
		{
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIUICanvasLayer::GetCurrentRootWidget( )
	{
		CGUIWidget* pDlgRoot = NULL;
		if( m_pPopupWidget )
		{
			return m_pPopupWidget;
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
	void CGUIUICanvasLayer::SetPopupWidget( CGUIWidget* pWidget )
	{
		if( m_pPopupWidget == pWidget )
		{
			return;
		}
		if( m_pPopupWidget )
		{
			m_pPopupWidget->GetOnClosedSignal().disconnect( this );
		}
		m_pPopupWidget = pWidget;
		if( m_pPopupWidget )
		{
			m_pPopupWidget->GetOnClosedSignal().connect( this, &CGUIUICanvasLayer::OnWidgetClosed );
		}
	}
	//------------------------------------------------------------------------------
	CGUIWidget* CGUIUICanvasLayer::GetPopupWidget( ) const
	{
		return m_pPopupWidget;
	}
	//------------------------------------------------------------------------------
	void CGUIUICanvasLayer::OnWidgetClosed(CGUIWidget* pWidget)
	{
		if( pWidget == m_pPopupWidget )
		{
			m_pPopupWidget = NULL;
		}
	}
	//------------------------------------------------------------------------------

}
