/** 
* @file wximageselectdlg.h
* @brief the dialog to select image file
* @author ken
* @date 2010-12-28
*/

#ifndef __KEN_IMAGE_SELECT_DLG_20101228_H__
#define __KEN_IMAGE_SELECT_DLG_20101228_H__

//============================================================================//
// include
//============================================================================// 
#include "wxresourceselectdlgbase.h"


//============================================================================//
// class
//============================================================================//

class WxImageSelectDialog : public WxResourceSelectDialogBase
{
public:
	WxImageSelectDialog( wxWindow* parent );

protected:
	virtual class WxResourcePreviewPanelBase* GeneratePreviewPanel( wxWindow* pParent );

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_IMAGE_SELECT_DLG_20101228_H__
