/** 
* @file guitexture_opengles_android.cpp
* @brief use opengl to render gui
* @author ken
* @date 2011-04-29
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/render_opengles_android/guitexture_opengles_android.h>
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
	CGUITexture_opengles_android::CGUITexture_opengles_android(IGUIInterfaceRender* pRender)
		:CGUITexture_opengl_base(pRender)
	{
	}
	//------------------------------------------------------------------------------
	CGUITexture_opengles_android::~CGUITexture_opengles_android()
	{

	}
	//------------------------------------------------------------------------------
}//namespace guiex
