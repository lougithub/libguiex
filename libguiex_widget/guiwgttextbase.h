/** 
* @file guiwgttextbase.h
* @brief base class for widget who want to use text.
* @author ken
* @date 2011-04-01
*/

#ifndef __GUI_WGTTEXTBASE_20110401_H__
#define __GUI_WGTTEXTBASE_20110401_H__


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
	* @class CGUIWgtTextBase
	*/
	class GUIEXPORT CGUIWgtTextBase : public CGUIWidget
	{
	public:
		virtual void SetTextContent(const CGUIStringW& rText);
		virtual void SetTextInfo(const CGUIStringInfo& rInfo);

		virtual void SetTextContentUTF8( const CGUIString& rString);
		virtual CGUIString GetTextContentUTF8( ) const;

		virtual void SetTextAlignmentVert( ETextAlignmentVert eAlignment );
		virtual void SetTextAlignmentHorz( ETextAlignmentHorz eAlignment );

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

		const CGUIStringW& GetTextContent( ) const;
		const CGUIStringInfo& GetTextInfo( ) const;

		bool IsTextContentEmpty() const;


		void SetTextColor(const CGUIColor& rColor );

		ETextAlignmentHorz GetTextAlignmentHorz( ) const;
		ETextAlignmentVert GetTextAlignmentVert( ) const;

	protected:
		CGUIWgtTextBase( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		void InitTextBase();

	protected:
		//text
		CGUIStringEx m_strText;
		ETextAlignmentHorz m_eTextAlignmentHorz;
		ETextAlignmentVert m_eTextAlignmentVert;
	};


}//namespace guiex

#endif //__GUI_WGTTEXTBASE_20110401_H__

