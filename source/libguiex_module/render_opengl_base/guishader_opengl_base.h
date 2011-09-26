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

		uint32 GetProgramId() const;

	protected:
		friend class IGUIRender_opengl_base;
		CGUIShader_opengl_base(IGUIInterfaceRender* pRender);

		int32 LoadAndCompile(const CGUIString& rVertexShaderFileName, const CGUIString& rFragmentShaderFileName);
		uint32 BuildShader(const CGUIString& rSource, uint32 shaderType);
		void DestroyShader();

	protected:
		uint32 m_uProgramId;
		uint32 m_uVertexShader;
		uint32 m_uFragmentShader;
	};
}//namespace guiex

#endif //__GUI_SHADER_OPENGL_BASE_20110923_H__
