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
	*
	* scrollbar_v_background
	* scrollbar_v_arrow_0_normal
	* scrollbar_v_arrow_0_hover
	* scrollbar_v_arrow_0_disable
	* scrollbar_v_arrow_0_push
	* scrollbar_v_arrow_0_hover_overlay
	* scrollbar_v_arrow_0_push_overlay
	* scrollbar_v_arrow_1_normal
	* scrollbar_v_arrow_1_hover
	* scrollbar_v_arrow_1_disable
	* scrollbar_v_arrow_1_push
	* scrollbar_v_arrow_1_hover_overlay
	* scrollbar_v_arrow_1_push_overlay
	* scrollbar_v_slide_normal
	* scrollbar_v_slide_hover
	* scrollbar_v_slide_disable
	* scrollbar_v_slide_push
	* scrollbar_v_slide_hover_overlay
	* scrollbar_v_slide_push_overlay
	*
	* scrollbar_h_background
	* scrollbar_h_arrow_0_normal
	* scrollbar_h_arrow_0_hover
	* scrollbar_h_arrow_0_disable
	* scrollbar_h_arrow_0_push
	* scrollbar_h_arrow_0_hover_overlay
	* scrollbar_h_arrow_0_push_overlay
	* scrollbar_h_arrow_1_normal
	* scrollbar_h_arrow_1_hover
	* scrollbar_h_arrow_1_disable
	* scrollbar_h_arrow_1_push
	* scrollbar_h_arrow_1_hover_overlay
	* scrollbar_h_arrow_1_push_overlay
	* scrollbar_h_slide_normal
	* scrollbar_h_slide_hover
	* scrollbar_h_slide_disable
	* scrollbar_h_slide_push
	* scrollbar_h_slide_hover_overlay
	* scrollbar_h_slide_push_overlay
	*
	*/
	class GUIEXPORT CGUIWgtScrollbarContainer : public CGUIWidget
	{
	public:
		virtual ~CGUIWgtScrollbarContainer(  );

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty );

		virtual const CGUIRect*	GetClipArea() const;

	public:
		void ForceShowVertScrollbar( bool bForce );
		bool IsForceShowVertScrollbar() const;

		void ForceShowHorzScrollbar( bool bForce );
		bool IsForceShowHorzScrollbar() const;

		void ShowVertScrollbar(bool bShow);
		bool IsVertScrollbarShow(void) const;

		void ShowHorzScrollbar(bool bShow);
		bool IsHorzScrollbarShow(void) const;

		void SetPixelPerVertValue(uint32 nPixelPerValue);
		uint32 GetPixelPerVertValue( ) const;

		void SetPixelPerHorzValue(uint32 nPixelPerValue);
		uint32 GetPixelPerHorzValue( ) const;

		void SetVertRange( uint32 nRange );
		uint32 GetVertRange( ) const;

		void SetHorzRange( uint32 nRange );
		uint32 GetHorzRange( ) const;

		void SetVertPos( uint32 nPos );
		uint32 GetVertPos( ) const;

		void SetHorzPos( uint32 nPos );
		uint32 GetHorzPos( ) const;

		void SetVertPageSize( uint32 nPage );
		uint32 GetVertPageSize( ) const;

		void SetHorzPageSize( uint32 nPage );
		uint32 GetHorzPageSize( ) const;

		const CGUIRect& GetVisibleClientArea() const;

	protected:
		CGUIWgtScrollbarContainer( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitScrollbarContainer();

		virtual void RefreshSelf();
		virtual void OnCreate();
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

		virtual bool IsAddChildToTail() const;
		
		virtual CGUISize GetDesiredVirtualClientSize( ) = 0;

	protected:	//!< callback function
		virtual uint32 OnScrollbarScroll( CGUIEventScrollbar* pEvent );

	protected:
		CGUIWgtScrollbar* m_pScrollbarVert; //!< vertical scroll-bar widget
		CGUIWgtScrollbar* m_pScrollbarHorz; //!< horizontal scroll-bar widget

		uint32 m_nPixelPerVertValue;
		uint32 m_nPixelPerHorzValue;

		CGUIRect m_aVisibleClientArea;

		bool m_bForceShowVertScrollbar;
		bool m_bForceShowHorzScrollbar;

	private:
		//GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtScrollbarContainer);
	};

}//namespace libguiex



#endif //__GUI_WGTSCROLLBARCONTAINER_20070518_H__
