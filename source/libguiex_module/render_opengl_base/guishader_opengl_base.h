/** 
* @file guishader_opengl_base.h
* @brief 
* @author ken
* @date 2011-09-23
*/

#ifndef __GUI_SHADER_OPENGL_BASE_20110923_H__
#define __GUI_SHADER_OPENGL_BASE_20110923_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guishaderimp.h>

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

	class GUIEXPORT CGUIShader_opengl_base : public CGUIShaderImp
	{
	public:
		virtual ~CGUIShader_opengl_base();

	protected:
		friend class IGUIRender_opengl_base;
		CGUIShader_opengl_base(IGUIInterfaceRender* pRender);

	protected:
	};
}//namespace guiex

#endif //__GUI_SHADER_OPENGL_BASE_20110923_H__
