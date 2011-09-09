/** 
* @file wxsoundselectdlg.h
* @brief the dialog to select sound file
* @author ken
* @date 2010-12-28
*/

#ifndef __KEN_SOUND_SELECT_DLG_20110108_H__
#define __KEN_SOUND_SELECT_DLG_20110108_H__

//============================================================================//
// include
//============================================================================// 
#include "wxresourceselectdlgbase.h"

//============================================================================//
// class
//============================================================================// 

class WxSoundSelectDialog : public WxResourceSelectDialogBase
{
public:
	WxSoundSelectDialog( wxWindow* parent );

protected:
	void OnPlay(wxCommandEvent& WXUNUSED(event));

protected:

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_SOUND_SELECT_DLG_20110108_H__
