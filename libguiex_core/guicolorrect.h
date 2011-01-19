/** 
* @file guicolorrect.h
* @brief color rect used for render texture
* @author ken
* @date 2006-07-05
*/

#ifndef __GUI_COLORRECT_20060424_H__
#define __GUI_COLORRECT_20060424_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guicolor.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIColorRect
	{
	public:
		CGUIColorRect(void);
		CGUIColorRect(const CGUIColor& col);
		CGUIColorRect(const CGUIColor& top_left, const CGUIColor& top_right, const CGUIColor& bottom_left, const CGUIColor& bottom_right);

		void SetAlpha(real alpha);
		void SetColours(const CGUIColor	& col);

		bool IsMonochromatic() const;

		CGUIColorRect GetSubRectangle( real left, real right, real top, real bottom ) const;

		CGUIColor GetColorAtPoint( real x, real y ) const;

		void ModulateAlpha(real alpha);

		//CGUIColorRect& operator*=(const CGUIColorRect& other);

	public:
		CGUIColor m_top_left, m_top_right, m_bottom_left, m_bottom_right;		//<! CGUIColorRect component colours
	};

}//namespace guiex

#endif //__GUI_COLORRECT_20060424_H__
