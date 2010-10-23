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
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guicolorrect.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guirenderrect.h>



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
		,m_bQueueing(true)
	{
		m_nCommandIdx = 0;
		m_nVertexIdx = 0;
		m_nCurrentTexture = -1;
		m_eLastCommand = eCommand_None;
		m_bWireFrame = false;
		m_bEnableScissor = true;
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

		m_vecVertex.resize(OPENGL_RENDERER_VBUFF_CAPACITY);

		m_nCommandIdx = 0;
		m_nVertexIdx = 0;
		m_nCurrentTexture = -1;
		m_eLastCommand = eCommand_None;

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
	void	IGUIRender_opengl::SetWireFrame( bool bWireFrame)
	{
		m_bWireFrame = bWireFrame;
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::EnableScissor( bool bEnable)
	{
		m_bEnableScissor = bEnable;
	}
	//------------------------------------------------------------------------------
	IGUIRender_opengl::SVertex* IGUIRender_opengl::AllocateVertexNode(uint32 nNum)
	{
		if( m_vecVertex.size() <= m_nVertexIdx+nNum )
		{
			m_vecVertex.resize(m_nVertexIdx + nNum + 100);
		}
		uint32 nIdx = m_nVertexIdx;
		m_nVertexIdx += nNum;
		return &m_vecVertex.front()+nIdx;
	}
	//------------------------------------------------------------------------------
	IGUIRender_opengl::SRenderCommand* IGUIRender_opengl::GetLastCommandNode()
	{
		GUI_ASSERT( m_nCommandIdx>0, "[IGUIRender_opengl::GetLastCommandNode]: command list is empty");
		return &m_vecCommand.front()+m_nCommandIdx-1;
	}
	//------------------------------------------------------------------------------
	IGUIRender_opengl::SRenderCommand* IGUIRender_opengl::AllocateCommandNode(ERenderCommand eCommand)
	{
		if( m_vecCommand.size() <= m_nCommandIdx )
		{
			m_vecCommand.resize(m_nCommandIdx + 100);
		}
		SRenderCommand* pCommand = &m_vecCommand.front()+m_nCommandIdx++;
		m_eLastCommand = eCommand;
		pCommand->m_nID = eCommand;
		return pCommand;
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::AddRenderTexture(const CGUIRect& rDestRect, real z, 
		const CGUITextureImp* pTexture, const CGUIRect& rTextureRect, 
		EImageOperation eImageOperation, 				
		GUIARGB  rColor_topleft,
		GUIARGB  rColor_topright,
		GUIARGB  rColor_bottomleft,
		GUIARGB  rColor_bottomright)
	{
		//check texture
		if( ((CGUITexture_opengl*)pTexture)->GetOGLTexid() != m_nCurrentTexture )
		{
			m_nCurrentTexture = ((CGUITexture_opengl*)pTexture)->GetOGLTexid();
			SRenderCommand* pCommand = AllocateCommandNode(eCommand_SetTexture);
			pCommand->m_nPara1 = m_nCurrentTexture;
		}

		//set vertex
		if( m_eLastCommand == eCommand_DrawVertex )
		{
			SRenderCommand* pCommand = GetLastCommandNode();
			pCommand->m_nPara2 += 2;
		}
		else
		{
			SRenderCommand* pCommand = AllocateCommandNode(eCommand_DrawVertex);
			pCommand->m_nPara1 = m_nVertexIdx;
			pCommand->m_nPara2 = 2;
		}

		SVertex* pVertex = AllocateVertexNode(VERTEX_PER_TEXTURE);

		//set texture coordinate
		SetTexCoordinate(pVertex, rTextureRect, eImageOperation);

		real fLeft = rDestRect.m_fLeft;
		real fRight = rDestRect.m_fRight;
		real fBottom = rDestRect.m_fBottom;
		real fTop = rDestRect.m_fTop;

		long oglcolor_topleft = ColorToOpengl(rColor_topleft);
		long oglcolor_bottomleft = ColorToOpengl(rColor_bottomleft);
		long oglcolor_bottomright = ColorToOpengl(rColor_bottomright);
		long oglcolor_topright = ColorToOpengl(rColor_topright);


		//vert0
		pVertex[0].vertex[0] = fLeft;
		pVertex[0].vertex[1] = fTop;
		pVertex[0].vertex[2] = z;
		pVertex[0].color     = oglcolor_topleft;

		//vert1
		pVertex[1].vertex[0] = fLeft;
		pVertex[1].vertex[1] = fBottom;
		pVertex[1].vertex[2] = z;
		pVertex[1].color     = oglcolor_bottomleft;     

		//vert2
		pVertex[2].vertex[0] = fRight;
		pVertex[2].vertex[1] = fBottom;
		pVertex[2].vertex[2] = z;
		pVertex[2].color     = oglcolor_bottomright;

		//vert3
		pVertex[3].vertex[0] = fRight;
		pVertex[3].vertex[1] = fTop;
		pVertex[3].vertex[2] = z;
		pVertex[3].color     = oglcolor_topright;      

		//vert4
		pVertex[4].vertex[0] = fLeft;
		pVertex[4].vertex[1] = fTop;
		pVertex[4].vertex[2] = z;
		pVertex[4].color     = oglcolor_topleft;

		//vert5
		pVertex[5].vertex[0] = fRight;
		pVertex[5].vertex[1] = fBottom;
		pVertex[5].vertex[2] = z;
		pVertex[5].color     = oglcolor_bottomright;


		// if not queuing, render directly (as in, right now!)
		if (!m_bQueueing)
		{
			BeginRender();
			RenderVBuffer();
			EndRender();
		}
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::AddRenderTexture(const CGUIRenderRect& rRenderRect, 
		real z, 
		const CGUITextureImp* pTexture, 
		const CGUIRect& rTextureRect, 
		EImageOperation eImageOperation,
		GUIARGB rColor_topleft,
		GUIARGB rColor_topright,
		GUIARGB rColor_bottomleft,
		GUIARGB rColor_bottomright)
	{
		//check texture
		if( ((CGUITexture_opengl*)pTexture)->GetOGLTexid() != m_nCurrentTexture )
		{
			m_nCurrentTexture = ((CGUITexture_opengl*)pTexture)->GetOGLTexid();
			SRenderCommand* pCommand = AllocateCommandNode(eCommand_SetTexture);
			pCommand->m_nPara1 = m_nCurrentTexture;
		}

		//set vertex
		if( m_eLastCommand == eCommand_DrawVertex )
		{
			SRenderCommand* pCommand = GetLastCommandNode();
			pCommand->m_nPara2 += 2;
		}
		else
		{
			SRenderCommand* pCommand = AllocateCommandNode(eCommand_DrawVertex);
			pCommand->m_nPara1 = m_nVertexIdx;
			pCommand->m_nPara2 = 2;
		}

		SVertex* pVertex = AllocateVertexNode(VERTEX_PER_TEXTURE);

		//set texture coordinate
		SetTexCoordinate(pVertex, rTextureRect, eImageOperation);

		long oglcolor_topleft = ColorToOpengl(rColor_topleft);
		long oglcolor_bottomleft = ColorToOpengl(rColor_bottomleft);
		long oglcolor_bottomright = ColorToOpengl(rColor_bottomright);
		long oglcolor_topright = ColorToOpengl(rColor_topright);


		//vert0
		pVertex[0].vertex[0] = rRenderRect.m_vecVertex[0].m_aVector.x;
		pVertex[0].vertex[1] = rRenderRect.m_vecVertex[0].m_aVector.y;
		pVertex[0].vertex[2] = z;
		pVertex[0].color     = oglcolor_topleft;

		//vert1
		pVertex[1].vertex[0] = rRenderRect.m_vecVertex[3].m_aVector.x;
		pVertex[1].vertex[1] = rRenderRect.m_vecVertex[3].m_aVector.y;
		pVertex[1].vertex[2] = z;
		pVertex[1].color     = oglcolor_bottomleft;     

		//vert2
		pVertex[2].vertex[0] = rRenderRect.m_vecVertex[2].m_aVector.x;
		pVertex[2].vertex[1] = rRenderRect.m_vecVertex[2].m_aVector.y;
		pVertex[2].vertex[2] = z;
		pVertex[2].color     = oglcolor_bottomright;

		//vert3
		pVertex[3].vertex[0] = rRenderRect.m_vecVertex[1].m_aVector.x;
		pVertex[3].vertex[1] = rRenderRect.m_vecVertex[1].m_aVector.y;
		pVertex[3].vertex[2] = z;
		pVertex[3].color     = oglcolor_topright;      

		//vert4
		pVertex[4].vertex[0] = rRenderRect.m_vecVertex[0].m_aVector.x;
		pVertex[4].vertex[1] = rRenderRect.m_vecVertex[0].m_aVector.y;
		pVertex[4].vertex[2] = z;
		pVertex[4].color     = oglcolor_topleft;

		//vert5
		pVertex[5].vertex[0] = rRenderRect.m_vecVertex[2].m_aVector.x;
		pVertex[5].vertex[1] = rRenderRect.m_vecVertex[2].m_aVector.y;
		pVertex[5].vertex[2] = z;
		pVertex[5].color     = oglcolor_bottomright;


		// if not queuing, render directly (as in, right now!)
		if (!m_bQueueing)
		{
			BeginRender();
			RenderVBuffer();
			EndRender();
		}
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::AddScissor( const CGUIRect& rClipRect)
	{		
		GLint nLeft = (GLint)(floor(rClipRect.m_fLeft) + 0.5f);
		GLint nTop = (GLint)(floor(rClipRect.m_fTop) + 0.5f);
		GLint nRight = (GLint)(ceil(rClipRect.m_fRight) + 0.5f);
		GLint nBottom = (GLint)(ceil(rClipRect.m_fBottom) + 0.5f);

		SClipRect aRect;
		aRect.x = nLeft;
		aRect.y = GLint(CGUIWidgetSystem::Instance()->GetScreenHeight() - nBottom);
		aRect.width = nRight - nLeft;
		aRect.height = nBottom - nTop;
		if( aRect.x != m_aCurrentClipRect.x ||
			aRect.y != m_aCurrentClipRect.y ||
			aRect.width != m_aCurrentClipRect.width ||
			aRect.height != m_aCurrentClipRect.height)
		{
			m_aCurrentClipRect = aRect;

			//set scissor
			SRenderCommand* pCommand = AllocateCommandNode(eCommand_SetScissor);
			pCommand->m_nPara1 = m_aCurrentClipRect.x;
			pCommand->m_nPara2 = m_aCurrentClipRect.y;
			pCommand->m_nPara3 = m_aCurrentClipRect.width;
			pCommand->m_nPara4 = m_aCurrentClipRect.height;

			if( !m_bQueueing )
			{
				BeginRender();
				RenderVBuffer();
				EndRender();
			}
		}
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::BeginRender(void)
	{
		//save current attributes
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		//glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, m_bWireFrame ? GL_LINE : GL_FILL);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		const CGUISize& rSize = CGUIWidgetSystem::Instance()->GetScreenSize();
		gluOrtho2D(0.0, rSize.m_fWidth,rSize.m_fHeight,0.0 );

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();	

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);

		if( m_bEnableScissor )
		{
			glEnable(GL_SCISSOR_TEST);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}

		glInterleavedArrays(GL_T2F_C4UB_V3F , 0, &m_vecVertex.front());
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::EndRender(void)
	{		
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix(); 
		glMatrixMode(GL_PROJECTION);
		glPopMatrix(); 


		//restore former attributes
		glPopAttrib();
		glPopClientAttrib();

		m_nCurrentTexture = -1;
	}
	//------------------------------------------------------------------------------
	void IGUIRender_opengl::RenderVBuffer(void)
	{
		//do command
		TListCommand::iterator itorEnd =  m_vecCommand.begin()+m_nCommandIdx;
		for( TListCommand::iterator itor = m_vecCommand.begin();
			itor != itorEnd;
			++itor)
		{
			const SRenderCommand& rCommand = *itor;
			switch(rCommand.m_nID)
			{
			case eCommand_SetTexture:
				glBindTexture(GL_TEXTURE_2D, rCommand.m_nPara1);
				break;
			case eCommand_SetScissor:
				glScissor(rCommand.m_nPara1, rCommand.m_nPara2, rCommand.m_nPara3, rCommand.m_nPara4);
				break;
			case eCommand_DrawVertex:
				glDrawArrays(GL_TRIANGLES, rCommand.m_nPara1, rCommand.m_nPara2*3);
				break;
			default:
				throw CGUIException("[IGUIRender_dx9::RenderVBuffer]:unknown render command");
			}
		}

		ClearRenderList();
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::DoRender(void)
	{
		BeginRender();
		RenderVBuffer();
		EndRender();
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

			//vert4
			pTexture[4].tex[0] = tex.m_fRight;
			pTexture[4].tex[1] = tex.m_fTop;

			//vert5
			pTexture[5].tex[0] = tex.m_fLeft;
			pTexture[5].tex[1] = tex.m_fBottom;
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

			//vert4
			pTexture[4].tex[0] = tex.m_fLeft;
			pTexture[4].tex[1] = tex.m_fBottom;

			//vert5
			pTexture[5].tex[0] = tex.m_fRight;
			pTexture[5].tex[1] = tex.m_fTop;
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

			//vert4
			pTexture[4].tex[0] = tex.m_fRight;
			pTexture[4].tex[1] = tex.m_fTop;

			//vert5
			pTexture[5].tex[0] = tex.m_fLeft;
			pTexture[5].tex[1] = tex.m_fBottom;
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

			//vert4
			pTexture[4].tex[0] = tex.m_fLeft;
			pTexture[4].tex[1] = tex.m_fBottom;

			//vert5
			pTexture[5].tex[0] = tex.m_fRight;
			pTexture[5].tex[1] = tex.m_fTop;
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

			//vert4
			pTexture[4].tex[0] = tex.m_fLeft;
			pTexture[4].tex[1] = tex.m_fTop;

			//vert5
			pTexture[5].tex[0] = tex.m_fRight;
			pTexture[5].tex[1] = tex.m_fBottom;
			break;
		}
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::ClearRenderList(void)
	{
		m_nCommandIdx = 0;
		m_nVertexIdx = 0;
		m_eLastCommand = eCommand_None;
	}
	//------------------------------------------------------------------------------
	void	IGUIRender_opengl::EnableRenderQueue(bool bEnable)
	{
		m_bQueueing = bEnable;
	}
	//------------------------------------------------------------------------------
	bool	IGUIRender_opengl::IsRenderQueueEnabled(void) const
	{
		return m_bQueueing;
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

}//namespace guiex
