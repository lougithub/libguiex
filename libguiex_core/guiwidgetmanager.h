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
#include "guiwidgetfactory.h"
#include <vector>
#include <map>
#include <set>

//============================================================================//
// declare
//============================================================================// 

#define GUI_CREATE_WIDGET(type, widgetname, scenename)		\
	::guiex::CGUIWidgetManager::Instance()->CreateWidget(type,widgetname, scenename)

#define GUI_INTERNAL_WIDGET_FLAG	"__internal__"

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

		//widget related function
		CGUIWidget* CreateWidget( const CGUIString& rType, const CGUIString& rWidgetName, const CGUIString& rSceneName );
		template<class T>
		T* CreateWidget( const CGUIString& rWidgetName, const CGUIString& rSceneName );
		template<class T>
		T* CreateCustomWidget( const CGUIString& rWidgetName, const CGUIString& rSceneName );
		bool HasWidget(  const CGUIString& rWidgetName, const CGUIString& rSceneName );
		CGUIWidget* GetWidget( const CGUIString& rWidgetName, const CGUIString& rSceneName );
		void DestroyWidget( CGUIWidget* pWidget );
		void DelayedDestroyWidget( CGUIWidget* pWidget );
		template<class T>
		T* GetWidgetWithTypeCheck( const CGUIString& rWidgetName, const CGUIString& rSceneName );

		//page related functions
		CGUIWidget*	LoadPage( const CGUIString& rSceneName, const CGUIString& rPageName );
		void AddPage(  CGUIWidget* pPage, const CGUIString& rPageName = CGUIString());
		CGUIWidget* GetPageByWidgetName( const CGUIString& rWidgetName, const CGUIString& rSceneName );
		CGUIWidget* GetPage( const CGUIString& rPageName , const CGUIString& rSceneName );
		bool HasPage(const CGUIString& rWidgetName, const CGUIString& rSceneName) const;
		bool HasPage( CGUIWidget* pPage) const;
		void ReleaseAllPages( );

		//dynamic page related functions
		CGUIWidget*	LoadDynamicPage( const CGUIString& rPageName, const CGUIString& rSceneName, const CGUIString& rWorkingSceneName );
		void AddDynamicPage(  CGUIWidget* pPage );
		bool HasDynamicPage( CGUIWidget* pPage ) const;

		void RefreshGarbage();

		static bool IsInternalName( const CGUIString& rWidgetName );
		static const CGUIString& MakeInternalName( CGUIString& rWidgetName );

	protected:
		bool TryRemovePage( CGUIWidget* pWidget );
		bool TryRemoveDynamicPage( CGUIWidget* pWidget );
		bool TryAddToWidgetPool( CGUIWidget* pWidget );

	protected:

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

		std::vector<CGUIWidget*> m_vecGarbage; //garbage

	private:
		static CGUIWidgetManager* m_pSingleton;
	};

	template< class T >
	inline T* CGUIWidgetManager::CreateWidget( const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		CGUIWidget* pWidget = CreateWidget( T::StaticGetType(), rWidgetName, rSceneName );
		GUI_ASSERT( pWidget->GetType() == T::StaticGetType(), "wrong Widget type" );
		return static_cast<T*>( pWidget );
	}	

	template< class T >
	inline T* CGUIWidgetManager::CreateCustomWidget( const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		T* pWidget = new T( rWidgetName, rSceneName );
		GUI_ASSERT( pWidget->GetType() == T::StaticGetType(), "wrong Widget type" );
		if( false == TryAddToWidgetPool( pWidget ))
		{
			CGUIWidgetFactory::Instance()->DestoryWidget( pWidget );
			return NULL;
		}
		return pWidget;
	}	

	template<class T>
	inline T* CGUIWidgetManager::GetWidgetWithTypeCheck( const CGUIString& rWidgetName, const CGUIString& rSceneName )
	{
		CGUIWidget* pWidget = GetWidget( rWidgetName, rSceneName );
		GUI_ASSERT( pWidget, "failed to get widget" );
		GUI_ASSERT( pWidget->GetType() == T::StaticGetType(), "wrong Widget type" );
		return static_cast<T*>( pWidget );
	}	

	GUIEXPORT CGUIWidget* GetWidget( const CGUIString& rWidgetName, const CGUIString& rSceneName );
	GUIEXPORT CGUIWidget* LoadDynamicPage( const CGUIString& rPageName, const CGUIString& rSceneName, const CGUIString& rWorkingSceneName );

}//namespace guiex

#endif		//__GUI_WIDGETMANAGER_20101125_H__

