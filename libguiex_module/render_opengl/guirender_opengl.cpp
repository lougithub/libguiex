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



//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	void TryThrowOpenglError( const char* info )
	{
		int errorcode = glGetError();
		if( GL_NO_ERROR != errorcode )
		{
			switch( errorcode )
			{
			case GL_INVALID_ENUM:
				throw CGUIException("error find in opengl in <%s>, error is <%s>!" ,info, "GL_INVALID_ENUM");
				break;
			case GL_INVALID_VALUE:
				throw CGUIException("error find in opengl in <%s>, error is <%s>!" ,info, "GL_INVALID_VALUE");
				break;
			case GL_INVALID_OPERATION:
				throw CGUIException("error find in opengl in <%s>, error is <%s>!" ,info, "GL_INVALID_OPERATION");
				break;
			case GL_STACK_OVERFLOW:
				throw CGUIException("error find in opengl in <%s>, error is <%s>!" ,info, "GL_STACK_OVERFLOW");
				break;
			case GL_STACK_UNDERFLOW:
				throw CGUIException("error find in opengl in <%s>, error is <%s>!" ,info, "GL_STACK_UNDERFLOW");
				break;
			case GL_OUT_OF_MEMORY:
				throw CGUIException("error find in opengl in <%s>, error is <%s>!" ,info, "GL_OUT_OF_MEMORY");
				break;			
			default:
				throw CGUIException("error find in opengl in <%s>, error is <0x%x>!" ,info, errorcode);
			}
		}
	}
