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
	CGUIStringRenderInfo::CGUIStringRenderInfo()
		:m_uFontID(0)
		,m_fFontScale(1.0f)
	{
	}
	//------------------------------------------------------------------------------
	CGUIStringRenderInfo::CGUIStringRenderInfo(const CGUIStringRenderInfo& rInfo)
	{
		*this = rInfo;
	}
	//------------------------------------------------------------------------------
	CGUIStringRenderInfo::CGUIStringRenderInfo(uint16 uFontID, real fFontScale,const CGUIColor& rColor)
		:m_uFontID(uFontID)
		,m_fFontScale(fFontScale)
		,m_aColor(rColor)
	{
	}
	//------------------------------------------------------------------------------
	bool CGUIStringRenderInfo::operator==(const CGUIStringRenderInfo& rInfo ) const
	{
		if( m_uFontID == rInfo.m_uFontID &&
			m_fFontScale == rInfo.m_fFontScale &&
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
	bool CGUIStringRenderInfo::operator!=(const CGUIStringRenderInfo& rInfo ) const
	{
		return !(*this == rInfo);
	}
	//------------------------------------------------------------------------------
	const CGUIStringRenderInfo& CGUIStringRenderInfo::operator=(const CGUIStringRenderInfo& rInfo )
	{
		m_uFontID = rInfo.m_uFontID;
		m_fFontScale = rInfo.m_fFontScale;
		m_aColor = rInfo.m_aColor;
		return *this;
	}
	//------------------------------------------------------------------------------
	CGUIStringRender::CGUIStringRender()
	{
	}
	//------------------------------------------------------------------------------
	CGUIStringRender::CGUIStringRender(const CGUIStringRender& rString )
		:m_strContent(rString.m_strContent)
	{
		*this = rString;
	}
	//------------------------------------------------------------------------------
	CGUIStringRender::CGUIStringRender( const CGUIStringW& str )
		:m_strContent(str)
	{
	}
	//------------------------------------------------------------------------------
	CGUIStringRender::CGUIStringRender(const CGUIString& rString )
	{
		AppMultiByteToWideChar( rString, this->m_strContent );
	}
	//------------------------------------------------------------------------------
	CGUIStringRender::~CGUIStringRender()
	{
	}	
	//------------------------------------------------------------------------------ 
	const CGUIStringRender& CGUIStringRender::operator=( const CGUIStringRender& rOther )
	{
		m_aStringInfo = rOther.m_aStringInfo;
		m_strContent = rOther.m_strContent;

		return *this;
	}
	//------------------------------------------------------------------------------ 
	/**
	* @brief set string default info
	*/
	void CGUIStringRender::SetStringInfo(const CGUIStringRenderInfo* pInfo)
	{
		m_aStringInfo = *pInfo;
	}
	//------------------------------------------------------------------------------ 
	const CGUIStringRenderInfo& CGUIStringRender::GetStringInfo() const
	{
		return m_aStringInfo;
	}
	//------------------------------------------------------------------------------ 
	CGUIStringRenderInfo&	CGUIStringRender::GetStringInfo() 
	{
		return m_aStringInfo;
	}
	//------------------------------------------------------------------------------ 
	/**
	* @brief set string content
	*/
	void CGUIStringRender::SetContent( const CGUIStringW& rContent)
	{
		m_strContent = rContent;
	}
	//------------------------------------------------------------------------------ 
	/**
	* @brief get string content
	*/
	const CGUIStringW& CGUIStringRender::GetContent( ) const
	{
		return m_strContent;
	}
	//------------------------------------------------------------------------------ 
	/**
	* @brief get string content
	*/
	CGUIStringW& CGUIStringRender::GetContent( )
	{
		return m_strContent;
	}
	//------------------------------------------------------------------------------ 
}	//namespace guiex


