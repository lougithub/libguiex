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



//============================================================================//
// declare
//============================================================================// 
#define NOTEBOOK_PAGE_PROP	0
#define NOTEBOOK_PAGE_IMAGE 1
#define NOTEBOOK_PAGE_EVENT	2
#define NOTEBOOK_APPEARANCE_PAGE_NAME	"Appearance"
#define NOTEBOOK_EVENT_PAGE_NAME		"CallbackEvent"
#define NOTEBOOK_IMAGE_PAGE_NAME		"Image"


class CProperty;


//load widget config to property sheet
extern void LoadWidgetConfig( wxPropertyGridManager* pSheetMgr, const std::string& rType,guiex::CGUIWidget* pWidget = NULL );

//Add Property to property sheet
extern void	SetPropertyByType( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, const guiex::CGUIProperty* pDefaultProp, guiex::CGUIWidget* pWidget );

//set property sheet by event category
extern void	SetPropertyPageByEvent(wxPropertyGridManager* pSheetMgr, const std::string& rEvent );

//generate gui property
extern void	GenerateGUIProperty( wxPropertyGridManager* pSheetMgr, wxPGProperty* pPGProperty, guiex::CGUIProperty& rProperty );

//generate gui property
extern void GenerateGUIProperty( wxPropertyGridManager* pSheetMgr, guiex::CGUIProperty& rSet );

//create a widget from property sheet
extern guiex::CGUIWidget* CreateWidget( wxPropertyGridManager* pSheetMgr, const char* strType, const char* strName, guiex::CGUIWidget* pParent);

//update image used in the system
extern void	UpdateImageNameList();

//update as used in the system
extern void	UpdateAsNameList();

#endif	//__KEN_PROPERTYSHEETFUNC_20070928_H__
