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
#include <guiframeworkeditorbase.h>
using namespace guiex;

//============================================================================//
// class
//============================================================================//
class CGUIFrameworkEditor : public CGUIFrameworkEditorBase
{
public:
	CGUIFrameworkEditor( );

	void RegisterOpenglInterface();
	void UnregisterOpenglInterface();

	void EditorSetupLogSystem( );

protected:
	virtual void RegisterInterfaces_Render( );
	virtual void SetupLogSystem( );
	virtual void PostRender( IGUIInterfaceRender* pRender );

protected:

public:
	static CGUIFrameworkEditor* ms_pFramework;
};

#endif //__KEN_FRAMEWORK_EDITOR_20101221_H__
