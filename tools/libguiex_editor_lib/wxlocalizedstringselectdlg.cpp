/** 
* @file wxlocalizationselectdlg.cpp
* @brief the dialog to select localization.
* @author ken
* @date 2011-04-06
*/

//============================================================================//
// include
//============================================================================// 
#include "wxlocalizedstringselectdlg.h"
#include "wxlocalizedstringpreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxLocalizedstringSelectDialog, WxResourceSelectDialogBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxLocalizedstringSelectDialog::WxLocalizedstringSelectDialog( wxWindow* parent )
:WxResourceSelectDialogBase( parent, _T("select localization"), CGUIResourcePool::Instance()->GetLocalizationList())
{	
}
//------------------------------------------------------------------------------
WxResourcePreviewPanelBase* WxLocalizedstringSelectDialog::GeneratePreviewPanel( wxWindow* pParent )
{
	return new WxLocalizedStringPreviewPanel(pParent);
}
//------------------------------------------------------------------------------
