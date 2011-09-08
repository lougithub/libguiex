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
#include <wx/wxprec.h>
#include <libguiex_core/guiex.h>

//============================================================================//
// class
//============================================================================// 

class WxAnimationCanvas: public wxScrolledWindow
{
public:
	WxAnimationCanvas( wxWindow *parent );
	~WxAnimationCanvas();

	void OnPaint( wxPaintEvent &event );
	void OnTimer(wxTimerEvent& event);

	void SetAnimationData( guiex::CGUIAnimationData* pData )
	{
		if( m_pAnimData != pData )
		{
			m_pAnimData = pData;
			UpdaterBitmapData();
		}
	}

protected:
	void UpdaterBitmapData();
	void ClearBitmapData();

protected:
	wxTimer m_timer;

	guiex::CGUIAnimationData* m_pAnimData;

	std::vector<wxBitmap*> m_vecBitmaps;
	guiex::uint32 m_nCurrentFrame;
	float m_fElapsedTime;

	DECLARE_EVENT_TABLE()
};


class WxAnimationSelectDialog : public wxDialog
{
public:
	WxAnimationSelectDialog( wxWindow* parent );

	const wxString& GetAnimationName() const
	{
		return m_strAnimationName;
	}

protected:
	void OnOK(wxCommandEvent& WXUNUSED(event));
	void OnCANCEL(wxCommandEvent& WXUNUSED(event));
	void OnListBoxSelect(wxCommandEvent& WXUNUSED(event));

protected:
	WxAnimationCanvas* m_pAnimationCanvas;
	wxListBox* m_pListBox;
	wxString m_strAnimationName;

private:
	DECLARE_EVENT_TABLE()
};

#endif //__KEN_ANIMATION_SELECT_DLG_20110907_H__
