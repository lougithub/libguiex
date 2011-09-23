/** 
* @file guirender_opengles_ios.cpp
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/render_opengles_ios/guirender_opengles_ios.h>
#include <libguiex_module/render_opengles_ios/guitexture_opengles_ios.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guicolorrect.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guirenderrect.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <libguiex_core/guicamera.h>
#include <libguiex_core/guitexture.h>

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIRender_opengles_ios);
	//------------------------------------------------------------------------------
	const char* IGUIRender_opengles_ios::StaticGetModuleName()
	{
		return "IGUIRender_opengles_ios";
	}	
	//------------------------------------------------------------------------------
	IGUIRender_opengles_ios::IGUIRender_opengles_ios()
	:IGUIRender_opengl_base(StaticGetModuleName())
	{
	}
	//------------------------------------------------------------------------------
	int32 IGUIRender_opengles_ios::DoInitialize(void* pData )
	{
		TRY_THROW_OPENGL_ERROR("IGUIRender_opengles_ios::DoInitialize: begin");

		// Create the framebuffer object and attach the color buffer.
		GLuint framebuffer;
		GLuint renderbuffer;
		glGenRenderbuffersOES(1, &renderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);		
		glGenFramebuffersOES(1, &framebuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
									 GL_COLOR_ATTACHMENT0_OES,
									 GL_RENDERBUFFER_OES,
									 renderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);	
		
		//call parent initialize
		if( 0 != IGUIRender_opengl_base::DoInitialize( pData ) )
		{
			return -1;
		}
		
		////set texture
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//glEnable(GL_TEXTURE_2D);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);		
		
		TRY_THROW_OPENGL_ERROR("IGUIRender_opengles_ios::DoInitialize: end");		

		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::GenRenderbuffers(uint32 n, uint32* renderbuffers)
	{
		glGenRenderbuffersOES(n, renderbuffers);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::BindRenderbuffer( uint32 renderbuffer)
	{
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::GetBindingRenderBuffer( int32* renderbuffer )
	{
		glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::DeleteRenderbuffers(uint32 n, const uint32* renderbuffers)
	{
		glDeleteRenderbuffersOES(n, renderbuffers);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::RenderbufferStorage_Depth( uint32 width, uint32 height)
	{
		glRenderbufferStorageOES( GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, width, height);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::FramebufferRenderbuffer_Depth( uint32 renderbuffer )
	{
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, renderbuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::GenFramebuffers( uint32 n, uint32* framebuffers )
	{
		glGenFramebuffersOES( n, framebuffers );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::DeleteFramebuffers( uint32 n, const uint32* framebuffers )
	{
		glDeleteFramebuffersOES( n, framebuffers );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::BindFramebuffer( uint32 framebuffer )
	{
		glBindFramebufferOES( GL_FRAMEBUFFER_OES, framebuffer );
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::GetBindingFrameBuffer( int32* framebuffer )
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, framebuffer);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::FramebufferTexture2D_Color( const CGUITexture* pTexture, int32 level )
	{
		glFramebufferTexture2DOES( GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, ((const CGUITexture_opengles_ios*)pTexture->GetTextureImplement())->GetOGLTexid(), level );
	}
	//------------------------------------------------------------------------------
	bool IGUIRender_opengles_ios::CheckFramebufferStatus( )
	{
		GLuint status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
		return status == GL_FRAMEBUFFER_COMPLETE_OES;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengles_ios::ClearDepth(real depth)
	{
		glClearDepthf( depth );
	}
	//-----------------------------------------------------------------------------
	void IGUIRender_opengles_ios::Perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
	{	
		GLfloat xmin, xmax, ymin, ymax;
		
		ymax = zNear * (GLfloat)tanf(fovy * (float)M_PI / 360);
		ymin = -ymax;
		xmin = ymin * aspect;
		xmax = ymax * aspect;
		
		glFrustumf(xmin, xmax,
				   ymin, ymax,
				   zNear, zFar);	
	}
	//-----------------------------------------------------------------------------
	void IGUIRender_opengles_ios::LookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
				   GLfloat centerx, GLfloat centery, GLfloat centerz,
				   GLfloat upx, GLfloat upy, GLfloat upz)
	{
		GLfloat m[16];
		GLfloat x[3], y[3], z[3];
		GLfloat mag;
		
		/* Make rotation matrix */
		
		/* Z vector */
		z[0] = eyex - centerx;
		z[1] = eyey - centery;
		z[2] = eyez - centerz;
		mag = (float)sqrtf(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
		if (mag) {
			z[0] /= mag;
			z[1] /= mag;
			z[2] /= mag;
		}
		
		/* Y vector */
		y[0] = upx;
		y[1] = upy;
		y[2] = upz;
		
		/* X vector = Y cross Z */
		x[0] = y[1] * z[2] - y[2] * z[1];
		x[1] = -y[0] * z[2] + y[2] * z[0];
		x[2] = y[0] * z[1] - y[1] * z[0];
		
		/* Recompute Y = Z cross X */
		y[0] = z[1] * x[2] - z[2] * x[1];
		y[1] = -z[0] * x[2] + z[2] * x[0];
		y[2] = z[0] * x[1] - z[1] * x[0];
		
		/* cross product gives area of parallelogram, which is < 1.0 for
		 * non-perpendicular unit-length vectors; so normalize x, y here
		 */
		
		mag = (float)sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
		if (mag) {
			x[0] /= mag;
			x[1] /= mag;
			x[2] /= mag;
		}
		
		mag = (float)sqrtf(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
		if (mag) {
			y[0] /= mag;
			y[1] /= mag;
			y[2] /= mag;
		}
		
#define M(row,col)  m[col*4+row]
		M(0, 0) = x[0];
		M(0, 1) = x[1];
		M(0, 2) = x[2];
		M(0, 3) = 0.0f;
		M(1, 0) = y[0];
		M(1, 1) = y[1];
		M(1, 2) = y[2];
		M(1, 3) = 0.0f;
		M(2, 0) = z[0];
		M(2, 1) = z[1];
		M(2, 2) = z[2];
		M(2, 3) = 0.0f;
		M(3, 0) = 0.0f;
		M(3, 1) = 0.0f;
		M(3, 2) = 0.0f;
		M(3, 3) = 1.0f;
#undef M
		
		glMultMatrixf(m);

	
		/* Translate Eye to Origin */
		glTranslatef(-eyex, -eyey, -eyez);
	}
	//-----------------------------------------------------------------------------	
}//namespace guiex
