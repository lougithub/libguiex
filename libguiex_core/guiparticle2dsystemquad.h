/** 
* @file guiparticlesystemquad.h
* @brief 
* @author ken
* @date 2011-01-12
*/


#ifndef __GUI_PARTICLE2DSYSTEMQUAD_20110112_H__
#define __GUI_PARTICLE2DSYSTEMQUAD_20110112_H__

//============================================================================//
// include
//============================================================================// 
#include "guiparticle2dsystem.h"


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIRect;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class CGUIParticle2DSystemQuad : public CGUIParticle2DSystem
	{
	public:
		CGUIParticle2DSystemQuad( int numberOfParticles );
		virtual ~CGUIParticle2DSystemQuad();

		virtual void Render( IGUIInterfaceRender* pRender, const CGUIMatrix4& rWorldMatrix );

	protected:
		void InitTexCoordsWithUVRect( const CGUIRect& rUVRect );
		void InitIndices();

		virtual void UpdateQuadWithParticle( CGUIParticle2D* particle, const CGUIVector2& rNewPos );
		virtual void PostStep();

	protected:
		SR_V2F_C4F_T2F_Quad *quads; // quads to be rendered
		uint16 *indices; // indices
#if CC_USES_VBO
		GLuint quadsID;		// VBO id
#endif
	};
}


#endif //__GUI_PARTICLE2DSYSTEMQUAD_20110112_H__