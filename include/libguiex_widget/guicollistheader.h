/** 
* @file guicollistheader.h
* @brief column list header
* @author ken
* @date 2007-08-03
*/

#ifndef __GUI_COLLISTHEADER_20070803_H__
#define __GUI_COLLISTHEADER_20070803_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiwidget.h>
#include <libguiex_core\guiwidgetgenerator.h>
#include <vector>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIColListHeaderSegment;
}//namespace guiex

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIColListHeader
	* @brief header of column list
	* used image name:
	* - COLUMN_HEADER_NORMAL
	* - COLUMN_HEADER_HOVER
	* - COLUMN_HEADER_PUSH
	*/
	class GUIEXPORT CGUIColListHeader : public CGUIWidget
	{
	public:
		/**
		* @brief base class constructor
		*/
		CGUIColListHeader( const CGUIString& rName, const CGUIString& rProjectName );

		/**
		* @brief base class destructor
		*/
		virtual ~CGUIColListHeader(void);

		/**
		* @brief Return the number of segment
		* @return number of segment
		*/
		uint32	GetSegmentCount(void) const;

		/**
		* @brief get the segment by given index
		* @param nIndex segment index, start from zero
		*/
		CGUIColListHeaderSegment*	GetSegmentByIndex(uint32 nIndex) const;

		/**
		* @brief get the segment by given id
		* return pointer of CGUIColListHeaderSegment object, If more than one segment 
		* has the same ID, only the first one will be returned.
		*/
		CGUIColListHeaderSegment*	GetSegmentByID(uint32 id) const;

		/**
		* @brief get the segment by given text content.
		*/
		CGUIColListHeaderSegment*	GetSegmentByText(const wchar_t* pText) const;

		/**
		* @brief Set the column to be used as the sort key.
		*/
		void	SetSortSegment(uint32 nIdx);

		/**
		* @brief return the current sort segment pointer
		*/
		CGUIColListHeaderSegment*	GetSortSegment(void) const;

		/**
		* @brief Return the zero based index of the current sort column.  
		There must be at least one column to successfully call this method.
		*/
		uint32	GetSortSegmentIndex(void) const;

		/**
		* @brief Return the zero based index of the specified segment.
		*/
		uint32	GetSegmentIndex(const CGUIColListHeaderSegment* pSegment) const;

		/** 
		* @brief set segment text info
		*/
		void	SetSegmentTextInfo(const CGUIStringInfo& rInfo);

		/** 
		* @brief get segment text info
		*/
		const CGUIStringInfo&	GetSegmentTextInfo() const;

		/**
		* @brief add a segment
		*/
		void	AddSegment( const wchar_t* pText, uint32 nId );

		/**
		* @brief set default size of segment
		*/
		void	SetDefaultSegmentSize( const CGUISize& rSize);

		/**
		* @brief get default size of segment
		*/
		const CGUISize& GetDefaultSegmentSize( ) const;

		/**
		* @brief get segment width by specified size
		*/
		real	GetSegmentWidthByIndex( uint32 nIdx)const;


		/**
		* @brief get the width of all segments
		*/
		real	GetSegmentTotalWidth( ) const;

		/**
		* @brief get segment position by specified size
		*/
		CGUIVector2	GetSegmentPosByIndex( uint32 nIdx) const;

	protected:	//!< callback function



	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIColListHeader( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		//init item
		void	InitColListHeader();

		//render self
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName,CGUIImage* pImage );

		/** 
		 * @brief layout the segments
		 */
		void		LayoutSegment();

	protected:
		typedef	std::vector<CGUIColListHeaderSegment*>		TVecSegment;
		TVecSegment	m_vecSegment;							//segment list

		CGUIColListHeaderSegment*	m_pSortedSegment;		//sorted segment

		CGUIString					m_strSegmentType;		//type of segment
		CGUIStringInfo			m_aSegmentTextInfo;		//text information of segment
		CGUISize					m_aSegmentDefaultSize;	//default size of segment

	private:
		static CGUIString	ms_strType;
	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIColListHeader);

}//namespace guiex

#endif //__GUI_COLLISTHEADER_20070803_H__
