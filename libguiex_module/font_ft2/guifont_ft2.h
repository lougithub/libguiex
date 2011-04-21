/** 
* @file guifont_ft2.h
* @brief use freetype2 as font engine
* @author ken
* @date 2006-08-06
*/

#ifndef __GUI_FONT_FT2_20060806_H__
#define __GUI_FONT_FT2_20060806_H__

//============================================================================//
// include
//============================================================================// 
#include "guifontdata_ft2.h"

#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guirect.h>
#include <libguiex_core/guivector2.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <vector>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIImage;
	class CGUIFontFace_ft2;
}



//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/**
	* @class IGUIFont_ft2
	* @brief use libfreetype2 as font engine,
	* support all font file type that libfreetye2 supports
	*/
	class GUIEXPORT IGUIFont_ft2 : public IGUIInterfaceFont
	{
	public:
		IGUIFont_ft2();
		virtual ~IGUIFont_ft2();

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

		FT_Library& GetFTLibrary();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

		CGUIFontData_ft2* GetFontData( uint16 uFontID );

		/**
		* @brief get kerning between two character
		*/
		int32 GetKerningGap(
			uint16 uFontID, 
			SCharData_ft2* pLeftData, 
			SCharData_ft2* pRightData,
			uint32 nSize);

	protected:
		FT_Library m_pFtLib;

		typedef std::vector<CGUIFontData_ft2*> TVecFontData;
		TVecFontData m_arrayFontDatas;
		typedef std::map<CGUIString, CGUIFontFace_ft2*> TMapFontFace;
		TMapFontFace m_mapFontFace;

		bool m_bEnableKerning;	/// flag whether enable kerning

	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();
}//namespace guiex

#endif //__GUI_FONT_FT2_20060806_H__
