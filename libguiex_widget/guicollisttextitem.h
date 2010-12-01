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
		CGUIColListTextItem( const CGUIString& rName, const CGUIString& rSceneName );

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
		virtual void		SetTextContent(const CGUIStringW& rText);
		//set text size
		virtual void		SetTextInfo( const CGUIStringInfo& rInfo);


		/**
		* @brief create a text item
		*/
		static CGUIColListTextItem*	CreateTextItem(
			const CGUIStringW& rText, 
			const CGUIStringInfo& rInfo,
			const CGUIString& rSceneName,
			uint32 nItemID = 0, 
			void* pItemData = NULL,
			bool bDisable = false);



	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIColListTextItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		//init item
		void	InitTextItem();

		//update text size
		void	UpdateTextSize();

		//render self
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIColListTextItem);
	};


}//namespace guiex

#endif //__GUI_COLLISTTEXTITEM_20070802_H__
