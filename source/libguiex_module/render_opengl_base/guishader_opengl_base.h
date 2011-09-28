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
		enum EShaderCachedAttributeLoc
		{
			eSCAL_Position = 0,
			eSCAL_Color,
			eSCAL_TexCoord,

			_eSCAL_MAX_,
		};
		enum EShaderCachedUniformLoc
		{
			eSCUL_ModelViewProjectionMatrix = 0,

			_eSCUL_MAX_,
		};

	public:
		virtual ~CGUIShader_opengl_base();

		uint32 GetProgramId() const;
		static void UseShader( CGUIShader_opengl_base* pShader );

		int32 GetCachedAttributeLoc( EShaderCachedAttributeLoc eLoc );
		int32 GetCachedUniformLoc( EShaderCachedUniformLoc eLoc );

	protected:
		friend class IGUIRender_opengl_base;
		CGUIShader_opengl_base(IGUIInterfaceRender* pRender);

		int32 LoadAndCompile(const CGUIString& rVertexShaderFileName, const CGUIString& rFragmentShaderFileName);
		uint32 BuildShader(const CGUIString& rSource, uint32 shaderType);
		void DestroyShader();
		void CacheAttributeLoc();
		void CacheUniformLoc();

	protected:
		uint32 m_uProgramId;
		uint32 m_uVertexShader;
		uint32 m_uFragmentShader;

		int32 m_arrayCachedAttributeLoc[_eSCAL_MAX_];
		int32 m_arrayCachedUniformLoc[_eSCUL_MAX_];
	};
}//namespace guiex

#endif //__GUI_SHADER_OPENGL_BASE_20110923_H__
