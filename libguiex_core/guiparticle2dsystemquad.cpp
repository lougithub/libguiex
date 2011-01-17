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
	CGUIParticle2DSystemQuad::CGUIParticle2DSystemQuad( int numberOfParticles )
		:CGUIParticle2DSystem( numberOfParticles )
	{
		// allocating data space
		quads = (SR_V2F_C4F_T2F_Quad*)calloc( sizeof(quads[0]) * totalParticles, 1 );
		indices = (uint16*)calloc( sizeof(indices[0]) * totalParticles * 6, 1 );

		if( !quads || !indices ) 
		{
			if( quads )
			{
				free( quads );
			}
			if(indices)
			{
				free(indices);
			}
			throw CGUIException(" Particle system: not enough memory");
			return;
		}

		// initialize only once the texCoords and the indices
		InitTexCoordsWithUVRect( CGUIRect(0.f,0.f,1.f,1.f) );
		InitIndices();

#if CC_USES_VBO
		// create the VBO buffer
		glGenBuffers(1, &quadsID);

		// initial binding
		glBindBuffer(GL_ARRAY_BUFFER, quadsID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quads[0])*totalParticles, quads,GL_DYNAMIC_DRAW);	
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	}
	//------------------------------------------------------------------------------
	CGUIParticle2DSystemQuad::~CGUIParticle2DSystemQuad()
	{
		free(quads);
		free(indices);
#if CC_USES_VBO
		glDeleteBuffers(1, &quadsID);
#endif
	}
	//------------------------------------------------------------------------------
	// rect is in Points coordinates.
	void CGUIParticle2DSystemQuad::InitTexCoordsWithUVRect( const CGUIRect& rUVRect )
	{
		for( uint32 i=0; i<totalParticles; i++) 
		{
			// bottom-left vertex:
			quads[i].bl.texCoords.u = rUVRect.m_fLeft;
			quads[i].bl.texCoords.v = rUVRect.m_fBottom;
			// bottom-right vertex:
			quads[i].br.texCoords.u = rUVRect.m_fRight;
			quads[i].br.texCoords.v = rUVRect.m_fBottom;
			// top-left vertex:
			quads[i].tl.texCoords.u = rUVRect.m_fLeft;
			quads[i].tl.texCoords.v = rUVRect.m_fTop;
			// top-right vertex:
			quads[i].tr.texCoords.u = rUVRect.m_fRight;
			quads[i].tr.texCoords.v = rUVRect.m_fTop;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystemQuad::InitIndices()
	{
		for( int16 i=0;i<int16(totalParticles);i++) 
		{
			const int16 i6 = i*6;
			const int16 i4 = i*4;
			indices[i6+0] = (int16) i4+0;
			indices[i6+1] = (int16) i4+1;
			indices[i6+2] = (int16) i4+2;

			indices[i6+5] = (int16) i4+1;
			indices[i6+4] = (int16) i4+2;
			indices[i6+3] = (int16) i4+3;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystemQuad::UpdateQuadWithParticle( CGUIParticle2D* particle, const CGUIVector2& rNewPos )
	{
		// colors
		SR_V2F_C4F_T2F_Quad *quad = &(quads[particleIdx]);
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
			quad->bl.vertices.y = rNewPos.y - size_2;

			// bottom-right vertex:
			quad->br.vertices.x = rNewPos.x + size_2;
			quad->br.vertices.y = rNewPos.y - size_2;

			// top-left vertex:
			quad->tl.vertices.x = rNewPos.x - size_2;
			quad->tl.vertices.y = rNewPos.y + size_2;

			// top-right vertex:
			quad->tr.vertices.x = rNewPos.x + size_2;
			quad->tr.vertices.y = rNewPos.y + size_2;				
		}
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystemQuad::PostStep()
	{
#if CC_USES_VBO
		glBindBuffer(GL_ARRAY_BUFFER, quadsID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quads[0])*particleCount, quads);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	}
	//------------------------------------------------------------------------------
	void CGUIParticle2DSystemQuad::Render( IGUIInterfaceRender* pRender, const CGUIMatrix4& rWorldMatrix )
	{
		pRender->DrawQuads(
			rWorldMatrix, 
			texture->GetTextureImplement(), 
			blendFunc,
			quads,
			indices,
			particleCount
			);
	}
	//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

	/*
	// overriding draw method
	-(void) draw
	{	
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Unneeded states: -

	glBindTexture(GL_TEXTURE_2D, [texture_ name]);

	#define kQuadSize sizeof(quads[0].bl)

	#if CC_USES_VBO
	glBindBuffer(GL_ARRAY_BUFFER, quadsID);

	glVertexPointer(2,GL_FLOAT, kQuadSize, 0);

	glColorPointer(4, GL_FLOAT, kQuadSize, (GLvoid*) offsetof(ccV2F_C4F_T2F,colors) );

	glTexCoordPointer(2, GL_FLOAT, kQuadSize, (GLvoid*) offsetof(ccV2F_C4F_T2F,texCoords) );
	#else // vertex array list

	NSUInteger offset = (NSUInteger) quads;

	// vertex
	NSUInteger diff = offsetof( ccV2F_C4F_T2F, vertices);
	glVertexPointer(2,GL_FLOAT, kQuadSize, (GLvoid*) (offset+diff) );

	// color
	diff = offsetof( ccV2F_C4F_T2F, colors);
	glColorPointer(4, GL_FLOAT, kQuadSize, (GLvoid*)(offset + diff));

	// tex coords
	diff = offsetof( ccV2F_C4F_T2F, texCoords);
	glTexCoordPointer(2, GL_FLOAT, kQuadSize, (GLvoid*)(offset + diff));		

	#endif // ! CC_USES_VBO

	BOOL newBlend = blendFunc_.src != CC_BLEND_SRC || blendFunc_.dst != CC_BLEND_DST;
	if( newBlend )
	glBlendFunc( blendFunc_.src, blendFunc_.dst );

	NSAssert( particleIdx == particleCount, @"Abnormal error in particle quad");
	glDrawElements(GL_TRIANGLES, particleIdx*6, GL_UNSIGNED_SHORT, indices);

	// restore blend state
	if( newBlend )
	glBlendFunc( CC_BLEND_SRC, CC_BLEND_DST );

	#if CC_USES_VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	#endif

	// restore GL default state
	// -
	}
	*/
}
