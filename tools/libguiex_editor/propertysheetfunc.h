/** 
* @file propertysheetfunc.h
* @brief used to manipulate the sheet
* @author ken
* @date 2007-09-28
*/

#ifndef __KEN_PROPERTYSHEETFUNC_20070928_H__
#define __KEN_PROPERTYSHEETFUNC_20070928_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>
#include <libguiex_core/guiex.h>

using namespace guiex;
class wxPropertyGridManager;

//============================================================================//
// declare
//============================================================================// 
extern void UpdateGridProperties( wxPropertyGridManager* pSheetMgr, const std::string& rWidgetType,CGUIWidget* pWidget = NULL );
extern void	UpdateGridProperty( wxPropertyGridManager* pSheetMgr, CGUIWidget* pWidget, const CGUIString& rPropertyName, const CGUIString& rPropertyType );

extern void GenerateGUIProperties( wxPropertyGridManager* pSheetMgr, CGUIProperty& rSet );


//============================================================================//
// class
//============================================================================// 
class CWidgetPropertyCache
{
public:
	CWidgetPropertyCache();
	CWidgetPropertyCache( CGUIWidget* pWidget );

	void SetCache( CGUIWidget* pWidget );
	bool HasCache() const;
	
	CGUIWidget*GenerateWidget( );
	CGUIWidget*GenerateWidget( const CGUIString& rWidgetName );
	CGUIWidget*GenerateWidget( const CGUIString& rWidgetName, const CGUIString& rParentName );

	const CGUIString& GetWidgetType() const;

protected:
	CGUIProperty m_aProperty;
	CGUIString m_aWidgetName;
	CGUIString m_aWidgetType;
};




#endif	//__KEN_PROPERTYSHEETFUNC_20070928_H__
