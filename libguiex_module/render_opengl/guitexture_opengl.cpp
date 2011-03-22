/** 
* @file guitexture_opengl.cpp
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\render_opengl\guitexture_opengl.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guidatachunk.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <libguiex_core/guiinterfaceimageloader.h>

#if defined(GUIEX_PLATFORM_WIN32)
#include <windows.h>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUITexture_opengl::CGUITexture_opengl(IGUIInterfaceRender* pRender)
		:CGUITexture_opengl_base(pRender)
	{
	}
	//------------------------------------------------------------------------------
	CGUITexture_opengl::~CGUITexture_opengl()
	{
	}
	//------------------------------------------------------------------------------

}//namespace guiex
