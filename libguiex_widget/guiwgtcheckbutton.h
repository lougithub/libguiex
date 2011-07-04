/** 
* @file guiwgtcheckbutton.h
* @brief checkbutton used in the system
* @author ken
* @date 2006-08-03
*/



#ifndef __GUI_WGTCHECKBUTTON_20060803_H__
#define __GUI_WGTCHECKBUTTON_20060803_H__

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
	* @class CGUIWgtCheckButton
	* @brief button class.
	* used image name:
	*		-normal
	*		-normal_checked
	*		-hover
	*		-hover_checked
	*		-disable
	*		-disable_checked
	*		-push
	*		-push_checked
	*		-hover_overlay
	*		-checked_overlay
	*/
	class GUIEXPORT CGUIWgtCheckButton : public CGUIWidget
	{
	public:
		CGUIWgtCheckButton( const CGUIString& rName, const CGUIString& rSceneName );

		virtual void SetChecked(bool bChecked);
		bool IsChecked() const;

	protected:
		CGUIWgtCheckButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		
		void InitCheckButton();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

	protected:	//!< callback function
		virtual uint32 OnMouseEnter( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeave( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftClick(CGUIEventMouse* pEvent);
		virtual uint32 OnChecked( CGUIEventNotification* pEvent );
		virtual uint32 OnUnchecked( CGUIEventNotification* pEvent );

	protected:
		bool m_bHovering; ///whether in hovering state
		bool m_bPushing; ///whether button is pushed
		bool m_bChecked; ///whether button is checked

		CGUIImage* m_pImageNormal;
		CGUIImage* m_pImageHovering;
		CGUIImage* m_pImageDisable;
		CGUIImage *m_pImagePush;
		CGUIImage* m_pImageNormalChecked;
		CGUIImage* m_pImageHoveringChecked;
		CGUIImage* m_pImageDisableChecked;
		CGUIImage* m_pImagePushChecked;
		CGUIImage* m_pImageHoverOverlay;
		CGUIImage* m_pImageCheckedOverlay;

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtCheckButton);
	};

}//namespace libguiex


#endif //__GUI_WGTCHECKBUTTON_20060803_H__

