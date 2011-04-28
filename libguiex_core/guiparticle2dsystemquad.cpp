/** 
* @file guiparticlesystemquad.cpp
* @brief 
* @author ken
* @date 2011-01-12
*/


//============================================================================//
// include
//============================================================================// 
#include "guiparticle2dsystemquad.h"
#include "guiexception.h"
#include "guimath.h"
#include "guirect.h"
#include "guiinterfacerender.h"
#include "guimatrix4.h"
#include "guitexture.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUIParticle2DSystemQuad::CGUIParticle2DSystemQuad( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIParticle2DSystem( rName, rSceneName )
		,m_pQuads(NULL)
		,m_pIndices(NULL)
	{
	}
	//------------------------------------------------------------------------------
	CGUIParticle2DSystemQuad::~CGUIParticle2DSystemQuad()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIParticle2DSystemQuad::DoLoad()
	{
		if( 0 != CGUIParticle2DSystem::DoLoad() )
		{
			return -1;
		}

		// allocating data space
		m_pQuads = new SR_V2F_C4F_T2F_Quad[totalParticles];
		m_pIndices = new uint16[totalParticles * 6];

		if( !m_pQuads || !m_pIndices ) 
		{
			if( m_pQuads )
			{
				delete[] m_pQuads;
				m_pQuads = NULL;
			}
			if(m_pIndices)
			{
				delete[] m_pIndices;
				m_pIndices = NULL;
			}
			CGUIException::ThrowException(" Particle system: not enough memory");
			return -1;
		}

		// initialize only once the texCoords and the indices
		InitTexCoordsWithUVRect( CGUIRect(0.f,0.f,1.f,1.f) );
		InitIndices();

#if CC_USES_VBO
		// create the VBO buffer
		glGenBuffers(1, &m_pQuadsID);

		// initial binding
		glBindBuffer(GL_ARRAY_BUFFER, m_pQuadsID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0])*totalParticles, m_pQuads,GL_DYNAMIC_DRAW);	
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystemQuad::DoUnload()
	{
		if( m_pQuads )
		{
			delete[] m_pQuads;
			m_pQuads = NULL;
		}
		if( m_pIndices )
		{
			delete[] m_pIndices;
			m_pIndices = NULL;
		}
#if CC_USES_VBO
		glDeleteBuffers(1, &quadsID);
#endif

		CGUIParticle2DSystem::DoUnload();
	}
	//------------------------------------------------------------------------------
	// rect is in Points coordinates.
	void CGUIParticle2DSystemQuad::InitTexCoordsWithUVRect( const CGUIRect& rUVRect ) const
	{
		for( uint32 i=0; i<totalParticles; i++) 
		{
			// bottom-left vertex:
			m_pQuads[i].bl.texCoords.u = rUVRect.m_fLeft;
			m_pQuads[i].bl.texCoords.v = rUVRect.m_fBottom;
			// bottom-right vertex:
			m_pQuads[i].br.texCoords.u = rUVRect.m_fRight;
			m_pQuads[i].br.texCoords.v = rUVRect.m_fBottom;
			// top-left vertex:
			m_pQuads[i].tl.texCoords.u = rUVRect.m_fLeft;
			m_pQuads[i].tl.texCoords.v = rUVRect.m_fTop;
			// top-right vertex:
			m_pQuads[i].tr.texCoords.u = rUVRect.m_fRight;
			m_pQuads[i].tr.texCoords.v = rUVRect.m_fTop;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystemQuad::InitIndices() const
	{
		for( int16 i=0;i<int16(totalParticles);i++) 
		{
			const int16 i6 = i*6;
			const int16 i4 = i*4;
			m_pIndices[i6+0] = (int16) i4+0; //tl
			m_pIndices[i6+1] = (int16) i4+1; //bl
			m_pIndices[i6+2] = (int16) i4+2; //tr

			m_pIndices[i6+3] = (int16) i4+3; //br
			m_pIndices[i6+4] = (int16) i4+2; //tr
			m_pIndices[i6+5] = (int16) i4+1; //bl
		}
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystemQuad::UpdateQuadWithParticle( CGUIParticle2D* particle, const CGUIVector2& rNewPos )
	{
		// colors
		SR_V2F_C4F_T2F_Quad *quad = &(m_pQuads[particleIdx]);
		ConvGUIColor_2_C4f( particle->color, quad->bl.colors);
		ConvGUIColor_2_C4f( particle->color, quad->br.colors);
		ConvGUIColor_2_C4f( particle->color, quad->tl.colors);
		ConvGUIColor_2_C4f( particle->color, quad->tr.colors);

		// vertices
		real size_2 = particle->size/2;
		if( particle->rotation )
		{
			real x1 = -size_2;
			real y1 = -size_2;

			real x2 = size_2;
			real y2 = size_2;
			real x = rNewPos.x;
			real y = rNewPos.y;

			real r = (real)-CGUIMath::DegreesToRadians(particle->rotation);
			real cr = cosf(r);
			real sr = sinf(r);
			real ax = x1 * cr - y1 * sr + x;
			real ay = x1 * sr + y1 * cr + y;
			real bx = x2 * cr - y1 * sr + x;
			real by = x2 * sr + y1 * cr + y;
			real cx = x2 * cr - y2 * sr + x;
			real cy = x2 * sr + y2 * cr + y;
			real dx = x1 * cr - y2 * sr + x;
			real dy = x1 * sr + y2 * cr + y;

			// bottom-left
			quad->bl.vertices.x = ax;
			quad->bl.vertices.y = ay;

			// bottom-right vertex:
			quad->br.vertices.x = bx;
			quad->br.vertices.y = by;

			// top-left vertex:
			quad->tl.vertices.x = dx;
			quad->tl.vertices.y = dy;

			// top-right vertex:
			quad->tr.vertices.x = cx;
			quad->tr.vertices.y = cy;
		} 
		else
		{
			// bottom-left vertex:
			quad->bl.vertices.x = rNewPos.x - size_2;
			quad->bl.vertices.y = rNewPos.y + size_2;

			// bottom-right vertex:
			quad->br.vertices.x = rNewPos.x + size_2;
			quad->br.vertices.y = rNewPos.y + size_2;

			// top-left vertex:
			quad->tl.vertices.x = rNewPos.x - size_2;
			quad->tl.vertices.y = rNewPos.y - size_2;

			// top-right vertex:
			quad->tr.vertices.x = rNewPos.x + size_2;
			quad->tr.vertices.y = rNewPos.y - size_2;				
		}
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystemQuad::PostStep()
	{
#if CC_USES_VBO
		glBindBuffer(GL_ARRAY_BUFFER, quadsID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_pQuads[0])*particleCount, m_pQuads);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystemQuad::Render( IGUIInterfaceRender* pRender )
	{
		CGUIParticle2DSystem::Render( pRender );

		SGUIBlendFunc oldBlendFunc;
		pRender->GetBlendFunc( oldBlendFunc );

		if( blendFunc.src != oldBlendFunc.src ||
			blendFunc.dst != oldBlendFunc.dst )
		{
			pRender->SetBlendFunc( blendFunc );
		}

		pRender->DrawQuads(
			texture, 
			m_pQuads,
			m_pIndices,
			particleCount );

		if( blendFunc.src != oldBlendFunc.src ||
			blendFunc.dst != oldBlendFunc.dst )
		{
			pRender->SetBlendFunc( oldBlendFunc );
		}
	}
	//------------------------------------------------------------------------------
}
