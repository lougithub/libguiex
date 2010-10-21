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
#define GUI_WIDGET_GENERATOR_DECLARE(widget)		\
	class GUIEXPORT widget##_Generator : public guiex::CGUIWidgetGenerator	\
	{														\
	public:													\
		virtual CGUIWidget*	GenerateWidget(const CGUIString& rName, const CGUIString& rProjectName) const;			\
		virtual void DestroyWidget(const CGUIWidget* pWidget) const;\
		static widget##_Generator* GenerateSelf();					\
		virtual void DestroySelf();							\
	protected:												\
		widget##_Generator();										\
	};														\
	extern "C" GUIEXPORT guiex::CGUIWidgetGenerator* GetGenerator_##widget();
		
#define GUI_WIDGET_GENERATOR_IMPLEMENT(widget)	\
	widget##_Generator::widget##_Generator()									\
	:CGUIWidgetGenerator(#widget)							\
	{														\
	}														\
	CGUIWidget* widget##_Generator::GenerateWidget(const CGUIString& rName, const CGUIString& rProjectName) const			\
	{														\
		return new widget(rName, rProjectName);									\
	}														\
	void widget##_Generator::DestroyWidget(const CGUIWidget* pWidget) const\
	{														\
		delete pWidget;										\
	}														\
	widget##_Generator* widget##_Generator::GenerateSelf()					\
	{														\
		return new widget##_Generator;								\
	}														\
	void widget##_Generator::DestroySelf()							\
	{														\
		delete this;									\
	}														\
	extern "C" guiex::CGUIWidgetGenerator* GetGenerator_##widget()			\
	{														\
		return widget##_Generator::GenerateSelf();					\
	}


#define GUI_WIDGET_ALLGENERATOR_DECLARE( )							\
	extern "C"  GUIEXPORT guiex::CGUIWidgetGenerator** GetAllGenerators();

#define GUI_WIDGET_ALLGENERATOR_IMPLEMENT_BEGIN( )					\
	extern "C"  GUIEXPORT guiex::CGUIWidgetGenerator** GetAllGenerators()	\
	{																\
		static std::vector<CGUIWidgetGenerator*> aList;				\
		aList.clear();

#define GUI_WIDGET_ALLGENERATOR_IMPLEMENT_ADD( widget )				\
		aList.push_back( GetGenerator_##widget());

#define GUI_WIDGET_ALLGENERATOR_IMPLEMENT_END( )					\
		aList.push_back(NULL);										\
		return (CGUIWidgetGenerator**)&aList[0];												\
	}

}	//namespace guiex


namespace guiex
{
//============================================================================//
// declare
//============================================================================// 
class CGUIWidget;
}//namespace guiex


namespace guiex
{
//============================================================================//
// class
//============================================================================// 
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
	virtual CGUIWidget*	GenerateWidget( const CGUIString& m_strName, const CGUIString& rProjectName) const=0;

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
