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
	class CGUIImage;
	class CGUIRect;
	class CGUIColor;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIAnimation : public CGUIResource
	{
	public:
		virtual ~CGUIAnimation( );

		void Update( real fDeltaTime );

		void Draw( IGUIInterfaceRender* pRender,
			const CGUIRect& rDestRect,
			real z, 
			real fAlpha	);

		void Draw( IGUIInterfaceRender* pRender,
			const CGUIRect& rDestRect,
			real z, 
			const CGUIColor& rColor,
			real fAlpha	);

		const CGUISize& GetSize( );

		bool IsLooping() const;
		bool IsPlaying() const;

		void Reset();

	protected:
		/**
		* @brief constructor
		* @param nInterval interval time between two frame.in millisecond
		* @param rListFileName list contains all image's name
		*/
		CGUIAnimation(
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const std::vector<CGUIString>& rImageNames,  
			real fInterval,
			bool bLooping,
			const CGUISize& rSize);

		virtual int32 DoLoad();
		virtual void DoUnload();

	protected:
		friend class CGUIAnimationManager;

		typedef std::vector<CGUIImage*> TImageArray;
		TImageArray	m_vecImages; //images

		real m_fInterval;
		real m_fDeltaTime; //!< used by animation
		uint32 m_nFrame; //!< current frame

		CGUISize m_aAnimationSize;

		bool m_bLooping;
	};

}//namespace guiex


#endif	//__GUI_ANIMATION_20060810_H__
