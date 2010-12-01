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
		CGUIMouseCursor();
		~CGUIMouseCursor();

		static CGUIMouseCursor* Instance();

		void SetCursor(const CGUIString& rCursorName);
		CGUIString GetCursor() const;

		void Render(IGUIInterfaceRender* pRender);

		void SetPosition(const CGUIVector2& rPoint);

		void SetVisible( bool bVisible);
		bool IsVisible( ) const;

	protected:

		typedef std::map<CGUIString, std::pair<CGUIAnimation*,CGUIVector2> >	TMapCursor;
		TMapCursor	m_aMapCursor;	//!< image info map, key is mouse image name

		CGUIAnimation*	m_pAnimation; //!< current animation
		CGUIVector2 m_aCurrentTagPoint;	//!< current tag point

		CGUIVector2 m_aMousePoint; //!< mouse's point

		bool m_bVisible; //!< whether cursor will be shown or not
		CGUIString m_strCurrentCursor; //!< the name of current cursor	

	private:
		static CGUIMouseCursor* m_pSingleton;
	};


}//namespace guiex


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	/**
	* @brief get mouse cursor
	*/
	GUIEXPORT CGUIMouseCursor* GetMouseCursor();

}//namespace guiex



#endif	//__GUI_MOUSECURSOR_H_20060718__
