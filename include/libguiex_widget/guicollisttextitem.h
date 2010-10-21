/** 
* @file guicollisttextitem.h
* @brief column list box item for text
* @author ken
* @date 2007-08-02
*/
#ifndef __GUI_COLLISTTEXTITEM_20070802_H__
#define __GUI_COLLISTTEXTITEM_20070802_H__

//============================================================================//
// include
//============================================================================// 
#include "guicollistitem.h"


//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/**
	* @class CGUIColListTextItem
	* @brief text item for list box
	*/
	class GUIEXPORT CGUIColListTextItem : public CGUIColListItem
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIColListTextItem( const CGUIString& rName, const CGUIString& rProjectName );

		/**
		* @brief destructor
		*/
		virtual ~CGUIColListTextItem(  );

		/**
		* @brief get size of this item
		*/
		virtual	real		GetHeight(void);
		virtual	real		GetWidth(void);

		//set text
		virtual void		SetTextContent(const wchar_t* pText);
		//set text size
		virtual void		SetTextInfo( const CGUIStringExInfo& rInfo);


		/**
		* @brief create a text item
		*/
		static CGUIColListTextItem*	CreateTextItem(
			const wchar_t* pText, 
			const CGUIStringExInfo& rInfo,
			const CGUIString& rProjectName,
			uint32 nItemID = 0, 
			void* pItemData = NULL,
			bool bDisable = false);



	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIColListTextItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		//init item
		void	InitTextItem();

		//update text size
		void	UpdateTextSize();

		//render self
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

	protected:


	private:
		static CGUIString	ms_strType;
	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIColListTextItem);

}//namespace guiex

#endif //__GUI_COLLISTTEXTITEM_20070802_H__
