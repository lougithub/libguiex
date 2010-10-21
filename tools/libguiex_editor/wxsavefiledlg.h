/** 
* @file savefiledlg.h
* @brief the dialog to help save this to file
* @author ken
* @date 2007-09-25
*/

#ifndef __KEN_SAVE_FILE_DLG_20070925_H__
#define __KEN_SAVE_FILE_DLG_20070925_H__


//============================================================================//
// include
//============================================================================// 


//============================================================================//
// declare
//============================================================================// 
enum
{
	ID_DC_BTN_OK = wxID_HIGHEST+100,
	ID_DC_BTN_NO,
	ID_DC_BTN_CANCEL,
};


//============================================================================//
// class
//============================================================================// 
class WxDialogClose : public wxDialog
{
public:
	WxDialogClose(wxWindow* parent, const wxArrayString& rFileArray);

protected:
	void	OnOK(wxCommandEvent& WXUNUSED(event));
	void	OnCANCEL(wxCommandEvent& WXUNUSED(event));
	void	OnNO(wxCommandEvent& WXUNUSED(event));

private:
	DECLARE_EVENT_TABLE()

protected:
	wxArrayString m_arrayFile;
};

#endif //__KEN_SAVE_FILE_DLG_20070925_H__
