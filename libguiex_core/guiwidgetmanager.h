/** 
* @file guiwidgetmanager.h
* @brief widget manager.
* @author ken
* @date 2010-11-25
*/

#ifndef __GUI_WIDGETMANAGER_20101125_H__
#define __GUI_WIDGETMANAGER_20101125_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guiwidget.h"
#include <vector>
#include <map>
#include <set>

//============================================================================//
// declare
//============================================================================// 

#define GUI_CREATE_WIDGET(type, widgetname, scenename)		\
	::guiex::CGUIWidgetManager::Instance()->CreateWidget(type,widgetname, scenename)

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWidgetManager
	* @brief widget manager
	* 
	*/
	class GUIEXPORT CGUIWidgetManager
	{
	public:
		CGUIWidgetManager();
		virtual ~CGUIWidgetManager();

		static CGUIWidgetManager* Instance(); 

		CGUIWidget* CreateWidget( const CGUIString& rType, const CGUIString& rWidgetName, const CGUIString& rSceneName );
		template<class T>
		T* CreateWidget( const CGUIString& rWidgetName, const CGUIString& rSceneName )
		{
			CGUIWidget* pWidget = CreateWidget( T::StaticGetType(), rWidgetName, rSceneName );
			GUI_ASSERT( pWidget->GetType() == T::StaticGetType(), "wrong Widget type" );
			return static_cast<T*>( pWidget );
		}	
		bool HasWidget(  const CGUIString& rWidgetName, const CGUIString& rSceneName );
		CGUIWidget* GetWidget( const CGUIString& rWidgetName, const CGUIString& rSceneName );
		void DestroyWidget( CGUIWidget* pWidget );
		template<class T>
		T* GetWidgetWithTypeCheck( const CGUIString& rWidgetName, const CGUIString& rSceneName )
		{
			CGUIWidget* pWidget = GetWidget( rWidgetName, rSceneName );
			GUI_ASSERT( pWidget, "failed to get widget" );
			GUI_ASSERT( pWidget->GetType() == T::StaticGetType(), "wrong Widget type" );
			return static_cast<T*>( pWidget );
		}	

		CGUIWidget*	LoadPage( const CGUIString& rPageName, const CGUIString& rSceneName);
		void AddPage(  CGUIWidget* pPage, const CGUIString& rPageName = CGUIString());
		CGUIWidget* GetPageByWidgetName( const CGUIString& rWidgetName, const CGUIString& rSceneName );
		CGUIWidget* GetPageByPageName( const CGUIString& rPageName, const CGUIString& rSceneName );
		bool HasPage(const CGUIString& rWidgetName, const CGUIString& rSceneName) const;
		bool HasPage( CGUIWidget* pPage) const;
		void DestroyPage( CGUIWidget* pPage);
		void DestroyAllPages( );

		CGUIWidget*	LoadDynamicPage( const CGUIString& rPageName, const CGUIString& rSceneName, const CGUIString& rWorkingSceneName );
		void AddDynamicPage(  CGUIWidget* pPage );
		bool HasDynamicPage( CGUIWidget* pPage ) const;
		void DestroyDynamicPage( CGUIWidget* pPage);

	protected:
		//widget list
		typedef std::map<CGUIString , std::map<CGUIString, CGUIWidget*> > TMapWidget; //scene name, widget name, widget
		TMapWidget m_aMapWidget; ///contain all widget create by system which has name
		
		struct SPageInfo
		{
			CGUIWidget* m_pPage;
			CGUIString m_strPageName;
		};
		typedef std::vector< SPageInfo > TVecPage; //<widget pointer, configure file£¨page name)
		TVecPage m_vecPage;

		typedef std::set<CGUIWidget*> TDynamicPage;
		TDynamicPage m_setDynamicPageList;

	private:
		static CGUIWidgetManager* m_pSingleton;
	};

	GUIEXPORT CGUIWidget* GetWidget( const CGUIString& rWidgetName, const CGUIString& rSceneName );
	GUIEXPORT CGUIWidget* LoadDynamicPage( const CGUIString& rPageName, const CGUIString& rSceneName, const CGUIString& rWorkingSceneName );

}//namespace guiex

#endif		//__GUI_WIDGETMANAGER_20101125_H__

