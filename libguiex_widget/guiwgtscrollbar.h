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
// enum
//============================================================================// 
namespace guiex
{
	///type for scrollbar
	enum EScrollbarType
	{
		eSB_VERTICAL = 0,
		eSB_HORIZONAL
	};
	
	class CGUIWgtScrollbarArrow;
	class CGUIWgtScrollbarSlide;
}//namespace guiex


//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class CGUIWgtScrollbar
	* @brief scrollbar,will send scroll message to parent
	* used image name:
	* SCROLLBAR_BG
	* SCROLLBAR_ARROW_NORMAL_0
	* SCROLLBAR_ARROW_HOVER_0
	* SCROLLBAR_ARROW_DISABLE_0
	* SCROLLBAR_ARROW_PUSH_0
	* SCROLLBAR_ARROW_HOVER_OVERLAY_0
	* SCROLLBAR_ARROW_PUSH_OVERLAY_0
	* SCROLLBAR_ARROW_NORMAL_1
	* SCROLLBAR_ARROW_HOVER_1
	* SCROLLBAR_ARROW_DISABLE_1
	* SCROLLBAR_ARROW_PUSH_1
	* SCROLLBAR_ARROW_HOVER_OVERLAY_1
	* SCROLLBAR_ARROW_PUSH_OVERLAY_1
	* SCROLLBAR_SLIDE_NORMAL
	* SCROLLBAR_SLIDE_HOVER
	* SCROLLBAR_SLIDE_DISABLE
	* SCROLLBAR_SLIDE_PUSH
	* SCROLLBAR_SLIDE_HOVER_OVERLAY
	* SCROLLBAR_SLIDE_PUSH_OVERLAY
	*/ 
	class GUIEXPORT CGUIWgtScrollbar : public CGUIWidget
	{
	public:
		CGUIWgtScrollbar( const CGUIString& rName, const CGUIString& rSceneName );
		virtual ~CGUIWgtScrollbar();

		static CGUIWgtScrollbar* FromWidget( CGUIWidget* pWidget );

		///////////////////////////////////////////////////////////
		// value related function

		///set page size
		void SetPageSize(uint32 nPageSize);

		///get page size
		uint32 GetPageSize() const;

		/**
		* @brief set range of scrollbar
		*/
		void SetRange( uint32 nRange );

		/**
		* @brief get range of scrollbar
		*/
		uint32 GetRange(  ) const;

		///get minimum position of scrollbar slide
		uint32 GetMinPos() const;                        

		///set minimum position of scrollbar slide
		void SetMinPos(uint32 nMinPos);      

		///get maximum position of scrollbar slide
		uint32 GetMaxPos() const;                        

		///set maximum position of scrollbar slide
		void SetMaxPos(uint32 nMaxPos);     

		void SetScrollbarType( EScrollbarType eScrollbarType );
		EScrollbarType GetScrollbarType( ) const;

		///get current position of scrollbar slide
		uint32 GetCurrentPos() const;                                 

		///set current position of scrollbar slide
		void SetCurrentPos(uint32 nPos);

		///increase position of scrollbar slide
		void IncreasePos();

		///increase position of scrollbar slide
		void DecreasePos();

		///make this scrollbar could be positioned by parent automatically
		void EnableAutoPosition(bool	bAutoPos);

		///whether this scrollbar could be position automatically or not
		bool IsAutoPosition() const;

		///whether notify parent when scroll value change
		void EnableNotifyParent(bool bEnable);

		///whether notify parent when scroll value change
		bool IsNotifyParent() const;

		void SetScrollbarHost( CGUIWidget* pWgtHost );

	protected:
		CGUIWgtScrollbar( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitScrollbar();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void RefreshSelf();
		virtual void OnCreate();
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

		/// update scrollbar's position and size when parent changes size or others
		void UpdateScrollbar();

		/// update scrollbar by value
		void UpdateValue();

		void GenerateNotifyEvent();

		/// get rect of area where slide could stay.it's a local rect
		CGUIRect GetSlideArea( );

		/// get bg size
		CGUISize GetBGSize();

	protected:	//!< callback function
		virtual uint32 OnOpen( CGUIEventNotification* pEvent );
		virtual uint32 OnParentSizeChange( CGUIEventSize* pEvent );
		virtual uint32 OnClose( CGUIEventNotification* pEvent );
		virtual uint32 OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32 OnScrollbarScroll( CGUIEventScrollbar* pEvent );

	protected:
		//scrollbar parameter
		//uint32 m_nMinSlideLength;	///< the minimum length of slide
		uint32 m_nMaxPos; ///< maximum value of the scrollbar
		uint32 m_nMinPos; ///< minimum value of the scrollbar
		uint32 m_nCurPos; ///< current value of the scrollbar
		uint32 m_nPageSize; ///< page size value of the scrollbar

		EScrollbarType m_eScrollbarType; ///type of scrollbar
		bool m_bAutoPosition;	///the parent could position this scrollbar automatically
		bool m_bAutoNotifyParent;///whether notify parent when scrollbar value change

		//image
		CGUIImage* m_pImageBg; ///background

		//child
		friend class CGUIWgtScrollbarSlide;
		friend class CGUIWgtScrollbarArrow;
		CGUIWgtScrollbarArrow* m_pArrow[2]; ///arrow of scrollbar
		CGUIWgtScrollbarSlide* m_pSlide; ///slide of scrollbar

		CGUIWidget* m_pWgtHost;

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtScrollbar);
	};

}//namespace libguiex



#endif //__GUI_WGTSCROLLBAR_20060807_H__

