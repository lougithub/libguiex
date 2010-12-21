/** 
* @file guiwgtprogress.h
* @brief progress bar
* @author ken
* @date 2006-09-25
*/



#ifndef __GUI_WGTPROGRESS_20060924_H__
#define __GUI_WGTPROGRESS_20060924_H__

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
	* @class CGUIWgtProgress
	* @brief button class.
	* used image name:
	*		- BACKGROUND_IMG
	*		- FOREGROUND_IMG
	*/
	class GUIEXPORT CGUIWgtProgress : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtProgress( const CGUIString& rName, const CGUIString& rSceneName );

		/**
		* @brief set maximum value of progress
		*/
		void SetMaximumProgress( real nMaxmium );

		/**
		* @brief get maximum value of progress
		*/
		real GetMaximumProgress() const;

		/**
		* @brief set current value of progress
		*/
		void SetCurrentProgress( real fValue );

		/**
		* @brief set current value of progress
		*/
		real GetCurrentProgress( ) const;


	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtProgress( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitProgress();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnCreate();
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:	//!< callback function


	protected:
		const CGUIImage* m_pImageBg; ///< background
		const CGUIImage* m_pImageFg; ///< foreground

		real m_fMaximumValue; ///< maximum value of progress
		real m_fCurrentValue; ///< current value of progress

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtProgress);
	};

}//namespace libguiex



#endif //__GUI_WGTPROGRESS_20060924_H__
