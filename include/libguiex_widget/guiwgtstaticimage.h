/** 
* @file guiwgtstaticimage.h
* @brief used to show a static image
* @author ken
* @date 2006-07-19
*/

#ifndef __GUI_WGTSTATICIMAGE_20060719_H__
#define __GUI_WGTSTATICIMAGE_20060719_H__

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
	* @class CGUIWgtStaticImage
	* @brief to show a static image
	* used image name:
	*		-BGIMAGE
	*/
	class GUIEXPORT CGUIWgtStaticImage : public CGUIWgtStatic
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtStaticImage( const CGUIString& rName, const CGUIString& rProjectName );

		/** 
		* @brief set widget parameter's value
		* for example: SetValue("alpha", "0.5")
		*/
		virtual void SetValue(const CGUIString& rName, const CGUIString& rValue);

		/**
		* @brief get widget parameter's value
		* for example: GetValue("alpha_local") will return "0.5"
		*/
		virtual CGUIString	GetValue(const CGUIString& rName) const;

		/**
		* @brief select a image to show
		*/
		void SetCurrentImage( const CGUIString& rImageName);

		/**
		* @brief get image name which is showing now
		*/
		const CGUIString& GetCurrentImage(  ) const;

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtStaticImage( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		///initialize check button
		void InitStaticImage();

		/**
		* @brief override the OnSetImage function
		*/
		virtual void OnSetImage( const CGUIString& rName,CGUIImage* pImage );

	protected:
		CGUIImage*		m_pImageBG;
		CGUIString		m_strImageName;

	protected:
		static CGUIString ms_strType;

	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtStaticImage);

}//namespace guiex

#endif //__GUI_WGTSTATICIMAGE_20060719_H__

