/** 
* @file guirender_opengl.cpp
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/render_opengl/guirender_opengl.h>
#include <libguiex_module/render_opengl/guitexture_opengl.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guicolorrect.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guirenderrect.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <libguiex_core/guicamera.h>
#include <libguiex_core/guitexture.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIRender_opengl);
	//------------------------------------------------------------------------------
	const char* IGUIRender_opengl::StaticGetModuleName()
	{
		return "IGUIRender_opengl";
	}
	//------------------------------------------------------------------------------
	IGUIRender_opengl::IGUIRender_opengl()
		:IGUIRender_opengl_base( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	IGUIRender_opengl::~IGUIRender_opengl()
	{
	}
	//------------------------------------------------------------------------------
	int IGUIRender_opengl::DoInitialize(void* pData)
	{
		TRY_THROW_OPENGL_ERROR( "render interface initialize" );

		//init glew
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			throw CGUIException("[IGUIRender_opengl::DoInitialize]: failed to init glew: %s", glewGetErrorString(err));
			return -1;
		}

		//call parent initialize
		if( 0 != IGUIRender_opengl_base::DoInitialize( pData ) )
		{
			return -1;
		}

		TRY_THROW_OPENGL_ERROR( "render interface initialize end" );
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::GenFramebuffers( uint32 n, uint32* framebuffers )
	{
		glGenFramebuffers( n, framebuffers );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::DeleteFramebuffers( uint32 n, const uint32* framebuffers )
	{
		glDeleteFramebuffers( n, framebuffers );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::BindFramebuffer( uint32 framebuffer )
	{
		glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::GetBindingFrameBuffer( int32* framebuffer )
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, framebuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::GenRenderbuffers(uint32 n, uint32* renderbuffers)
	{
		glGenRenderbuffers(n, renderbuffers);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::BindRenderbuffer( uint32 renderbuffer)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::GetBindingRenderBuffer( int32* renderbuffer )
	{
		glGetIntegerv(GL_RENDERBUFFER_BINDING, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::RenderbufferStorage_Depth( uint32 width, uint32 height)
	{
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::FramebufferTexture2D_Color( const CGUITexture* pTexture, int32 level )
	{
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ((const CGUITexture_opengl*)pTexture->GetTextureImplement())->GetOGLTexid(), level );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::FramebufferRenderbuffer_Depth( uint32 renderbuffer )
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::DeleteRenderbuffers(uint32 n, const uint32* renderbuffers)
	{
		glDeleteRenderbuffers(n, renderbuffers);
	}
	//------------------------------------------------------------------------------
	bool IGUIRender_opengl::CheckFramebufferStatus( )
	{
		GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		return status == GL_FRAMEBUFFER_COMPLETE;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::BeginRender(void)
	{
		TRY_THROW_OPENGL_ERROR("BeginRender start");

		IGUIRender_opengl_base::BeginRender();

		TRY_THROW_OPENGL_ERROR("BeginRender end");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::EndRender(void)
	{		
		TRY_THROW_OPENGL_ERROR("EndRender start");

		IGUIRender_opengl_base::EndRender();

		TRY_THROW_OPENGL_ERROR("EndRender end");
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	/**
	* @brief Creates a 'null' Texture object.
	* @return a newly created Texture object.  The returned Texture object has no size or imagery 
	* associated with it, and is generally of little or no use.
	*/
	CGUITextureImp*	IGUIRender_opengl::CreateTexture(void)
	{
		CGUITexture_opengl* pTexture = new CGUITexture_opengl(this);
		AddTexture(pTexture);
		return pTexture;
	}
	//------------------------------------------------------------------------------
	CGUITextureImp*	IGUIRender_opengl::CreateTexture(const CGUIString& filename)
	{
		CGUITextureImp* pTexture = this->CreateTexture();
		if( pTexture->LoadFromFile(filename) != 0 )
		{
			//failed
			RemoveTexture( pTexture );
			delete pTexture;
			return NULL;
		}
		return pTexture;
	}
	//-----------------------------------------------------------------------------
	CGUITextureImp*	IGUIRender_opengl::CreateTexture(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat)
	{
		CGUITextureImp* pTexture = this->CreateTexture();
		((CGUITexture_opengl*)pTexture)->SetOpenglTextureSize(nWidth,nHeight,ePixelFormat);
		return pTexture;
	}
	//-----------------------------------------------------------------------------
	void IGUIRender_opengl::ClearDepth(real depth)
	{
		glClearDepth( depth );
	}
	//-----------------------------------------------------------------------------
	void IGUIRender_opengl::Perspective(real fovy, real aspect, real zNear, real zFar)
	{	
		gluPerspective(fovy, aspect, zNear, zFar);
	}
	//-----------------------------------------------------------------------------
	void IGUIRender_opengl::LookAt(real eyex, real eyey, real eyez,
				   real centerx, real centery, real centerz,
				   real upx, real upy, real upz)
	{
		gluLookAt( eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	}
	//------------------------------------------------------------------------------
	/** 
	 * @brief toggle wire frame.
	 */
	void IGUIRender_opengl::SetWireFrame( bool bWireFrame)
	{
		if( bWireFrame )
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
			glDisable( GL_CULL_FACE );
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable( GL_CULL_FACE );
		}
	}
	//------------------------------------------------------------------------------
	bool IGUIRender_opengl::IsWireFrame( ) const
	{
		GLint nPolygonMode[2];
		glGetIntegerv( GL_POLYGON_MODE, nPolygonMode );
		return (nPolygonMode[1]==GL_LINE);
	}
	//-----------------------------------------------------------------------------
	
}//namespace guiex
