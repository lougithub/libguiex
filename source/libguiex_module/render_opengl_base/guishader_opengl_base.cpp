/** 
* @file guishader_opengl_base.cpp
* @brief 
* @author ken
* @date 2011-09-23
*/


//============================================================================//
// include
//============================================================================// 
#include "guishader_opengl_base.h"
#include "guirender_opengl_base.h"
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <libguiex_core/guiinterfacemanager.h>

#include <libguiex_module/render_opengl_base/guiopenglheader.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIShader_opengl_base::CGUIShader_opengl_base(IGUIInterfaceRender* pRender)
		:CGUIShaderImp(pRender)
		,m_uProgramId(0)
		,m_uVertexShader(0)
		,m_uFragmentShader(0)
	{
	}
	//------------------------------------------------------------------------------
	CGUIShader_opengl_base::~CGUIShader_opengl_base()
	{
		DestroyShader();
	}
	//------------------------------------------------------------------------------
	uint32 CGUIShader_opengl_base::GetProgramId() const
	{
		return m_uProgramId;
	}
	//------------------------------------------------------------------------------
	void CGUIShader_opengl_base::DestroyShader()
	{
#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)
		if( m_uVertexShader != 0 )
		{
			glDetachShader( m_uProgramId, m_uVertexShader );
			glDeleteShader(m_uVertexShader);
			m_uVertexShader = 0;
		}
		if( m_uFragmentShader != 0 )
		{
			glDetachShader( m_uProgramId, m_uFragmentShader );
			glDeleteShader(m_uFragmentShader);
			m_uFragmentShader = 0;
		}
		if( m_uProgramId != 0 )
		{
			glDeleteProgram( m_uProgramId );
			m_uProgramId = 0;
		}
#endif	//#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)
	}
	//------------------------------------------------------------------------------
	int32 CGUIShader_opengl_base::LoadAndCompile(const CGUIString& rVertexShaderFileName, const CGUIString& rFragmentShaderFileName)
	{
#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)

		DestroyShader();

		//create shader
		m_uVertexShader = BuildShader(rVertexShaderFileName, GL_VERTEX_SHADER);
		m_uFragmentShader = BuildShader(rFragmentShaderFileName, GL_FRAGMENT_SHADER);

		m_uProgramId = glCreateProgram();
		glAttachShader(m_uProgramId, m_uVertexShader);
		glAttachShader(m_uProgramId, m_uFragmentShader);
		glLinkProgram(m_uProgramId);

		GLint linkSuccess;
		glGetProgramiv(m_uProgramId, GL_LINK_STATUS, &linkSuccess);
		if (linkSuccess == GL_FALSE) 
		{
			char messages[256];
			glGetProgramInfoLog(m_uProgramId, sizeof(messages), 0, &messages[0]);
			GUI_THROW( GUI_FORMAT( "[CGUIShader_opengl_base::LoadAndCompile]: %s", messages ));

			return -1;
		}
		CacheAttributeLoc();
		CacheUniformLoc();

#endif	//#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)
		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIShader_opengl_base::BuildShader(const CGUIString& rSource, uint32 shaderType)
	{
#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)

		///read file
		IGUIInterfaceFileSys* pFileSys =  CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		CGUIDataChunk aShaderDataChunk;
		if( 0 != pFileSys->ReadFile( rSource, aShaderDataChunk, IGUIInterfaceFileSys::eOpenMode_String ))
		{
			//failed
			GUI_THROW( GUI_FORMAT("[CGUIShader_opengl_base::BuildShader]: failed to read file <%s>!", rSource.c_str()));
			return -1;
		}

		GLuint shaderHandle = glCreateShader(shaderType);
		const char* data = (const char*)aShaderDataChunk.GetDataPtr();
		glShaderSource(shaderHandle, 1, &data, 0);
		glCompileShader(shaderHandle);

		GLint compileSuccess;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
		if (compileSuccess == GL_FALSE) 
		{
			char messages[256];
			glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
			GUI_THROW( GUI_FORMAT( "[CGUIShader_opengl_base::BuildShader]: %s", messages ));
			return 0;
		}
		return shaderHandle;
#else	////#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)
		return 0;
#endif	//#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)
	}
	//------------------------------------------------------------------------------
	void CGUIShader_opengl_base::UseShader( CGUIShader_opengl_base* pShader )
	{
#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)
		if( !pShader)
		{
			glUseProgram( 0 );
		}
		else
		{
			glUseProgram( pShader->GetProgramId() );
		}
#endif	//#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)
	}
	//------------------------------------------------------------------------------
	void CGUIShader_opengl_base::CacheAttributeLoc()
	{
#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)
		m_arrayCachedAttributeLoc[eSCAL_Position] = glGetAttribLocation( m_uProgramId, "a_v4Position");
		m_arrayCachedAttributeLoc[eSCAL_Color] = glGetAttribLocation( m_uProgramId, "a_v4Color");
		m_arrayCachedAttributeLoc[eSCAL_TexCoord] = glGetAttribLocation( m_uProgramId, "a_v2TexCoord");
#endif	//#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)
	}
	//------------------------------------------------------------------------------
	int32 CGUIShader_opengl_base::GetCachedAttributeLoc( EShaderCachedAttributeLoc eLoc )
	{
		return m_arrayCachedAttributeLoc[eLoc];
	}
	//------------------------------------------------------------------------------
	void CGUIShader_opengl_base::CacheUniformLoc()
	{
#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)
		m_arrayCachedUniformLoc[eSCUL_ModelViewProjectionMatrix] = glGetUniformLocation( m_uProgramId, "u_m4ModelViewProjectionMatrix");
#endif	//#if defined(GUIEX_RENDER_OPENGL) || defined(GUIEX_RENDER_OPENGL_ES2)
	}
	//------------------------------------------------------------------------------
	int32 CGUIShader_opengl_base::GetCachedUniformLoc( EShaderCachedUniformLoc eLoc )
	{
		return m_arrayCachedUniformLoc[eLoc];
	}
	//------------------------------------------------------------------------------
}//namespace guiex
