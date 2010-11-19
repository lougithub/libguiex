/** 
 * @file guistring.h
 * @brief string used in this system, which could convert 
 * string code automatically
 * @author ken
 * @date 2006-04-09
 */

#ifndef __GUI_STRING_EX_20060409_H__
#define __GUI_STRING_EX_20060409_H__

//============================================================================//
// 	include
//============================================================================// 
#include "guibase.h"
#include "guicolor.h"
#include "guistring.h"
#include  <string>
#include  <vector>

//============================================================================//
// 	class
//============================================================================// 

namespace guiex
{

	class GUIEXPORT CGUIStringInfo
	{	

	public:
		CGUIStringInfo();
		CGUIStringInfo(int16 nFontIdx,uint16 nFontSize,const CGUIColor& rColor);
		CGUIStringInfo(const CGUIStringInfo& rInfo);

		bool operator==(const CGUIStringInfo& rInfo ) const;
		bool operator!=(const CGUIStringInfo& rInfo ) const;
		const CGUIStringInfo& operator=(const CGUIStringInfo& rInfo );

	public:
		int16 m_nFontIdx;
		uint16 m_nFontSize;
		CGUIColor m_aColor;
	};

	class GUIEXPORT CGUIStringEx
	{
	public:
		CGUIStringEx( );
		CGUIStringEx( const CGUIStringEx& rString );
		CGUIStringEx( const CGUIStringW& str );
		CGUIStringEx( const CGUIString& rString );
		~CGUIStringEx( );

		void SetStringInfo(const CGUIStringInfo* pInfo);
		const CGUIStringInfo& GetStringInfo() const;
		CGUIStringInfo& GetStringInfo();

		void SetContent( const CGUIStringW& rContent);
		const CGUIStringW& GetContent( ) const;
		CGUIStringW& GetContent( );

		const CGUIStringEx& operator=(const CGUIStringEx& rOther );

	public:
		CGUIStringInfo m_aStringInfo;
		CGUIStringW	m_strContent;
	};

}//namespace guiex


#endif //__GUI_STRING_EX_20060409_H__




