/** 
* @file savefiledlg.cpp
* @brief the dialog to help save this to file
* @author ken
* @date 2007-09-25
*/

//============================================================================//
// include
//============================================================================// 
#include "wximageselectdlg.h"
#include "wximagepreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 

//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxImageSelectDialog, WxResourceSelectDialogBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxImageSelectDialog::WxImageSelectDialog( wxWindow* parent )
:WxResourceSelectDialogBase( parent, _T("select image"), CGUIResourcePool::Instance()->GetImageList())
{	
}
//------------------------------------------------------------------------------
WxResourcePreviewPanelBase* WxImageSelectDialog::GeneratePreviewPanel( wxWindow* pParent )
{
	return new WxImagePreviewPanel(pParent);
}
//------------------------------------------------------------------------------
