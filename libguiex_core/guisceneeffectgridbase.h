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
#include "guiscenecapture.h"
#include "guivector2.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIInterfaceRender;
	struct SR_T2F;
	struct SR_V3F;
	struct SR_V3F;
}

//============================================================================//
// class
//============================================================================//
namespace guiex
{
	class GUIEXPORT CGUISceneEffectGridBase : public CGUISceneCapture
	{
	public:
		CGUISceneEffectGridBase( const CGUISize& rSceneSize, const CGUIIntSize& rGridSize );

		virtual int32 Initialize( );
		virtual void Release( );

		void SetTextureFlipped( bool bFlipped );
		bool IsTextureFlipped() const;

	protected:
		CGUIIntSize m_aGridSize;
		CGUIVector2 m_aStep;
		bool m_bIsTextureFlipped;
	};
}

#endif //__KEN_GUISCENEEFFECTGRIDBASE_20110316_H__
