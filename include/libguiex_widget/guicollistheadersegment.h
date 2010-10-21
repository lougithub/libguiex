/** 
* @file guicollistheadersegment.h
* @brief segment of column's list header
* @author ken
* @date 2007-08-03
*/

#ifndef __GUI_COLLISTHEADERSEGMENT_20070803_H__
#define __GUI_COLLISTHEADERSEGMENT_20070803_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiwidget.h>
#include <libguiex_core\guiwidgetgenerator.h>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{

}//namespace guiex

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIColListHeaderSegment
	* @brief segment of column's list header
	* used image name:
	* - COLUMN_HEADER_NORMAL
	* - COLUMN_HEADER_HOVER
	* - COLUMN_HEADER_PUSH
	*/
	class GUIEXPORT CGUIColListHeaderSegment : public CGUIWidget
	{
	public:
		/**
		* @brief base class constructor
		*/
		CGUIColListHeaderSegment( const CGUIString& rName , const CGUIString& rProjectName);

		/**
		* @brief base class destructor
		*/
		virtual ~CGUIColListHeaderSegment(void);


		/**
		* @brief create this widget
		*/
		virtual int32 Create();


		/**
		* @brief set segment's id
		*/
		void	SetID(uint32 nId);

		/**
		* @brief get segment's id
		*/
		uint32	GetID( ) const;

		/**
		 * @brief Enumeration of possible values for sorting direction
		*/
		enum ESortDirection
		{
			eDir_None,		//!< Items under this segment should not be sorted.
			eDir_Ascending,	//!< Items under this segment should be sorted in ascending order.
			eDir_Descending	//!< Items under this segment should be sorted in descending order.
		};

		/** 
		 * @brief set sorting direction
		 */
		void	SetSortDirection(ESortDirection eDir);

		/** 
		* @brief get sorting direction
		*/
		ESortDirection	GetSortDirection() const;


		/**
		* @brief Return whether drag moving is enabled for this segment.
		*/
		bool	IsDragMovingEnabled(void) const;


		/**
		* @brief Set whether drag moving is allowed for this segment.
		*/
		void	SetDragMovingEnabled(bool setting);

		/**
		* @brief Return whether the segment is clickable.
		*/
		bool	IsClickable(void) const;

		/**
		* @brief Set whether the segment is clickable.
		*/
		void	SetClickable(bool setting);

		/**
		* @brief Return whether the segment is currently in its hovering state.
		*/
		bool    IsHovering(void) const;

		/**
		\brief
		Return whether the segment is currently in its pushed state.
		*/
		bool    IsPushed(void) const;


		/**
		* @brief Return whether the splitter is currently in its hovering state.
		*/
		bool    IsSplitterHovering(void) const;


		/**
		* @brief Return whether the segment is currently being drag-moved.
		*/
		bool    IsBeingDragMoved(void) const;


		/**
		* @brief Return whether the segment is currently being drag-sized.
		*/
		bool    IsBeingDragSized(void) const;

		/**
		* @brief Return whether this segment can be sized.
		*/
		bool	IsSizingEnabled(void) const;

		/**
		* @brief Set whether this segment can be sized.
		*/
		void	SetSizingEnabled(bool setting);


	protected:	//!< callback function
		virtual uint32		OnMouseEnter( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeave( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32		OnMouseLeftUp( CGUIEventMouse* pEvent );


	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIColListHeaderSegment( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		//init item
		void	InitColListHeaderSegment();

		//render self
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName,CGUIImage* pImage );

	protected:
		uint32	m_nID;		///id of the segment

		ESortDirection	m_eSortDir;	///sorting direction of segment

		//image
		CGUIImage*		m_pImageNormal;
		CGUIImage*		m_pImageHovering;
		CGUIImage*		m_pImagePush;

		bool			m_bAllowClicks;		//!<is segment allowed to click
		bool			m_bHovering;		//!<state of segment
		bool			m_bPushing;			//!<state of segment
		bool			m_bSplitterHover;	//!< True if the mouse is over the splitter
		bool			m_bSizingEnabled;	//!< true when sizing is enabled for this segment.
		bool			m_bMovingEnabled;	//!< True when drag-moving is enabled for this segment;
		bool			m_bDragMoving;		//!< true when segment is being drag moved.
		bool			m_bDragSizing;		//!< true when we are being sized.

	private:
		static CGUIString	ms_strType;
	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIColListHeaderSegment);

}//namespace guiex

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	inline void	CGUIColListHeaderSegment::SetID(uint32 nId)
	{
		m_nID = nId;
	}
	//------------------------------------------------------------------------------
	inline uint32	CGUIColListHeaderSegment::GetID( ) const
	{
		return m_nID;
	}
	//------------------------------------------------------------------------------
	inline CGUIColListHeaderSegment::ESortDirection	CGUIColListHeaderSegment::GetSortDirection() const
	{
		return m_eSortDir;
	}
	//------------------------------------------------------------------------------
	inline bool	CGUIColListHeaderSegment::IsDragMovingEnabled(void) const
	{
		return m_bMovingEnabled;
	}
	//------------------------------------------------------------------------------
	inline bool	CGUIColListHeaderSegment::IsClickable(void) const
	{
		return m_bAllowClicks;
	}
	//------------------------------------------------------------------------------
	inline bool	CGUIColListHeaderSegment::IsHovering(void) const  
	{
		return m_bHovering;
	}
	//------------------------------------------------------------------------------
	inline bool	CGUIColListHeaderSegment::IsPushed(void) const 
	{
		return m_bPushing;
	}
	//------------------------------------------------------------------------------
	inline bool	CGUIColListHeaderSegment::IsSplitterHovering(void) const 
	{
		return m_bSplitterHover;
	}
	//------------------------------------------------------------------------------
	inline bool	CGUIColListHeaderSegment::IsBeingDragMoved(void) const  
	{
		return m_bDragMoving;
	}
	//------------------------------------------------------------------------------
	inline bool	CGUIColListHeaderSegment::IsBeingDragSized(void) const  
	{
		return m_bDragSizing;
	}
	//------------------------------------------------------------------------------
	inline bool	CGUIColListHeaderSegment::IsSizingEnabled(void) const			
	{
		return m_bSizingEnabled;
	}
	//------------------------------------------------------------------------------
}	//namespace guiex

#endif //__GUI_COLLISTHEADERSEGMENT_20070803_H__
