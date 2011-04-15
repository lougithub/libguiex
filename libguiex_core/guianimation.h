/** 
* @file guianimation.h
* @brief animation
* @author ken
* @date 2006-08-10
*/

#ifndef __GUI_ANIMATION_20060810_H__
#define __GUI_ANIMATION_20060810_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiresource.h"
#include "guitimer.h"
#include "guisize.h"
#include <vector>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIInterfaceRender;
	class CGUIRect;
	class CGUITexture;
	class CGUIRenderRect;
	class CGUIMatrix4;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIAnimation : public CGUIResource
	{
	public:
		/**
		* @brief destructor
		*/
		virtual ~CGUIAnimation( );

		/**
		* @brief update the animation
		*/
		void Update( real fDeltaTime );

		void Draw( IGUIInterfaceRender* pRender,
			const CGUIRect& rDestRect,
			real z, 
			real fAlpha	);

		const CGUISize& GetSize( );

	protected:
		/**
		* @brief constructor
		* @param nInterval interval time between two frame.in millisecond
		* @param rFileName image name
		* @param rListTexRect show which part of image will be considered as a frame of animation
		*/
		CGUIAnimation( 
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const CGUIString& rFileName, 
			const std::vector<CGUIRect>& rUVRects,
			real fInterval,
			const CGUISize& rSize);

		/**
		* @brief constructor
		* @param nInterval interval time between two frame.in millisecond
		* @param rListFileName list contains all image's name
		*/
		CGUIAnimation(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const std::vector<CGUIString>& rFileNames,  
			real fInterval,
			const CGUISize& rSize);

		/**
		* @brief constructor
		* @param nInterval interval time between two frame.in millisecond
		* @param rListFileName list contains all image's name
		*/
		CGUIAnimation(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const std::vector<CGUIString>& rFileNames,  
			const std::vector<CGUIRect>& rUVRects,
			real fInterval,
			const CGUISize& rSize);

		virtual int32 DoLoad();
		virtual void DoUnload();

	protected:
		friend class CGUIAnimationManager;
		std::vector<CGUITexture*> m_vecTextures;
		std::vector<CGUIRect> m_vecUVRects;

		std::vector<CGUIString>	m_vecFileNames; //resource path

		real		m_fInterval;
		real		m_fDeltaTime;			//!< used by animation
		uint32		m_nFrame;				//!< current frame

		enum EUVAnimType
		{
			eUVAnimType_SingleFile = 0,
			eUVAnimType_MultiFile,
		};
		EUVAnimType eUVAnimType;

		CGUISize m_aAnimationSize;
	};

}//namespace guiex


#endif	//__GUI_ANIMATION_20060810_H__
