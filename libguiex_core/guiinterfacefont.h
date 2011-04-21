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
#include "guifontdata.h"


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUISize;
	class CGUIRect;
	class IGUIInterfaceRender;
	class CGUIMatrix4;
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
		IGUIInterfaceFont( const char* szModuleName );

		/**
		* @brief destructor
		*/
		virtual ~IGUIInterfaceFont();

		virtual CGUIFontData* CreateFontData( 
			const CGUIString& rName, 
			const CGUIString& rSceneName,
			const SFontInfo& rFontInfo ) = 0;
		virtual void DestroyFontData( CGUIFontData* pData ) = 0;

		/**
		* @brief get string width
		*/
		real GetStringWidth( const CGUIStringRender& rString, int32 nStartPos = 0, int32 nEndPos = -1);

		virtual real GetFontHeight( const CGUIStringRenderInfo& rInfo ) = 0;

		/**
		* @brief get string size
		*/
		virtual CGUISize GetCharacterSize( wchar_t charCode, const CGUIStringRenderInfo& rInfo ) = 0;

		/**
		* @brief enable kerning
		*/
		virtual void EnableKerning( bool bEnable ) = 0;

		/**
		* @brief draw a character
		* @param rPos left-bottom position
		*/
		virtual void DrawCharacter(IGUIInterfaceRender* pRender, 
			wchar_t charCode, 
			const CGUIStringRenderInfo& rInfo,
			const CGUIVector2& rPos,
			real fAlpha) = 0;

		/**
		* @brief draw string
		* @param rPos left-bottom position
		*/
		virtual void DrawString(IGUIInterfaceRender* pRender, 
			const CGUIStringRender& rString, 
			const CGUIVector2& rPos,
			real fAlpha,
			int32 nStartPos = 0,
			int32 nEndPos = -1)=0;

		/**
		* @brief draw string
		*/
		virtual void DrawString(IGUIInterfaceRender* pRender, 
			const CGUIStringRender& rString, 
			const CGUIRect&	rStringRect,
			ETextAlignmentHorz uTextAlignmentHorz,
			ETextAlignmentVert uTextAlignmentVert,
			real fAlpha,
			int32 nStartPos = 0,
			int32 nEndPos = -1)=0;

	public: 
		static const char* StaticGetModuleType();
	};

}//namespace guiex



#endif //__GUI_INTERFACE_FONT_20060805_H_


