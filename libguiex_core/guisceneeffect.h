/** 
 * @file guisceneeffect.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-15
 */


#ifndef __KEN_GUISCENECAPTURE_20110315_H__
#define	__KEN_GUISCENECAPTURE_20110315_H__


//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "guireference.h"
#include "guiintsize.h"
#include "guisize.h"
#include "guirendertype.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIInterfaceRender;
	class CGUITexture;
}

//============================================================================//
// class
//============================================================================//
namespace guiex
{
	class GUIEXPORT CGUISceneEffect : public CGUIReference
	{
	public:
		CGUISceneEffect( const CGUISize& rSceneSize );

		virtual int32 Initialize(  );
		virtual void Release( );

		virtual void RefRelease();

		virtual void BeforeRender( IGUIInterfaceRender* pRender );
		virtual void AfterRender( IGUIInterfaceRender* pRender );

	protected:
		virtual ~CGUISceneEffect();
		virtual void ProcessCaptureTexture( IGUIInterfaceRender* pRender ) = 0;

	protected:
		CGUITexture* m_pTexture; //!< texture

		CGUISize m_aSceneSize;
	
	private:
		uint32 m_fbo;
		int32 m_oldfbo;

		SGUIBlendFunc m_aBlendFunc;
	};
}

#endif //__KEN_GUISCENECAPTURE_20110315_H__
