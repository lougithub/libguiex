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

//============================================================================//
// declare
//============================================================================// 
#define NOTEBOOK_PAGE_APPEARANCE	0
#define NOTEBOOK_PAGE_IMAGE 1
#define NOTEBOOK_PAGE_EVENT	2
#define NOTEBOOK_APPEARANCE_PAGE_NAME	"Appearance"
#define NOTEBOOK_EVENT_PAGE_NAME "Event"
#define NOTEBOOK_IMAGE_PAGE_NAME "Image"

extern void	SelectGridPropertyPage(wxPropertyGridManager* pSheetMgr, const std::string& rEvent );
extern void UpdateGridProperties( wxPropertyGridManager* pSheetMgr, const std::string& rType,CGUIWidget* pWidget = NULL );
extern void	UpdateGridProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, const CGUIProperty& aProp );
extern void	UpdateGridAndGuiProperty( wxPropertyGridManager* pSheetMgr, CGUIWidget* pWidget, const CGUIString& rPropertyName );

extern void	GenerateGUIProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, CGUIProperty& rProperty );
extern void GenerateGUIProperties( wxPropertyGridManager* pSheetMgr, CGUIProperty& rSet );





#endif	//__KEN_PROPERTYSHEETFUNC_20070928_H__
