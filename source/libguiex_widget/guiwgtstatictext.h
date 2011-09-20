/** 
* @file guiwgtstatictext.h
* @brief used to show a static text
* @author ken
* @date 2006-08-06
*/

#ifndef __GUI_WGTSTATICTEXT_20060806_H__
#define __GUI_WGTSTATICTEXT_20060806_H__


//============================================================================//
// include
//============================================================================// 
#include "guiwgttextbase.h"
#include <libguiex_core/guiwidgetgenerator.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class CGUIWgtStaticText
	* @brief to show a static text
	*/
	class GUIEXPORT CGUIWgtStaticText : public CGUIWgtTextBase
	{
	public:
		CGUIWgtStaticText( const CGUIString& rName, const CGUIString& rSceneName );

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

		virtual void SetTextContent(const CGUIStringW& rText);
		virtual void SetTextInfo(const CGUIStringRenderInfo& rInfo);

		void SetMultiLine( bool bMultiLine );
		bool IsMultiLine( ) const;

		void SetClipText( bool bClip );
		bool IsClipText( ) const;
		
	protected:
		CGUIWgtStaticText( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		void InitStaticText();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void RefreshSelf( );
		virtual void OnCreate();

		void UpdateStringContent();

	protected:
		virtual uint32 OnSizeChanged( CGUIEventSize* pEvent );

	protected:
		//line break
		/**
		* @brief struct used to store information about a formatted line within the paragraph.
		*/
		struct SLineInfo
		{
			uint32	m_nStartIdx;     //!< Starting index for this line.
			uint32	m_nLength;       //!< Code point length of this line.
			real m_fLineHeight;   //!< line height
		};
		typedef std::vector<SLineInfo>   TLineList;  //!< Type for collection of LineInfos.
		TLineList m_aLineList;			//!< line list
		bool m_bMultiLine;
		bool m_bClipText;
	
	protected:
		static wchar ms_wLineBreak;

		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtStaticText);
	};


}//namespace guiex

#endif //__GUI_WGTSTATICTEXT_20060806_H__

