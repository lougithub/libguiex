/** 
* @file guiwgtcombobox.h
* @brief combobox used in guiex system
* @author ken
* @date 2006-09-22
*/

#ifndef __GUI_WGTCOMBOBOX_20060918_H__
#define __GUI_WGTCOMBOBOX_20060918_H__

//============================================================================//
// include
//============================================================================// 
#include "guiwgttextbase.h"
#include <libguiex_core/guiwidgetgenerator.h>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWgtListBoxItem;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtComboBox
	* @brief ComboBox
	* used image name:
	* - background
	*
	* - droplist_background
	*
	* droplist_scrollbar_v_background
	* droplist_scrollbar_v_arrow_0_normal
	* droplist_scrollbar_v_arrow_0_hover
	* droplist_scrollbar_v_arrow_0_disable
	* droplist_scrollbar_v_arrow_0_push
	* droplist_scrollbar_v_arrow_0_hover_overlay
	* droplist_scrollbar_v_arrow_0_push_overlay
	* droplist_scrollbar_v_arrow_1_normal
	* droplist_scrollbar_v_arrow_1_hover
	* droplist_scrollbar_v_arrow_1_disable
	* droplist_scrollbar_v_arrow_1_push
	* droplist_scrollbar_v_arrow_1_hover_overlay
	* droplist_scrollbar_v_arrow_1_push_overlay
	* droplist_scrollbar_v_slide_normal
	* droplist_scrollbar_v_slide_hover
	* droplist_scrollbar_v_slide_disable
	* droplist_scrollbar_v_slide_push
	* droplist_scrollbar_v_slide_hover_overlay
	* droplist_scrollbar_v_slide_push_overlay
	*
	* droplist_scrollbar_h_background
	* droplist_scrollbar_h_arrow_0_normal
	* droplist_scrollbar_h_arrow_0_hover
	* droplist_scrollbar_h_arrow_0_disable
	* droplist_scrollbar_h_arrow_0_push
	* droplist_scrollbar_h_arrow_0_hover_overlay
	* droplist_scrollbar_h_arrow_0_push_overlay
	* droplist_scrollbar_h_arrow_1_normal
	* droplist_scrollbar_h_arrow_1_hover
	* droplist_scrollbar_h_arrow_1_disable
	* droplist_scrollbar_h_arrow_1_push
	* droplist_scrollbar_h_arrow_1_hover_overlay
	* droplist_scrollbar_h_arrow_1_push_overlay
	* droplist_scrollbar_h_slide_normal
	* droplist_scrollbar_h_slide_hover
	* droplist_scrollbar_h_slide_disable
	* droplist_scrollbar_h_slide_push
	* droplist_scrollbar_h_slide_hover_overlay
	* droplist_scrollbar_h_slide_push_overlay
	*/
	class GUIEXPORT CGUIWgtComboBox : public CGUIWgtTextBase
	{
	public:
		CGUIWgtComboBox( const CGUIString& rName, const CGUIString& rSceneName );
		~CGUIWgtComboBox( );

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty );

		void SetDropListSize( const CGUISize& rSize );
		const CGUISize&	GetDropListSize(  ) const;

		void SetSelectedItem(uint32 nIdx );
		void SetSelectedItem( CGUIWgtListBoxItem* pItem );

		bool HasSelectedItem() const;
		const CGUIWgtListBoxItem* GetSelectedItem() const;
		uint32	GetSelectedItemIndex() const;

		uint32 GetItemCount(void) const	;

		CGUIWgtListBoxItem*	GetItemByIndex(uint32 nIndex) const;
		uint32 GetItemIndex(const CGUIWgtListBoxItem* pItem) const;

		void AddItem(CGUIWidget* pItem);
		void RemoveItem( CGUIWidget* pItem);
		void RemoveItem( uint32 nIndex);

	protected:
		CGUIWgtComboBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitComboBox();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );
		virtual void OnCreate();

	protected:	//!< callback function
		virtual uint32 OnMouseLeftDown( CGUIEventMouse* pEvent );

	protected:
		friend class CGUIWgtComboBoxDropList;
		CGUIWgtComboBoxDropList* m_pDropList;

		uint32 m_nSelectedItemIdx; /// selected item

		CGUIImage* m_pImageBG; /// image for combobox's background

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtComboBox);
	};

}//namespace guiex

#endif	//__GUI_WGTCOMBOBOX_20060918_H__
