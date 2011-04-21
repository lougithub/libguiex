/** 
* @file guifont_dummy.h
* @brief use freetype2 as font engine
* @author ken
* @date 2010-11-21
*/

#ifndef __GUI_FONT_DUMMY_20101121_H__
#define __GUI_FONT_DUMMY_20101121_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guirect.h>
#include <libguiex_core/guivector2.h>
#include "guifontdata_dummy.h"

#include <map>
#include <vector>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIImage;
}



//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/**
	* @class IGUIFont_dummy
	* @brief use libfreetype2 as font engine,
	* support all font file type that libfreetye2 supports
	*/
	class GUIEXPORT IGUIFont_dummy : public IGUIInterfaceFont
	{
	public:
		IGUIFont_dummy();
		virtual ~IGUIFont_dummy();

		virtual CGUIFontData* CreateFontData( 
			const CGUIString& rName, 
			const CGUIString& rSceneName,
			const SFontInfo& rFontInfo );
		virtual void DestroyFontData( CGUIFontData* pData );

		virtual void EnableKerning( bool bEnable );

		virtual void DrawCharacter(
			IGUIInterfaceRender* pRender, 
			wchar_t charCode, 
			const CGUIStringRenderInfo& rInfo,
			const CGUIVector2& rPos,
			real fAlpha);

		virtual void DrawString(
			IGUIInterfaceRender* pRender, 
			const CGUIStringRender& rString, 
			const CGUIVector2& rPos,
			real fAlpha,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		virtual void DrawString(
			IGUIInterfaceRender* pRender, 
			const CGUIStringRender& rString, 
			const CGUIRect&	rStringRect,
			ETextAlignmentHorz uTextAlignmentHorz,
			ETextAlignmentVert uTextAlignmentVert,
			real fAlpha,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		virtual real GetFontHeight( const CGUIStringRenderInfo& rInfo );
		virtual CGUISize GetCharacterSize( wchar_t charCode, const CGUIStringRenderInfo& rInfo );

		virtual void DeleteSelf();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

		CGUIFontData_dummy* GetFontData( uint16 uFontID );

	protected:
		typedef std::vector<CGUIFontData_dummy*> TVecFontData;
		TVecFontData m_arrayFontDatas;

	public: 
		static const char* StaticGetModuleName();
	};	

	GUI_INTERFACE_DECLARE();
}//namespace guiex

#endif //__GUI_FONT_DUMMY_20101121_H__
