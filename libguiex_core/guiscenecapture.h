/** 
 * @file guiscenecapture.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-15
 */


#ifndef __KEN_GUISCENECAPTURE_20100315_H__
#define	__KEN_GUISCENECAPTURE_20100315_H__


//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "guireference.h"

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
	class GUIEXPORT CGUISceneCapture : public CGUIReference
	{
	public:
		CGUISceneCapture( );

		void Initialize( uint32 uTextureWidth, uint32 uTextureHeight );
		void Release( );

		virtual void RefRelease();

		virtual void BeforeRender( IGUIInterfaceRender* pRender );
		virtual void AfterRender( IGUIInterfaceRender* pRender );

	protected:
		virtual ~CGUISceneCapture();
		virtual void ProcessCaptureTexture( IGUIInterfaceRender* pRender ) = 0;

	protected:
		CGUITexture* m_pTexture; //!< texture
	};
}

#endif //__KEN_GUISCENECAPTURE_20100315_H__