#if GUI_DEBUG
# define TRY_THROW_OPENGL_ERROR(info)	TryThrowOpenglError(info)
#else
# define TRY_THROW_OPENGL_ERROR(info)	
#endif
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIRender_opengl);
	//------------------------------------------------------------------------------
	IGUIRender_opengl::IGUIRender_opengl()
		:m_maxTextureSize(0)
		,m_bEnableClip(false)
	{
		m_nCurrentTexture = -1;
		m_bWireFrame = false;
	}
	//------------------------------------------------------------------------------
	IGUIRender_opengl::~IGUIRender_opengl()
	{
	}
	//------------------------------------------------------------------------------
	int IGUIRender_opengl::DoInitialize(void* )
	{
		TRY_THROW_OPENGL_ERROR( "render interface initialize" );
		
		// get the maximum available texture size.
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);

		glGetIntegerv( GL_STENCIL_BITS, &m_nStencilBits);
		m_nMaxStencilRef = (1<<m_nStencilBits) - 1;
		if( m_nMaxStencilRef < 2 )
		{
			GUI_TRACE( "[IGUIRender_opengl::DoInitialize]: stencil is disabled\n" );
		}

		makeGLMatrix( m_aWholeScreenRect.m_gl_world_matrix, CGUIMatrix4::IDENTITY );

		ResetZValue();
		m_nCurrentTexture = -1;
		m_nCurrentStencilRef = 0;

		return 0;
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::DoDestroy()
	{
		DestroyAllTexture();
	}
	//------------------------------------------------------------------------------
	bool IGUIRender_opengl::IsSupportStencil()
	{
		return m_nMaxStencilRef >= 2;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::BeginRender(void)
	{
		TRY_THROW_OPENGL_ERROR("BeginRender start");

		//save current attributes
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT_AND_BACK, m_bWireFrame ? GL_LINE : GL_FILL);

		//update projection matrix
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		const CGUISize& rSize = GSystem->GetScreenSize();
#if 1
		glOrtho(0.0, rSize.m_fWidth,rSize.m_fHeight,0.0, -100000,100000 );
#else
		real fPerspectiveDegree = 45;
		gluPerspective( fPerspectiveDegree, rSize.m_fWidth/rSize.m_fHeight, 0.1, 100000 );
		real fZDistance = rSize.m_fHeight/2 / CGUIMath::Tan( CGUIDegree(fPerspectiveDegree/2));
		gluLookAt( 
			rSize.m_fWidth/2,rSize.m_fHeight/2,-fZDistance,
			rSize.m_fWidth/2,rSize.m_fHeight/2,0, 
			0,-1,0);
#endif

		//update modelview matrix
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();	

		//disable lighting
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		//enable smooth shade
		glShadeModel(GL_SMOOTH);

		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);

		//set blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

		//set texture
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_SCISSOR_TEST);

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
			m_aWholeScreenRect.m_aClipRect.SetSize( rSize );
		}
		glInterleavedArrays(GL_T2F_C4UB_V3F , 0, m_pVertex);

		m_nCurrentTexture = -1;
		m_nCurrentStencilRef = 0;

		TRY_THROW_OPENGL_ERROR("BeginRender end");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::EndRender(void)
	{		
		TRY_THROW_OPENGL_ERROR("EndRender start");

		//restore model view matrix
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix(); 

		//restore projection matrix
		glMatrixMode(GL_PROJECTION);
		glPopMatrix(); 


		//restore former attributes
		glPopAttrib();
		glPopClientAttrib();

		//reset current texture
		m_nCurrentTexture = -1;

		TRY_THROW_OPENGL_ERROR("EndRender end");
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief toggle wire frame.
	*/
	void IGUIRender_opengl::SetWireFrame( bool bWireFrame)
	{
		m_bWireFrame = bWireFrame;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::DrawRect(const CGUIMatrix4& rWorldMatrix,
		const CGUIRect& rDestRect, 
		real fLineWidth,
		real z,
		GUIARGB rColor_topleft,
		GUIARGB rColor_topright,
		GUIARGB rColor_bottomleft,
		GUIARGB rColor_bottomright )
	{
		glInterleavedArrays(GL_C4UB_V3F , 0, m_pVertexForLine);
		glDisable(GL_TEXTURE_2D);
		glLineWidth( fLineWidth );


		//set modelview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		makeGLMatrix( m_gl_matrix, rWorldMatrix );
		glMultMatrixf( m_gl_matrix );

		real fLeft = rDestRect.m_fLeft;
		real fRight = rDestRect.m_fRight;
		real fBottom = rDestRect.m_fBottom;
		real fTop = rDestRect.m_fTop;

		long oglcolor_topleft = ColorToOpengl(rColor_topleft);
		long oglcolor_bottomleft = ColorToOpengl(rColor_bottomleft);
		long oglcolor_bottomright = ColorToOpengl(rColor_bottomright);
		long oglcolor_topright = ColorToOpengl(rColor_topright);

		//vert0
		m_pVertexForLine[0].vertex[0] = fLeft;
		m_pVertexForLine[0].vertex[1] = fTop;
		m_pVertexForLine[0].vertex[2] = z;
		m_pVertexForLine[0].color     = oglcolor_topleft;

		//vert1
		m_pVertexForLine[1].vertex[0] = fLeft;
		m_pVertexForLine[1].vertex[1] = fBottom;
		m_pVertexForLine[1].vertex[2] = z;
		m_pVertexForLine[1].color     = oglcolor_bottomleft;     

		//vert2
		m_pVertexForLine[2].vertex[0] = fRight;
		m_pVertexForLine[2].vertex[1] = fBottom;
		m_pVertexForLine[2].vertex[2] = z;
		m_pVertexForLine[2].color     = oglcolor_bottomright;

		//vert3
		m_pVertexForLine[3].vertex[0] = fRight;
		m_pVertexForLine[3].vertex[1] = fTop;
		m_pVertexForLine[3].vertex[2] = z;
		m_pVertexForLine[3].color     = oglcolor_topright;      

		glDrawArrays( GL_LINE_LOOP , 0, 4 );

		glEnable(GL_TEXTURE_2D);
		glInterleavedArrays(GL_T2F_C4UB_V3F , 0, m_pVertex);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::DrawLine(const CGUIMatrix4& rWorldMatrix,
		const CGUIVector2 &rBegin, 
		const CGUIVector2 &rEnd, 
		real fLineWidth,
		real z,
		GUIARGB rColor_begin,
		GUIARGB rColor_end )
	{
		glInterleavedArrays(GL_C4UB_V3F , 0, m_pVertexForLine);
		glDisable(GL_TEXTURE_2D);
		glLineWidth( fLineWidth );


		//set modelview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		makeGLMatrix( m_gl_matrix, rWorldMatrix );
		glMultMatrixf( m_gl_matrix );

		long oglcolor_begin = ColorToOpengl(rColor_begin);
		long oglcolor_end= ColorToOpengl(rColor_end);

		//vert0
		m_pVertexForLine[0].vertex[0] = rBegin.x;
		m_pVertexForLine[0].vertex[1] = rBegin.y;
		m_pVertexForLine[0].vertex[2] = z;
		m_pVertexForLine[0].color     = rColor_begin;

		//vert1
		m_pVertexForLine[1].vertex[0] = rEnd.x;
		m_pVertexForLine[1].vertex[1] = rEnd.y;
		m_pVertexForLine[1].vertex[2] = z;
		m_pVertexForLine[1].color = rColor_end;     

		glDrawArrays( GL_LINES, 0, 2 );

		glEnable(GL_TEXTURE_2D);
		glInterleavedArrays(GL_T2F_C4UB_V3F , 0, m_pVertex);
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief add a texture into render list
	*/
	void IGUIRender_opengl::DrawTile(
		const CGUIMatrix4& rWorldMatrix,
		const CGUIRect& rDestRect, real z, 
		const CGUITextureImp* pTexture, const CGUIRect& rTextureRect, 
		EImageOrientation eImageOrientation, 				
		GUIARGB  rColor_topleft,
		GUIARGB  rColor_topright,
		GUIARGB  rColor_bottomleft,
		GUIARGB  rColor_bottomright)
	{
		//set modelview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		makeGLMatrix( m_gl_matrix, rWorldMatrix );
		glMultMatrixf( m_gl_matrix );

		//check texture
		if( ((CGUITexture_opengl*)pTexture)->GetOGLTexid() != m_nCurrentTexture )
		{
			m_nCurrentTexture = ((CGUITexture_opengl*)pTexture)->GetOGLTexid();
			glBindTexture(GL_TEXTURE_2D, m_nCurrentTexture);
		}

		//set texture coordinate
		SetTexCoordinate(m_pVertex, rTextureRect, eImageOrientation);

		real fLeft = rDestRect.m_fLeft;
		real fRight = rDestRect.m_fRight;
		real fBottom = rDestRect.m_fBottom;
		real fTop = rDestRect.m_fTop;

		long oglcolor_topleft = ColorToOpengl(rColor_topleft);
		long oglcolor_bottomleft = ColorToOpengl(rColor_bottomleft);
		long oglcolor_bottomright = ColorToOpengl(rColor_bottomright);
		long oglcolor_topright = ColorToOpengl(rColor_topright);

		//vert0
		m_pVertex[0].vertex[0] = fLeft;
		m_pVertex[0].vertex[1] = fTop;
		m_pVertex[0].vertex[2] = z;
		m_pVertex[0].color     = oglcolor_topleft;

		//vert1
		m_pVertex[1].vertex[0] = fLeft;
		m_pVertex[1].vertex[1] = fBottom;
		m_pVertex[1].vertex[2] = z;
		m_pVertex[1].color     = oglcolor_bottomleft;     

		//vert2
		m_pVertex[2].vertex[0] = fRight;
		m_pVertex[2].vertex[1] = fTop;
		m_pVertex[2].vertex[2] = z;
		m_pVertex[2].color     = oglcolor_topright;

		//vert3
		m_pVertex[3].vertex[0] = fRight;
		m_pVertex[3].vertex[1] = fBottom;
		m_pVertex[3].vertex[2] = z;
		m_pVertex[3].color     = oglcolor_bottomright;      

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief add a texture into render list
	*/
	void IGUIRender_opengl::PushClipRect( const CGUIMatrix4& rMatrix, const CGUIRect& rClipRect )
	{
		m_arrayClipRects.push_back( SClipRect() );
		makeGLMatrix( m_arrayClipRects.back().m_gl_world_matrix, rMatrix );
		m_arrayClipRects.back().m_aClipRect = rClipRect;

		if( m_bEnableClip && IsSupportStencil())
		{
			UpdateStencil();
		}
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::PopClipRect( )
	{
		GUI_ASSERT( m_arrayClipRects.empty() == false, "no clip rect to pop" );
		m_arrayClipRects.pop_back();

		if( m_bEnableClip && IsSupportStencil())
		{
			UpdateStencil();
		}
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::UpdateStencil()
	{
		glInterleavedArrays(GL_V3F , 0, m_pVertexForStencil);

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

		glInterleavedArrays(GL_T2F_C4UB_V3F , 0, m_pVertex);
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::RenderRectForStencil( const SClipRect& rRect )
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
		m_pVertexForStencil[0].vertex[0] = fLeft;
		m_pVertexForStencil[0].vertex[1] = fTop;
		m_pVertexForStencil[0].vertex[2] = 1.0f;

		//vert1
		m_pVertexForStencil[1].vertex[0] = fLeft;
		m_pVertexForStencil[1].vertex[1] = fBottom;
		m_pVertexForStencil[1].vertex[2] = 1.0f;

		//vert2
		m_pVertexForStencil[2].vertex[0] = fRight;
		m_pVertexForStencil[2].vertex[1] = fTop;
		m_pVertexForStencil[2].vertex[2] = 1.0f;

		//vert3
		m_pVertexForStencil[3].vertex[0] = fRight;
		m_pVertexForStencil[3].vertex[1] = fBottom;
		m_pVertexForStencil[3].vertex[2] = 1.0f;

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::SetTexCoordinate(SVertex* pTexture, const CGUIRect& tex, EImageOrientation eImageOrientation)
	{
		switch(eImageOrientation)
		{
		case eImageOrientation_FlipHorizon:
			//vert0
			pTexture[0].tex[0] = tex.m_fRight;
			pTexture[0].tex[1] = tex.m_fTop;

			//vert1
			pTexture[1].tex[0] = tex.m_fRight;
			pTexture[1].tex[1] = tex.m_fBottom;

			//vert2
			pTexture[2].tex[0] = tex.m_fLeft;
			pTexture[2].tex[1] = tex.m_fTop;

			//vert3
			pTexture[3].tex[0] = tex.m_fLeft;
			pTexture[3].tex[1] = tex.m_fBottom;
			break;

		case eImageOrientation_FlipVertical:
			//vert0
			pTexture[0].tex[0] = tex.m_fLeft;
			pTexture[0].tex[1] = tex.m_fBottom;

			//vert1
			pTexture[1].tex[0] = tex.m_fLeft;
			pTexture[1].tex[1] = tex.m_fTop;

			//vert2
			pTexture[2].tex[0] = tex.m_fRight;
			pTexture[2].tex[1] = tex.m_fBottom;

			//vert3
			pTexture[3].tex[0] = tex.m_fRight;
			pTexture[3].tex[1] = tex.m_fTop;
			break;

		case eImageOrientation_90CCW:
			//vert0
			pTexture[0].tex[0] = tex.m_fRight;
			pTexture[0].tex[1] = tex.m_fTop;

			//vert1
			pTexture[1].tex[0] = tex.m_fLeft;
			pTexture[1].tex[1] = tex.m_fTop;

			//vert2
			pTexture[2].tex[0] = tex.m_fRight;
			pTexture[2].tex[1] = tex.m_fBottom;

			//vert3
			pTexture[3].tex[0] = tex.m_fLeft;
			pTexture[3].tex[1] = tex.m_fBottom;
			break;

		case eImageOrientation_90CW:
			//vert0
			pTexture[0].tex[0] = tex.m_fLeft;
			pTexture[0].tex[1] = tex.m_fBottom;

			//vert1
			pTexture[1].tex[0] = tex.m_fRight;
			pTexture[1].tex[1] = tex.m_fBottom;

			//vert2
			pTexture[2].tex[0] = tex.m_fLeft;
			pTexture[2].tex[1] = tex.m_fTop;

			//vert3
			pTexture[3].tex[0] = tex.m_fRight;
			pTexture[3].tex[1] = tex.m_fTop;
			break;

		case eImageOrientation_Normal:
		default:
			//vert0
			pTexture[0].tex[0] = tex.m_fLeft;
			pTexture[0].tex[1] = tex.m_fTop;

			//vert1
			pTexture[1].tex[0] = tex.m_fLeft;
			pTexture[1].tex[1] = tex.m_fBottom;

			//vert2
			pTexture[2].tex[0] = tex.m_fRight;
			pTexture[2].tex[1] = tex.m_fTop;

			//vert3
			pTexture[3].tex[0] = tex.m_fRight;
			pTexture[3].tex[1] = tex.m_fBottom;
			break;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Creates a 'null' Texture object.
	* @return a newly created Texture object.  The returned Texture object has no size or imagery 
	* associated with it, and is generally of little or no use.
	*/
	CGUITextureImp*	IGUIRender_opengl::CreateTexture(void)
	{
		CGUITexture_opengl* pTexture = new CGUITexture_opengl(this);
		m_setTexture.insert(pTexture);
		return pTexture;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Create a Texture object using the given image file.
	* @param filename String object that specifies the path and filename of the image file to use 
	* when creating the texture.
	* return a newly created Texture object.  The initial contents of the texture memory is the 
	* requested image file.
	* @note Textures are always created with a size that is a power of 2.  If the file you specify 
	* is of a size that is not a power of two, the final size will be rounded up.  Additionally, 
	* textures are always square, so the ultimate size is governed by the larger of the width and 
	* height of the specified file.  You can check the ultimate sizes by querying the texture after creation.
	*/
	CGUITextureImp*	IGUIRender_opengl::CreateTexture(const CGUIString& filename)
	{
		CGUITexture_opengl* pTexture = new CGUITexture_opengl(this);
		if( pTexture->LoadFromFile(filename) != 0 )
		{
			//failed
			delete pTexture;
			return NULL;
		}
		m_setTexture.insert(pTexture);
		return pTexture;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief Create a Texture object with the given pixel dimensions as specified by \a size.  
	* NB: Textures are always square.
	* param size real value that specifies the size used for the width and height when creating 
	* the new texture.
	* @return a newly created Texture object.  The initial contents of the texture memory is 
	* undefined / random.
	* note Textures are always created with a size that is a power of 2.  If you specify a size that 
	* is not a power of two, the final size will be rounded up.  So if you specify a size of 1024, the 
	* texture will be (1024 x 1024), however, if you specify a size of 1025, the texture will be 
	* (2048 x 2048).  You can check the ultimate size by querying the texture after creation.
	*/	
	CGUITextureImp*	IGUIRender_opengl::CreateTexture(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat)
	{
		CGUITexture_opengl* pTexture = new CGUITexture_opengl(this);
		pTexture->SetOpenglTextureSize(nWidth,nHeight,ePixelFormat);
		m_setTexture.insert(pTexture);
		return pTexture;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Destroy the given Texture object.
	* @param texture pointer to the Texture object to be destroyed
	*/
	void IGUIRender_opengl::DestroyTexture(CGUITextureImp* texture)
	{
		if (texture != NULL)
		{
			TSetTexture::iterator itor = m_setTexture.find(texture);
			GUI_ASSERT( itor!= m_setTexture.end(),"failed to destroy texture");
			m_setTexture.erase(itor);

			delete texture;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Destroy all textures
	*/
	void IGUIRender_opengl::DestroyAllTexture()
	{
		while( m_setTexture.empty() == false)
		{
			DestroyTexture(*m_setTexture.begin());
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return the current width of the display in pixels
	* @return real value equal to the current width of the display in pixels.
	*/
	uint16	IGUIRender_opengl::GetWidth(void) const
	{
		GUI_ASSERT(0, "not implemented");
		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return the current height of the display in pixels
	* @return real value equal to the current height of the display in pixels.
	*/
	uint16	IGUIRender_opengl::GetHeight(void) const
	{
		GUI_ASSERT(0, "not implemented");
		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return the maximum texture size available
	* @return Size of the maximum supported texture in pixels (textures are always assumed to be square)
	*/
	uint32	IGUIRender_opengl::GetMaxTextureSize(void) const
	{
		return m_maxTextureSize;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return the horizontal display resolution dpi
	* @return horizontal resolution of the display in dpi.
	*/
	uint32	IGUIRender_opengl::GetHorzScreenDPI(void) const
	{
		return 96;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Return the vertical display resolution dpi
	* @return vertical resolution of the display in dpi.
	*/
	uint32	IGUIRender_opengl::GetVertScreenDPI(void) const
	{
		return 96;
	}
	//------------------------------------------------------------------------------
	long IGUIRender_opengl::ColorToOpengl(GUIARGB col) const
	{
		return ((col& 0xFF000000) |				//A
			((col & 0x00FF0000)>>16)  		|			//R
			(col & 0x0000FF00)  		|		//G
			((col & 0x000000FF)<<16)					//B
			);

	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::makeGLMatrix(real gl_matrix[16], const CGUIMatrix4& m)
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
	void IGUIRender_opengl::EnableClip( bool bEnable )
	{
		m_bEnableClip = bEnable;
	}
	//-----------------------------------------------------------------------------

}//namespace guiex
