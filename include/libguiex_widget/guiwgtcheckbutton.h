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
	*		-BTN_NORMAL
	*		-BTN_NORMAL_CHECKED
	*		-BTN_HOVER
	*		-BTN_HOVER_CHECKED
	*		-BTN_DISABLE
	*		-BTN_DISABLE_CHECKED
	*		-BTN_PUSH
	*		-BTN_PUSH_CHECKED
	*		-BTN_HOVER_OVERLAY
	*		-BTN_CHECKED_OVERLAY
	*/
	class GUIEXPORT CGUIWgtCheckButton : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtCheckButton( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief set widget to check state
		* @notice this function won't generate OnCheck event
		*/
		virtual void SetCheck(bool bChecked);

		/**
		* @brief get state of checkbutton
		*/
		bool IsCheck() const;

		/**
		* @brief load widget config from property
		*/
		virtual CGUIProperty* GenerateProperty(const CGUIString& rName, const CGUIString& rType );

		/**
		* @brief load widget config from property
		*/
		virtual void ProcessProperty( const CGUIProperty* pProperty);

		static CGUIWgtCheckButton* FromWidget( CGUIWidget* pWidget );

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtCheckButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		///initialize check button
		void InitCheckButton();

		/**
		* @brief override the OnSetImage function
		*/
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:	//!< callback function
		virtual uint32		OnMouseEnter( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeave( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftClick(CGUIEventMouse* pEvent);
		virtual uint32		OnChecked( CGUIEventNotification* pEvent );
		virtual uint32		OnUnchecked( CGUIEventNotification* pEvent );

	protected:
		bool	m_bHovering;	///whether in hovering state
		bool	m_bPushing;		///whether button is pushed
		bool	m_bChecked;		///whether button is checked

		const CGUIImage* m_pImageNormal;
		const CGUIImage* m_pImageHovering;
		const CGUIImage* m_pImageDisable;
		const CGUIImage *m_pImagePush;
		const CGUIImage* m_pImageNormalChecked;
		const CGUIImage* m_pImageHoveringChecked;
		const CGUIImage* m_pImageDisableChecked;
		const CGUIImage* m_pImagePushChecked;
		const CGUIImage* m_pImageHoverOverlay;
		const CGUIImage* m_pImageCheckedOverlay;

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtCheckButton);
	};




}//namespace libguiex



#endif //__GUI_WGTCHECKBUTTON_20060803_H__

