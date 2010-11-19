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

#include <map>





//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIImage;
	class CGUIFont_ft2;
	class CGUIFontData_ft2;
	class CFontFace;
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

		//!<
		virtual void ReleaseAllResource();

		/**
		* @brief load font face from file
		* @return 0 for successful, vice versa
		*/
		virtual int32 LoadFontFace( const CGUIString& rFilePathName, int32 nFontFaceIdx );

		/**
		* @brief unload font face
		*/
		virtual int32 UnloadFontFace( int32 nFontFaceIdx );

		/**
		* @brief enable kerning
		*/
		virtual void EnableKerning( bool bEnable );

		/**
		* @brief draw a character
		*/
		virtual void DrawCharacter(IGUIInterfaceRender* pRender, 
			const CGUIMatrix4& rWorldMatrix,
			wchar_t charCode, 
			const CGUIStringInfo& rInfo,
			const CGUIVector2& rPos,
			real fAlpha);

		/**
		* @brief draw string
		*/
		virtual void DrawString(IGUIInterfaceRender* pRender, 
			const CGUIMatrix4& rWorldMatrix,
			const CGUIStringEx& rString, 
			const CGUIVector2& rPos,
			real fAlpha,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		/**
		* @brief draw string
		*/
		virtual void DrawString(IGUIInterfaceRender* pRender, 
			const CGUIMatrix4& rWorldMatrix,
			const CGUIStringEx& rString, 
			const CGUIRect&	rStringRect,
			const uint8& uTextAlignment,
			real fAlpha,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		/**
		* @brief get string size
		*/
		virtual const CGUISize& GetCharacterSize(int32 nFontFaceIdx, wchar_t charCode, uint32 nSize);

		/**
		* @brief used to delete this object
		*/
		virtual void DeleteSelf();

	protected:
		/** 
		* @brief initialize render
		* @return 0 for success
		*/
		virtual int DoInitialize(void* );

		/** 
		* @brief destroy render
		* @return 0 for success
		*/
		virtual void DoDestroy();

		/**
		* @brief get font by given parameter, if failed to find it,
		* create this font.
		*/
		CGUIFontData_ft2* GetFont( int32 nFontFaceIdx, wchar_t charCode,uint32 nSize );

		/// load font from file
		CGUIFontData_ft2* LoadFont(CFontFace* pFontFace, CGUIFont_ft2* pFont,wchar_t charCode,uint32 nSize);

		/**
		* @brief get kerning between two character
		*/
		int32 GetKerningGap(int32 nFontFaceIdx, 
			CGUIFontData_ft2* pLeftData, 
			CGUIFontData_ft2* pRightData,
			uint32 nSize);

	protected:
		void* m_pFtLib;

		//map for font face
		typedef std::map<int32, CFontFace*> TMapFace;
		TMapFace m_mapFace;

		bool m_bEnableKerning;	/// flag whether enable kerning
	};

	GUI_INTERFACE_DECLARE();
}//namespace guiex

#endif //__GUI_FONT_FT2_20060806_H__
