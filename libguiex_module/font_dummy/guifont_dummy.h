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
		/**
		* @brief constructor
		*/
		IGUIFont_dummy();

		/**
		* @brief destructor
		*/
		virtual ~IGUIFont_dummy();

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
			const CGUIMatrix4& rWorldMatrix,
			wchar_t charCode, 
			const CGUIStringInfo& rInfo,
			const CGUIVector2& rPos,
			real fAlpha);

		/**
		* @brief draw string
		*/
		virtual void DrawString(
			IGUIInterfaceRender* pRender, 
			const CGUIMatrix4& rWorldMatrix,
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
			const CGUIMatrix4& rWorldMatrix,
			const CGUIStringEx& rString, 
			const CGUIRect&	rStringRect,
			const uint8& uTextAlignment,
			real fAlpha,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		virtual const CGUISize& GetCharacterSize(int32 nFontFaceIdx, wchar_t charCode, uint32 nSize);

		virtual void DeleteSelf();

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

	public: 
		static const char* StaticGetModuleName();
	};	

	GUI_INTERFACE_DECLARE();
}//namespace guiex

#endif //__GUI_FONT_DUMMY_20101121_H__
