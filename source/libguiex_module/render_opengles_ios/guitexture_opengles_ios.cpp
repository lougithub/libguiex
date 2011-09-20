/** 
* @file guitexture_opengles_ios.cpp
* @brief use opengl to render gui
* @author ken
* @date 2010-11-09
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/render_opengles_ios/guitexture_opengles_ios.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guidatachunk.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <libguiex_core/guiinterfaceimageloader.h>
#include <libguiex_core/guigarbagecollector.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUITexture_opengles_ios::CGUITexture_opengles_ios(IGUIInterfaceRender* pRender)
		:CGUITexture_opengl_base(pRender)
	{
	}
	//------------------------------------------------------------------------------
	CGUITexture_opengles_ios::~CGUITexture_opengles_ios()
	{

	}
	//------------------------------------------------------------------------------
}//namespace guiex
