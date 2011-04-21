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
		enum EButtonState
		{
			eButtonState_Normal = 0,
			eButtonState_Hover,
			eButtonState_Push,
			eButtonState_Disable,

			__eButtonState_NUM__,
		};
		CGUIWgtButton( const CGUIString& rName, const CGUIString& rSceneName );

		void SetTextContent( const CGUIStringW& rText, EButtonState eButtonState );
		const CGUIStringW& GetTextContent( EButtonState eButtonState ) const;

		void SetTextInfo( const CGUIStringRenderInfo& rInfo, EButtonState eButtonState );
		const CGUIStringRenderInfo& GetTextInfo( EButtonState eButtonState ) const;

		void SetTextContentUTF8( const CGUIString& rString, EButtonState eButtonState);
		CGUIString GetTextContentUTF8( EButtonState eButtonState ) const;

		const CGUIStringRender&	GetText( EButtonState eButtonState ) const;

		bool IsTextContentEmpty( EButtonState eButtonState ) const;

		void SetStringOffset( const CGUIVector2& rPos);
		const CGUIVector2& GetStringOffset(  ) const;

		void SetTextColor(const CGUIColor& rColor, EButtonState eButtonState);

		void SetTextAlignmentVert( ETextAlignmentVert eAlignment );
		void SetTextAlignmentHorz( ETextAlignmentHorz eAlignment );
		ETextAlignmentHorz GetTextAlignmentHorz( ) const;
		ETextAlignmentVert GetTextAlignmentVert( ) const;

	protected:
		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

	protected:
		CGUIWgtButton( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitButton();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void RefreshSelf();
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:	//!< callback function
		virtual uint32 OnMouseEnter( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeave( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftUp( CGUIEventMouse* pEvent );

	protected:
		bool m_bHovering;
		bool m_bPushing;

		CGUIImage* m_pImage[__eButtonState_NUM__];
		CGUIImage* m_pImageHoverOverlay;

		CGUIStringRender m_strText[__eButtonState_NUM__];

		ETextAlignmentHorz m_eTextAlignmentHorz;
		ETextAlignmentVert m_eTextAlignmentVert;

		CGUIVector2 m_aTextOffset; //relative pos for button text
		CGUIRect m_aStringArea;

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtButton);
	};

}//namespace libguiex



#endif //__GUI_WGTBUTTON_20060608_H__
