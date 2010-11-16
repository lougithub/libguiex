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
		/**
		* @brief register module
		* @return 
		*		- 0 for success
		*		- -1 for failed
		* @exception CGUIException
		*/
		int32	RegisterModule( const CGUIString& rModuleName );

		/**
		* @brief register module
		* @return 
		*		- 0 for success
		*		- -1 for failed
		* @exception CGUIException
		*/
		int32	UnregisterModule( const CGUIString& rModuleName );

		/**
		* @brief unregister all module
		*/
		void	UnregisterAllModule();

		/**
		* @brief register generator
		* @return 
		*		- 0 for success
		*		- -1 for failed
		* @exception CGUIException
		*/
		int32	RegisterGenerator(CGUIWidgetGenerator* pGenarator);

		/**
		* @brief register generator
		* @param pFunName string name of function which create the generator,
		* this function is located in the dll,if the value is NULL,the default
		* name is GetGenerator_<rWidgetType>
		* @return 
		*		- 0 for success
		*		- -1 for failed
		* @exception CGUIException
		*/
		int32	RegisterGenerator(const CGUIString& rWidgetType, 
			const CGUIString& rModuleName, 
			const char* pFunName =NULL );

		/**
		* @brief register all widget generator in this module
		* @param pFunName string name of function which create all generator,
		* this function is located in the dll,if the value is NULL,the default
		* name is GetAllGenerator_<rModuleName>
		* @return 
		*		- 0 for success
		*		- -1 for failed
		* @exception CGUIException
		*/
		int32	RegisterAllGenerator(const CGUIString& rModuleName, const char* pFunName =NULL );

		/**
		* @brief unregister a generator
		* @param pGenerator the pointer of a generator
		* @return 
		*		- 0 for success
		*		- -1 for failed
		* @exception CGUIException
		*/
		int32	UnregisterGenerator(CGUIWidgetGenerator* pGenerator);

		/**
		* @brief ungister a generator
		* @param rType the type of a generator
		* @return 
		*		- 0 for success
		*		- -1 for failed
		* @exception CGUIException
		*/
		int32 UnregisterGenerator(const CGUIString& rType );

		/**
		* @brief unregister all generator
		*/
		void UnregisterAllGenerator();

		/**
		* @brief create a widget by widget name
		* @return pointer of widget, NULL for failed to find this widget
		* @exception CGUIException
		*/
		CGUIWidget* CreateWidget(const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief destroy widget
		* @return 
		*	- 0 for success
		*	- -1 for failed
		*/
		int32	DestoryWidget( CGUIWidget* pWidget);

	protected:
		//for singleton
		CGUIWidgetFactory();
		~CGUIWidgetFactory();

	private:
		//map for generator
		typedef std::map<CGUIString, CGUIWidgetGenerator*> TMapGenerator;
		TMapGenerator	m_mapGenerator;

		//map for module, the key of map is module name
		typedef std::map<CGUIString, GUI_DYNLIB_HANDLE> TMapModule;
		TMapModule		m_mapModule;

	public:
		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUIWidgetFactory);	
	};

	//implement for singleton
	//GUI_SINGLETON_IMPLEMENT(CGUIWidgetFactory, CGUIWidgetFactory );

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





