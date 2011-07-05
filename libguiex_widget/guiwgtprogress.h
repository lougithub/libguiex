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
	*		- background
	*		- foreground
	*		- border
	*		- spark
	*/
	class GUIEXPORT CGUIWgtProgress : public CGUIWidget
	{
	public:
		CGUIWgtProgress( const CGUIString& rName, const CGUIString& rSceneName );

		void SetMaxValue( real nMaxmium );
		real GetMaxValue() const;

		void SetCurrentValue( real fValue );
		real GetCurrentValue( ) const;


	protected:
		CGUIWgtProgress( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitProgress();

		virtual void RefreshSelf();
		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnCreate();
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:
		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

	protected:	//!< callback function

	protected:
		CGUIImage* m_pImageBg; ///< background
		CGUIImage* m_pImageFg; ///< foreground
		CGUIImage* m_pImageSpark; ///< spark
		CGUIImage* m_pImageBorder; ///< border

		real m_fMaximumValue; ///< maximum value of progress
		real m_fCurrentValue; ///< current value of progress

		int16 m_nBGAdjustLeft;
		int16 m_nBGAdjustRight;
		int16 m_nBGAdjustTop;
		int16 m_nBGAdjustBottom;
		CGUIRect m_aBgRenderArea;

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtProgress);
	};

}//namespace libguiex


#endif //__GUI_WGTPROGRESS_20060924_H__
