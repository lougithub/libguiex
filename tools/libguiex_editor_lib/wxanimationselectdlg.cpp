/** 
* @file wxanimationselectdlg.cpp
* @brief the dialog to select animation file
* @author ken
* @date 2011-09-07
*/

//============================================================================//
// include
//============================================================================// 
#include "wxanimationselectdlg.h"
#include "wxanimationpreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"

//============================================================================//
// function
//============================================================================// 

//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxAnimationSelectDialog, WxResourceSelectDialogBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxAnimationSelectDialog::WxAnimationSelectDialog( wxWindow* parent )
:WxResourceSelectDialogBase( parent, _T("select animation"), CGUIResourcePool::Instance()->GetAnimationList() )
{
}
//------------------------------------------------------------------------------
WxResourcePreviewPanelBase* WxAnimationSelectDialog::GeneratePreviewPanel( wxWindow* pParent )
{
	return new WxAnimationPreviewPanel(pParent);
}
//------------------------------------------------------------------------------
