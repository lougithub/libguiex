/** 
* @file guiwgtanimation.h
* @brief base class, for static widget
* @author ken
* @date 2006-08-10
*/


#ifndef __GUI_WGTANIMATION_20060810_H__
#define __GUI_WGTANIMATION_20060810_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class CGUIWgtAnimation
	* @brief used to play animation
	* used animation name:
	*		-ANIMATION_DEFAULT 
	*		- or set animation by SetValue("Animation", name), but you should set size manually
	*/
	class GUIEXPORT CGUIWgtAnimation : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtAnimation( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief destructor
		*/
		virtual ~CGUIWgtAnimation();

		/**
		* @brief add animation
		*/
		virtual void SetAnimation(const CGUIString& rAnimationName, CGUIAnimation* pAnimation);

		/**
		* @brief select a animation to play
		*/
		void SetCurrentAnimation( const CGUIString& rAnimationName);

		/**
		* @brief get animation name which is playing now
		*/
		const CGUIString& GetCurrentAnimation(  ) const;

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtAnimation( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		virtual void UpdateSelf( real fDeltaTime );

	protected:
		///initialize animation
		void InitAnimation();

	protected:
		CGUIAnimation* m_pAnimationCur;
		CGUIString m_pAnimationName;

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtAnimation);
	};

}//namespace guiex

#endif //__GUI_WGTANIMATION_20060810_H__
