/** 
 * @file guimousecursor.h
 * @brief mouse cursor, used to draw mouse
 * @author ken
 * @date 2006-07-18
 */

#ifndef __GUI_MOUSECURSOR_H_20060718__
#define __GUI_MOUSECURSOR_H_20060718__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guisingleton.h"
#include "guiimage.h"
#include "guitimer.h"

#include <vector>
#include <map>
//============================================================================//
// class
//============================================================================// 
namespace guiex
{
class IGUIInterfaceRender;
class CGUIRect;
class CGUIAnimation;
}//namespace guiex

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
/**
 * @class CGUIException
 * @brief class base class of exception, used for guiex system..
 */
class GUIEXPORT CGUIMouseCursor
{
public:
	/**
	 * @brief destructor
	 */
	~CGUIMouseCursor();

	///**
	// * @brief add a image
	// * @param rTagPoint the point which used to align the image.
	// * it is a unified coordinate, from 0 to 1.
	// */
	//int32	AddCursor(const CGUIString& rCursorName, 
	//	const CGUIString& rImageName, 
	//	const CGUIRect& rTexRect = CGUIRect(0,0,1,1), 
	//	const CGUIVector2& rTagPoint=CGUIVector2(0,0) );
	//
	///**
	// * @brief add a animation, in this function, all frame are put in one image.
	// * use different part of image to show the animation
	// * @param nInterval interval time between two frame.in millisecond
	// * @param rImageName image name
	// * @param rListTexRect show which part of image will be considered as a frame of animation
	// */
	//int32	AddCursor( const CGUIString& rCursorName, 
	//	const CGUIString& rImageName, 
	//	const std::vector<CGUIRect>& rListTexRect, 
	//	uint32 nInterval,
	//	const CGUIVector2& rTagPoint=CGUIVector2(0,0));

	///**
	// * @brief add a animation, in this function, the frame are put in different image.
	// * @param nInterval interval time between two frame.in millisecond
	// * @param rListImageName list contains all image's name
	// */
	//int32	AddCursor( const CGUIString& rCursorName, 
	//	const std::vector<CGUIString>& rListImageName,  
	//	uint32 nInterval,
	//	const CGUIVector2& rTagPoint=CGUIVector2(0,0));

	///**
	// * @brief add a animation.
	// *	@param pAnimation animation of cursor
	// */
	////int32	AddCursor( const CGUIString& rCursorName, const CGUIAnimation& aAnimation );

	///**
	// * @brief remove cursor by given name.
	// */
	//void	RemoveCursor(const CGUIString& rCursorName);
	//
	///**
	// * @brief remove all cursors
	// */
	//void	RemoveAllCursor();

	///**
	// * @brief show the selected cursor by name
	// */
	void	SetCursor(const CGUIString& rCursorName);

	/**
	 * @brief get the name of current cursor
	 */
	CGUIString	GetCursor() const;

	/**
	 * @brief show the cursor one one time
	 */
	//void	SetCursorOneTime(const CGUIString& rCursorName);

	/**
	 * @brief render the mouse
	 */
	void	Render(IGUIInterfaceRender* pRender);

	/**
	 * @brief set cursor position
	 */
	void	SetPosition(const CGUIVector2& rPoint);
	
	/**
	 * @brief show or hide cursor
	 */
	void	SetSelfVisible( bool bVisible);

	/**
	 * @brief is cursor visible
	 */
	bool	IsVisible( ) const;

protected:
	/**
	 * @brief constructor
	 */
	CGUIMouseCursor();

	typedef std::map<CGUIString, std::pair<CGUIAnimation*,CGUIVector2> >	TMapCursor;
	TMapCursor	m_aMapCursor;	//!< image info map, key is mouse image name

	
	CGUIAnimation*	m_pAnimation;	//!< current animation
	CGUIVector2		m_aCurrentTagPoint;		//!< current tag point
	
	CGUIVector2		m_aMousePoint;			//!< mouse's point

	bool			m_bVisible;				//!< whether cursor will be shown or not
	CGUIString		m_strCurrentCursor;		//!< the name of current cursor	

	GUI_SINGLETON_DECLARE_EX(CGUIMouseCursor);
};


}//namespace guiex



namespace guiex
{
//============================================================================//
// function
//============================================================================// 
	/**
	 * @brief get mouse cursor
	 */
	GUIEXPORT	CGUIMouseCursor*	GetMouseCursor();

}//namespace guiex



#endif	//__GUI_MOUSECURSOR_H_20060718__
