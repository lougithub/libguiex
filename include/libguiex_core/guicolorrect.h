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
// declare
//============================================================================// 
namespace guiex
{

}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	class GUIEXPORT CGUIColorRect
	{
	public:
		/**
		* @brief Default constructor
		*/
		CGUIColorRect(void);


		/**
		* @brief Constructor for CGUIColorRect objects (via single colour).
		*/
		CGUIColorRect(const CGUIColor& col);


		/**
		* @brief Constructor for CGUIColorRect objects
		*/
		CGUIColorRect(const CGUIColor& top_left, const CGUIColor& top_right, 
			const CGUIColor& bottom_left, const CGUIColor& bottom_right);


		/**
		* @brief Set the alpha value to use for all four corners of the CGUIColorRect.
		*/
		void	SetAlpha(real alpha);


		/** 
		* @brief Set the colour of all four corners simultaneously.
		*/
		void	SetColours(const CGUIColor	& col);

		/**
		* @brief Determinate whether the CGUIColorRect is monochromatic or variegated.
		*/
		bool	IsMonochromatic() const;


		/**
		* @brief Gets a portion of this CGUIColorRect as a subset CGUIColorRect
		* @return A CGUIColorRect from the specified range
		*/
		CGUIColorRect GetSubRectangle( real left, real right, real top, real bottom ) const;

		/**
		* @brief Get the colour at a point in the rectangle
		*/
		CGUIColor GetColorAtPoint( real x, real y ) const;


		/**
		* @brief Module the alpha components of each corner's colour by a constant.
		*/
		void	ModulateAlpha(real alpha);

		/** 
		* @brief Modulate all components of this colour rect with corresponding components from another colour rect.
		*/
		CGUIColorRect& operator*=(const CGUIColorRect& other);


	public:
		CGUIColor	m_top_left, m_top_right, m_bottom_left, m_bottom_right;		//<! CGUIColorRect component colours

	};

}//namespace guiex

#endif //__GUI_COLORRECT_20060424_H__
