/** 
* @file guirender_opengles_es1.cpp
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/render_opengl_es1/guirender_opengl_es1.h>
#include <libguiex_module/render_opengl_base/guitexture_opengl_base.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guicolorrect.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guirenderrect.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <libguiex_core/guicamera.h>
#include <libguiex_core/guitexture.h>

#include <libguiex_module/render_opengl_base/guiopenglheader.h>

#if defined(GUIEX_TARGET_WIN32)
#ifndef GL_GLEXT_PROTOTYPES
PFNGLISRENDERBUFFEROESPROC glIsRenderbufferOES;
PFNGLBINDRENDERBUFFEROESPROC glBindRenderbufferOES;
PFNGLDELETERENDERBUFFERSOESPROC glDeleteRenderbuffersOES;
PFNGLGENRENDERBUFFERSOESPROC glGenRenderbuffersOES;
PFNGLRENDERBUFFERSTORAGEOESPROC glRenderbufferStorageOES;
PFNGLGETRENDERBUFFERPARAMETERIVOESPROC glGetRenderbufferParameterivOES;
PFNGLISFRAMEBUFFEROESPROC glIsFramebufferOES;
PFNGLBINDFRAMEBUFFEROESPROC glBindFramebufferOES;
PFNGLDELETEFRAMEBUFFERSOESPROC glDeleteFramebuffersOES;
PFNGLGENFRAMEBUFFERSOESPROC glGenFramebuffersOES;
PFNGLCHECKFRAMEBUFFERSTATUSOESPROC glCheckFramebufferStatusOES;
PFNGLFRAMEBUFFERRENDERBUFFEROESPROC glFramebufferRenderbufferOES;
PFNGLFRAMEBUFFERTEXTURE2DOESPROC glFramebufferTexture2DOES;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOESPROC glGetFramebufferAttachmentParameterivOES;
PFNGLGENERATEMIPMAPOESPROC glGenerateMipmapOES;
PFNGLBLENDEQUATIONOESPROC glBlendEquationOES;
PFNGLBLENDFUNCSEPARATEOESPROC glBlendFuncSeparateOES;
PFNGLBLENDEQUATIONSEPARATEOESPROC glBlendEquationSeparateOES;
#endif
#endif


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIRender_opengl_es1);
	//------------------------------------------------------------------------------
	const char* IGUIRender_opengl_es1::StaticGetModuleName()
	{
		return "IGUIRender_opengl_es1";
	}	
	//------------------------------------------------------------------------------
	IGUIRender_opengl_es1::IGUIRender_opengl_es1()
	:IGUIRender_opengl_base(StaticGetModuleName())
	{
	}
	//------------------------------------------------------------------------------
	int32 IGUIRender_opengl_es1::DoInitialize(void* pData )
	{
		TRY_THROW_OPENGL_ERROR();

#if defined(GUIEX_TARGET_WIN32)
#	ifndef GL_GLEXT_PROTOTYPES
		::glIsRenderbufferOES = (PFNGLISRENDERBUFFEROESPROC)eglGetProcAddress("glIsRenderbufferOES");
		::glBindRenderbufferOES = (PFNGLBINDRENDERBUFFEROESPROC)eglGetProcAddress("glBindRenderbufferOES");
		::glDeleteRenderbuffersOES = (PFNGLDELETERENDERBUFFERSOESPROC)eglGetProcAddress("glDeleteRenderbuffersOES");
		::glGenRenderbuffersOES = (PFNGLGENRENDERBUFFERSOESPROC)eglGetProcAddress("glGenRenderbuffersOES");
		::glRenderbufferStorageOES = (PFNGLRENDERBUFFERSTORAGEOESPROC)eglGetProcAddress("glRenderbufferStorageOES");
		::glGetRenderbufferParameterivOES = (PFNGLGETRENDERBUFFERPARAMETERIVOESPROC)eglGetProcAddress("glGetRenderbufferParameterivOES");
		::glIsFramebufferOES = (PFNGLISFRAMEBUFFEROESPROC)eglGetProcAddress("glIsFramebufferOES");
		::glBindFramebufferOES = (PFNGLBINDFRAMEBUFFEROESPROC)eglGetProcAddress("glBindFramebufferOES");
		::glDeleteFramebuffersOES = (PFNGLDELETEFRAMEBUFFERSOESPROC)eglGetProcAddress("glDeleteFramebuffersOES");
		::glGenFramebuffersOES = (PFNGLGENFRAMEBUFFERSOESPROC)eglGetProcAddress("glGenFramebuffersOES");
		::glCheckFramebufferStatusOES = (PFNGLCHECKFRAMEBUFFERSTATUSOESPROC)eglGetProcAddress("glCheckFramebufferStatusOES");
		::glFramebufferRenderbufferOES = (PFNGLFRAMEBUFFERRENDERBUFFEROESPROC)eglGetProcAddress("glFramebufferRenderbufferOES");
		::glFramebufferTexture2DOES = (PFNGLFRAMEBUFFERTEXTURE2DOESPROC)eglGetProcAddress("glFramebufferTexture2DOES");
		::glGetFramebufferAttachmentParameterivOES = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOESPROC)eglGetProcAddress("glGetFramebufferAttachmentParameterivOES");
		::glGenerateMipmapOES = (PFNGLGENERATEMIPMAPOESPROC)eglGetProcAddress("glGenerateMipmapOES");
		::glBlendEquationOES = (PFNGLBLENDEQUATIONOESPROC)eglGetProcAddress("glBlendEquationOES");
		::glBlendFuncSeparateOES = (PFNGLBLENDFUNCSEPARATEOESPROC)eglGetProcAddress("glBlendFuncSeparateOES");
		::glBlendEquationSeparateOES = (PFNGLBLENDEQUATIONSEPARATEOESPROC)eglGetProcAddress("glBlendEquationSeparateOES");
#	endif
#endif

#if defined(GUIEX_TARGET_IOS)	

		// Create the framebuffer object and attach the color buffer.
		GLuint framebuffer;
		GLuint renderbuffer;
		glGenRenderbuffersOES(1, &renderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);		

		// Create a stencil buffer.
		GLuint stencil;
		glGenRenderbuffersOES(1, &stencil);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, stencil);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_STENCIL_INDEX8_OES, width, height);

		// Create the framebuffer object.
		glGenFramebuffersOES(1, &framebuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES,GL_RENDERBUFFER_OES,renderbuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES,GL_RENDERBUFFER_OES, stencil);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);	
		
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
	void IGUIRender_opengl_es1::BeginRender(void)
	{
		IGUIRender_opengl_base::BeginRender( );

		glDisable( GL_LIGHTING );

		glShadeModel( GL_SMOOTH );
		glEnable( GL_POINT_SMOOTH );
		glEnable( GL_LINE_SMOOTH );

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		// Set up various GL state.
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::GenRenderbuffers(uint32 n, uint32* renderbuffers)
	{
		glGenRenderbuffersOES(n, renderbuffers);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::BindRenderbuffer( uint32 renderbuffer)
	{
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::GetBindingRenderBuffer( int32* renderbuffer )
	{
		glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::DeleteRenderbuffers(uint32 n, const uint32* renderbuffers)
	{
		glDeleteRenderbuffersOES(n, renderbuffers);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::RenderbufferStorage_Depth( uint32 width, uint32 height)
	{
		glRenderbufferStorageOES( GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, width, height);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::FramebufferRenderbuffer_Depth( uint32 renderbuffer )
	{
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::GenFramebuffers( uint32 n, uint32* framebuffers )
	{
		glGenFramebuffersOES( n, framebuffers );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::DeleteFramebuffers( uint32 n, const uint32* framebuffers )
	{
		glDeleteFramebuffersOES( n, framebuffers );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::BindFramebuffer( uint32 framebuffer )
	{
		glBindFramebufferOES( GL_FRAMEBUFFER_OES, framebuffer );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::GetBindingFrameBuffer( int32* framebuffer )
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, framebuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::FramebufferTexture2D_Color( const CGUITexture* pTexture, int32 level )
	{
		glFramebufferTexture2DOES( GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, ((const CGUITexture_opengl_base*)pTexture->GetTextureImplement())->GetOGLTexid(), level );
	}
	//------------------------------------------------------------------------------
	bool IGUIRender_opengl_es1::CheckFramebufferStatus( )
	{
		GLuint status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
		return status == GL_FRAMEBUFFER_COMPLETE_OES;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_es1::ClearDepth(real depth)
	{
		glClearDepthf( depth );
	}
	//-----------------------------------------------------------------------------
}//namespace guiex
