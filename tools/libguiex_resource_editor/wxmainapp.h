/** 
* @file mainapp.h
* @brief entry of the application
* @author ken
* @date 2011-09-08
*/

#ifndef __KEN_MAINAPP_20110908_H__
#define __KEN_MAINAPP_20110908_H__

//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>


//============================================================================//
// declare
//============================================================================// 
class WxMainFrame;

//============================================================================//
// class
//============================================================================// 
class WxMainApp : public wxApp
{
public:
	WxMainApp();

	virtual bool OnInit();
	virtual int OnExit();

	const std::string& GetBaseDir( ) const;

protected:
	HMODULE	m_hSciDll;
	std::string	m_strBaseDir;
};

DECLARE_APP(WxMainApp)

extern WxMainFrame* GetMainFrame();

#endif //__KEN_MAINAPP_20110908_H__
