/** 
* @file wxfontselectdlg.cpp
* @brief the dialog to select font.
* @author ken
* @date 2011-04-22
*/

//============================================================================//
// include
//============================================================================// 
#include "wxfontselectdlg.h"
#include "wxfontpreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxFontSelectDialog, WxResourceSelectDialogBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxFontSelectDialog::WxFontSelectDialog( wxWindow* parent )
:WxResourceSelectDialogBase( parent, _T("select font"), CGUIResourcePool::Instance()->GetFontList())
{	
}
//------------------------------------------------------------------------------
WxResourcePreviewPanelBase* WxFontSelectDialog::GeneratePreviewPanel( wxWindow* pParent )
{
	return new WxFontPreviewPanel(pParent);
}
//------------------------------------------------------------------------------
