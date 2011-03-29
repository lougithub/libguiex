/** 
* @file guiwgtstatic.h
* @brief base class, for static widget
* @author ken
* @date 2006-07-19
*/


#ifndef __GUI_WGTSTATIC_20060719_H__
#define __GUI_WGTSTATIC_20060719_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class CGUIWgtStatic
	* @brief base class for static widget
	*/
	class GUIEXPORT CGUIWgtStatic : public CGUIWidget
	{
	public:

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtStatic( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );


	protected:
		///initialize static
		void InitStatic();
	};

}//namespace guiex

#endif //__GUI_WGTSTATIC_20060719_H__
