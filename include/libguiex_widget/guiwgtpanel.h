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
	*	- BGIMAGE
	*	- PANEL_BORDER_LEFT
	*	- PANEL_BORDER_TOP
	*	- PANEL_BORDER_BOTTOM
	*	- PANEL_BORDER_RIGHT
	*	- PANEL_BORDER_TOPLEFT
	*	- PANEL_BORDER_TOPRIGHT
	*	- PANEL_BORDER_BOTTOMLEFT
	*	- PANEL_BORDER_BOTTOMRIGHT
	*/
	class GUIEXPORT CGUIWgtPanel : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtPanel( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtPanel( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		///initialize check button
		void InitPanel();
		
		virtual void RefreshSelf();

		/**
		* @brief override the OnSetImage function
		*/
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

		const CGUIRect&	GetBorderRect(int32 eBorder);

		enum EPanelBorderLocation
		{
			PANEL_BORDER_TOPLEFT = 0,			
			PANEL_BORDER_TOPRIGHT,		
			PANEL_BORDER_BOTTOMLEFT,		
			PANEL_BORDER_BOTTOMRIGHT,		
			PANEL_BORDER_TOP,				
			PANEL_BORDER_LEFT,			
			PANEL_BORDER_BOTTOM,			
			PANEL_BORDER_RIGHT,
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
			const CGUIImage* m_pImageInfo;
			CGUIRect m_aRenderRect;
		} m_aBorderInfo[PANEL_BORDER_NONE];

		const CGUIImage* m_pImageBG;

		bool		m_bAutoResize;

	private:
		static CGUIString	ms_strType;
	};



	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtPanel);

}//namespace libguiex



#endif //__GUI_WGTPANEL_20060802_H__

