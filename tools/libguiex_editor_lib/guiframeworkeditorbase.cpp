/** 
* @file guiframeworkeditorbase.cpp
* @author ken
* @date 2010-12-21
*/


//============================================================================//
// include
//============================================================================//
#include "guiframeworkeditorbase.h"
#include "guiimageloader_editor.h"

#include <libguiex_module\render_opengl\guirender_opengl.h>

//game specified
#include "tdwgt_game_td.h"

#include <fstream>





//============================================================================//
// function
//============================================================================//
//------------------------------------------------------------------------------
CGUIFrameworkEditorBase::CGUIFrameworkEditorBase( )
:CGUIFramework( )
{
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditorBase::RegisterWidgetGenerators( )
{
	CGUIFramework::RegisterWidgetGenerators();

	//game_td widgets
	{
		CGUIWidgetGenerator** pGenerator = GetAllWidgetGenerators_Game_TD();
		while(*pGenerator)
		{
			CGUIWidgetFactory::Instance()->RegisterGenerator( *pGenerator);
			pGenerator ++;
		}
	}
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditorBase::RegisterInterfaces_ImageLoader( )
{
	GUI_REGISTER_INTERFACE_LIB( IGUIImageLoader_Editor);
}
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
