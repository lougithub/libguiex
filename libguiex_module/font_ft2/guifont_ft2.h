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
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guirect.h>
#include <libguiex_core/guivector2.h>
#include "guifontdata_ft2.h"

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
	* @class IGUIFont_ft2
	* @brief use libfreetype2 as font engine,
	* support all font file type that libfreetye2 supports
	*/
	class GUIEXPORT IGUIFont_ft2 : public IGUIInterfaceFont
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIFont_ft2();

		/**
		* @brief destructor
		*/
		virtual ~IGUIFont_ft2();

		virtual CGUIFontData* CreateFontData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontID );
		virtual void DestroyFontData( CGUIFontData* pData );

		/**
		* @brief enable kerning
		*/
		virtual void EnableKerning( bool bEnable );

		/**
		* @brief draw a character
		*/
		virtual void DrawCharacter(
			IGUIInterfaceRender* pRender, 
			wchar_t charCode, 
			const CGUIStringInfo& rInfo,
			const CGUIVector2& rPos,
			real fAlpha);

		/**
		* @brief draw string
		*/
		virtual void DrawString(
			IGUIInterfaceRender* pRender, 
			const CGUIStringEx& rString, 
			const CGUIVector2& rPos,
			real fAlpha,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		/**
		* @brief draw string
		*/
		virtual void DrawString(
			IGUIInterfaceRender* pRender, 
			const CGUIStringEx& rString, 
			const CGUIRect&	rStringRect,
			ETextAlignmentHorz uTextAlignmentHorz,
			ETextAlignmentVert uTextAlignmentVert,
			real fAlpha,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		virtual const CGUISize& GetCharacterSize(int32 nFontFaceIdx, wchar_t charCode, uint32 nSize);

		virtual void DeleteSelf();

		FT_Library& GetFTLibrary();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

		/**
		* @brief get font by given parameter, if failed to find it,
		* create this font.
		*/
		CGUICharData_ft2* GetFont( uint32 nFontFaceIdx, wchar_t charCode,uint32 nSize );

		/// load font from file
		CGUICharData_ft2* LoadFont( CGUIFontData_ft2* pFontFace, CGUICharsData_ft2* pFont,wchar_t charCode,uint32 nSize);

		/**
		* @brief get kerning between two character
		*/
		int32 GetKerningGap(
			uint32 nFontFaceIdx, 
			CGUICharData_ft2* pLeftData, 
			CGUICharData_ft2* pRightData,
			uint32 nSize);

	protected:
		FT_Library m_pFtLib;

		typedef std::vector<CGUIFontData_ft2*> TVecFontData;
		TVecFontData m_arrayFontDatas;

		bool m_bEnableKerning;	/// flag whether enable kerning

	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();
}//namespace guiex

#endif //__GUI_FONT_FT2_20060806_H__
