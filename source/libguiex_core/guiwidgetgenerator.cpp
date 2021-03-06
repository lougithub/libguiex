/** 
* @file guiwidgetgenerator.cpp
* @brief generator of the widget
* @author ken
* @date 2006-06-07
*/


//============================================================================//
// include
//============================================================================// 
#include "guiwidgetgenerator.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIWidgetGenerator::CGUIWidgetGenerator(const CGUIString& rString)
		:m_strType(rString)
	{

	}
	//------------------------------------------------------------------------------
	CGUIWidgetGenerator::~CGUIWidgetGenerator()
	{
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIWidgetGenerator::GetWidgetType() const
	{
		return m_strType;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
