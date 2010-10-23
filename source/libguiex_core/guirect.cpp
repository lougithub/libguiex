/** 
* @file guirect.cpp
* @brief CGUIRect used in this system
* @author ken
* @date 2006-04-24
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guirect.h>
#include <libguiex_core\guisize.h>
#include <libguiex_core\guivector2.h>
#include <libguiex_core\guiproperty.h>
#include <libguiex_core\guistringconvertor.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUIRect::CGUIRect(void) 
		:m_fTop(0.0f)
		,m_fBottom(0.0f)
		,m_fLeft(0.0f)
		,m_fRight(0.0f)
	{
	}
	//------------------------------------------------------------------------------
	CGUIRect::CGUIRect(real left, real top, real right, real bottom) :
	m_fTop(top),
		m_fBottom(bottom),
		m_fLeft(left),
		m_fRight(right)
	{
	}
	//------------------------------------------------------------------------------
	CGUIRect::CGUIRect(CGUIVector2 pos, CGUISize sz) :
	m_fTop(pos.y),
		m_fBottom(pos.y + sz.m_fHeight),
		m_fLeft(pos.x),
		m_fRight(pos.x + sz.m_fWidth)
	{
	}
	//------------------------------------------------------------------------------
	CGUIRect::CGUIRect(const CGUIRect& rRect)
	{
		SetRect(rRect);
	}
	//------------------------------------------------------------------------------
	CGUIRect CGUIRect::GetIntersection(const CGUIRect& rect) const
	{
		// check for total exclusion
		if ((m_fRight > rect.m_fLeft) &&
			(m_fLeft < rect.m_fRight) &&
			(m_fBottom > rect.m_fTop) &&
			(m_fTop < rect.m_fBottom))
		{
			return CGUIRect(
				(m_fLeft > rect.m_fLeft) ? m_fLeft : rect.m_fLeft,
				(m_fTop > rect.m_fTop) ? m_fTop : rect.m_fTop,
				(m_fRight < rect.m_fRight) ? m_fRight : rect.m_fRight,
				(m_fBottom < rect.m_fBottom) ? m_fBottom : rect.m_fBottom);
		}
		else
		{
			return CGUIRect(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIRect::IsPointInRect(const CGUIVector2& pt) const
	{
		if ((m_fLeft > pt.x) ||
			(m_fRight <= pt.x) ||
			(m_fTop > pt.y) ||
			(m_fBottom <= pt.y))
		{
			return false;
		}

		return true;
	}
	//------------------------------------------------------------------------------
	void CGUIRect::SetPosition(const CGUIVector2& pt)
	{
		CGUISize sz(GetSize());

		m_fLeft = pt.x;
		m_fTop  = pt.y;
		SetSize(sz);
	}
	//------------------------------------------------------------------------------
	CGUIRect& CGUIRect::ConstrainSizeMax(const CGUISize& sz)
	{
		if (GetWidth() > sz.m_fWidth)
		{
			SetWidth(sz.m_fWidth);
		}

		if (GetHeight() > sz.m_fHeight)
		{
			SetHeight(sz.m_fHeight);
		}

		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIRect& CGUIRect::ConstrainSizeMin(const CGUISize& sz)
	{
		if (GetWidth() < sz.m_fWidth)
		{
			SetWidth(sz.m_fWidth);
		}

		if (GetHeight() < sz.m_fHeight)
		{
			SetHeight(sz.m_fHeight);
		}

		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIRect& CGUIRect::ConstrainSize(const CGUISize& max_sz, const CGUISize& min_sz)
	{
		CGUISize curr_sz(GetSize());

		if (curr_sz.m_fWidth > max_sz.m_fWidth)
		{
			SetWidth(max_sz.m_fWidth);
		}
		else if (curr_sz.m_fWidth < min_sz.m_fWidth)
		{
			SetWidth(min_sz.m_fWidth);
		}

		if (curr_sz.m_fHeight > max_sz.m_fHeight)
		{
			SetHeight(max_sz.m_fHeight);
		}
		else if (curr_sz.m_fHeight < min_sz.m_fHeight)
		{
			SetHeight(min_sz.m_fHeight);
		}

		return *this;
	}

	//------------------------------------------------------------------------------
	void CGUIRect::SaveToProperty( CGUIProperty& rProperty ) const
	{
		rProperty.SetValue( CGUIStringConvertor::RectToString( *this ));
	}
	//------------------------------------------------------------------------------
	void CGUIRect::LoadFromProperty( const CGUIProperty& rProperty )
	{
		CGUIStringConvertor::StringToRect( rProperty.GetValue(), *this );
	}
	//------------------------------------------------------------------------------

}	//namespace guiex


