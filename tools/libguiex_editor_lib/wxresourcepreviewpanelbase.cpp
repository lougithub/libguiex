/** 
* @file wxresourcepreviewpanelbase.cpp
* @brief 
* @author ken
* @date 2011-09-10
*/


//============================================================================//
// include
//============================================================================// 
#include "wxresourcepreviewpanelbase.h"
#include "editorwidgetid.h"
#include "toolsmisc.h"

//============================================================================//
// function
//============================================================================// 


//------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( WxResourcePreviewPanelBase, wxPanel )
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxResourcePreviewPanelBase::WxResourcePreviewPanelBase( wxWindow* parent )
:wxScrolledWindow( parent )
{
}
//------------------------------------------------------------------------------
