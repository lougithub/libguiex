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
	*		- animation 
	*		- or set animation by SetValue("Animation", name), but you should set size manually
	*/
	class GUIEXPORT CGUIWgtAnimation : public CGUIWidget
	{
	public:
		CGUIWgtAnimation( const CGUIString& rName, const CGUIString& rSceneName );
		virtual ~CGUIWgtAnimation();

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

		CGUIAnimation* GetCurrentAnimation(  ) const;
		void SetCurrentAnimation( const CGUIString& rName );

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

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtAnimation);
	};

}//namespace guiex

#endif //__GUI_WGTANIMATION_20060810_H__
