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
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>

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
		CGUIWgtButton( const CGUIString& rName, const CGUIString& rSceneName );

		void SetBtnTextContent_Hover( const CGUIStringW& rText );
		void SetBtnTextInfo_Hover(const CGUIStringInfo& rInfo );
		void SetBtnTextContent_Disable( const CGUIStringW& rText );
		void SetBtnTextInfo_Disable(const CGUIStringInfo& rInfo );
		void SetBtnTextContent_Push( const CGUIStringW& rText );
		void SetBtnTextInfo_Push(const CGUIStringInfo& rInfo );

		virtual void SetTextContent(const CGUIStringW& rText);

		void SetStringOffset( const CGUIVector2& rPos);

		const CGUIVector2& GetStringOffset(  ) const;

		/**
		* @brief load widget config from property
		*/
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

		/** 
		 * @brief process property
		 */
		virtual void ProcessProperty( const CGUIProperty& rProperty);


	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		///initialize
		void InitButton();

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		virtual void RefreshSelf();

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:	//!< callback function
		virtual uint32		OnMouseEnter( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeave( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftUp( CGUIEventMouse* pEvent );


	protected:
		bool	m_bHovering;
		bool	m_bPushing;

		const CGUIImage* m_pImageNormal;
		const CGUIImage* m_pImageHovering;
		const CGUIImage* m_pImageDisable;
		const CGUIImage* m_pImagePush;
		const CGUIImage* m_pImageHoverOverlay;
		const CGUIImage* m_pImagePushOverlay;

		CGUIStringEx		m_strTextHoving;	//for hover state
		CGUIStringEx		m_strTextDisable;	//for disable state
		CGUIStringEx		m_strTextPush;		//for push state

		CGUIVector2			m_aTextOffset;		//relative pos for button text
		CGUIRect			m_aStringArea;

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtButton);
	};

}//namespace libguiex



#endif //__GUI_WGTBUTTON_20060608_H__
