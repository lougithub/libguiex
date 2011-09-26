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
#include <libguiex_module/render_opengl_base/guishader_opengl_base.h>
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

#if defined(GUIEX_TARGET_ANDROID)
#define OPENGL_ERROR GUI_TRACE
#else
#define OPENGL_ERROR GUI_THROW
#endif
	void TryThrowOpenglError( char *file, int line )
	{
		int errorcode = glGetError();
		while( GL_NO_ERROR != errorcode )
		{
			const GLubyte* sError = gluErrorString(errorcode);
			if (sError)
			{
				OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s : %d>, error is <%s>!" , file, line, sError));
			}
			else
			{
				switch( errorcode )
				{
				case GL_INVALID_ENUM:
					OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s : %d>, error is <%s>!" , file, line, "GL_INVALID_ENUM"));
					break;
				case GL_INVALID_VALUE:
					OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s : %d>, error is <%s>!" , file, line, "GL_INVALID_VALUE"));
					break;
				case GL_INVALID_OPERATION:
					OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s : %d>, error is <%s>!" , file, line, "GL_INVALID_OPERATION"));
					break;
				case GL_STACK_OVERFLOW:
					OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s : %d>, error is <%s>!" , file, line, "GL_STACK_OVERFLOW"));
					break;
				case GL_STACK_UNDERFLOW:
					OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s : %d>, error is <%s>!" , file, line, "GL_STACK_UNDERFLOW"));
					break;
				case GL_OUT_OF_MEMORY:
					OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s : %d>, error is <%s>!" , file, line, "GL_OUT_OF_MEMORY"));
					break;			
				default:
					OPENGL_ERROR( GUI_FORMAT("error find in opengl in <%s : %d>, error is <0x%x>!" , file, line, errorcode));
				}
			}
			errorcode = glGetError();
		}
	}


	//------------------------------------------------------------------------------
	EBlendFunc BlendFunc_GL2Engine( GLint nGlType )
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
	GLint BlendFunc_Engine2GL( EBlendFunc nEngineType )
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
		,m_pCurrentShader(NULL)
	{
	}
	//------------------------------------------------------------------------------
	IGUIRender_opengl_base::~IGUIRender_opengl_base()
	{
	}
	//------------------------------------------------------------------------------
	int IGUIRender_opengl_base::DoInitialize(void* )
	{
		TRY_THROW_OPENGL_ERROR();
		
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

		m_nCurrentStencilRef = 0;

		m_pCurrentShader = NULL;

		OnScreenSizeChange( GSystem->GetRawScreenSize() );
		
		TRY_THROW_OPENGL_ERROR();
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::OnScreenSizeChange( const CGUIIntSize& rSize )
	{
		glViewport(0,0,rSize.GetWidth(),rSize.GetHeight());

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DoDestroy()
	{
		UseShader( NULL );

		DestroyAllTexture();
		DestroyAllShader();

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::ClearColor( real red, real green, real blue, real alpha )
	{
		glClearColor( red, green, blue, alpha );

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::Clear( uint32 uFlag )
	{
		TRY_THROW_OPENGL_ERROR();

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

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::SetDepthTest( bool bEnable )
	{
		TRY_THROW_OPENGL_ERROR();

		if ( bEnable ) 
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable( GL_DEPTH_TEST );
		}

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::SetBlendFunc( const SGUIBlendFunc& rBlendFuncType )
	{
		TRY_THROW_OPENGL_ERROR();

		m_aBlendFunc = rBlendFuncType;
		GLenum src = BlendFunc_Engine2GL( rBlendFuncType.src );
		GLenum dst = BlendFunc_Engine2GL( rBlendFuncType.dst );
		glBlendFunc( src, dst );

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::GetBlendFunc( SGUIBlendFunc& rBlendFuncType )
	{
		//TRY_THROW_OPENGL_ERROR();
		//GLint src = 0;
		//GLint dst = 0;
		//glGetIntegerv( GL_BLEND_SRC, &src );
		//glGetIntegerv( GL_BLEND_DST, &dst );
		//rBlendFuncType.src = BlendFunc_GL2Engine( src );
		//rBlendFuncType.dst = BlendFunc_GL2Engine( dst );
		//TRY_THROW_OPENGL_ERROR();
		rBlendFuncType = m_aBlendFunc;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::SetViewport( int32 x, int32 y, uint32 width, uint32 height)
	{
		TRY_THROW_OPENGL_ERROR();

		glViewport( x, y, width, height );

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::CopyTexSubImage2D ( int32 level, int32 xoffset, int32 yoffset, int32 x, int32 y, uint32 width, uint32 height)
	{
		TRY_THROW_OPENGL_ERROR();

		glCopyTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, x, y, width, height);

		TRY_THROW_OPENGL_ERROR();
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
		TRY_THROW_OPENGL_ERROR();

		glPushMatrix();

		TRY_THROW_OPENGL_ERROR();
	}	
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::PopMatrix()
	{
		glPopMatrix();

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::MatrixMode( EMatrixMode eMode )
	{
		TRY_THROW_OPENGL_ERROR();

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

		TRY_THROW_OPENGL_ERROR();

	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::LoadIdentityMatrix( )
	{
		TRY_THROW_OPENGL_ERROR();

		glLoadIdentity();

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::MultMatrix( const CGUIMatrix4& rMatrix )
	{
		TRY_THROW_OPENGL_ERROR();

		makeGLMatrix( m_gl_matrix, rMatrix );
		glMultMatrixf( m_gl_matrix );

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	bool IGUIRender_opengl_base::IsSupportStencil()
	{
		return m_nMaxStencilRef >= 2;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::BeginRender(void)
	{
		TRY_THROW_OPENGL_ERROR();
		
		//clear screen
		glClearColor( 0.5f, 0.5f, 0.5f, 1 );
		glClearStencil( 0 );
		ClearDepth( 1.0f );
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear screen and depth buffer 

		//set gl property
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);

		glEnable( GL_BLEND );
		m_aBlendFunc.src = eBlendFunc_SRC_ALPHA;
		m_aBlendFunc.dst = eBlendFunc_ONE_MINUS_SRC_ALPHA;
		SetBlendFunc( m_aBlendFunc );

		glDisable( GL_DEPTH_TEST );
		glDepthFunc(GL_LEQUAL);

		glDisable( GL_SCISSOR_TEST );
		glDisable( GL_LIGHTING );

		glShadeModel( GL_SMOOTH );
		glEnable( GL_POINT_SMOOTH );
		glEnable( GL_LINE_SMOOTH );

		glEnable(GL_TEXTURE_2D);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_CULL_FACE);

		// Set up various GL state.
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		//update camera
		UpdateCamera();

		m_nCurrentStencilRef = 0;
		
		TRY_THROW_OPENGL_ERROR();		
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::EndRender(void)
	{		
		TRY_THROW_OPENGL_ERROR();
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
		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	CGUICamera* IGUIRender_opengl_base::ApplyCamera( CGUICamera* pCamera )
	{
		TRY_THROW_OPENGL_ERROR();

		CGUICamera* pOldCamera = m_pCamera;
		m_pCamera = pCamera;
		if( m_pCamera && m_pCamera != pOldCamera )
		{
			m_pCamera->SetDirty();
		}

		UpdateCamera();

		TRY_THROW_OPENGL_ERROR();
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
		TRY_THROW_OPENGL_ERROR();

		real fLeft = rDestRect.m_fLeft;
		real fRight = rDestRect.m_fRight;
		real fBottom = rDestRect.m_fBottom;
		real fTop = rDestRect.m_fTop;

		uint32 oglcolor_topleft = GUIColorToRenderColor(rColor_topleft);
		uint32 oglcolor_bottomleft = GUIColorToRenderColor(rColor_bottomleft);
		uint32 oglcolor_bottomright = GUIColorToRenderColor(rColor_bottomright);
		uint32 oglcolor_topright = GUIColorToRenderColor(rColor_topright);

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

		DrawPrimitive( GL_LINE_LOOP, m_pVertexForLine, 4 );

		glLineWidth( 1.0f );

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawCircle(
							const CGUIVector2& rCenter,
							real fRadius,
							real fLineWidth,
							real z,
							const CGUIColor& rColor )
	{
		TRY_THROW_OPENGL_ERROR();

		uint32 oglcolor = GUIColorToRenderColor(rColor);
		for (int i = 0; i < VERTEX_FOR_CIRCLE; i ++) 
		{
			// x value
			m_pVertexForCircle[i].vertices.x   = cos(i*CGUIMath::GUI_PI / 180.f) * fRadius + rCenter.x;
			// y value
			m_pVertexForCircle[i].vertices.y = sin(i*CGUIMath::GUI_PI / 180.f) * fRadius + rCenter.y;
			m_pVertexForCircle[i].vertices.z = z;
			m_pVertexForCircle[i].color.abgr = oglcolor;
		}
	
		glLineWidth( fLineWidth );

		DrawPrimitive( GL_LINE_LOOP, m_pVertexForCircle, VERTEX_FOR_CIRCLE );

		glLineWidth( 1.0f );
		
		TRY_THROW_OPENGL_ERROR();		
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
		TRY_THROW_OPENGL_ERROR();

		uint32 oglcolor_topleft = GUIColorToRenderColor(rColor_begin);
		uint32 oglcolor_bottomleft = GUIColorToRenderColor(rColor_end);

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

		DrawPrimitive( GL_LINES, m_pVertexForLine, 2);

		glLineWidth( 1.0f );

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawGrid( 
		const CGUITexture* pTexture,
		const SVertexFormat_T2F* pTextures,
		const SVertexFormat_V3F* pVerdices,
		uint16* pIndices,
		int16 nGridNum )
	{
		BindTexture( pTexture );
		glDisableClientState(GL_COLOR_ARRAY);	

		DrawIndexedPrimitive( m_nRenderMode_TRIANGLES, pVerdices, pTextures, pIndices, nGridNum*6 );

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawQuads(
		const CGUITexture* pTexture,
		const SVertexFormat_V2F_C4UB_T2F_Quad* pQuads,
		uint16* pIndices,
		int16 nQuadNum)
	{
		TRY_THROW_OPENGL_ERROR();

		BindTexture( pTexture );

		DrawIndexedPrimitive( m_nRenderMode_TRIANGLES, pQuads[0].vertices,pIndices,nQuadNum*6 );
		int16 kQuadSize = sizeof(pQuads[0].vertices[0]);

		TRY_THROW_OPENGL_ERROR();
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

		TRY_THROW_OPENGL_ERROR();

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
		TRY_THROW_OPENGL_ERROR();

		//bind texture
		BindTexture( pTexture );

		//set texture coordinate
		SetTexCoordinate(m_pVertex, rTextureRect, pTexture, eImageOrientation);

		real fLeft = rDestRect.m_fLeft;
		real fRight = rDestRect.m_fRight;
		real fBottom = rDestRect.m_fBottom;
		real fTop = rDestRect.m_fTop;

		uint32 oglcolor_topleft = GUIColorToRenderColor(rColor_topleft);
		uint32 oglcolor_bottomleft = GUIColorToRenderColor(rColor_bottomleft);
		uint32 oglcolor_bottomright = GUIColorToRenderColor(rColor_bottomright);
		uint32 oglcolor_topright = GUIColorToRenderColor(rColor_topright);

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

		DrawPrimitive( m_nRenderMode_TRIANGLE_STRIP,m_pVertex, 4 );

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawIndexedPrimitive( uint32 uMode, const SVertexFormat_V2F_C4UB_T2F* pVertexBuf, uint16* pIndicesBuf, uint32 uIndexNum )
	{
		TRY_THROW_OPENGL_ERROR();

		int32 offset = (int32) pVertexBuf;

		// vertex
		int32 diff = offsetof( SVertexFormat_V2F_C4UB_T2F, vertices);
		glVertexPointer(2,GL_FLOAT, sizeof(SVertexFormat_V2F_C4UB_T2F), (GLvoid*) (offset+diff) );

		// color
		diff = offsetof( SVertexFormat_V2F_C4UB_T2F, colors);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SVertexFormat_V2F_C4UB_T2F), (GLvoid*)(offset + diff));

		// tex coords
		diff = offsetof( SVertexFormat_V2F_C4UB_T2F, texCoords);
		glTexCoordPointer(2, GL_FLOAT, sizeof(SVertexFormat_V2F_C4UB_T2F), (GLvoid*)(offset + diff));		

		glDrawElements(m_nRenderMode_TRIANGLES, uIndexNum, GL_UNSIGNED_SHORT, pIndicesBuf);

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawIndexedPrimitive( uint32 uMode, const SVertexFormat_V3F* pVerdiceBuf, const SVertexFormat_T2F* pTexCoordBuf, uint16* pIndicesBuf, uint32 uIndexNum )
	{
		TRY_THROW_OPENGL_ERROR();

		glDisableClientState(GL_COLOR_ARRAY);	

		glVertexPointer(3, GL_FLOAT, sizeof(SVertexFormat_V3F), pVerdiceBuf);
		glTexCoordPointer(2, GL_FLOAT, sizeof(SVertexFormat_T2F), pTexCoordBuf);	

		glDrawElements(m_nRenderMode_TRIANGLES, uIndexNum, GL_UNSIGNED_SHORT, pIndicesBuf);

		glEnableClientState(GL_COLOR_ARRAY);

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawPrimitive( uint32 uMode, const SVertexFormat_T2F_C4UB_V3F* pVertexBuf, uint32 uVertexNum )
	{
		TRY_THROW_OPENGL_ERROR();

		int32 offset = (int32) pVertexBuf;
		int32 diff = offsetof( SVertexFormat_T2F_C4UB_V3F, vertices);
		glVertexPointer(3, GL_FLOAT, sizeof(SVertexFormat_T2F_C4UB_V3F), (GLvoid*) (offset+diff));
		diff = offsetof( SVertexFormat_T2F_C4UB_V3F, color);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SVertexFormat_T2F_C4UB_V3F), (GLvoid*) (offset+diff));
		diff = offsetof( SVertexFormat_T2F_C4UB_V3F, texCoords);
		glTexCoordPointer(2, GL_FLOAT, sizeof(SVertexFormat_T2F_C4UB_V3F), (GLvoid*) (offset+diff));

		glDrawArrays(uMode, 0, uVertexNum);

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawPrimitive( uint32 uMode, const SVertexFormat_V3F* pVertexBuf, uint32 uVertexNum )
	{
		TRY_THROW_OPENGL_ERROR();

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		int32 offset = (int32) pVertexBuf;
		int32 diff = offsetof( SVertexFormat_V3F, x);
		glVertexPointer(3, GL_FLOAT, sizeof(SVertexFormat_V3F), (GLvoid*) (offset+diff));
		glDrawArrays(uMode, 0, uVertexNum);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DrawPrimitive( uint32 uMode, const SVertexFormat_C4UB_V3F* pVertexBuf, uint32 uVertexNum )
	{
		TRY_THROW_OPENGL_ERROR();

		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		int32 offset = (int32) pVertexBuf;
		int32 diff = offsetof( SVertexFormat_C4UB_V3F, vertices);
		glVertexPointer(3, GL_FLOAT, sizeof(SVertexFormat_C4UB_V3F), (GLvoid*) (offset+diff));
		diff = offsetof( SVertexFormat_C4UB_V3F, color);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SVertexFormat_C4UB_V3F), (GLvoid*) (offset+diff));
		glDrawArrays(uMode, 0, uVertexNum);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief add a texture into render list
	*/
	void IGUIRender_opengl_base::PushClipRect( const CGUIRect& rClipRect )
	{
		TRY_THROW_OPENGL_ERROR();

		m_arrayClipRects.push_back( SClipRect() );
		glGetFloatv( GL_MODELVIEW_MATRIX, m_arrayClipRects.back().m_gl_world_matrix );
		m_arrayClipRects.back().m_aClipRect = rClipRect;

		if( m_bEnableClip && IsSupportStencil())
		{
			UpdateStencil();
		}

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::PopClipRect( )
	{
		TRY_THROW_OPENGL_ERROR();

		GUI_ASSERT( m_arrayClipRects.empty() == false, "no clip rect to pop" );
		m_arrayClipRects.pop_back();

		if( m_bEnableClip && IsSupportStencil())
		{
			UpdateStencil();
		}

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::UpdateStencil()
	{
		TRY_THROW_OPENGL_ERROR();

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

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::RenderRectForStencil( const SClipRect& rRect )
	{
		TRY_THROW_OPENGL_ERROR();

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

		DrawPrimitive( m_nRenderMode_TRIANGLE_STRIP, m_pVertexForStencil, 4 );

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::SetTexCoordinate(SVertexFormat_T2F_C4UB_V3F* pVertexInfo, CGUIRect tex, const CGUITexture* pTexture, EImageOrientation eImageOrientation)
	{
		TRY_THROW_OPENGL_ERROR();

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

		TRY_THROW_OPENGL_ERROR();
	}
//------------------------------------------------------------------------------
	/**
	* @brief Creates a 'null' Texture object.
	* @return a newly created Texture object.  The returned Texture object has no size or imagery 
	* associated with it, and is generally of little or no use.
	*/
	CGUITextureImp*	IGUIRender_opengl_base::CreateTexture(void)
	{
		TRY_THROW_OPENGL_ERROR();

		CGUITexture_opengl_base* pTexture = new CGUITexture_opengl_base(this);
		AddTexture(pTexture);

		TRY_THROW_OPENGL_ERROR();

		return pTexture;
	}
	//------------------------------------------------------------------------------
	CGUITextureImp*	IGUIRender_opengl_base::CreateTexture(const CGUIString& filename)
	{
		TRY_THROW_OPENGL_ERROR();

		CGUITextureImp* pTexture = this->CreateTexture();
		if( pTexture->LoadFromFile(filename) != 0 )
		{
			//failed
			RemoveTexture( pTexture );
			delete pTexture;
			return NULL;
		}

		TRY_THROW_OPENGL_ERROR();

		return pTexture;
	}
	//-----------------------------------------------------------------------------
	CGUITextureImp*	IGUIRender_opengl_base::CreateTexture(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat)
	{
		CGUITextureImp* pTexture = this->CreateTexture();
		((CGUITexture_opengl_base*)pTexture)->SetOpenglTextureSize(nWidth,nHeight,ePixelFormat);
		return pTexture;
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
		GUI_ASSERT( itor!= m_setTexture.end(), "failed to remove texture");
		m_setTexture.erase(itor);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::AddShader( CGUIShaderImp* pShader )
	{
		m_setShader.insert(pShader);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::RemoveShader( CGUIShaderImp* pShader )
	{
		TSetShader::iterator itor = m_setShader.find(pShader);
		GUI_ASSERT( itor!= m_setShader.end(), "failed to remove shader");
		m_setShader.erase(itor);
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

		TRY_THROW_OPENGL_ERROR();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DestroyAllShader()
	{
		while( m_setShader.empty() == false)
		{
			DestroyShader(*m_setShader.begin());
		}

		TRY_THROW_OPENGL_ERROR();
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
	uint32 IGUIRender_opengl_base::GUIColorToRenderColor(const CGUIColor& col) const
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

		TRY_THROW_OPENGL_ERROR();
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
	CGUIShaderImp* IGUIRender_opengl_base::CreateShader(const CGUIString& rVertexShaderFileName, const CGUIString& rFragmentShaderFileName)
	{
		CGUIShader_opengl_base * pShader = new CGUIShader_opengl_base(this);
		pShader->LoadAndCompile( rVertexShaderFileName, rFragmentShaderFileName );

		AddShader( pShader );
		return pShader;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl_base::DestroyShader(CGUIShaderImp* shader)
	{
		GUI_ASSERT( shader, "invalid shader pointer" );
		if (shader != NULL)
		{
			RemoveShader( shader );
			delete shader;
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
