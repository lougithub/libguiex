/** 
* @file guiinterfacefont.h
* @brief interface for font
* @author ken
* @date 2006-08-05
*/


#ifndef __GUI_INTERFACE_FONT_20060805_H_
#define __GUI_INTERFACE_FONT_20060805_H_

//============================================================================//
// include
//============================================================================// 
#include "guiinterface.h"
#include "guivector2.h"
#include "guistringex.h"
#include "guicolor.h"
#include "guicolorrect.h"


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUISize;
	class CGUIRect;
	class IGUIInterfaceRender;
	class CGUIMatrix4;
	class CGUIFontData;
}



//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class IGUIInterfaceFont 
	* @brief interface of font
	*/
	class GUIEXPORT IGUIInterfaceFont : public IGUIInterface
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIInterfaceFont();

		/**
		* @brief destructor
		*/
		virtual ~IGUIInterfaceFont();

		virtual CGUIFontData* CreateFontData( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontIndex ) = 0;
		virtual void DestroyFontData( CGUIFontData* pData ) = 0;

		/**
		* @brief get string width
		*/
		real GetStringWidth( const CGUIStringEx& rString, int32 nStartPos = 0, int32 nEndPos = -1);

		/**
		* @brief get string size
		*/
		virtual const CGUISize& GetCharacterSize(int32 nFontFaceIdx, wchar_t charCode, uint32 nSize) = 0;

		/**
		* @brief enable kerning
		*/
		virtual void EnableKerning( bool bEnable ) = 0;

		/**
		* @brief draw a character
		*/
		virtual void DrawCharacter(IGUIInterfaceRender* pRender, 
			const CGUIMatrix4& rWorldMatrix,
			wchar_t charCode, 
			const CGUIStringInfo& rInfo,
			const CGUIVector2& rPos,
			real fAlpha) = 0;

		/**
		* @brief draw string
		*/
		virtual void DrawString(IGUIInterfaceRender* pRender, 
			const CGUIMatrix4& rWorldMatrix,
			const CGUIStringEx& rString, 
			const CGUIVector2& rPos,
			real fAlpha,
			int32 nStartPos = 0,
			int32 nEndPos = -1)=0;

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
			int32 nEndPos = -1)=0;
	};

}//namespace guiex



#endif //__GUI_INTERFACE_FONT_20060805_H_


