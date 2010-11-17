/** 
* @file guicollistheader.cpp
* @brief column list header
* @author ken
* @date 2007-08-03
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guicollistheader.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_widget/guiwgtcolumnlist.h>
#include <libguiex_widget/guicollistheadersegment.h>
#include <libguiex_core/guiwidgetsystem.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIColListHeader);
	//------------------------------------------------------------------------------
	CGUIString CGUIColListHeader::ms_strType = "CGUIColListHeader";
	//------------------------------------------------------------------------------
	CGUIColListHeader::CGUIColListHeader( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(ms_strType, rName, rSceneName)
	{
		InitColListHeader();
	}
	//------------------------------------------------------------------------------
	CGUIColListHeader::CGUIColListHeader( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitColListHeader();
	}
	//------------------------------------------------------------------------------
	CGUIColListHeader::~CGUIColListHeader(void)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIColListHeader::InitColListHeader()
	{
		m_pSortedSegment = NULL;

		m_strSegmentType = "CGUIColListHeaderSegment";
	}
	//------------------------------------------------------------------------------
	uint32	CGUIColListHeader::GetSegmentCount(void) const
	{
		return static_cast<uint32>(m_vecSegment.size());
	}
	//------------------------------------------------------------------------------
	CGUIColListHeaderSegment*	CGUIColListHeader::GetSegmentByIndex(uint32 nIndex) const
	{
		if( nIndex >= m_vecSegment.size())
		{
			throw CGUIException("CGUIColListHeader::GetSegmentByIndex: index is outof range");
		}
		return m_vecSegment[nIndex];
	}
	//------------------------------------------------------------------------------
	CGUIColListHeaderSegment*	CGUIColListHeader::GetSegmentByID(uint32 id) const
	{
		for( TVecSegment::const_iterator itor = m_vecSegment.begin();
			itor != m_vecSegment.end(); 
			++itor)
		{
			if((*itor)->GetID() == id )
			{
				return (*itor);
			}
		}

		// No such segment found, throw exception
		throw CGUIException("CGUIColListHeader::GetSegmentByID: no segment which has the given id <%d>", id);
		return NULL;
	}
	//------------------------------------------------------------------------------
	CGUIColListHeaderSegment*	CGUIColListHeader::GetSegmentByText(const wchar_t* pText) const
	{
		for( TVecSegment::const_iterator itor = m_vecSegment.begin();
			itor != m_vecSegment.end(); 
			++itor)
		{
			if( wcscmp((*itor)->GetTextContent(), pText)==0)
			{
				return *itor;
			}
		}

		// no items matched.
		throw CGUIException("CGUIColListHeader::GetSegmentByID: no segment which has the text");
		return NULL;
	}
	//------------------------------------------------------------------------------
	void	CGUIColListHeader::SetSortSegment(uint32 nIdx)
	{
		if (nIdx >= m_vecSegment.size())
		{
			throw CGUIException("CGUIColListHeader::SetSortSegment - specified segment index is out of range.");
		}
		else
		{
			// if column is different to current sort segment
			if (m_pSortedSegment != m_vecSegment[nIdx])
			{
				// set sort direction on 'old' sort segment to none.
				//if (m_pSortedSegment)
				//{
				//	m_pSortedSegment->setSortDirection(ListHeaderSegment::None);
				//}

				// set-up new sort segment
				m_pSortedSegment = m_vecSegment[nIdx];
				//m_pSortedSegment->setSortDirection(d_sortDir);

				//// Fire sort column changed event
				//WindowEventArgs args(this);
				//onSortColumnChanged(args);
			}

		}
	}
	//------------------------------------------------------------------------------
	CGUIColListHeaderSegment*	CGUIColListHeader::GetSortSegment(void) const
	{
		return m_pSortedSegment;
	}
	//------------------------------------------------------------------------------
	uint32	CGUIColListHeader::GetSortSegmentIndex(void) const
	{
		return GetSegmentIndex(m_pSortedSegment);
	}
	//------------------------------------------------------------------------------
	uint32	CGUIColListHeader::GetSegmentIndex(const CGUIColListHeaderSegment* pSegment) const
	{
		for (uint32 i = 0; i < m_vecSegment.size(); ++i)
		{
			if (m_vecSegment[i] == pSegment)
			{
				return i;
			}
		}

		// No such segment found, throw exception
		throw CGUIException("CGUIColListHeader::GetSegmentIndex: the given segment isn't attached to this header");
		return GUI_INVALID;
	}
	//------------------------------------------------------------------------------
	void	CGUIColListHeader::SetSegmentTextInfo(const CGUIStringInfo& rInfo)
	{
		m_aSegmentTextInfo = rInfo;
	}
	//------------------------------------------------------------------------------
	const CGUIStringInfo&	CGUIColListHeader::GetSegmentTextInfo() const
	{
		return m_aSegmentTextInfo;
	}
	//------------------------------------------------------------------------------
	void	CGUIColListHeader::SetDefaultSegmentSize( const CGUISize& rSize)
	{
		m_aSegmentDefaultSize = rSize;
	}
	//------------------------------------------------------------------------------
	const CGUISize& CGUIColListHeader::GetDefaultSegmentSize( ) const
	{
		return m_aSegmentDefaultSize;
	}
	//------------------------------------------------------------------------------
	real	CGUIColListHeader::GetSegmentWidthByIndex( uint32 nIdx) const 
	{
		if( nIdx >= m_vecSegment.size())
		{
			throw CGUIException("CGUIColListHeader::GetSegmentWidthByIndex: the given index is out of range.");
		}
		return m_vecSegment[nIdx]->GetSize().m_fWidth;
	}
	//------------------------------------------------------------------------------
	real	CGUIColListHeader::GetSegmentTotalWidth( ) const
	{
		real fWidth = 0.0f;
		for( TVecSegment::const_iterator itor = m_vecSegment.begin();
			itor != m_vecSegment.end();
			++itor)
		{
			fWidth += (*itor)->GetSize().m_fWidth;
		}
		return fWidth;
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIColListHeader::GetSegmentPosByIndex( uint32 nIdx) const
	{
		if( nIdx >= m_vecSegment.size())
		{
			throw CGUIException("CGUIColListHeader::GetSegmentPosByIndex: the given index is out of range.");
		}

		CGUIVector2 aPos( 0.0f, 0.0f );
		for( uint32 i=0; i<nIdx; ++i)
		{
			aPos.x += m_vecSegment[i]->GetSize().m_fWidth;
		}

		return aPos;
	}
	//------------------------------------------------------------------------------
	void	CGUIColListHeader::AddSegment( const wchar_t* pText, uint32 nId )
	{
		CGUIColListHeaderSegment * pSegment = static_cast<CGUIColListHeaderSegment*>(GUI_CREATE_WIDGET(m_strSegmentType.c_str(), CGUIString("segment_")+CGUIWidgetSystem::Instance()->GenerateAnonymousName(), GetSceneName()));
		pSegment->SetParent(this);
		m_vecSegment.push_back(pSegment);
		pSegment->SetTextContent(pText);
		pSegment->SetTextInfo(GetSegmentTextInfo());

		CGUIImage* pImage = NULL;
		if( pImage = GetImage("COLUMN_HEADER_NORMAL"))
		{
			pSegment->SetImage("COLUMN_HEADER_NORMAL", pImage);
		}
		if( pImage = GetImage("COLUMN_HEADER_HOVER"))
		{
			pSegment->SetImage("COLUMN_HEADER_HOVER", pImage);
		}
		if( pImage = GetImage("COLUMN_HEADER_PUSH"))
		{
			pSegment->SetImage("COLUMN_HEADER_PUSH", pImage);
		}
		
		pSegment->SetSize( m_aSegmentDefaultSize);

		if(!m_pSortedSegment)
		{
			m_pSortedSegment = m_vecSegment[0];
		}

		LayoutSegment();
	}
	//------------------------------------------------------------------------------
	void		CGUIColListHeader::LayoutSegment()
	{
		CGUIVector2	aPos(0.0f, 0.0f);
		CGUISize	aSize(0.0f, 0.0f);

		for( TVecSegment::iterator itor = m_vecSegment.begin();
			itor != m_vecSegment.end(); 
			++itor)
		{
			CGUIColListHeaderSegment* pSegment = *itor;
			pSegment->SetLocalPosition(aPos);
			aPos.x += pSegment->GetSize().GetWidth();

			aSize.m_fHeight = max(aSize.m_fHeight, pSegment->GetSize().m_fHeight);
			aSize.m_fWidth = aPos.x;
		}

		SetSize( aSize );
	}
	//------------------------------------------------------------------------------




	//------------------------------------------------------------------------------
	void	CGUIColListHeader::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIColListHeader::RenderSelf(IGUIInterfaceRender* pRender)
	{

	}
	//------------------------------------------------------------------------------
}//namespace guiex


