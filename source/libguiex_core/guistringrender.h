/** 
 * @file guistring.h
 * @brief string used in this system, which could convert 
 * string code automatically
 * @author ken
 * @date 2006-04-09
 */

#ifndef __GUI_STRING_RENDER_20060409_H__
#define __GUI_STRING_RENDER_20060409_H__

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

	class GUIEXPORT CGUIStringRenderInfo
	{	
	public:
		CGUIStringRenderInfo();
		CGUIStringRenderInfo(uint16 uFontID, real fFontScale,const CGUIColor& rColor);
		CGUIStringRenderInfo(const CGUIStringRenderInfo& rInfo);

		bool operator==(const CGUIStringRenderInfo& rInfo ) const;
		bool operator!=(const CGUIStringRenderInfo& rInfo ) const;
		const CGUIStringRenderInfo& operator=(const CGUIStringRenderInfo& rInfo );

	public:
		uint16 m_uFontID;
		real m_fFontScale;
		CGUIColor m_aColor;
	};

	class GUIEXPORT CGUIStringRender
	{
	public:
		CGUIStringRender( );
		CGUIStringRender( const CGUIStringRender& rString );
		CGUIStringRender( const CGUIStringW& str );
		CGUIStringRender( const CGUIString& rString );
		~CGUIStringRender( );

		void SetStringInfo(const CGUIStringRenderInfo* pInfo);
		const CGUIStringRenderInfo& GetStringInfo() const;
		CGUIStringRenderInfo& GetStringInfo();

		void SetContent( const CGUIStringW& rContent);
		const CGUIStringW& GetContent( ) const;
		CGUIStringW& GetContent( );

		const CGUIStringRender& operator=(const CGUIStringRender& rOther );

	public:
		CGUIStringRenderInfo m_aStringInfo;
		CGUIStringW	m_strContent;
	};

}//namespace guiex


#endif //__GUI_STRING_RENDER_20060409_H__

