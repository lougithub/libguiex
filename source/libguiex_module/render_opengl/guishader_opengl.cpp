/** 
* @file guishader_opengl.cpp
* @brief 
* @author ken
* @date 2011-09-23
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/render_opengl/guishader_opengl.h>
#include <libguiex_module/render_opengl/guirender_opengl.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIShader_opengl::CGUIShader_opengl(IGUIInterfaceRender* pRender)
		:CGUIShader_opengl_base(pRender)
	{
	}
	//------------------------------------------------------------------------------
	CGUIShader_opengl::~CGUIShader_opengl()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIShader_opengl::LoadAndCompile(const CGUIString& rVertexShaderFileName, const CGUIString& rFragmentShaderFileName)
	{
		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
