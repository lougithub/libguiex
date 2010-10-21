/** 
* @file mainapp.h
* @brief entry of the application
* @author ken
* @date 2007-09-10
*/

#ifndef __KEN_MAINAPP_20070910_H__
#define __KEN_MAINAPP_20070910_H__


//============================================================================//
// include
//============================================================================// 


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

	virtual		bool OnInit();

	virtual		int OnExit();

	const std::string& GetBaseDir( ) const;

protected:
	HMODULE		m_hSciDll;
	std::string	m_strBaseDir;
};

DECLARE_APP(WxMainApp)

extern WxMainFrame* GetMainFrame();



#endif //__KEN_MAINAPP_20070910_H__

