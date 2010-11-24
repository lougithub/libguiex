/** 
* @file guiwidgetfactory.h
* @brief factory of widget, used to 
* generate and destory widget
* @author ken
* @date 2006-06-07
*/

#ifndef __GUI_WIDGETFACTORY_H_20060607__
#define __GUI_WIDGETFACTORY_H_20060607__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guisingleton.h"
#include "guistring.h"
#include <map>
#include "guidynlib.h"

//============================================================================//
// declare
//============================================================================// 

namespace guiex
{
	class CGUIWidget;
	class CGUIWidgetGenerator;

	typedef CGUIWidgetGenerator* (*FunCreateGenerator)(); 
	typedef CGUIWidgetGenerator** (*FunRegisterAllWidgets)(); 

}//namespace guiex



//============================================================================//
// define
//============================================================================// 

#define GUI_REGISTER_WIDGET_LIB(widgetname)	\
	::guiex::CGUIWidgetFactory::Instance()->RegisterGenerator(::guiex::widgetname##_Generator::GenerateSelf())


#define GUI_REGISTER_WIDGET(widgetname, dllname)	\
	CGUIWidgetFactory::Instance()->RegisterGenerator(widgetname, dllname)


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIWidgetFactory
	{
	public:
		CGUIWidgetFactory();
		~CGUIWidgetFactory();

		static CGUIWidgetFactory* Instance();

		int32 RegisterModule( const CGUIString& rModuleName );
		int32 UnregisterModule( const CGUIString& rModuleName );
		void UnregisterAllModule();

		int32 RegisterGenerator(CGUIWidgetGenerator* pGenarator);
		int32 RegisterGenerator(const CGUIString& rWidgetType, 
			const CGUIString& rModuleName, 
			const char* pFunName =NULL );

		int32 RegisterAllGenerator(const CGUIString& rModuleName, const char* pFunName =NULL );
		int32 UnregisterGenerator(CGUIWidgetGenerator* pGenerator);
		int32 UnregisterGenerator(const CGUIString& rType );
		void UnregisterAllGenerator();

		CGUIWidget* CreateWidget(const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		int32 DestoryWidget( CGUIWidget* pWidget);

	private:
		//map for generator
		typedef std::map<CGUIString, CGUIWidgetGenerator*> TMapGenerator;
		TMapGenerator m_mapGenerator;

		//map for module, the key of map is module name
		typedef std::map<CGUIString, GUI_DYNLIB_HANDLE> TMapModule;
		TMapModule m_mapModule;

	private:
		static CGUIWidgetFactory* m_pSingleton;
	};

}//namespace guiex



namespace guiex
{
	//============================================================================//
	// function
	//============================================================================// 
	/**
	* @brief get widget factory
	*/
	GUIEXPORT	CGUIWidgetFactory* GetWidgetFactory();

	/**
	* @brief register widget generator
	*/
	GUIEXPORT	int32 RegisterWidgetGenerator(
		const CGUIString& rWidgetType, 
		const CGUIString& rModuleName);

	/**
	* @brief unregister widget generator
	*/
	GUIEXPORT	int32 UnregisterWidgetGenerator(
		const CGUIString& rWidgetType);


}//namespace guiex

#endif //__GUI_WIDGETFACTORY_H_20060607__





