/** 
* @file wxfontselectdlg.h
* @brief the dialog to select font.
* @author ken
* @date 2011-04-22
*/

#ifndef __KEN_FONT_SELECT_DLG_20110422_H__
#define __KEN_FONT_SELECT_DLG_20110422_H__

//============================================================================//
// include
//============================================================================// 
#include "wxresourceselectdlgbase.h"

//============================================================================//
// class
//============================================================================// 

class WxFontSelectDialog : public WxResourceSelectDialogBase
{
public:
	WxFontSelectDialog( wxWindow* parent );

protected:
	virtual class WxResourcePreviewPanelBase* GeneratePreviewPanel( wxWindow* pParent );

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_FONT_SELECT_DLG_20110422_H__
