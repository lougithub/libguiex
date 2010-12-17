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
		GUI_ASSERT(m_aMapWidget.empty(), "shouldn't has any widget in map");

		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	bool CGUIWidgetManager::IsInternalWidget( const CGUIString& rWidgetName )
	{
		return (rWidgetName.find(GUI_INTERNAL_WIDGET_FLAG) != CGUIString::npos);
	}
	//------------------------------------------------------------------------------
	CGUIWidgetManager* CGUIWidgetManager::Instance()
	{
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create a widget by type and name
	* @return pointer of created widget
	*/
	CGUIWidget* CGUIWidgetManager::CreateWidget( const CGUIString& rType, const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		CGUIWidget* pWidget = NULL;

		if( !rWidgetName.empty() && !rSceneName.empty() )
		{
			//check whether this widget has exist
			TMapWidget::iterator itor = m_aMapWidget.find( rSceneName );
			std::map<CGUIString, CGUIWidget*>* pWidgetMap = NULL;
			if( itor != m_aMapWidget.end())
			{
				if( itor->second.find( rWidgetName ) != itor->second.end())
				{
					throw CGUIException( "[CGUIWidgetManager::CreateWidget]: the widget [%s : %s] has existed!", rSceneName.c_str(), rWidgetName.c_str());
				}
				pWidgetMap = &itor->second;
			}
			else
			{
				//add scene
				itor = m_aMapWidget.insert( std::make_pair( rSceneName, std::map<CGUIString, CGUIWidget*>())).first;
				pWidgetMap = &itor->second;
			}

			pWidget = CGUIWidgetFactory::Instance()->CreateWidget( rType, rWidgetName, rSceneName );
			pWidgetMap->insert(std::make_pair(rWidgetName, pWidget));
		}
		else
		{
			throw CGUIException( "CGUIWidgetManager::CreateWidget: lack name for type <%s>!", rType.c_str());
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
			throw CGUIException( "[CGUIWidgetManager::DestroyWidget]: Invalid parameter");
		}

		if( !pWidget->GetName().empty() && !pWidget->GetSceneName().empty())
		{
			//check
			TMapWidget::iterator itor = m_aMapWidget.find( pWidget->GetSceneName());
			if( itor  == m_aMapWidget.end())
			{
				throw CGUIException( "[CGUIWidgetManager::DestroyWidget]: the widget <%s : %s> doesn't exist!", pWidget->GetSceneName().c_str(), pWidget->GetName().c_str());
			}
			std::map<CGUIString, CGUIWidget*>::iterator  itor_widget = itor->second.find(pWidget->GetName());
			if( itor_widget  == itor->second.end())
			{
				throw CGUIException( "[CGUIWidgetManager::DestroyWidget]: the widget <%s : %s> doesn't exist!", pWidget->GetSceneName().c_str(), pWidget->GetName().c_str());
			}

			//delete it
			if( pWidget->GetParent())
			{
				pWidget->SetParent(NULL);
			}
			CGUIWidgetFactory::Instance()->DestoryWidget(pWidget);
			itor->second.erase(itor_widget);
			if( itor->second.empty())
			{
				m_aMapWidget.erase( itor );
			}
		}
		else
		{
			throw CGUIException( "CGUIWidgetManager::DestroyWidget: lack name for type <%s>!", pWidget->GetType().c_str());
		}
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

		throw CGUIException("[CGUIWidgetManager::GetWidget]: failed to get widget by name [%s : %s]",rSceneName.c_str(), rWidgetName.c_str());
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

		throw CGUIException( "[CGUIWidgetManager::GetPage]: failed to get page whose name is <%s : %s>!", rSceneName.c_str(), rWidgetName.c_str());
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
	void CGUIWidgetManager::ReleasePageImp( CGUIWidget* pPage )
	{
		if( pPage->IsOpen() )
		{
			throw CGUIException( "[CGUIWidgetManager::ReleasePage]: can't delete widget <%s> who is still opened!", pPage->GetName().c_str());
		}
		if( pPage->GetParent())
		{
			throw CGUIException( "[CGUIWidgetManager::ReleasePage]: can't delete widget <%s> who is still has a father!", pPage->GetName().c_str());
		}

		DestroyWidget(pPage);
	}
	//------------------------------------------------------------------------------
	void CGUIWidgetManager::ReleasePage( const CGUIString& rSceneName, const CGUIString& rPageName )
	{
		CGUIWidget* pPage = NULL;
		for( TVecPage::iterator itor = m_vecPage.begin(); itor != m_vecPage.end(); ++itor )
		{
			SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_strPageName == rPageName &&
				rPageInfo.m_pPage->GetSceneName() == rSceneName )
			{
				pPage = rPageInfo.m_pPage;
				m_vecPage.erase(itor);
				break;
			}
		}

		if( !pPage )
		{
			throw CGUIException( 
				"[CGUIWidgetManager::ReleasePage]: failed to find page whose scene name is <%s> and page name is <%s>!", 
				rSceneName.c_str(), rPageName.c_str() );
			return;
		}

		ReleasePageImp( pPage );
	}
	//------------------------------------------------------------------------------
	/**
	* @brief delete specify page
	*/
	void CGUIWidgetManager::ReleasePage( CGUIWidget* pPage)
	{
		GUI_ASSERT( pPage, "invalid parameter");

		bool bFound = false;
		for( TVecPage::iterator itor = m_vecPage.begin(); itor != m_vecPage.end(); ++itor )
		{
			SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_pPage == pPage )
			{
				m_vecPage.erase(itor);
				bFound = true;
				break;
			}
		}

		if( !bFound )
		{
			throw CGUIException( "[CGUIWidgetManager::ReleasePage]: failed to find page whose name is <%s>!", pPage->GetName().c_str());
			return;
		}

		ReleasePageImp( pPage );
	}
	//------------------------------------------------------------------------------
	void CGUIWidgetManager::ReleaseAllPages( )
	{
		while(m_vecPage.empty() == false )
		{
			CGUIWidget* pPage = m_vecPage[0].m_pPage;
			pPage->SetParent(NULL);
			ReleasePage(pPage);
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
			throw CGUIException( "[CGUIWidgetManager::AddPage]: the page name <%s> has existed!", pPage->GetName().c_str());
		}
		for( TVecPage::iterator itor = m_vecPage.begin();
			itor != m_vecPage.end();
			++itor)
		{
			const SPageInfo& rPageInfo = *itor;
			if( rPageInfo.m_strPageName == rPageName && 
				rPageInfo.m_pPage->GetSceneName() == pPage->GetSceneName() )
			{
				throw CGUIException( "[CGUIWidgetManager::AddPage]: the page's <%s> has existed!", rPageName.c_str());
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
			throw CGUIException(
				"[CGUIWidgetManager::LoadDynamicPage]: failed to load config file <%s>!",
				strRelPath.c_str());
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
	void CGUIWidgetManager::DestroyDynamicPage( CGUIWidget* pPage)
	{
		GUI_ASSERT( pPage, "invalid parameter");

		TDynamicPage::iterator itor = m_setDynamicPageList.begin();
		if( itor == m_setDynamicPageList.end())
		{
			throw CGUIException( "[CGUIWidgetManager::DestroyDynamicPage]: failed to find dynamic page whose name is <%s>!", pPage->GetName().c_str());
		}
		if( pPage->IsOpen() )
		{
			throw CGUIException( "[CGUIWidgetManager::DestroyDynamicPage]: can't delete widget <%s> who is still opened!", pPage->GetName().c_str());
		}
		if( pPage->GetParent())
		{
			throw CGUIException( "[CGUIWidgetManager::DestroyDynamicPage]: can't delete widget <%s> who is still has a father!", pPage->GetName().c_str());
		}

		m_setDynamicPageList.erase(itor);
		DestroyWidget(pPage);
	}
	//------------------------------------------------------------------------------

}//namespace guiex
