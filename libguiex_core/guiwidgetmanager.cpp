/** 
* @file guiwidgetmanager.h
* @brief widget manager.
* @author ken
* @date 2010-11-25
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwidgetmanager.h"
#include "guibase.h"
#include "guiwidget.h"
#include "guiwidgetfactory.h"
#include "guiexception.h"
#include "guiscenemanager.h"
#include "guisystem.h"
#include "guiconfigfileloader.h"

#include <algorithm>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUIEXPORT CGUIWidget* GetWidget(const CGUIString& rWidgetName, const CGUIString& rSceneName)
	{
		return CGUIWidgetManager::Instance()->GetWidget(rWidgetName, rSceneName);
	}
	//------------------------------------------------------------------------------
	GUIEXPORT CGUIWidget* LoadDynamicPage( const CGUIString& rPageName, const CGUIString& rSceneName, const CGUIString& rWorkingSceneName )
	{
		return CGUIWidgetManager::Instance()->LoadDynamicPage(rPageName, rSceneName, rWorkingSceneName);
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	CGUIWidgetManager * CGUIWidgetManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIWidgetManager::CGUIWidgetManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIWidgetManager::CGUIWidgetManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIWidgetManager::~CGUIWidgetManager()
	{
		//check
		GUI_ASSERT(m_vecGarbage.empty(), "[CGUIWidgetManager::~CGUIWidgetManager]: shouldn't has any garbate");
		GUI_ASSERT(m_aMapWidget.empty(), "[CGUIWidgetManager::~CGUIWidgetManager]: shouldn't has any widget");

		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	bool CGUIWidgetManager::IsInternalName( const CGUIString& rWidgetName )
	{
		return (rWidgetName.find(GUI_INTERNAL_WIDGET_FLAG) != CGUIString::npos);
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIWidgetManager::MakeInternalName( CGUIString& rWidgetName )
	{
		if( !IsInternalName( rWidgetName ))
		{
			rWidgetName += GUI_INTERNAL_WIDGET_FLAG;
		}
		return rWidgetName;
	}
	//------------------------------------------------------------------------------
	CGUIWidgetManager* CGUIWidgetManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	bool CGUIWidgetManager::TryAddToWidgetPool( CGUIWidget* pWidget )
	{
		GUI_ASSERT( pWidget, "invalid parameter" );

		if( pWidget->GetName().empty() )
		{
			GUI_THROW( GUI_FORMAT( "CGUIWidgetManager::TryAddToWidgetPool: lack name for type <%s>!", pWidget->GetType().c_str()));
			return false;
		}

		//check whether this widget has exist
		TMapWidget::iterator itorScene = m_aMapWidget.find( pWidget->GetSceneName() );
		std::map<CGUIString, CGUIWidget*>* pSceneWidgetMap = NULL;
		if( itorScene != m_aMapWidget.end())
		{
			if( itorScene->second.find( pWidget->GetName() ) != itorScene->second.end())
			{
				GUI_THROW( GUI_FORMAT( "[CGUIWidgetManager::TryAddToWidgetPool]: the widget [%s : %s] has existed!", pWidget->GetSceneName().c_str(), pWidget->GetName().c_str()));
				return false;
			}
			pSceneWidgetMap = &itorScene->second;
		}
		else
		{
			//add scene
			itorScene = m_aMapWidget.insert( std::make_pair( pWidget->GetSceneName(), std::map<CGUIString, CGUIWidget*>())).first;
			pSceneWidgetMap = &itorScene->second;
		}

		pSceneWidgetMap->insert(std::make_pair(pWidget->GetName(), pWidget));
		return true;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create a widget by type and name
	* @return pointer of created widget
	*/
	CGUIWidget* CGUIWidgetManager::CreateWidget( const CGUIString& rType, const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		CGUIWidget* pWidget = CGUIWidgetFactory::Instance()->CreateWidget( rType, rWidgetName, rSceneName );
		if( false == TryAddToWidgetPool( pWidget ))
		{
			CGUIWidgetFactory::Instance()->DestoryWidget( pWidget );
			return NULL;
		}
		return pWidget;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destroy widget
	*/
	void CGUIWidgetManager::DestroyWidget( CGUIWidget* pWidget )
	{
		if( !pWidget )
		{
			GUI_THROW(  "[CGUIWidgetManager::DestroyWidget]: Invalid parameter");
		}

		if( !pWidget->GetName().empty() )
		{
			//check
			TMapWidget::iterator itor = m_aMapWidget.find( pWidget->GetSceneName());
			if( itor == m_aMapWidget.end())
			{
				GUI_THROW( GUI_FORMAT( "[CGUIWidgetManager::DestroyWidget]: the widget <%s : %s> doesn't exist!", pWidget->GetSceneName().c_str(), pWidget->GetName().c_str()));
			}
			std::map<CGUIString, CGUIWidget*>::iterator  itor_widget = itor->second.find(pWidget->GetName());
			if( itor_widget == itor->second.end())
			{
				GUI_THROW( GUI_FORMAT( "[CGUIWidgetManager::DestroyWidget]: the widget <%s : %s> doesn't exist!", pWidget->GetSceneName().c_str(), pWidget->GetName().c_str()));
			}

			//clear widget pool
			itor->second.erase(itor_widget);
			if( itor->second.empty())
			{
				m_aMapWidget.erase( itor );
			}

			//clear page pool
			if( TryRemovePage( pWidget ) )
			{
				//is a page
			}
			else if( TryRemoveDynamicPage( pWidget ))
			{
				//is a dynamic page
			}

			//delete it
			if( pWidget->GetParent())
			{
				pWidget->SetParent(NULL);
			}
			CGUIWidgetFactory::Instance()->DestoryWidget(pWidget);
		}
		else
		{
			GUI_THROW( GUI_FORMAT( "CGUIWidgetManager::DestroyWidget: lack name for type <%s>!", pWidget->GetType().c_str()));
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidgetManager::DelayedDestroyWidget( CGUIWidget* pWidget )
	{
		GUI_ASSERT( pWidget, "invalid param" );
		pWidget->Close( );
		pWidget->SetParent( NULL );
		m_vecGarbage.push_back( pWidget );
	}
	//------------------------------------------------------------------------------
	void CGUIWidgetManager::RefreshGarbage()
	{
		if( m_vecGarbage.empty() )
		{
			return;
		}

		for( uint32 i=0; i<uint32(m_vecGarbage.size()); ++i )
		{
			CGUIWidget* pWidget = m_vecGarbage[i];
			if( pWidget )
			{
				DestroyWidget( pWidget );
			}
		}

		m_vecGarbage.clear( );
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get widget by given name
	* @return return NULL for error
	*/
	CGUIWidget* CGUIWidgetManager::GetWidget(  const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		TMapWidget::iterator itor = m_aMapWidget.find(rSceneName);
		if( itor != m_aMapWidget.end())
		{
			std::map<CGUIString, CGUIWidget*>::iterator itor_widget = itor->second.find( rWidgetName );
			if( itor_widget != itor->second.end())
			{
				return itor_widget->second;
			}
		}

		GUI_THROW( GUI_FORMAT("[CGUIWidgetManager::GetWidget]: failed to get widget by name [%s : %s]",rSceneName.c_str(), rWidgetName.c_str()));
		return NULL;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief has widget which has given name
	* @return return true for has, vice versa
	*/
	bool CGUIWidgetManager::HasWidget(  const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		TMapWidget::iterator itor = m_aMapWidget.find(rSceneName);
		if( itor != m_aMapWidget.end())
		{
			std::map<CGUIString, CGUIWidget*>::iterator itor_widget = itor->second.find( rWidgetName );
			if( itor_widget != itor->second.end())
			{
				return true;
			}
		}

		return false;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief read config file
	*/
	CGUIWidget*	CGUIWidgetManager::LoadPage( const CGUIString& rPageName, const CGUIString& rSceneName )
	{
		//load file
		CGUIString strRelPath = CGUISceneManager::Instance()->GetScenePath( rSceneName ) + rPageName;
		CGUIWidget* pPage = CGUIConfigFileLoader::LoadWidgetConfigFile( strRelPath, rSceneName, rPageName );
		if( !pPage )
		{
			return NULL;
		}

		//get page
		AddPage( pPage, rPageName );
		pPage->NotifyLoaded();
		return pPage;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get a page by name
	*/
	CGUIWidget* CGUIWidgetManager::GetPageByWidgetName( const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		for( TVecPage::iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			const SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_pPage->GetName() == rWidgetName && 
				rPageInfo.m_pPage->GetSceneName() == rSceneName )
			{
				return rPageInfo.m_pPage;
			}
		}

		GUI_THROW( GUI_FORMAT( "[CGUIWidgetManager::GetPage]: failed to get page whose name is <%s : %s>!", rSceneName.c_str(), rWidgetName.c_str()));
		return NULL;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get a page by page name and scene name
	*/
	CGUIWidget* CGUIWidgetManager::GetPage( const CGUIString& rPageName, const CGUIString& rSceneName )
	{
		for( TVecPage::iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_pPage->GetSceneName() == rSceneName && 
				rPageInfo.m_strPageName == rPageName )
			{
				return rPageInfo.m_pPage;
			}
		}

		return NULL;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidgetManager::TryRemovePage( CGUIWidget* pWidget )
	{
		for( TVecPage::iterator itor = m_vecPage.begin(); itor != m_vecPage.end(); ++itor )
		{
			SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_pPage == pWidget )
			{
				m_vecPage.erase(itor);
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	void CGUIWidgetManager::ReleaseAllPages( )
	{
		while(m_vecPage.empty() == false )
		{
			CGUIWidget* pPage = m_vecPage[0].m_pPage;
			pPage->SetParent(NULL);
			DestroyWidget(pPage);
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief is there a page whose name is given name
	*/
	bool CGUIWidgetManager::HasPage(const CGUIString& rWidgetName, const CGUIString& rSceneName) const
	{
		for( TVecPage::const_iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			const SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_pPage->GetName() == rWidgetName &&
				rPageInfo.m_pPage->GetSceneName() == rSceneName )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidgetManager::HasPage(CGUIWidget* pPage) const
	{
		TVecPage::const_iterator itor = m_vecPage.begin();
		for( ; itor != m_vecPage.end(); ++itor)
		{
			const SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_pPage == pPage )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief add a page to map
	*/
	void CGUIWidgetManager::AddPage( CGUIWidget* pPage, const CGUIString& rPageName )
	{
		GUI_ASSERT( pPage, "invalid parameter" );

		//check
		if( HasPage( pPage->GetName(), pPage->GetSceneName()))
		{
			GUI_THROW( GUI_FORMAT( "[CGUIWidgetManager::AddPage]: the page name <%s> has existed!", pPage->GetName().c_str()));
		}
		for( TVecPage::iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			const SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_strPageName == rPageName && 
				rPageInfo.m_pPage->GetSceneName() == pPage->GetSceneName() )
			{
				GUI_THROW( GUI_FORMAT( "[CGUIWidgetManager::AddPage]: the page's <%s> has existed!", rPageName.c_str()));
			}
		}

		SPageInfo aInfo;
		aInfo.m_pPage = pPage;
		aInfo.m_strPageName = rPageName;
		m_vecPage.push_back( aInfo );
	}
	//------------------------------------------------------------------------------
	/**
	* @brief read config file
	*/
	CGUIWidget*	CGUIWidgetManager::LoadDynamicPage( 
		const CGUIString& rPageName,
		const CGUIString& rSceneName,
		const CGUIString& rWorkingSceneName )
	{
		//load file
		CGUIString strRelPath = CGUISceneManager::Instance()->GetScenePath( rSceneName ) + rPageName;
		CGUIString strDynamicSceneName = rSceneName + GSystem->GenerateAnonymousName();
		CGUIWidget* pPage = CGUIConfigFileLoader::LoadWidgetConfigFile( strRelPath, strDynamicSceneName, rPageName );
		if( !pPage )
		{
			GUI_THROW( GUI_FORMAT(
				"[CGUIWidgetManager::LoadDynamicPage]: failed to load config file <%s>!",
				strRelPath.c_str()));
			return NULL;
		}

		//get page
		pPage->SetWorkingSceneName(rWorkingSceneName);
		AddDynamicPage( pPage );
		pPage->NotifyLoaded();
		return pPage;
	}
	//------------------------------------------------------------------------------
	void CGUIWidgetManager::AddDynamicPage( CGUIWidget* pPage )
	{
		GUI_ASSERT( HasDynamicPage( pPage )==false, "the dynamic page has existed" );
		m_setDynamicPageList.insert( pPage );
	}
	//------------------------------------------------------------------------------
	bool CGUIWidgetManager::HasDynamicPage( CGUIWidget* pPage ) const
	{
		if( m_setDynamicPageList.find( pPage ) != m_setDynamicPageList.end() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIWidgetManager::TryRemoveDynamicPage( CGUIWidget* pWidget )
	{
		TDynamicPage::iterator itor = m_setDynamicPageList.begin();
		if( itor != m_setDynamicPageList.end() )
		{
			m_setDynamicPageList.erase(itor);
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
