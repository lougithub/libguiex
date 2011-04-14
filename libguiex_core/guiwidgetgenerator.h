/** 
 * @file guiwidgetgenerator.h
 * @brief generator of the widget
 * @author ken
 * @date 2006-06-07
 */


#ifndef __GUI_WIDGETGENERATOR_20060607_H__
#define __GUI_WIDGETGENERATOR_20060607_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"

//============================================================================//
// define
//============================================================================// 
namespace guiex
{
#define GUI_WIDGET_GENERATOR_DECLARE(widget) \
public: \
	static const char* StaticGetType(); \
	friend class widget##_Generator; \
	class GUIEXPORT widget##_Generator : public CGUIWidgetGenerator \
	{ \
	public: \
		virtual CGUIWidget*	GenerateWidget(const CGUIString& rName, const CGUIString& rSceneName) const; \
		virtual void DestroyWidget(const CGUIWidget* pWidget) const; \
		static widget##_Generator* GenerateSelf(); \
		virtual void DestroySelf(); \
	protected: \
		widget##_Generator(); \
	}; 
		
#define GUI_WIDGET_GENERATOR_IMPLEMENT(widget) \
	const char* widget::StaticGetType()	\
	{return #widget;} \
	widget::widget##_Generator::widget##_Generator() \
	:CGUIWidgetGenerator(#widget) \
	{ \
	} \
	CGUIWidget* widget::widget##_Generator::GenerateWidget(const CGUIString& rName, const CGUIString& rSceneName) const \
	{ \
		return new widget(rName, rSceneName); \
	} \
	void widget::widget##_Generator::DestroyWidget(const CGUIWidget* pWidget) const \
	{ \
		delete pWidget; \
	} \
	widget::widget##_Generator* widget::widget##_Generator::GenerateSelf() \
	{ \
		return new widget::widget##_Generator; \
	} \
	void widget::widget##_Generator::DestroySelf() \
	{ \
		delete this; \
	} \
	extern "C" GUIEXPORT CGUIWidgetGenerator* GetGenerator_##widget() \
	{ \
		return widget::widget##_Generator::GenerateSelf(); \
	}

	#define GUI_CUSTOM_WIDGET_DECLARE(widget) \
	public: \
		static const char* StaticGetType();

	#define GUI_CUSTOM_WIDGET_IMPLEMENT(widget) \
	const char* widget::StaticGetType()	\
	{return #widget;}

}	//namespace guiex

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWidget;
}//namespace guiex


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIWidgetGenerator
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWidgetGenerator(const CGUIString& rString);

		/**
		* @brief destructor
		*/
		virtual ~CGUIWidgetGenerator();

		/** 
		* @brief generate a widget
		* @param unique name of the widget
		* @return pointer of the widget, return NULL for failed to generate it
		*/
		virtual CGUIWidget*	GenerateWidget( const CGUIString& m_strName, const CGUIString& rSceneName) const=0;

		/**
		* @brief destroy widget
		*/
		virtual void DestroyWidget(const CGUIWidget* pWidget) const= 0;

		/**
		* @brief destroy generator itself
		*/
		virtual void DestroySelf() = 0;

		/**
		* @brief get type of the widget
		*/
		virtual const CGUIString& GetWidgetType() const;

	protected:
		CGUIString	m_strType;
	};


}//namespace guiex

#endif //__GUI_WIDGETGENERATOR_20060607_H__
