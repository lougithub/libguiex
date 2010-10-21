/** 
* @file guiwgtbutton.h
* @brief widget: button
* @author ken
* @date 2006-06-08
*/


#ifndef __GUI_WGTBUTTON_20060608_H__
#define __GUI_WGTBUTTON_20060608_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiwidget.h>
#include <libguiex_core\guiwidgetgenerator.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtButton
	* @brief button class.
	* used image name:
	*		-BTN_NORMAL
	*		-BTN_HOVER
	*		-BTN_DISABLE
	*		-BTN_PUSH
	*		-BTN_HOVER_OVERLAY
	*		-BTN_PUSH_OVERLAY
	*/
	class GUIEXPORT CGUIWgtButton : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtButton( const CGUIString& rName, const CGUIString& rProjectName );

		/**
		* @brief create this widget
		*/
		virtual int32 Create();

		/**
		* @brief create this widget
		* @param pType should be one of following value
		*		-NORMAL
		*		-HOVER
		*		-DISABLE
		*		-PUSH
		*/
		void	SetBtnTextContent(const wchar_t* pText, const char* pType);

		/**
		* @brief create this widget
		*		-NORMAL
		*		-HOVER
		*		-DISABLE
		*		-PUSH
		*/
		void	SetBtnTextInfo(const CGUIStringExInfo& rInfo, const char* pType);

		virtual void	SetTextContent(const wchar_t* pText);

		void	SetRelativePos( const CGUIVector2& rPos);

		const CGUIVector2&	GetRelativePos(  ) const;

		/**
		* @brief load widget config from property
		*/
		virtual CGUIProperty*	GenerateProperty(const CGUIString& rName, const CGUIString& rType );

		/** 
		* @brief process property
		*/
		virtual void		ProcessProperty( const CGUIProperty* pProperty);

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		///initialize
		void InitButton();

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName,CGUIImage* pImage );

	protected:	//!< callback function
		virtual uint32		OnMouseEnter( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeave( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftUp( CGUIEventMouse* pEvent );


	protected:
		bool	m_bHovering;
		bool	m_bPushing;

		CGUIImage*	m_pImageNormal;
		CGUIImage*	m_pImageHovering;
		CGUIImage*	m_pImageDisable;
		CGUIImage*	m_pImagePush;
		CGUIImage*	m_pImageHoverOverlay;
		CGUIImage*	m_pImagePushOverlay;

		CGUIStringEx		m_strTextHoving;	//for hover state
		CGUIStringEx		m_strTextDisable;	//for disable state
		CGUIStringEx		m_strTextPush;		//for push state

		CGUIVector2			m_aRelativePos;		//relative pos for button text

	private:
		static CGUIString	ms_strType;
	};



	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtButton);

}//namespace libguiex



#endif //__GUI_WGTBUTTON_20060608_H__
