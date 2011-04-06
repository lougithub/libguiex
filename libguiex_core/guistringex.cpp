/** 
 * @file guistring.cpp
 * @brief string used in this system, which could convert 
 * string code automatically
 * @author ken
 * @date 2006-04-07
 */


//============================================================================//
// 	include
//============================================================================// 
#include <libguiex_core/guistringex.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiexception.h>

//============================================================================//
// 	function
//============================================================================// 
namespace guiex
{
	CGUIStringInfo::CGUIStringInfo()
		:m_nFontIdx(0)
		,m_nFontSize(16)
	{
	}
	//------------------------------------------------------------------------------
	CGUIStringInfo::CGUIStringInfo(const CGUIStringInfo& rInfo)
	{
		*this = rInfo;
	}
	//------------------------------------------------------------------------------
	CGUIStringInfo::CGUIStringInfo(int16 nFontIdx,uint16 nFontSize,const CGUIColor& rColor)
		:m_nFontIdx(nFontIdx)
		,m_nFontSize(nFontSize)
		,m_aColor(rColor)
	{
	}
	//------------------------------------------------------------------------------
	bool CGUIStringInfo::operator==(const CGUIStringInfo& rInfo ) const
	{
		if( m_nFontIdx == rInfo.m_nFontIdx &&
			m_nFontSize == rInfo.m_nFontSize &&
			m_aColor == rInfo.m_aColor)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIStringInfo::operator!=(const CGUIStringInfo& rInfo ) const
	{
		return !(*this == rInfo);
	}
	//------------------------------------------------------------------------------
	const CGUIStringInfo& CGUIStringInfo::operator=(const CGUIStringInfo& rInfo )
	{
		m_nFontIdx = rInfo.m_nFontIdx;
		m_nFontSize = rInfo.m_nFontSize;
		m_aColor = rInfo.m_aColor;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIStringEx::CGUIStringEx()
	{
	}
	//------------------------------------------------------------------------------
	CGUIStringEx::CGUIStringEx(const CGUIStringEx& rString )
		:m_strContent(rString.m_strContent)
	{
		*this = rString;
	}
	//------------------------------------------------------------------------------
	CGUIStringEx::CGUIStringEx( const CGUIStringW& str )
		:m_strContent(str)
	{
	}
	//------------------------------------------------------------------------------
	CGUIStringEx::CGUIStringEx(const CGUIString& rString )
	{
		AppMultiByteToWideChar( rString, this->m_strContent );
	}
	//------------------------------------------------------------------------------
	CGUIStringEx::~CGUIStringEx()
	{
	}	
	//------------------------------------------------------------------------------ 
	const CGUIStringEx& CGUIStringEx::operator=( const CGUIStringEx& rOther )
	{
		m_aStringInfo = rOther.m_aStringInfo;
		m_strContent = rOther.m_strContent;

		return *this;
	}
	//------------------------------------------------------------------------------ 
	/**
	* @brief set string default info
	*/
	void CGUIStringEx::SetStringInfo(const CGUIStringInfo* pInfo)
	{
		m_aStringInfo = *pInfo;
	}
	//------------------------------------------------------------------------------ 
	const CGUIStringInfo& CGUIStringEx::GetStringInfo() const
	{
		return m_aStringInfo;
	}
	//------------------------------------------------------------------------------ 
	CGUIStringInfo&	CGUIStringEx::GetStringInfo() 
	{
		return m_aStringInfo;
	}
	//------------------------------------------------------------------------------ 
	/**
	* @brief set string content
	*/
	void CGUIStringEx::SetContent( const CGUIStringW& rContent)
	{
		m_strContent = rContent;
	}
	//------------------------------------------------------------------------------ 
	/**
	* @brief get string content
	*/
	const CGUIStringW& CGUIStringEx::GetContent( ) const
	{
		return m_strContent;
	}
	//------------------------------------------------------------------------------ 
	/**
	* @brief get string content
	*/
	CGUIStringW& CGUIStringEx::GetContent( )
	{
		return m_strContent;
	}
	//------------------------------------------------------------------------------ 
}	//namespace guiex


