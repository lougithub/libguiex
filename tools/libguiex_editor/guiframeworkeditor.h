/** 
* @file guiframeworkeditor.h
* @author ken
* @date 2010-12-21
*/

#ifndef __KEN_FRAMEWORK_EDITOR_20101221_H__
#define __KEN_FRAMEWORK_EDITOR_20101221_H__

//============================================================================//
// include
//============================================================================//
#include <libguiex_framework/guiframework.h>
using namespace guiex;

//============================================================================//
// class
//============================================================================//
class CGUIFrameworkEditor : public CGUIFramework
{
public:
	CGUIFrameworkEditor( );

	void RegisterOpenglInterface();
	void UnregisterOpenglInterface();

	void SetCurrentCanvas( class WxEditorCanvas* pCanvas );

	void EditorSetupLogSystem( );

protected:
	virtual void PostRender( );
	virtual void SetupLogSystem( );

	virtual void RegisterInterfaces_Render( );

protected:
	class WxEditorCanvas* m_pCurrentCanvas;

public:
	static CGUIFrameworkEditor* ms_pFramework;
};

#endif //__KEN_FRAMEWORK_EDITOR_20101221_H__
