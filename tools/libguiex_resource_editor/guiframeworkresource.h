/** 
* @file guiframeworkresource.h
* @author ken
* @date 2011-09-08
*/

#ifndef __KEN_FRAMEWORK_RESOURCE_20110908_H__
#define __KEN_FRAMEWORK_RESOURCE_20110908_H__

//============================================================================//
// include
//============================================================================//
#include <guiframeworkeditorbase.h>
using namespace guiex;

//============================================================================//
// class
//============================================================================//
class CGUIFrameworkResource : public CGUIFrameworkEditorBase
{
public:
	CGUIFrameworkResource( );
	void EditorSetupLogSystem( );
	void RegisterOpenglInterface();
	void UnregisterOpenglInterface();

protected:
	virtual void RegisterWidgetGenerators( );
	virtual void RegisterInterfaces_Render( );
	virtual void SetupLogSystem( );
	
protected:

public:
	static CGUIFrameworkResource* ms_pFramework;
};

#endif //__KEN_FRAMEWORK_RESOURCE_20110908_H__
