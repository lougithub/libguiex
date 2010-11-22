/** 
* @file guiwidgetscrollbarcontainer.h
* @brief widget: scrollbar container
* @author ken
* @date 2007-05-18
*/


#ifndef __GUI_WGTSCROLLBARCONTAINER_20070518_H__
#define __GUI_WGTSCROLLBARCONTAINER_20070518_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWgtScrollbar;
}//namespace guiex

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtScrollbarContainer
	* @brief scrollbar container class.
	* used image name.
	* usage:	GetClientArea(): the area where the client widget can render, maybe 
	*			bigger than container bounds.
	*			GetClipArea(): the area where the client can show, 
	*			clip)area = bound_area - scrollbar_area
	*
	* - SCROLLBAR_VERTIC_BG
	* - SCROLLBAR_VERTIC_ARROW_NORMAL_0
	* - SCROLLBAR_VERTIC_ARROW_HOVER_0
	* - SCROLLBAR_VERTIC_ARROW_DISABLE_0
	* - SCROLLBAR_VERTIC_ARROW_PUSH_0
	* - SCROLLBAR_VERTIC_ARROW_NORMAL_1
	* - SCROLLBAR_VERTIC_ARROW_HOVER_1
	* - SCROLLBAR_VERTIC_ARROW_DISABLE_1
	* - SCROLLBAR_VERTIC_ARROW_PUSH_1
	* - SCROLLBAR_VERTIC_SLIDE_NORMAL
	* - SCROLLBAR_VERTIC_SLIDE_HOVER
	* - SCROLLBAR_VERTIC_SLIDE_DISABLE
	* - SCROLLBAR_VERTIC_SLIDE_PUSH
	*
	* - SCROLLBAR_HORIZON_BG
	* - SCROLLBAR_HORIZON_ARROW_NORMAL_0
	* - SCROLLBAR_HORIZON_ARROW_HOVER_0
	* - SCROLLBAR_HORIZON_ARROW_DISABLE_0
	* - SCROLLBAR_HORIZON_ARROW_PUSH_0
	* - SCROLLBAR_HORIZON_ARROW_NORMAL_1
	* - SCROLLBAR_HORIZON_ARROW_HOVER_1
	* - SCROLLBAR_HORIZON_ARROW_DISABLE_1
	* - SCROLLBAR_HORIZON_ARROW_PUSH_1
	* - SCROLLBAR_HORIZON_SLIDE_NORMAL
	* - SCROLLBAR_HORIZON_SLIDE_HOVER
	* - SCROLLBAR_HORIZON_SLIDE_DISABLE
	* - SCROLLBAR_HORIZON_SLIDE_PUSH
	*
	*/
	class GUIEXPORT CGUIWgtScrollbarContainer : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtScrollbarContainer( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief destructor
		*/
		virtual ~CGUIWgtScrollbarContainer(  );

		/**
		* @brief create this widget
		*/
		virtual void Create();

	public:
		/** 
		* @brief Return whether the vertical scroll bar is always shown.
		*/
		bool	IsVertScrollbarAlwaysShown(void) const;

		/**
		* @brief Set whether the vertical scroll bar should always be shown.
		*/
		void	ForceVertScrollbar(bool bShow);

		/**
		* @brief Return whether the horizontal scroll bar is always shown.
		*/
		bool	IsHorzScrollbarAlwaysShown(void) const;

		/**
		* @brief Set whether the horizontal scroll bar should always be shown.
		*/
		void	ForceHorzScrollbar(bool bShow);

		void SetScrollbarAutoPosition( bool bFlag ); 
		bool IsScrollbarAutoPosition( ) const;


	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtScrollbarContainer( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		///initialize
		void			InitScrollbarContainer();

		/// render
		virtual void	RenderSelf(IGUIInterfaceRender* pRender);

		virtual void	RefreshSelf();

		//get actual page size
		void			UpdateScrollbars(void);
		
		virtual void	UpdateClientArea(void);

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:	//!< callback function
		virtual uint32		OnScrollbarScroll( CGUIEventScrollbar* pEvent );


	protected:
		//scrollbar relative
		CGUIWgtScrollbar*		m_pScrollbarVert;		//!< vertical scroll-bar widget
		CGUIWgtScrollbar*		m_pScrollbarHorz;		//!< horizontal scroll-bar widget
		bool					m_bForceVertScroll;		//!< true if vertical scrollbar should always be displayed
		bool					m_bForceHorzScroll;		//!< true if horizontal scrollbar should always be displayed

		bool m_bIsScrollbarAutoPosition;

	private:
		static CGUIString	ms_strType;
	};



	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtScrollbarContainer);

}//namespace libguiex



#endif //__GUI_WGTSCROLLBARCONTAINER_20070518_H__
