/** 
* @file guiwgtpanel.h
* @brief panel, can be used with 
* CGUIWgtFrame as a client area
* @author ken
* @date 2006-08-02
*/


#ifndef __GUI_WGTPANEL_20060802_H__
#define __GUI_WGTPANEL_20060802_H__

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
	* @class CGUIWgtPanel
	* @brief the panel's size will be resize automatically
	* by parent's client size
	* used image name:
	*	- bg
	*	- border_left
	*	- border_top
	*	- border_bottom
	*	- border_right
	*	- border_topleft
	*	- border_topright
	*	- border_bottomleft
	*	- border_bottomright
	*/
	class GUIEXPORT CGUIWgtPanel : public CGUIWidget
	{
	public:
		CGUIWgtPanel( const CGUIString& rName, const CGUIString& rSceneName );

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

	protected:
		CGUIWgtPanel( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitPanel();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void RefreshSelf();

		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

		const CGUIRect&	GetBorderRect(int32 eBorder);

		enum EPanelBorderLocation
		{
			border_topleft = 0,			
			border_topright,		
			border_bottomleft,		
			border_bottomright,		
			border_top,				
			border_left,			
			border_bottom,			
			border_right,
			PANEL_BORDER_NONE,			
		};

		//!< get border width
		real GetBorderWidth(EPanelBorderLocation eBorder)
		{
			return	m_aBorderInfo[eBorder].m_aSize.m_fWidth;
		}

		//!< get border height
		real GetBorderHeight(EPanelBorderLocation eBorder)
		{
			return	m_aBorderInfo[eBorder].m_aSize.m_fHeight;
		}

		//!< get border size
		CGUISize GetBorderSize(EPanelBorderLocation eBorder)
		{
			return	m_aBorderInfo[eBorder].m_aSize;
		}


	protected:
		struct
		{
			CGUISize m_aSize;
			CGUIImage* m_pImageInfo;
			CGUIRect m_aRenderRect;
		} m_aBorderInfo[PANEL_BORDER_NONE];

		CGUIImage* m_pImageBG;

		bool m_bAutoResize;
		int16 m_nBGAdjustLeft;
		int16 m_nBGAdjustRight;
		int16 m_nBGAdjustTop;
		int16 m_nBGAdjustBottom;
		CGUIRect m_aBgRenderArea;

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtPanel);
	};

}//namespace libguiex

#endif //__GUI_WGTPANEL_20060802_H__
