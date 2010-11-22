/** 
* @file guiwidgetscrollpanel.h
* @brief widget: panel with scrollbar
* @author ken
* @date 2009-11-09
*/


#ifndef __GUI_WGTSCROLLPANEL_20091109_H__
#define __GUI_WGTSCROLLPANEL_20091109_H__

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
	* @class CGUIWgtScrollPanel
	* @brief scrollbar container class.
	* used image name.
	* usage:	m_aClientRect: the area where the client widget can render
	*			m_aClientClipRect: the area where the client can show
	*
	* - BGIMAGE
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
	class GUIEXPORT CGUIWgtScrollPanel : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtScrollPanel( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief destructor
		*/
		virtual ~CGUIWgtScrollPanel(  );

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
		void	ForceVertScrollbarShow(bool bShow);

		/**
		* @brief Return whether the horizontal scroll bar is always shown.
		*/
		bool	IsHorzScrollbarAlwaysShown(void) const;

		/**
		* @brief Set whether the horizontal scroll bar should always be shown.
		*/
		void	ForceHorzScrollbarShow(bool bShow);

		void	UseSelfCreatedScrollbar( bool bSelfCreated );
		bool	IsUseSelfCreatedScrollbar() const;

		static CGUIWgtScrollPanel*	FromWidget( CGUIWidget* pWidget );


		/**
		* @brief load widget config from property
		*/
		virtual CGUIProperty*	GenerateProperty(const CGUIString& rName, const CGUIString& rType );

		/**
		* @brief process property
		*/
		virtual void		ProcessProperty( const CGUIProperty* pProperty);

		virtual void		SetVerticalScrollbar( const CGUIString& rName);
		virtual void		SetHorizonalScrollbar( const CGUIString& rName);

		//get actual page size
		void				UpdateScrollbars(void);

	protected:
		///initialize
		void				InitScrollPanel();

		/// render
		virtual void		RenderSelf(IGUIInterfaceRender* pRender);

		virtual void		UpdateDirtyRect();

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:	//!< callback function
		virtual uint32		OnOpen( CGUIEventNotification* pEvent );
		virtual uint32		OnScaleChange( CGUIEventNotification* pEvent );
		virtual uint32		OnSizeChanged( CGUIEventSize* pEvent );

		virtual uint32		OnScrollbarScroll( CGUIEventScrollbar* pEvent );


	protected:
		//scrollbar relative
		CGUIString				m_strVertScrollbarName;
		CGUIString				m_strHorzScrollbarName;
		CGUIWgtScrollbar*		m_pScrollbarVert;		//!< vertical scroll-bar widget
		CGUIWgtScrollbar*		m_pScrollbarHorz;		//!< horizontal scroll-bar widget
		bool					m_bForceVertScrollShow;		//!< true if vertical scrollbar should always be displayed
		bool					m_bForceHorzScrollShow;		//!< true if horizontal scrollbar should always be displayed

		bool					m_UseSelfCreatedScrollbar;

		CGUIImage*				m_pPanelBG;
		CGUIRect				m_aScrollClientRect;				//client rect, in global dimension


	private:
		static CGUIString	ms_strType;
	};



	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtScrollPanel);

}//namespace libguiex



#endif //__GUI_WGTSCROLLPANEL_20091109_H__
