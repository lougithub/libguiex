/** 
* @file guilistboxtextitem.h
* @brief list box item for text
* @author ken
* @date 2006-09-18
*/
#ifndef __GUI_LISTBOXTEXTITEM_20060918_H__
#define __GUI_LISTBOXTEXTITEM_20060918_H__

//============================================================================//
// include
//============================================================================// 
#include "guilistboxitem.h"


//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/**
	* @class CGUIListBoxTextItem
	* @brief text item for list box
	*/
	class GUIEXPORT CGUIListBoxTextItem : public CGUIListBoxItem
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIListBoxTextItem( const CGUIString& rName, const CGUIString& rProjectName );

		/**
		* @brief destructor
		*/
		virtual ~CGUIListBoxTextItem(  );

		/**
		* @brief get size of this item
		*/
		virtual	real		GetHeight(void);
		virtual	real		GetWidth(void);

		//set text
		virtual void		SetTextContent(const wchar_t* pText);
		//set text size
		virtual void		SetTextInfo( const CGUIStringExInfo& rInfo);




	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIListBoxTextItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

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

	GUI_WIDGET_GENERATOR_DECLARE(CGUIListBoxTextItem);

}//namespace guiex

#endif //__GUI_LISTBOXTEXTITEM_20060918_H__
