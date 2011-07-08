/** 
* @file guiwgtscrollbar.h
* @brief scrollbar
* @author ken
* @date 2006-08-07
*/

#ifndef __GUI_WGTSCROLLBAR_20060807_H__
#define __GUI_WGTSCROLLBAR_20060807_H__

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
	* @class CGUIWgtScrollbar
	* @brief scrollbar,will send scroll message to parent
	* used image name:
	* background
	* arrow_0_normal
	* arrow_0_hover
	* arrow_0_disable
	* arrow_0_push
	* arrow_0_hover_overlay
	* arrow_0_push_overlay
	* arrow_1_normal
	* arrow_1_hover
	* arrow_1_disable
	* arrow_1_push
	* arrow_1_hover_overlay
	* arrow_1_push_overlay
	* slide_normal
	* slide_hover
	* slide_disable
	* slide_push
	* slide_hover_overlay
	* slide_push_overlay
	*/ 
	class GUIEXPORT CGUIWgtScrollbar : public CGUIWidget
	{
	public:
		CGUIWgtScrollbar( const CGUIString& rName, const CGUIString& rSceneName );
		virtual ~CGUIWgtScrollbar();

		void SetPageSize(uint32 nPageSize);
		uint32 GetPageSize() const;

		void SetRange( uint32 nRange );
		uint32 GetRange(  ) const;

		void SetMinPos(uint32 nMinPos);      
		uint32 GetMinPos() const;                        

		void SetMaxPos(uint32 nMaxPos);     
		uint32 GetMaxPos() const;                        

		void SetOrientation( EOrientation eScrollbarType );
		EOrientation GetOrientation( ) const;

		void SetCurrentPos(uint32 nPos);
		uint32 GetCurrentPos() const;                                 

		void IncreasePos();
		void DecreasePos();

		void SetNotifyParent(bool bEnable);
		bool IsNotifyParent() const;

	protected:
		CGUIWgtScrollbar( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitScrollbar();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void RefreshSelf();
		virtual void OnCreate();
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

		void UpdateScrollbar();

		void GenerateNotifyEvent();

		/// get rect of area where slide could stay.it's a local rect
		CGUIRect GetSlideArea( );

	protected:
		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

	protected:	//!< callback function
		virtual uint32 OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32 OnScrollbarScroll( CGUIEventScrollbar* pEvent );

	protected:
		uint32 m_nMaxPos; ///< maximum value of the scrollbar
		uint32 m_nMinPos; ///< minimum value of the scrollbar
		uint32 m_nCurPos; ///< current value of the scrollbar
		uint32 m_nPageSize; ///< page size value of the scrollbar

		EOrientation m_eOrientation; ///orientation of scrollbar
		bool m_bAutoNotifyParent;///whether notify parent when scrollbar value change

		//image
		CGUIImage* m_pImageBg; ///background

		//child
		friend class CGUIWgtScrollbarSlide;
		friend class CGUIWgtScrollbarArrow;
		CGUIWgtScrollbarArrow* m_pArrow[2]; ///arrow of scrollbar
		CGUIWgtScrollbarSlide* m_pSlide; ///slide of scrollbar

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtScrollbar);
	};

}//namespace libguiex



#endif //__GUI_WGTSCROLLBAR_20060807_H__
