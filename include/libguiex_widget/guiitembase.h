/** 
* @file guiitembase.h
* @brief base class of item, can be used for list box or manu
* @author ken
* @date 2007-07-31
*/

#ifndef __GUI_ITEMBASE_20070731_H__
#define __GUI_ITEMBASE_20070731_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>

//============================================================================//
// declare
//============================================================================// 


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIItemBase
	* @brief base item for list box
	* used image name:
	*/
	class GUIEXPORT CGUIItemBase : public CGUIWidget
	{
	public:
		/**
		* @brief base class constructor
		*/
		CGUIItemBase( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief base class destructor
		*/
		virtual ~CGUIItemBase(void);

		/**
		* @brief get the ID assigned to this list box item.
		*/
		uint32		GetID(void) const;

		/**
		* @brief get the ID assigned to this list box item.
		*/
		void		SetID(uint32 nItemId);

		/**
		* @brief get user data assigned to this item.
		*/
		void*		GetUserData(void) const;

		/**
		* @brief set user data
		*/
		void		SetUserData(void* pUserData);

		/**
		* @brief	return whether this item is selected.
		*/
		bool		IsSelected(void) const;

		/**
		* @brief	select this item
		*/
		void		SetSelected(bool bSelect);

		/**
		* @brief	return whether this item is disabled.
		*/
		bool		IsDisabled(void) const;

		/**
		* @brief	make this item disable
		*/
		void		SetDisabled(bool bDisable);

		/**
		* @brief set image which will be draw if item is selected
		*/
		void		SetSelectionImage(CGUIImage* pImage);

		/** 
		* @brief Return the rendered pixel size of this list box item.
		*/
		virtual	real		GetHeight(void);
		virtual	real		GetWidth(void);

		/**
		* @brief Less-than operator, compares item texts.
		*/
		virtual	bool	operator<(const CGUIItemBase& rItem) const;


		/**
		* @brief Greater-than operator, compares item texts.
		*/
		virtual	bool	operator>(const CGUIItemBase& rItem) const;


		//set item information
		virtual void	SetItemInfo(
			const CGUIStringW& rText, 
			const CGUIStringInfo& rInfo,
			uint32 nItemID = 0, 
			void* pItemData = NULL,
			bool bDisable = false);


	protected:	//!< callback function

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIItemBase( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		//init item
		void	InitItemBase();

		//render self
		virtual void RenderSelf(IGUIInterfaceRender* pRender);


	protected:

		uint32				m_nItemID;			//!< item id
		void*				m_pUserData;		//!< user data
		bool				m_bSelected;		//!< whether this item is selected
		CGUIImage*			m_pImageSelection;	//!< selection image 

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIItemBase);
	};
}//namespace guiex

#endif //__GUI_ITEMBASE_20070731_H__
