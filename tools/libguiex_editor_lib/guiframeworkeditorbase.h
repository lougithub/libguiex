/** 
* @file guiframeworkeditorbase.h
* @author ken
* @date 2010-12-21
*/

#ifndef __KEN_FRAMEWORK_EDITORBASE_20101221_H__
#define __KEN_FRAMEWORK_EDITORBASE_20101221_H__

//============================================================================//
// include
//============================================================================//
#include <libguiex_framework/guiframework.h>
using namespace guiex;

//============================================================================//
// class
//============================================================================//
class CGUIFrameworkEditorBase : public CGUIFramework
{
public:
	CGUIFrameworkEditorBase( );

protected:
	virtual void RegisterInterfaces_ImageLoader( );
	virtual void RegisterWidgetGenerators( );
};

#endif //__KEN_FRAMEWORK_EDITORBASE_20101221_H__
