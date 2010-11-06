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
#include <libguiex_core\guistringex.h>
#include <libguiex_core\guistringconvertor.h>
#include <libguiex_core\guiexception.h>


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
	bool	CGUIStringInfo::operator==(const CGUIStringInfo& rInfo ) const
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
	bool	CGUIStringInfo::operator!=(const CGUIStringInfo& rInfo ) const
	{
		return !(*this == rInfo);
	}
	//------------------------------------------------------------------------------
	const CGUIStringInfo&	CGUIStringInfo::operator=(const CGUIStringInfo& rInfo )
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
		m_vecInfo = rString.m_vecInfo;
		m_aDefaultInfo =rString.m_aDefaultInfo;
	}
	//------------------------------------------------------------------------------
	CGUIStringEx::CGUIStringEx(const wchar_t* pChar)
		:m_strContent(pChar)
	{

	}
	//------------------------------------------------------------------------------
	CGUIStringEx::CGUIStringEx(const std::wstring& str)
		:m_strContent(str)
	{

	}
	//------------------------------------------------------------------------------
	CGUIStringEx::CGUIStringEx(const CGUIString& rString, const char* pSrcCode )
	{
		MultiByteToWideChar(rString, *this, pSrcCode);
	}
	//------------------------------------------------------------------------------
	CGUIStringEx::~CGUIStringEx()
	{
	}	
	//------------------------------------------------------------------------------ 
	void	CGUIStringEx::SetDefaultInfo(const CGUIStringInfo* pInfo)
	{
		m_aDefaultInfo = *pInfo;
		m_vecInfo.clear();
	}
	//------------------------------------------------------------------------------ 
	const CGUIStringInfo&	CGUIStringEx::GetDefaultInfo() const
	{
		return m_aDefaultInfo;
	}
	//------------------------------------------------------------------------------ 
	void	CGUIStringEx::SetContent(const wchar_t* pContent)
	{
		Clear();
		m_strContent = pContent;
	}
	//------------------------------------------------------------------------------ 
	const wchar_t*	CGUIStringEx::GetContent( ) const
	{
		return m_strContent.c_str();
	}
	//------------------------------------------------------------------------------ 
	uint32	CGUIStringEx::Size(void) const
	{
		return static_cast<uint32>(m_strContent.size());
	}
	//------------------------------------------------------------------------------ 
	bool		CGUIStringEx::Empty() const
	{
		return m_strContent.empty();
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Clear()
	{
		m_strContent.clear();
		m_vecInfo.clear();
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::EraseStringInfo(uint32 beginIdx, uint32 endIdx)
	{
		if( !m_vecInfo.empty())
		{
			m_vecInfo.erase( m_vecInfo.begin()+beginIdx, m_vecInfo.begin()+endIdx);
		}
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::InsertStringInfo(uint32 idx, uint32 count, const CGUIStringInfo* pInfo)
	{
		GUI_ASSERT(count, "invalid count");

		if( pInfo && *pInfo != m_aDefaultInfo)
		{
			//not a default info
			if( m_vecInfo.empty())
			{
				//resize it
				m_vecInfo.resize(Size()-count, m_aDefaultInfo);
				for( uint32 i=0; i<count; ++i)
				{
					m_vecInfo[idx+i] = *pInfo;
				}
			}
			else
			{
				if( idx > m_vecInfo.size())
				{
					throw CGUIException("failed in [CGUIStringEx::InsertStringInfo], the size of info is %d, the insert position is %d",m_vecInfo.size(), idx);
				}
				else
				{
					m_vecInfo.insert(m_vecInfo.begin()+idx,count, *pInfo);
				}
			}
		}
		else
		{
			//default info
			if( !m_vecInfo.empty())
			{
				if( idx > m_vecInfo.size())
				{
					throw CGUIException("failed in [CGUIStringEx::InsertStringInfo], the size of info is %d, the insert position is %d",m_vecInfo.size(), idx);
				}
				else
				{
					m_vecInfo.insert(m_vecInfo.begin()+idx,count, m_aDefaultInfo);
				}
			}
		}
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::ReplaceStringInfo(uint32 idx, uint32 count, const CGUIStringInfo* pInfo)
	{
		if( !pInfo)
		{
			return;
		}
		else
		{
			if( idx+count > m_vecInfo.size())
			{
				throw CGUIException("failed in [CGUIStringEx::ReplaceStringInfo], the size of info is %d, the insert position is %d, the count is %d",
					m_vecInfo.size(), idx, count);
			}
			else
			{
				for( uint32 i=0; i<count; ++i)
				{
					m_vecInfo[i+idx] = *pInfo;
				}
			}
		}
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Replace(uint32 idx, wchar_t cChar,uint32 nCount, const CGUIStringInfo* pInfo)
	{
		if( idx+nCount > Size() )
		{
			throw CGUIException("failed in [CGUIStringEx::Replace], the size of string is %d, the replace position is %d, count is %d",Size(), idx, nCount);
		}

		m_strContent.replace(idx, nCount,nCount, cChar);
		ReplaceStringInfo(idx, nCount, pInfo);
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Insert( uint32 idx, wchar_t cChar,uint32 nCount, const CGUIStringInfo* pInfo)
	{
		if( idx > Size() )
		{
			throw CGUIException("failed in [CGUIStringEx::Insert], the size of string is %d, the insert position is %d",Size(), idx);
		}

		m_strContent.insert(idx, nCount, cChar);
		InsertStringInfo(idx, nCount, pInfo);
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Insert( uint32 idx, const wchar_t* pChar, const CGUIStringInfo* pInfo)
	{
		if( idx > Size())
		{
			throw CGUIException("failed in [CGUIStringEx::Insert], the size of info is %d, the insert position is %d",Size(), idx);
		}

		m_strContent.insert(idx, pChar);
		InsertStringInfo(idx, wcslen(pChar), pInfo);
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Insert( uint32 idx, const wchar_t* pChar,uint32 nCount, const CGUIStringInfo* pInfo )
	{
		if( idx > Size())
		{
			throw CGUIException("failed in [CGUIStringEx::Insert], the size of info is %d, the insert position is %d",Size(), idx);
		}

		m_strContent.insert(idx, pChar, nCount);
		InsertStringInfo(idx, nCount, pInfo);
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Insert(uint32 idx, const CGUIStringEx& rString )
	{
		if( idx > Size())
		{
			throw CGUIException("failed in [CGUIStringEx::Insert], the size of info is %d, the insert position is %d",Size(), idx);
		}

		m_strContent.insert(idx, rString.m_strContent);
		if( rString.m_vecInfo.empty())
		{
			//default info
			InsertStringInfo(idx, rString.Size(), &rString.GetDefaultInfo());
		}
		else
		{
			for( uint32 i=0; i<rString.Size();++i)
			{
				InsertStringInfo( i+idx, 1, &rString.m_vecInfo[i] );
			}
		}
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Insert(uint32 idx, const CGUIString& rString, const CGUIStringInfo* pInfo,const char* szCode )
	{
		CGUIStringEx aString(rString, szCode);
		aString.SetDefaultInfo(pInfo);

		Insert(idx, aString);
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Erase(uint32 beginIdx, uint32 endIdx)
	{
		if( beginIdx > endIdx ||
			endIdx > Size())
		{
			throw CGUIException("failed in [CGUIStringEx::Erase], begin idx is <%d>, end idx is <%d>, size is <%d>",beginIdx,endIdx,Size() );
		}

		m_strContent.erase(m_strContent.begin() + beginIdx, m_strContent.begin()+endIdx);
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Append(const wchar_t cChar,uint32 nCount, const CGUIStringInfo* pInfo)
	{
		Insert(Size(), cChar, nCount, pInfo);
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Append(const wchar_t* pChar, const CGUIStringInfo* pInfo)
	{
		Insert(Size(), pChar, pInfo);
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Append(const wchar_t* pChar, uint32 nCount, const CGUIStringInfo* pInfo )
	{
		Insert(Size(), pChar,nCount, pInfo);
	}
	//------------------------------------------------------------------------------ 
	void		CGUIStringEx::Assign(const CGUIStringEx& rString)
	{
		Clear();
		SetDefaultInfo(&rString.GetDefaultInfo());
		if( !rString.Empty())
		{
			Insert(0, rString);
		}
	}
	//------------------------------------------------------------------------------ 
	const CGUIStringEx&	CGUIStringEx::operator=(const CGUIStringEx& rStr)
	{
		Assign(rStr);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	const CGUIStringEx&	CGUIStringEx::operator+=(const wchar_t* pChar)
	{
		Append( pChar);
		return *this;
	}
	//------------------------------------------------------------------------------ 
	bool	CGUIStringEx::operator<(const CGUIStringEx& rStr) const
	{
		return m_strContent<rStr.m_strContent;
	}
	//------------------------------------------------------------------------------ 
	bool	CGUIStringEx::operator>(const CGUIStringEx& rStr) const
	{
		return m_strContent>rStr.m_strContent;
	}
	//------------------------------------------------------------------------------ 
	bool	CGUIStringEx::operator==(const CGUIStringEx& rStr) const
	{
		return m_strContent==rStr.m_strContent;
	}
	//------------------------------------------------------------------------------ 
	wchar_t		CGUIStringEx::GetCharacter(uint32 idx) const
	{
		if( idx > Size())
		{
			throw CGUIException("failed in [CGUIStringEx::GetCharacter], the size of info is %d, the insert position is %d",Size(), idx);
		}
		return m_strContent[idx];
	}
	//------------------------------------------------------------------------------ 
	const CGUIStringInfo&		CGUIStringEx::GetInfo(uint32 idx) const
	{
		if( m_vecInfo.empty())
		{
			return m_aDefaultInfo;
		}
		else
		{
			if( idx > m_vecInfo.size())
			{
				throw CGUIException("failed in [CGUIStringEx::GetCharacter], the size of info is %d, the insert position is %d", m_vecInfo.size(), idx);
			}
			return m_vecInfo[idx];
		}
	}
	//------------------------------------------------------------------------------ 

}	//namespace guiex


