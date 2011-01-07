/** 
* @file wxtoolspgmanager.h
* @brief 
* @author ken
* @date 2011-01-05
*/

#ifndef __KEN_TOOLSPGMAMAGER_20110105_H__
#define __KEN_TOOLSPGMAMAGER_20110105_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include <wx/propgrid/manager.h>
#include <string>
#include <vector>

//============================================================================//
// class
//============================================================================// 
class WxToolsPGManager : public wxPropertyGridManager
{
public:
	WxToolsPGManager(  wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style );

	void ToolsClearPage( );
	void ToolsSelectPage( const std::string& rPageName );
	wxPGProperty* ToolsGetProperty( const std::string& rName, const std::string& rType ) const;

protected:
	std::vector<std::string> m_vecPropertyPages;
};

#endif //__KEN_TOOLSPGMAMAGER_20110105_H__
