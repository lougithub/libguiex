/** 
* @file guishader_opengl.h
* @brief 
* @author ken
* @date 2011-09-23
*/

#ifndef __GUI_SHADER_OPENGL_20110923_H__
#define __GUI_SHADER_OPENGL_20110923_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/render_opengl_base/guishader_opengl_base.h>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIRender_opengl_base;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	class GUIEXPORT CGUIShader_opengl : public CGUIShader_opengl_base
	{
	public:
		virtual ~CGUIShader_opengl();

	protected:
		friend class IGUIRender_opengl;
		CGUIShader_opengl(IGUIInterfaceRender* pRender);

		int32 LoadAndCompile(const CGUIString& rVertexShaderFileName, const CGUIString& rFragmentShaderFileName);

	protected:
	};
}//namespace guiex

#endif //__GUI_SHADER_OPENGL_20110923_H__
