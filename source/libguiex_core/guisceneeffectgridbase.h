/** 
 * @file guisceneeffectgridbase.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-16
 */


#ifndef __KEN_GUISCENEEFFECTGRIDBASE_20110316_H__
#define	__KEN_GUISCENEEFFECTGRIDBASE_20110316_H__


//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "guisceneeffect.h"
#include "guivector2.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIInterfaceRender;
	struct SVertexFormat_T2F;
	struct SVertexFormat_V3F;
	struct SVertexFormat_V3F;
}

//============================================================================//
// class
//============================================================================//
namespace guiex
{
	class GUIEXPORT CGUISceneEffectGridBase : public CGUISceneEffect
	{
	public:
		CGUISceneEffectGridBase( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize );

		virtual int32 Initialize( );
		virtual void Release( );

		virtual void Reset( ) = 0;

		void SetTextureFlipped( bool bFlipped );
		bool IsTextureFlipped() const;

		const CGUIVector2& GetStep() const;

	protected:
		CGUIIntSize m_aGridSize;
		CGUIVector2 m_aStep;
		bool m_bIsTextureFlipped;
	};
}

#endif //__KEN_GUISCENEEFFECTGRIDBASE_20110316_H__
