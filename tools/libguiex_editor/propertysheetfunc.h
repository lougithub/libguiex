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
extern void UpdateGridProperties( wxPropertyGridManager* pSheetMgr, const std::string& rType,CGUIWidget* pWidget = NULL );
extern void	UpdateGridAndGuiProperty( wxPropertyGridManager* pSheetMgr, CGUIWidget* pWidget, const CGUIString& rPropertyName, const CGUIString& rPropertyType );

extern void GenerateGUIProperties( wxPropertyGridManager* pSheetMgr, CGUIProperty& rSet );





#endif	//__KEN_PROPERTYSHEETFUNC_20070928_H__
