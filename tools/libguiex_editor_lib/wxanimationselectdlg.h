/** 
* @file wxanimationselectdlg.h
* @brief the dialog to select animation file
* @author ken
* @date 2011-09-07
*/

#ifndef __KEN_ANIMATION_SELECT_DLG_20110907_H__
#define __KEN_ANIMATION_SELECT_DLG_20110907_H__

//============================================================================//
// include
//============================================================================//
#include "wxresourceselectdlgbase.h"


//============================================================================//
// class
//============================================================================// 
class WxAnimationSelectDialog : public WxResourceSelectDialogBase
{
public:
	WxAnimationSelectDialog( wxWindow* parent );

protected:
	virtual class WxResourcePreviewPanelBase* GeneratePreviewPanel( wxWindow* pParent );

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_ANIMATION_SELECT_DLG_20110907_H__
