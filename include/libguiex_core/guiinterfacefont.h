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


		/**
		* @brief get string width
		*/
		real GetStringWidth(
			const CGUIStringEx& rString,
			int32 nStartPos = 0,
			int32 nEndPos = -1);

		/**
		* @brief get string size
		*/
		virtual const CGUISize& GetCharacterSize(int32 nFontFaceIdx,
			wchar_t charCode,
			uint32 nSize) = 0;

		//!< release all used resource, such as texture and image
		virtual void	ReleaseAllResource() = 0;


		/**
		* @brief load font face from file
		* @param nIdx index of font face. this interface should support
		* more than one font face.
		* @return 0 for successful, vice versa
		*/
		virtual int32			LoadFontFace( const CGUIString& rFilePathName, int32 nIdx ) = 0;

		/**
		* @brief unload font face
		*/
		virtual int32			UnloadFontFace( int32 nIdx ) = 0;

		/**
		* @brief enable kerning
		*/
		virtual void			EnableKerning( bool bEnable ) = 0;

		/**
		* @brief draw a character
		*/
		virtual void DrawCharacter(IGUIInterfaceRender* pRender, 
			wchar_t charCode, 
			const CGUIStringExInfo& rInfo,
			const CGUIVector2& rPos,
			const CGUISize& rScale,
			real	fAlpha,
			const CGUIRect* pClipRect= NULL) = 0;

		/**
		* @brief draw a character without setting clip rect
		*/
		virtual void DrawCharacterWithoutScissor(IGUIInterfaceRender* pRender, 
			wchar_t charCode, 
			const CGUIStringExInfo& rInfo,
			const CGUIVector2& rPos,
			const CGUISize& rScale,
			real	fAlpha) = 0;

		/**
		* @brief draw string
		*/
		virtual void DrawString(IGUIInterfaceRender* pRender, 
			const CGUIStringEx& rString, 
			const CGUIVector2& rPos,
			const CGUISize& rScale,
			real			fAlpha,
			const CGUIRect* pClipRect = NULL,
			int32 nStartPos = 0,
			int32 nEndPos = -1)=0;

		/**
		* @brief draw string
		*/
		virtual void DrawString(IGUIInterfaceRender* pRender, 
			const CGUIStringEx& rString, 
			const CGUIRect&	rStringRect,
			const uint8&	uTextAlignment,
			const CGUISize& rScale,
			real			fAlpha,
			const CGUIRect* pClipRect = NULL,
			int32 nStartPos = 0,
			int32 nEndPos = -1)=0;
	};

}//namespace guiex



#endif //__GUI_INTERFACE_FONT_20060805_H_


