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
#include <libguiex_widget/guiwgtstatic.h>
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
	class GUIEXPORT CGUIWgtStaticText : public CGUIWgtStatic
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtStaticText( const CGUIString& rName, const CGUIString& rProjectName );
	
		virtual int32 Create();
		virtual void SetTextContent(const wchar_t* pText);
		virtual void SetTextInfo(const CGUIStringInfo& rInfo);

		void SetMultiLine( bool bMultiLine );
		bool IsMultiLine( ) const;

		/**
		* @brief load widget config from property
		*/
		virtual int32 GenerateProperty( CGUIProperty& rProperty );

		/** 
		 * @brief process property
		 */
		virtual void ProcessProperty( const CGUIProperty& rProperty);


	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtStaticText( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		///initialize static
		void InitStaticText();

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		
		virtual void RefreshSelf( );

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
		TLineList			m_aLineList;			//!< line list
		bool				m_bMultiLine;
	
	protected:
		static		wchar_t		ms_wLineBreak;
		static		CGUIString	ms_strType;

	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtStaticText);

}//namespace guiex

#endif //__GUI_WGTSTATICTEXT_20060806_H__

