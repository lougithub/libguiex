/** 
* @file outputpanel.h
* @brief panel for outputting message
* @author ken
* @date 2007-09-12
*/

#ifndef __KEN_OUTPUT_PANEL_20070912_H__
#define __KEN_OUTPUT_PANEL_20070912_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>

//============================================================================//
// class
//============================================================================// 
class WxOutputPanel : public wxTextCtrl
{
public:
	WxOutputPanel( wxWindow *parent, wxWindowID id );
	~WxOutputPanel(){};

protected:
	wxStaticText*		m_pTextCtrl;
};

#endif	//__KEN_OUTPUT_PANEL_20070912_H__
