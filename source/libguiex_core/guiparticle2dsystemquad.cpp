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
#include "guilogmsgmanager.h"

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
		m_pQuads = new SVertexFormat_V2F_T2F_C4UB_Quad[m_uTotalParticles];
		m_pIndices = new uint16[m_uTotalParticles * 6];

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
			GUI_THROW( " Particle system: not enough memory");
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuads[0])*m_uTotalParticles, m_pQuads,GL_DYNAMIC_DRAW);	
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
		for( uint32 i=0; i<m_uTotalParticles; i++) 
		{
			// bottom-left vertex:
			m_pQuads[i].vertices[eQuad_BottomLeft].texCoords.u = rUVRect.m_fLeft;
			m_pQuads[i].vertices[eQuad_BottomLeft].texCoords.v = rUVRect.m_fBottom;
			// bottom-right vertex:
			m_pQuads[i].vertices[eQuad_BottomRight].texCoords.u = rUVRect.m_fRight;
			m_pQuads[i].vertices[eQuad_BottomRight].texCoords.v = rUVRect.m_fBottom;
			// top-left vertex:
			m_pQuads[i].vertices[eQuad_TopLeft].texCoords.u = rUVRect.m_fLeft;
			m_pQuads[i].vertices[eQuad_TopLeft].texCoords.v = rUVRect.m_fTop;
			// top-right vertex:
			m_pQuads[i].vertices[eQuad_TopRight].texCoords.u = rUVRect.m_fRight;
			m_pQuads[i].vertices[eQuad_TopRight].texCoords.v = rUVRect.m_fTop;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystemQuad::InitIndices() const
	{
		for( int16 i=0;i<int16(m_uTotalParticles);i++) 
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
		uint32 particleIdx = particle - m_pParticles;
		GUI_ASSERT( particleIdx < m_uParticleCount, "invalid particle" );

		// colors
		SVertexFormat_V2F_T2F_C4UB_Quad *quad = &(m_pQuads[particleIdx]);
		for( uint32 i=0; i<4; ++i )
		{
			quad->vertices[i].colors.abgr = particle->color.GetAsABGR();
		}

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
			quad->vertices[eQuad_BottomLeft].vertices.x = ax;
			quad->vertices[eQuad_BottomLeft].vertices.y = ay;

			// bottom-right vertex:
			quad->vertices[eQuad_BottomRight].vertices.x = bx;
			quad->vertices[eQuad_BottomRight].vertices.y = by;

			// top-left vertex:
			quad->vertices[eQuad_TopLeft].vertices.x = dx;
			quad->vertices[eQuad_TopLeft].vertices.y = dy;

			// top-right vertex:
			quad->vertices[eQuad_TopRight].vertices.x = cx;
			quad->vertices[eQuad_TopRight].vertices.y = cy;
		} 
		else
		{
			// bottom-left vertex:
			quad->vertices[eQuad_BottomLeft].vertices.x = rNewPos.x - size_2;
			quad->vertices[eQuad_BottomLeft].vertices.y = rNewPos.y + size_2;

			// bottom-right vertex:
			quad->vertices[eQuad_BottomRight].vertices.x = rNewPos.x + size_2;
			quad->vertices[eQuad_BottomRight].vertices.y = rNewPos.y + size_2;

			// top-left vertex:
			quad->vertices[eQuad_TopLeft].vertices.x = rNewPos.x - size_2;
			quad->vertices[eQuad_TopLeft].vertices.y = rNewPos.y - size_2;

			// top-right vertex:
			quad->vertices[eQuad_TopRight].vertices.x = rNewPos.x + size_2;
			quad->vertices[eQuad_TopRight].vertices.y = rNewPos.y - size_2;				
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
			m_pTexture, 
			m_pQuads,
			m_pIndices,
			m_uParticleCount );

		if( blendFunc.src != oldBlendFunc.src ||
			blendFunc.dst != oldBlendFunc.dst )
		{
			pRender->SetBlendFunc( oldBlendFunc );
		}
	}
	//------------------------------------------------------------------------------
}
