/** 
* @file guirender_opengl_es2.cpp
* @brief use opengl es2 to render gui
* @author ken
* @date 2011-09-27
*/



//============================================================================//
// include
//============================================================================// 
#include "guirender_opengl_es2.h"
#include <libguiex_module/render_opengl_base/guitexture_opengl_base.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guicolorrect.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guirenderrect.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <libguiex_core/guicamera.h>
#include <libguiex_core/guitexture.h>

#include <libguiex_module/render_opengl_base/guiopenglheader.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIRender_opengl_es2);
	//------------------------------------------------------------------------------
	const char* IGUIRender_opengl_es2::StaticGetModuleName()
	{
		return "IGUIRender_opengl_es2";
	}	
	//------------------------------------------------------------------------------
	IGUIRender_opengl_es2::IGUIRender_opengl_es2()
	:IGUIRender_opengl_base(StaticGetModuleName())
	{
	}
	//------------------------------------------------------------------------------
	int32 IGUIRender_opengl_es2::DoInitialize(void* pData )
	{
		TRY_THROW_OPENGL_ERROR();

#if defined(GUIEX_TARGET_IOS)	

		// Create the framebuffer object and attach the color buffer.
		GLuint framebuffer;
		GLuint renderbuffer;
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);		
		
		// Create a stencil buffer.
		GLuint stencil;
		glGenRenderbuffers(1, &stencil);
		glBindRenderbuffer(GL_RENDERBUFFER, stencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, GSystem->GetRawScreenSize().m_uWidth, GSystem->GetRawScreenSize().m_uHeight);
		
		// Create the framebuffer object.		
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil);
		
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);	
		
#endif

		
		//call parent initialize
		if( 0 != IGUIRender_opengl_base::DoInitialize( pData ) )
		{
			return -1;
		}
		
		
		TRY_THROW_OPENGL_ERROR();		

		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::GenRenderbuffers(uint32 n, uint32* renderbuffers)
	{
		glGenRenderbuffers(n, renderbuffers);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::BindRenderbuffer( uint32 renderbuffer)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::GetBindingRenderBuffer( int32* renderbuffer )
	{
		glGetIntegerv(GL_RENDERBUFFER_BINDING, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::DeleteRenderbuffers(uint32 n, const uint32* renderbuffers)
	{
		glDeleteRenderbuffers(n, renderbuffers);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::RenderbufferStorage_Depth( uint32 width, uint32 height)
	{
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::FramebufferRenderbuffer_Depth( uint32 renderbuffer )
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::GenFramebuffers( uint32 n, uint32* framebuffers )
	{
		glGenFramebuffers( n, framebuffers );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::DeleteFramebuffers( uint32 n, const uint32* framebuffers )
	{
		glDeleteFramebuffers( n, framebuffers );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::BindFramebuffer( uint32 framebuffer )
	{
		glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::GetBindingFrameBuffer( int32* framebuffer )
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, framebuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::FramebufferTexture2D_Color( const CGUITexture* pTexture, int32 level )
	{
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ((const CGUITexture_opengl_base*)pTexture->GetTextureImplement())->GetOGLTexid(), level );
	}
	//------------------------------------------------------------------------------
	bool IGUIRender_opengl_es2::CheckFramebufferStatus( )
	{
		GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		return status == GL_FRAMEBUFFER_COMPLETE;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es2::ClearDepth(real depth)
	{
		glClearDepthf( depth );
	}
	//-----------------------------------------------------------------------------
}//namespace guiex
