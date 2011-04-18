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
		CGUIWgtAnimation( const CGUIString& rName, const CGUIString& rSceneName );
		virtual ~CGUIWgtAnimation();

		void SetCurrentAnimation( const CGUIString& rAnimationName);
		const CGUIString& GetCurrentAnimationName(  ) const;
		CGUIAnimation* GetCurrentAnimation(  ) const;

	protected:
		CGUIWgtAnimation( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void UpdateSelf( real fDeltaTime );

		virtual void OnSetAnimation( const CGUIString& rName, CGUIAnimation* pAnimation ); 

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
