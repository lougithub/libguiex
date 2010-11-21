/** 
* @file guirender_cocoa.cpp
* @brief use freetype2 as font engine
* @author ken
* @date 2010-11-10
*/



//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/font_cocoa/guifont_cocoa.h>
#include <libguiex_module/font_cocoa/guifontdata_cocoa.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiimagemanager.h>
#include <libguiex_core/guitextureimp.h>
#include <libguiex_core/guitexture.h>
#include <libguiex_core/guitexturemanager.h>
#include <libguiex_core/guicolorrect.h>
#include <libguiex_core/guisize.h>
#include <libguiex_core/guivector2.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <libguiex_core/guiperfmonitor.h>
#include <libguiex_core/guisceneinfo.h>
#include <libguiex_core/guisceneinfomanager.h>

#include <algorithm>


//============================================================================//
// define
//============================================================================// 
#define GUI_FT2_TEXTURE_SIZE	512

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIFont_cocoa);
	//------------------------------------------------------------------------------
	IGUIFont_cocoa::IGUIFont_cocoa()
	{
	}
	//------------------------------------------------------------------------------
	IGUIFont_cocoa::~IGUIFont_cocoa()
	{
	}
	//------------------------------------------------------------------------------
	void IGUIFont_cocoa::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	int IGUIFont_cocoa::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIFont_cocoa::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	void	IGUIFont_cocoa::EnableKerning( bool bEnable )
	{
	}
	//------------------------------------------------------------------------------
	void IGUIFont_cocoa::DrawCharacter(
		IGUIInterfaceRender* pRender, 
		const CGUIMatrix4& rWorldMatrix,
		wchar_t charCode, 
		const CGUIStringInfo& rInfo,
		const CGUIVector2& rPos,
		real fAlpha)
	{
	
	}
	//------------------------------------------------------------------------------
	void IGUIFont_cocoa::DrawString(IGUIInterfaceRender* pRender, 
		const CGUIMatrix4& rWorldMatrix,
		const CGUIStringEx& rString, 
			const CGUIRect&	rStringRect,
			const uint8&	uTextAlignment,
			real			fAlpha,
			int32 nStartPos,
			int32 nEndPos)
	{
	
	}
	//------------------------------------------------------------------------------
	void IGUIFont_cocoa::DrawString(IGUIInterfaceRender* pRender, 
		const CGUIMatrix4& rWorldMatrix,
		const CGUIStringEx& rString, 
		const CGUIVector2& rPos,
		real fAlpha,
		int32 nStartPos,
		int32 nEndPos)
	{

	}
	//------------------------------------------------------------------------------
	const CGUISize& IGUIFont_cocoa::GetCharacterSize(int32 nFontFaceIdx,wchar_t charCode,uint32 nSize)
	{
		static CGUISize aSize;
		aSize.SetValue( nSize, nSize );
		return aSize;
	}
	//------------------------------------------------------------------------------
	CGUIFontData* IGUIFont_cocoa::CreateFontData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontIndex )
	{
		return new CGUIFontData_cocoa(rName, rSceneName, rPath, nFontIndex);
	}
	//------------------------------------------------------------------------------
	void IGUIFont_cocoa::DestroyFontData( CGUIFontData* pData )
	{
		delete pData;
	}
	//------------------------------------------------------------------------------
}//guiex

