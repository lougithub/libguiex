/** 
* @file guirender_opengl.cpp
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/render_opengl_base/guirender_opengl_base.h>
#include <libguiex_module/render_opengl_base/guitexture_opengl_base.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guicolorrect.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guirenderrect.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <libguiex_core/guicamera.h>
#include <libguiex_core/guitexture.h>

#if defined(GUIEX_TARGET_WIN32)
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#elif defined(GUIEX_TARGET_IOS)
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#elif defined(GUIEX_TARGET_MACOS)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#elif defined(GUIEX_TARGET_ANDROID)
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#error "unknown target"	
#endif


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------

#if defined(GUIEX_TARGET_ANDROID)
#define OPENGL_ERROR GUI_TRACE
#else
#define OPENGL_ERROR GUI_THROW
#endif
	void TryThrowOpenglError( const char* info )
	{
		int errorcode = glGetError();
		if( GL_NO_ERROR != errorcode )
		{
			switch( errorcode )
			{
			case GL_INVALID_ENUM:
				OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s>, error is <%s>!" ,info, "GL_INVALID_ENUM"));
				break;
			case GL_INVALID_VALUE:
				OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s>, error is <%s>!" ,info, "GL_INVALID_VALUE"));
				break;
			case GL_INVALID_OPERATION:
				OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s>, error is <%s>!" ,info, "GL_INVALID_OPERATION"));
				break;
			case GL_STACK_OVERFLOW:
				OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s>, error is <%s>!" ,info, "GL_STACK_OVERFLOW"));
				break;
			case GL_STACK_UNDERFLOW:
				OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s>, error is <%s>!" ,info, "GL_STACK_UNDERFLOW"));
				break;
			case GL_OUT_OF_MEMORY:
				OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s>, error is <%s>!" ,info, "GL_OUT_OF_MEMORY"));
				break;			
			default:
				OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s>, error is <0x%x>!" ,info, errorcode));
			}
		}
	}

	//------------------------------------------------------------------------------
	EBlendFunc BlendFunc_GL2Engin( GLint nGlType )
	{
		switch( nGlType )
		{
		case GL_ONE:
			return eBlendFunc_ONE;
		case GL_ZERO:
			return eBlendFunc_ZERO;
		case GL_SRC_ALPHA:
			return eBlendFunc_SRC_ALPHA;
		case GL_ONE_MINUS_SRC_ALPHA:
			return eBlendFunc_ONE_MINUS_SRC_ALPHA;
		case GL_SRC_COLOR:
			return eBlendFunc_SRC_COLOR;
		case GL_DST_COLOR:
			return eBlendFunc_DST_COLOR;
		case GL_ONE_MINUS_SRC_COLOR:
			return eBlendFunc_ONE_MINUS_SRC_COLOR;
		case GL_ONE_MINUS_DST_COLOR:
			return eBlendFunc_ONE_MINUS_DST_COLOR;
		case GL_DST_ALPHA:
			return eBlendFunc_DST_ALPHA;
		case GL_ONE_MINUS_DST_ALPHA:
			return eBlendFunc_ONE_MINUS_DST_ALPHA;
		case GL_SRC_ALPHA_SATURATE:
			return eBlendFunc_SRC_ALPHA_SATURATE;
		default:
			GUI_FORCE_ASSERT( "unknown blend func type" );
			return eBlendFunc_ONE;
		}
	}
	//------------------------------------------------------------------------------
	GLint BlendFunc_Engin2GL( EBlendFunc nEngineType )
	{
		switch( nEngineType )
		{
		case eBlendFunc_ONE:
			return GL_ONE;
		case eBlendFunc_ZERO:
			return GL_ZERO;
		case eBlendFunc_SRC_ALPHA:
			return GL_SRC_ALPHA;
		case eBlendFunc_ONE_MINUS_SRC_ALPHA:
			return GL_ONE_MINUS_SRC_ALPHA;
		case eBlendFunc_SRC_COLOR:
			return GL_SRC_COLOR;
		case eBlendFunc_DST_COLOR:
			return GL_DST_COLOR;
		case eBlendFunc_ONE_MINUS_SRC_COLOR:
			return GL_ONE_MINUS_SRC_COLOR;
		case eBlendFunc_ONE_MINUS_DST_COLOR:
			return GL_ONE_MINUS_DST_COLOR;
		case eBlendFunc_DST_ALPHA:
			return GL_DST_ALPHA;
		case eBlendFunc_ONE_MINUS_DST_ALPHA:
			return GL_ONE_MINUS_DST_ALPHA;
		case eBlendFunc_SRC_ALPHA_SATURATE:
			return GL_SRC_ALPHA_SATURATE;
		default:
			GUI_FORCE_ASSERT( "unknown blend func type" );
			return GL_ONE;
		}
	}
	//------------------------------------------------------------------------------
	//GLenum BufferMode_Engin2GL( EBufferMode eMode )
	//{
	//	switch ( eMode )
	//	{
	//	case eBufferMode_Back:
	//		return GL_BACK;
	//	case eBufferMode_Front:
	//		return GL_FRONT;
	//	default:
	//		GUI_FORCE_ASSERT( "unknown buffer mode" );
	//		return GL_BACK;
	//	}
	//}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIRender_opengl_base);
	//------------------------------------------------------------------------------
	IGUIRender_opengl_base::IGUIRender_opengl_base( const char* szModuleName )
		:IGUIInterfaceRender( szModuleName )
		,m_maxTextureSize(0)
		,m_bEnableClip(true)
		,m_pCamera(NULL)
		,m_bDrawWireframe(false)
		,m_nRenderMode_TRIANGLE_STRIP(GL_TRIANGLE_STRIP)
		,m_nRenderMode_TRIANGLES(GL_TRIANGLES)
	{
	}
	//------------------------------------------------------------------------------
	IGUIRender_opengl_base::~IGUIRender_opengl_base()
	{
	}
	//------------------------------------------------------------------------------
	int IGUIRender_opengl_base::DoInitialize(void* )
	{
		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::DoInitialize: begin");
		
		// get the maximum available texture size.
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);

		//get stencil info
		glClearStencil( 0 );
		glGetIntegerv( GL_STENCIL_BITS, &m_nStencilBits);
		m_nMaxStencilRef = (1<<m_nStencilBits) - 1;
		if( m_nMaxStencilRef < 2 )
		{
			GUI_TRACE( "[IGUIRender_opengl_base::DoInitialize]: stencil is disabled\n" );
		}
		else
		{
			glEnable( GL_STENCIL_TEST );	
		}
		makeGLMatrix( m_aWholeScreenRect.m_gl_world_matrix, CGUIMatrix4::IDENTITY );

		//set gl property
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);

		glEnable( GL_BLEND );
		m_aBlendFunc.src = eBlendFunc_SRC_ALPHA;
		m_aBlendFunc.dst = eBlendFunc_ONE_MINUS_SRC_ALPHA;
		SetBlendFunc( m_aBlendFunc );

		//glEnable( GL_DEPTH_TEST );
		glDisable( GL_DEPTH_TEST );
		glDepthFunc(GL_LEQUAL);
		ClearDepth( 1.0f );

		glDisable( GL_SCISSOR_TEST );
		glDisable( GL_LIGHTING );

		glShadeModel( GL_SMOOTH );
		glEnable( GL_POINT_SMOOTH );
		glEnable( GL_LINE_SMOOTH );

		glEnable(GL_TEXTURE_2D);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glEnable(GL_CULL_FACE);

		// Set up various GL state.
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		m_nCurrentStencilRef = 0;

		OnScreenSizeChange( GSystem->GetRawScreenSize() );
		
		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::DoInitialize: end");
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::OnScreenSizeChange( const CGUIIntSize& rSize )
	{
		glViewport(0,0,rSize.GetWidth(),rSize.GetHeight());

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::OnScreenSizeChange");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DoDestroy()
	{
		DestroyAllTexture();

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::DoDestroy");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::ClearColor( real red, real green, real blue, real alpha )
	{
		glClearColor( red, green, blue, alpha );

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::ClearColor");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::Clear( uint32 uFlag )
	{
		GLbitfield bitfield = 0;
		if( uFlag & eRenderBuffer_COLOR_BIT )
		{
			bitfield |= GL_COLOR_BUFFER_BIT;
		}
		if( uFlag & eRenderBuffer_DEPTH_BIT )
		{
			bitfield |= GL_DEPTH_BUFFER_BIT;
		}
		if( uFlag & eRenderBuffer_STENCIL_BIT )
		{
			bitfield |= GL_STENCIL_BUFFER_BIT;
		}
		glClear( bitfield );

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::Clear");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::SetDepthTest( bool bEnable )
	{
		if ( bEnable ) 
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable( GL_DEPTH_TEST );
		}

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::SetDepthTest");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::SetBlendFunc( const SGUIBlendFunc& rBlendFuncType )
	{
		m_aBlendFunc = rBlendFuncType;
		GLenum src = BlendFunc_Engin2GL( rBlendFuncType.src );
		GLenum dst = BlendFunc_Engin2GL( rBlendFuncType.dst );
		glBlendFunc( src, dst );
		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::SetBlendFunc");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::GetBlendFunc( SGUIBlendFunc& rBlendFuncType )
	{
		//TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::GetBlendFunc: begin");
		//GLint src = 0;
		//GLint dst = 0;
		//glGetIntegerv( GL_BLEND_SRC, &src );
		//glGetIntegerv( GL_BLEND_DST, &dst );
		//rBlendFuncType.src = BlendFunc_GL2Engin( src );
		//rBlendFuncType.dst = BlendFunc_GL2Engin( dst );
		//TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::GetBlendFunc: end");
		rBlendFuncType = m_aBlendFunc;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::SetViewport( int32 x, int32 y, uint32 width, uint32 height)
	{
		glViewport( x, y, width, height );

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::SetViewport");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::CopyTexSubImage2D ( int32 level, int32 xoffset, int32 yoffset, int32 x, int32 y, uint32 width, uint32 height)
	{
		glCopyTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, x, y, width, height);

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::CopyTexSubImage2D");
	}
	//------------------------------------------------------------------------------
	//void IGUIRender_opengl_base::DrawBuffer( EBufferMode mode )
	//{
	//	glDrawBuffer( BufferMode_Engin2GL(mode) );
	//}
	//------------------------------------------------------------------------------
	//void IGUIRender_opengl_base::ReadBuffer( EBufferMode mode )
	//{
	//	glReadBuffer( BufferMode_Engin2GL(mode) );
	//}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::PushMatrix()
	{
		glPushMatrix();

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::PushMatrix");
	}	
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::PopMatrix()
	{
		glPopMatrix();

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::PopMatrix");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::MatrixMode( EMatrixMode eMode )
	{
		switch( eMode )
		{
		case eMatrixMode_MODELVIEW:
			glMatrixMode( GL_MODELVIEW );
			return;

		case eMatrixMode_PROJECTION:
			glMatrixMode( GL_PROJECTION );
			return;

		default:
			GUI_THROW( "IGUIRender_opengl_base::MatrixMode: unknown matrix mode");
			return;
		}

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::MatrixMode");

	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::LoadIdentityMatrix( )
	{
		glLoadIdentity();

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::LoadIdentityMatrix");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::MultMatrix( const CGUIMatrix4& rMatrix )
	{
		makeGLMatrix( m_gl_matrix, rMatrix );
		glMultMatrixf( m_gl_matrix );

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::MultMatrix");
	}
	//------------------------------------------------------------------------------
	bool IGUIRender_opengl_base::IsSupportStencil()
	{
		return m_nMaxStencilRef >= 2;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::BeginRender(void)
	{
		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::BeginRender: begin");
		
		//clear screen
		glClearColor( 0.5f, 0.5f, 0.5f, 1 );
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear screen and depth buffer 

		//update camera
		UpdateCamera();

		m_nCurrentStencilRef = 0;
		
		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::BeginRender: end");		
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::EndRender(void)
	{		
		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::EndRender: ");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::UpdateCamera( )
	{
		if( m_pCamera && m_pCamera->IsDirty() )
		{
			m_pCamera->ClearDirty();

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			Perspective( m_pCamera->GetFov(), m_pCamera->GetAspectRatio(), m_pCamera->GetNearPlane(), m_pCamera->GetFarPlane() );
			LookAt( m_pCamera->GetEye().x, m_pCamera->GetEye().y, m_pCamera->GetEye().z,
				m_pCamera->GetCenter().x, m_pCamera->GetCenter().y, m_pCamera->GetCenter().z,
				m_pCamera->GetUp().x, m_pCamera->GetUp().y, m_pCamera->GetUp().z );

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::UpdateCamera: ");
	}
	//------------------------------------------------------------------------------
	CGUICamera* IGUIRender_opengl_base::ApplyCamera( CGUICamera* pCamera )
	{
		CGUICamera* pOldCamera = m_pCamera;
		m_pCamera = pCamera;
		if( m_pCamera && m_pCamera != pOldCamera )
		{
			m_pCamera->SetDirty();
		}

		UpdateCamera();

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::ApplyCamera");
		return pOldCamera;
	}

	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawRect(
		const CGUIRect& rDestRect, 
		real fLineWidth,
		real z,
		const CGUIColor& rColor_topleft,
		const CGUIColor& rColor_topright,
		const CGUIColor& rColor_bottomleft,
		const CGUIColor& rColor_bottomright )
	{
		real fLeft = rDestRect.m_fLeft;
		real fRight = rDestRect.m_fRight;
		real fBottom = rDestRect.m_fBottom;
		real fTop = rDestRect.m_fTop;

		long oglcolor_topleft = ColorToOpengl(rColor_topleft);
		long oglcolor_bottomleft = ColorToOpengl(rColor_bottomleft);
		long oglcolor_bottomright = ColorToOpengl(rColor_bottomright);
		long oglcolor_topright = ColorToOpengl(rColor_topright);

		//vert0
		m_pVertexForLine[0].vertices.x = fLeft;
		m_pVertexForLine[0].vertices.y = fTop;
		m_pVertexForLine[0].vertices.z = z;
		m_pVertexForLine[0].color.abgr = oglcolor_topleft;

		//vert1
		m_pVertexForLine[1].vertices.x = fLeft;
		m_pVertexForLine[1].vertices.y = fBottom;
		m_pVertexForLine[1].vertices.z = z;
		m_pVertexForLine[1].color.abgr = oglcolor_bottomleft;     

		//vert2
		m_pVertexForLine[2].vertices.x = fRight;
		m_pVertexForLine[2].vertices.y = fBottom;
		m_pVertexForLine[2].vertices.z = z;
		m_pVertexForLine[2].color.abgr = oglcolor_bottomright;

		//vert3
		m_pVertexForLine[3].vertices.x = fRight;
		m_pVertexForLine[3].vertices.y = fTop;
		m_pVertexForLine[3].vertices.z = z;
		m_pVertexForLine[3].color.abgr = oglcolor_topright;      

		glLineWidth( fLineWidth );
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		int32 offset = (int32) m_pVertexForLine;
		int32 diff = offsetof( SR_C4UB_V3F, vertices);
		glVertexPointer(3, GL_FLOAT, sizeof(SR_C4UB_V3F), (GLvoid*) (offset+diff));
		diff = offsetof( SR_C4UB_V3F, color);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SR_C4UB_V3F), (GLvoid*) (offset+diff));
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glLineWidth( 1.0f );

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::DrawRect: ");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawCircle(
							const CGUIVector2& rCenter,
							real fRadius,
							real fLineWidth,
							real z,
							const CGUIColor& rColor )
	{
		for (int i = 0; i < VERTEX_FOR_CIRCLE; i ++) 
		{
			// x value
			m_pVertexForCircle[i].x   = cos(i*CGUIMath::GUI_PI / 180.f) * fRadius + rCenter.x;
			// y value
			m_pVertexForCircle[i].y = sin(i*CGUIMath::GUI_PI / 180.f) * fRadius + rCenter.y;
			m_pVertexForCircle[i].z = z;
		}
	
		glLineWidth( fLineWidth );
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		
		glVertexPointer(3, GL_FLOAT, sizeof(SR_V3F), m_pVertexForCircle);
		glColor4f(rColor.GetRed(), rColor.GetGreen(), rColor.GetBlue(), rColor.GetAlpha());
		glDrawArrays(GL_LINE_LOOP, 0, VERTEX_FOR_CIRCLE);
		
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glLineWidth( 1.0f );
		
		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::DrawCircle: ");		
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawLine(
		const CGUIVector2 &rBegin, 
		const CGUIVector2 &rEnd, 
		real fLineWidth,
		real z,
		const CGUIColor& rColor_begin,
		const CGUIColor& rColor_end )
	{
		long oglcolor_topleft = ColorToOpengl(rColor_begin);
		long oglcolor_bottomleft = ColorToOpengl(rColor_end);

		//vert0
		m_pVertexForLine[0].vertices.x = rBegin.x;
		m_pVertexForLine[0].vertices.y = rBegin.y;
		m_pVertexForLine[0].vertices.z = z;
		m_pVertexForLine[0].color.abgr = oglcolor_topleft;

		//vert1
		m_pVertexForLine[1].vertices.x = rEnd.x;
		m_pVertexForLine[1].vertices.y = rEnd.y;
		m_pVertexForLine[1].vertices.z = z;
		m_pVertexForLine[1].color.abgr = oglcolor_bottomleft;     

		glLineWidth( fLineWidth );
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		int32 offset = (int32) m_pVertexForLine;
		int32 diff = offsetof( SR_C4UB_V3F, vertices);
		glVertexPointer(3, GL_FLOAT, sizeof(SR_C4UB_V3F), (GLvoid*) (offset+diff));
		diff = offsetof( SR_C4UB_V3F, color);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SR_C4UB_V3F), (GLvoid*) (offset+diff));
		glDrawArrays(GL_LINES, 0, 2);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glLineWidth( 1.0f );

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::DrawLine: ");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawGrid( 
		const CGUITexture* pTexture,
		const SR_T2F* pTextures,
		const SR_V3F* pVerdices,
		uint16* pIndices,
		int16 nGridNum )
	{
		BindTexture( pTexture );
		glDisableClientState(GL_COLOR_ARRAY);	

		// vertex
		glVertexPointer(3,GL_FLOAT, sizeof(SR_V3F), pVerdices );

		// tex coords
		glTexCoordPointer(2, GL_FLOAT, sizeof(SR_T2F), pTextures);		

		glDrawElements(m_nRenderMode_TRIANGLES, nGridNum*6, GL_UNSIGNED_SHORT, pIndices);

		// restore GL default state
		glEnableClientState(GL_COLOR_ARRAY);

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::DrawGrid: ");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawQuads(
		const CGUITexture* pTexture,
		const SR_V2F_C4F_T2F_Quad* pQuads,
		uint16* pIndices,
		int16 nQuadNum)
	{
		BindTexture( pTexture );

		int16 kQuadSize = sizeof(pQuads[0].bl);

		int32 offset = (int32) pQuads;

		// vertex
		int32 diff = offsetof( SR_V2F_C4F_T2F, vertices);
		glVertexPointer(2,GL_FLOAT, kQuadSize, (GLvoid*) (offset+diff) );

		// color
		diff = offsetof( SR_V2F_C4F_T2F, colors);
		glColorPointer(4, GL_FLOAT, kQuadSize, (GLvoid*)(offset + diff));

		// tex coords
		diff = offsetof( SR_V2F_C4F_T2F, texCoords);
		glTexCoordPointer(2, GL_FLOAT, kQuadSize, (GLvoid*)(offset + diff));		

		//glDrawArrays( GL_TRIANGLE_STRIP, 0, nQuadNum * 4 );

		glDrawElements(m_nRenderMode_TRIANGLES, nQuadNum*6, GL_UNSIGNED_SHORT, pIndices);

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::DrawQuads: ");
	}


	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::BindTexture( const CGUITexture* pTexture )
	{
		if( !pTexture || !pTexture->GetTextureImplement() )
		{
			glBindTexture(GL_TEXTURE_2D, 0);

		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, ((const CGUITexture_opengl_base*)pTexture->GetTextureImplement())->GetOGLTexid());
		}

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::BindTexture: ");

	}
	//------------------------------------------------------------------------------
	/** 
	* @brief add a texture into render list
	*/
	void IGUIRender_opengl_base::DrawTile(
		const CGUIRect& rDestRect, 
		real z, 
		const CGUITexture* pTexture, 
		const CGUIRect& rTextureRect, 
		EImageOrientation eImageOrientation, 				
		const CGUIColor& rColor_topleft,
		const CGUIColor& rColor_topright,
		const CGUIColor& rColor_bottomleft,
		const CGUIColor& rColor_bottomright)
	{
		//bind texture
		BindTexture( pTexture );

		//set texture coordinate
		SetTexCoordinate(m_pVertex, rTextureRect, pTexture, eImageOrientation);

		real fLeft = rDestRect.m_fLeft;
		real fRight = rDestRect.m_fRight;
		real fBottom = rDestRect.m_fBottom;
		real fTop = rDestRect.m_fTop;

		long oglcolor_topleft = ColorToOpengl(rColor_topleft);
		long oglcolor_bottomleft = ColorToOpengl(rColor_bottomleft);
		long oglcolor_bottomright = ColorToOpengl(rColor_bottomright);
		long oglcolor_topright = ColorToOpengl(rColor_topright);

		//vert0
		m_pVertex[0].vertices.x= fLeft;
		m_pVertex[0].vertices.y = fTop;
		m_pVertex[0].vertices.z = z;
		m_pVertex[0].color.abgr = oglcolor_topleft;

		//vert1
		m_pVertex[1].vertices.x = fLeft;
		m_pVertex[1].vertices.y = fBottom;
		m_pVertex[1].vertices.z = z;
		m_pVertex[1].color.abgr = oglcolor_bottomleft;     

		//vert2
		m_pVertex[2].vertices.x = fRight;
		m_pVertex[2].vertices.y = fTop;
		m_pVertex[2].vertices.z = z;
		m_pVertex[2].color.abgr = oglcolor_topright;

		//vert3
		m_pVertex[3].vertices.x = fRight;
		m_pVertex[3].vertices.y = fBottom;
		m_pVertex[3].vertices.z = z;
		m_pVertex[3].color.abgr = oglcolor_bottomright;      

		int16 kSize = sizeof(SR_T2F_C4UB_V3F);
		int32 offset = (int32) m_pVertex;
		int32 diff = offsetof( SR_T2F_C4UB_V3F, vertices);
		glVertexPointer(3, GL_FLOAT, kSize, (GLvoid*) (offset+diff));
		diff = offsetof( SR_T2F_C4UB_V3F, color);
		glColorPointer(4, GL_UNSIGNED_BYTE, kSize, (GLvoid*) (offset+diff));
		diff = offsetof( SR_T2F_C4UB_V3F, texCoords);
		glTexCoordPointer(2, GL_FLOAT, kSize, (GLvoid*) (offset+diff));

		glDrawArrays( m_nRenderMode_TRIANGLE_STRIP, 0, 4 );

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::DrawTile: ");
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief add a texture into render list
	*/
	void IGUIRender_opengl_base::PushClipRect( const CGUIRect& rClipRect )
	{
		m_arrayClipRects.push_back( SClipRect() );
		glGetFloatv( GL_MODELVIEW_MATRIX, m_arrayClipRects.back().m_gl_world_matrix );
		m_arrayClipRects.back().m_aClipRect = rClipRect;

		if( m_bEnableClip && IsSupportStencil())
		{
			UpdateStencil();
		}
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::PopClipRect( )
	{
		GUI_ASSERT( m_arrayClipRects.empty() == false, "no clip rect to pop" );
		m_arrayClipRects.pop_back();

		if( m_bEnableClip && IsSupportStencil())
		{
			UpdateStencil();
		}
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::UpdateStencil()
	{
		//clear stencil buffer to 1 for all area visible now
		glClearStencil( 0 );
		glClear( GL_STENCIL_BUFFER_BIT );
		m_nCurrentStencilRef = 0;

		// Set color mask and disable texture
		glColorMask( false, false, false, false );		
		glDisable( GL_TEXTURE_2D );

		glStencilFunc( GL_EQUAL, m_nCurrentStencilRef, m_nCurrentStencilRef );
		glStencilOp( GL_ZERO, GL_ZERO, GL_INCR );

		//render clip rect to stencil buffer
		for( std::vector<SClipRect>::iterator itor =  m_arrayClipRects.begin();
			itor != m_arrayClipRects.end();
			++itor )
		{
			SClipRect& rClipRect = *itor;
			RenderRectForStencil( rClipRect );

			++m_nCurrentStencilRef;
			if( m_nCurrentStencilRef == m_nMaxStencilRef-1 )
			{
				//reach max
				glStencilFunc( GL_EQUAL, m_nCurrentStencilRef, m_nCurrentStencilRef );
				glStencilOp( GL_ZERO, GL_ZERO, GL_INVERT );

				RenderRectForStencil( m_aWholeScreenRect );
				m_nCurrentStencilRef = 1;
			}
			glStencilFunc( GL_EQUAL, m_nCurrentStencilRef, m_nCurrentStencilRef );
		}

		//restore color and texture state
		glColorMask( true, true, true, true );		
		glEnable( GL_TEXTURE_2D );

		//reset stencil state
		glStencilFunc( GL_EQUAL, m_nCurrentStencilRef, m_nCurrentStencilRef );
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::UpdateStencil: ");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::RenderRectForStencil( const SClipRect& rRect )
	{
		//set matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf( rRect.m_gl_world_matrix );

		float fLeft = rRect.m_aClipRect.m_fLeft;
		float fRight = rRect.m_aClipRect.m_fRight;
		float fBottom = rRect.m_aClipRect.m_fBottom;
		float fTop = rRect.m_aClipRect.m_fTop;

		//vert0
		m_pVertexForStencil[0].x = fLeft;
		m_pVertexForStencil[0].y = fTop;
		m_pVertexForStencil[0].z = 1.0f;

		//vert1
		m_pVertexForStencil[1].x = fLeft;
		m_pVertexForStencil[1].y = fBottom;
		m_pVertexForStencil[1].z = 1.0f;

		//vert2
		m_pVertexForStencil[2].x = fRight;
		m_pVertexForStencil[2].y = fTop;
		m_pVertexForStencil[2].z = 1.0f;

		//vert3
		m_pVertexForStencil[3].x = fRight;
		m_pVertexForStencil[3].y = fBottom;
		m_pVertexForStencil[3].z = 1.0f;

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		int32 offset = (int32) m_pVertexForStencil;
		int32 diff = offsetof( SR_V3F, x);
		glVertexPointer(3, GL_FLOAT, sizeof(SR_V3F), (GLvoid*) (offset+diff));
		glDrawArrays(m_nRenderMode_TRIANGLE_STRIP, 0, 4);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::RenderRectForStencil: ");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::SetTexCoordinate(SR_T2F_C4UB_V3F* pVertexInfo, CGUIRect tex, const CGUITexture* pTexture, EImageOrientation eImageOrientation)
	{
		//process for bottom-up texture
		tex.m_fTop = pTexture->UVConvertTopleft2Engine_v( tex.m_fTop );
		tex.m_fBottom = pTexture->UVConvertTopleft2Engine_v( tex.m_fBottom );
		
		switch(eImageOrientation)
		{
		case eImageOrientation_Normal:
			break;

		case eImageOrientation_FlipHorizon:
			std::swap( tex.m_fLeft, tex.m_fRight );
			break;

		case eImageOrientation_FlipVertical:
			std::swap( tex.m_fTop, tex.m_fBottom );
			break;

		case eImageOrientation_90CCW:
			//vert0
			pVertexInfo[0].texCoords.u = tex.m_fRight;
			pVertexInfo[0].texCoords.v = tex.m_fTop;

			//vert1
			pVertexInfo[1].texCoords.u = tex.m_fLeft;
			pVertexInfo[1].texCoords.v = tex.m_fTop;

			//vert2
			pVertexInfo[2].texCoords.u = tex.m_fRight;
			pVertexInfo[2].texCoords.v = tex.m_fBottom;

			//vert3
			pVertexInfo[3].texCoords.u = tex.m_fLeft;
			pVertexInfo[3].texCoords.v = tex.m_fBottom;
			return;

		case eImageOrientation_90CW:
			//vert0
			pVertexInfo[0].texCoords.u = tex.m_fLeft;
			pVertexInfo[0].texCoords.v = tex.m_fBottom;

			//vert1
			pVertexInfo[1].texCoords.u = tex.m_fRight;
			pVertexInfo[1].texCoords.v = tex.m_fBottom;

			//vert2
			pVertexInfo[2].texCoords.u = tex.m_fLeft;
			pVertexInfo[2].texCoords.v = tex.m_fTop;

			//vert3
			pVertexInfo[3].texCoords.u = tex.m_fRight;
			pVertexInfo[3].texCoords.v = tex.m_fTop;
			return;
		}
		
		//vert0
		pVertexInfo[0].texCoords.u = tex.m_fLeft;
		pVertexInfo[0].texCoords.v = tex.m_fTop;

		//vert1
		pVertexInfo[1].texCoords.u = tex.m_fLeft;
		pVertexInfo[1].texCoords.v = tex.m_fBottom;

		//vert2
		pVertexInfo[2].texCoords.u = tex.m_fRight;
		pVertexInfo[2].texCoords.v = tex.m_fTop;

		//vert3
		pVertexInfo[3].texCoords.u = tex.m_fRight;
		pVertexInfo[3].texCoords.v = tex.m_fBottom;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::AddTexture( CGUITextureImp* pTexture )
	{
		m_setTexture.insert(pTexture);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::RemoveTexture( CGUITextureImp* pTexture )
	{
		TSetTexture::iterator itor = m_setTexture.find(pTexture);
		GUI_ASSERT( itor!= m_setTexture.end(), "failed to destroy texture");
		m_setTexture.erase(itor);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DestroyTexture(CGUITextureImp* texture)
	{
		GUI_ASSERT( texture, "invalid texture pointer" );
		if (texture != NULL)
		{
			RemoveTexture( texture );
			delete texture;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Destroy all textures
	*/
	void IGUIRender_opengl_base::DestroyAllTexture()
	{
		while( m_setTexture.empty() == false)
		{
			DestroyTexture(*m_setTexture.begin());
		}

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::DestroyAllTexture");
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return the current width of the display in pixels
	* @return real value equal to the current width of the display in pixels.
	*/
	uint16	IGUIRender_opengl_base::GetWidth(void) const
	{
		GUI_ASSERT(0, "not implemented");
		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return the current height of the display in pixels
	* @return real value equal to the current height of the display in pixels.
	*/
	uint16	IGUIRender_opengl_base::GetHeight(void) const
	{
		GUI_ASSERT(0, "not implemented");
		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return the maximum texture size available
	* @return Size of the maximum supported texture in pixels (textures are always assumed to be square)
	*/
	uint32	IGUIRender_opengl_base::GetMaxTextureSize(void) const
	{
		return m_maxTextureSize;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return the horizontal display resolution dpi
	* @return horizontal resolution of the display in dpi.
	*/
	uint32	IGUIRender_opengl_base::GetHorzScreenDPI(void) const
	{
		return 96;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return the vertical display resolution dpi
	* @return vertical resolution of the display in dpi.
	*/
	uint32	IGUIRender_opengl_base::GetVertScreenDPI(void) const
	{
		return 96;
	}
	//------------------------------------------------------------------------------
	long IGUIRender_opengl_base::ColorToOpengl(const CGUIColor& col) const
	{
		return col.GetAsABGR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::makeGLMatrix(real gl_matrix[16], const CGUIMatrix4& m)
	{
		size_t x = 0;
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				gl_matrix[x] = m[j][i];
				x++;
			}
		}
	}
	//-----------------------------------------------------------------------------
	void IGUIRender_opengl_base::EnableClip( bool bEnable )
	{
		m_bEnableClip = bEnable;

		if( m_bEnableClip )
		{
			glEnable( GL_STENCIL_TEST );	
			if( !IsSupportStencil() )
			{
				//update stencil
				glGetIntegerv( GL_STENCIL_BITS, &m_nStencilBits);
				m_nMaxStencilRef = (1<<m_nStencilBits) - 1;
			}
		}
		else
		{
			glDisable( GL_STENCIL_TEST );	
			const CGUIIntSize& rSize = GSystem->GetScreenSize();
			m_aWholeScreenRect.m_aClipRect.SetSize( rSize );
		}

		TRY_THROW_OPENGL_ERROR("IGUIRender_opengl_base::EnableClip");
	}
	//------------------------------------------------------------------------------
	bool IGUIRender_opengl_base::IsEnableClip( ) const
	{
		return m_bEnableClip;
	}
	//-----------------------------------------------------------------------------
	void IGUIRender_opengl_base::SetWireFrame( bool bWireFrame)
	{
		m_bDrawWireframe = bWireFrame;
		if( m_bDrawWireframe )
		{
			m_nRenderMode_TRIANGLE_STRIP = GL_LINE_LOOP;
			m_nRenderMode_TRIANGLES = GL_LINE_LOOP;
		}
		else
		{
			m_nRenderMode_TRIANGLE_STRIP = GL_TRIANGLE_STRIP;
			m_nRenderMode_TRIANGLES = GL_TRIANGLES;
		}
	}
	//-----------------------------------------------------------------------------
	bool IGUIRender_opengl_base::IsWireFrame( ) const
	{
		return m_bDrawWireframe;
	}
	//-----------------------------------------------------------------------------
}//namespace guiex
