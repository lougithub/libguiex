/** 
* @file wxsoundselectdlg.cpp
* @brief 
* @author ken
* @date 2010-01-08
*/

//============================================================================//
// include
//============================================================================// 
#include "wxsoundselectdlg.h"
#include "wxsoundpreviewpanel.h"
#include "guiresourcepool.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 



//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxSoundSelectDialog, WxResourceSelectDialogBase )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxSoundSelectDialog::WxSoundSelectDialog( wxWindow* parent )
:WxResourceSelectDialogBase( parent, _T("select sound"), CGUIResourcePool::Instance()->GetSoundList())
{

}
//------------------------------------------------------------------------------
WxResourcePreviewPanelBase* WxSoundSelectDialog::GeneratePreviewPanel( wxWindow* pParent )
{
	return new WxSoundPreviewPanel(pParent);
}
//------------------------------------------------------------------------------
