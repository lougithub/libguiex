/** 
* @file guirender_opengl.cpp
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\render_opengl\guirender_opengl.h>
#include <libguiex_module\render_opengl\guitexture_opengl.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guicolorrect.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guirenderrect.h>



//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIRender_opengl);
	//------------------------------------------------------------------------------
	IGUIRender_opengl::IGUIRender_opengl()
		:m_maxTextureSize(0)
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
		// get the maximum available texture size.
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);

		ResetZValue();
		m_nCurrentTexture = -1;

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
	void IGUIRender_opengl::TestOpenglError( const char* info )
	{
		int errorcode = glGetError();
		if( GL_NO_ERROR != errorcode )
		{
			printf("error find in opengles: <%s>  : ", info);

			switch( errorcode )
			{
			case GL_INVALID_ENUM:
				printf("GL_INVALID_ENUM\n");
				break;
			case GL_INVALID_VALUE:
				printf("GL_INVALID_VALUE\n");
				break;
			case GL_INVALID_OPERATION:
				printf("GL_INVALID_OPERATION\n");
				break;
			case GL_STACK_OVERFLOW:
				printf("GL_STACK_OVERFLOW\n");
				break;
			case GL_STACK_UNDERFLOW:
				printf("GL_STACK_UNDERFLOW\n");
				break;
			case GL_OUT_OF_MEMORY:
				printf("GL_OUT_OF_MEMORY\n");
				break;			
			default:
				printf("unknown opengl error: 0x%x\n", errorcode);
			}

			assert( GL_NO_ERROR == errorcode);
		}

	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::SetWireFrame( bool bWireFrame)
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
		TestOpenglError("DrawRect 1");

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

		TestOpenglError("DrawRect 2");
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::DrawTile(const CGUIMatrix4& rWorldMatrix,
		const CGUIRect& rDestRect, real z, 
		const CGUITextureImp* pTexture, const CGUIRect& rTextureRect, 
		EImageOperation eImageOperation, 				
		GUIARGB  rColor_topleft,
		GUIARGB  rColor_topright,
		GUIARGB  rColor_bottomleft,
		GUIARGB  rColor_bottomright)
	{
		TestOpenglError("DrawTile 1");

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
		SetTexCoordinate(m_pVertex, rTextureRect, eImageOperation);

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
		m_pVertex[2].vertex[1] = fBottom;
		m_pVertex[2].vertex[2] = z;
		m_pVertex[2].color     = oglcolor_bottomright;

		//vert3
		m_pVertex[3].vertex[0] = fRight;
		m_pVertex[3].vertex[1] = fTop;
		m_pVertex[3].vertex[2] = z;
		m_pVertex[3].color     = oglcolor_topright;      

		glDrawArrays( GL_POLYGON, 0, 4 );

		TestOpenglError("DrawTile 2");
	}
	
	void	IGUIRender_opengl::PushClipRect( const CGUIMatrix4& rMatrix, const CGUIRect& rClipRect )
	{
		m_arrayClipRects.push_back( SClipRect() );
		makeGLMatrix( m_arrayClipRects.back().m_gl_world_matrix, rMatrix );
		m_arrayClipRects.back().m_aClipRect = rClipRect;

		UpdateStencil();
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::PopClipRect( )
	{
		GUI_ASSERT( m_arrayClipRects.empty() == false, "no clip rect to pop" );
		m_arrayClipRects.pop_back();

		UpdateStencil();
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::BeginRender(void)
	{
		TestOpenglError("BeginRender 1");

		//save current attributes
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT_AND_BACK, m_bWireFrame ? GL_LINE : GL_FILL);

		//update projection matrix
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		const CGUISize& rSize = CGUIWidgetSystem::Instance()->GetScreenSize();
#if 1
		gluOrtho2D(0.0, rSize.m_fWidth,rSize.m_fHeight,0.0 );
#else
		gluPerspective( 90, rSize.m_fWidth/rSize.m_fHeight, 0.1, 1000 );
		gluLookAt( rSize.m_fWidth/2,rSize.m_fHeight/2,-rSize.m_fWidth/2,rSize.m_fWidth/2,rSize.m_fHeight/2,0, 0,-1,0);
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

		glEnable( GL_STENCIL_TEST );	
		glInterleavedArrays(GL_T2F_C4UB_V3F , 0, m_pVertex);

		m_nCurrentTexture = -1;

		TestOpenglError("BeginRender 2");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::EndRender(void)
	{		
		TestOpenglError("EndRender 1");

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

		TestOpenglError("EndRender 2");
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::UpdateStencil()
	{
		TestOpenglError("UpdateStencil 1");

		glInterleavedArrays(GL_V3F , 0, m_pVertexForStencil);
		
		//clear stencil buffer to 1 for all area visible now
		glClearStencil( 0 );
		glClear( GL_STENCIL_BUFFER_BIT );

		// Set color mask and disable texture
		glColorMask( false, false, false, false );		
		glDisable( GL_TEXTURE_2D );

		// Enable stencil buffer for "marking" the floor 
		glEnable( GL_STENCIL_TEST );	

		glStencilFunc( GL_EQUAL, 1, 1 );
		glStencilOp( GL_ZERO, GL_ZERO, GL_KEEP );

		//render clip rect to stencil buffer
		for( std::vector<SClipRect>::iterator itor =  m_arrayClipRects.begin();
			itor != m_arrayClipRects.end();
			++itor )
		{
			SClipRect& rClipRect = *itor;
			RenderRectForStencil( rClipRect );
		}

		//restore color and texture state
		glColorMask( true, true, true, true );		
		glEnable( GL_TEXTURE_2D );

		//reset stencil state
		glStencilFunc( GL_EQUAL, 1, 1 );
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

		glInterleavedArrays(GL_T2F_C4UB_V3F , 0, m_pVertex);

		TestOpenglError("UpdateStencil 2");
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::RenderRectForStencil( const SClipRect& rRect )
	{
		TestOpenglError("RenderRectForStencil 1");

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
		m_pVertexForStencil[2].vertex[1] = fBottom;
		m_pVertexForStencil[2].vertex[2] = 1.0f;

		//vert3
		m_pVertexForStencil[3].vertex[0] = fRight;
		m_pVertexForStencil[3].vertex[1] = fTop;
		m_pVertexForStencil[3].vertex[2] = 1.0f;

		glDrawArrays( GL_POLYGON, 0, 4 );

		TestOpenglError("RenderRectForStencil 2");
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::SetTexCoordinate(SVertex* pTexture, const CGUIRect& tex, EImageOperation eImageOperation)
	{
		switch(eImageOperation)
		{
		case IMAGE_FLIPHORIZON:
			//vert0
			pTexture[0].tex[0] = tex.m_fRight;
			pTexture[0].tex[1] = tex.m_fTop;

			//vert1
			pTexture[1].tex[0] = tex.m_fRight;
			pTexture[1].tex[1] = tex.m_fBottom;

			//vert2
			pTexture[2].tex[0] = tex.m_fLeft;
			pTexture[2].tex[1] = tex.m_fBottom;

			//vert3
			pTexture[3].tex[0] = tex.m_fLeft;
			pTexture[3].tex[1] = tex.m_fTop;
			break;

		case IMAGE_FLIPVERTICAL:
			//vert0
			pTexture[0].tex[0] = tex.m_fLeft;
			pTexture[0].tex[1] = tex.m_fBottom;

			//vert1
			pTexture[1].tex[0] = tex.m_fLeft;
			pTexture[1].tex[1] = tex.m_fTop;

			//vert2
			pTexture[2].tex[0] = tex.m_fRight;
			pTexture[2].tex[1] = tex.m_fTop;

			//vert3
			pTexture[3].tex[0] = tex.m_fRight;
			pTexture[3].tex[1] = tex.m_fBottom;
			break;

		case IMAGE_ROTATE90CCW:
			//vert0
			pTexture[0].tex[0] = tex.m_fRight;
			pTexture[0].tex[1] = tex.m_fTop;

			//vert1
			pTexture[1].tex[0] = tex.m_fLeft;
			pTexture[1].tex[1] = tex.m_fTop;

			//vert2
			pTexture[2].tex[0] = tex.m_fLeft;
			pTexture[2].tex[1] = tex.m_fBottom;

			//vert3
			pTexture[3].tex[0] = tex.m_fRight;
			pTexture[3].tex[1] = tex.m_fBottom;
			break;

		case IMAGE_ROTATE90CW:
			//vert0
			pTexture[0].tex[0] = tex.m_fLeft;
			pTexture[0].tex[1] = tex.m_fBottom;

			//vert1
			pTexture[1].tex[0] = tex.m_fRight;
			pTexture[1].tex[1] = tex.m_fBottom;

			//vert2
			pTexture[2].tex[0] = tex.m_fRight;
			pTexture[2].tex[1] = tex.m_fTop;

			//vert3
			pTexture[3].tex[0] = tex.m_fLeft;
			pTexture[3].tex[1] = tex.m_fTop;
			break;

		case IMAGE_NONE:
		default:
			//vert0
			pTexture[0].tex[0] = tex.m_fLeft;
			pTexture[0].tex[1] = tex.m_fTop;

			//vert1
			pTexture[1].tex[0] = tex.m_fLeft;
			pTexture[1].tex[1] = tex.m_fBottom;

			//vert2
			pTexture[2].tex[0] = tex.m_fRight;
			pTexture[2].tex[1] = tex.m_fBottom;

			//vert3
			pTexture[3].tex[0] = tex.m_fRight;
			pTexture[3].tex[1] = tex.m_fTop;
			break;
		}
	}
	//------------------------------------------------------------------------------
	CGUITextureImp*	IGUIRender_opengl::CreateTexture(void)
	{
		CGUITexture_opengl* pTexture = new CGUITexture_opengl(this);
		m_setTexture.insert(pTexture);
		return pTexture;
	}
	//------------------------------------------------------------------------------
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
	CGUITextureImp*	IGUIRender_opengl::CreateTexture(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat)
	{
		CGUITexture_opengl* pTexture = new CGUITexture_opengl(this);
		pTexture->SetOpenglTextureSize(nWidth,nHeight,ePixelFormat);
		m_setTexture.insert(pTexture);
		return pTexture;
	}
	//------------------------------------------------------------------------------
	void		IGUIRender_opengl::DestroyTexture(CGUITextureImp* texture)
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
	void		IGUIRender_opengl::DestroyAllTexture()
	{
		while( m_setTexture.empty() == false)
		{
			DestroyTexture(*m_setTexture.begin());
		}
	}
	//------------------------------------------------------------------------------
	uint16	IGUIRender_opengl::GetWidth(void) const
	{
		GUI_ASSERT(0, "not implemented");
		return 0;
	}
	//------------------------------------------------------------------------------
	uint16	IGUIRender_opengl::GetHeight(void) const
	{
		GUI_ASSERT(0, "not implemented");
		return 0;
	}
	//------------------------------------------------------------------------------
	uint32	IGUIRender_opengl::GetMaxTextureSize(void) const
	{
		return m_maxTextureSize;
	}
	//------------------------------------------------------------------------------
	uint32	IGUIRender_opengl::GetHorzScreenDPI(void) const
	{
		return 96;
	}
	//------------------------------------------------------------------------------
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
}//namespace guiex
