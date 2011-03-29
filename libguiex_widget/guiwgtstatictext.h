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
#include <libguiex_core/guiwidget.h>
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
	class GUIEXPORT CGUIWgtStaticText : public CGUIWidget
	{
	public:
		CGUIWgtStaticText( const CGUIString& rName, const CGUIString& rSceneName );
	
		void SetTextContent(const CGUIStringW& rText);
		const CGUIStringW& GetTextContent( ) const;

		void SetTextInfo(const CGUIStringInfo& rInfo);
		const CGUIStringInfo& GetTextInfo( ) const;

		void SetTextContentUTF8( const CGUIString& rString);
		CGUIString GetTextContentUTF8( ) const;

		void SetTextColor(const CGUIColor& rColor );

		void SetTextAlignmentVert( ETextAlignmentVert eAlignment );
		void SetTextAlignmentHorz( ETextAlignmentHorz eAlignment );
		ETextAlignmentHorz GetTextAlignmentHorz( ) const;
		ETextAlignmentVert GetTextAlignmentVert( ) const;

		void SetMultiLine( bool bMultiLine );
		bool IsMultiLine( ) const;

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);


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
			uint32	m_nLineHeight;   //!< line height
		};
		typedef std::vector<SLineInfo>   TLineList;  //!< Type for collection of LineInfos.
		TLineList m_aLineList;			//!< line list
		bool m_bMultiLine;
	
		//text
		CGUIStringEx m_strText;
		ETextAlignmentHorz m_eTextAlignmentHorz;
		ETextAlignmentVert m_eTextAlignmentVert;

	protected:
		static wchar_t ms_wLineBreak;

		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtStaticText);
	};


}//namespace guiex

#endif //__GUI_WGTSTATICTEXT_20060806_H__

