/** 
* @file guimenutextitem.h
* @brief text item for menu
* @author ken
* @date 2007-07-31
*/
#ifndef __GUI_MENUTEXTITEM_20070731_H__
#define __GUI_MENUTEXTITEM_20070731_H__

//============================================================================//
// include
//============================================================================// 
#include "guimenuitem.h"



//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIMenuTextItem
	* @brief text item for list box
	*/
	class GUIEXPORT CGUIMenuTextItem : public CGUIMenuItem
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIMenuTextItem( const CGUIString& rName, const CGUIString& rProjectName );

		/**
		* @brief destructor
		*/
		virtual ~CGUIMenuTextItem(  );

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
		CGUIMenuTextItem( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

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

	GUI_WIDGET_GENERATOR_DECLARE(CGUIMenuTextItem);

}//namespace guiex

#endif //__GUI_MENUTEXTITEM_20070731_H__
