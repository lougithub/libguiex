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
		virtual ~CGUIParticle2DSystemQuad();

		virtual void Render( IGUIInterfaceRender* pRender );
		
	protected:
		friend class CGUIParticle2DManager;
		CGUIParticle2DSystemQuad( const CGUIString& rName, const CGUIString& rSceneName );

		virtual int32 DoLoad() const;
		virtual void DoUnload();

	protected:
		void InitTexCoordsWithUVRect( const CGUIRect& rUVRect ) const;
		void InitIndices() const;

		virtual void UpdateQuadWithParticle( CGUIParticle2D* particle, const CGUIVector2& rNewPos );
		virtual void PostStep();

	protected:
		mutable SR_V2F_C4F_T2F_Quad *quads; // quads to be rendered
		mutable uint16 *indices; // indices
#if CC_USES_VBO
		GLuint quadsID;		// VBO id
#endif
	};
}


#endif //__GUI_PARTICLE2DSYSTEMQUAD_20110112_H__