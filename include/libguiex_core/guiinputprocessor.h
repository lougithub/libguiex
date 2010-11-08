/** 
* @file guiinputprocessor.h
* @brief widget input processor
* @author ken
* @date 2007-05-24
*/


#ifndef __GUI_INPUTPROCESSOR_20070524_H__
#define __GUI_INPUTPROCESSOR_20070524_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guivector2.h"
#include "guiinterfacemouse.h"
#include "guiinterfacekeyboard.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWidget;
	class IGUIInterfaceMouse;
	class CMouseTracker;
	class CDragTracker;
	class CGUIWidgetSystem;
	class IGUIInterfaceKeyboard;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIInputProcessor
	{
	public:
		/** 
		 * @brief constructor
		 */
		CGUIInputProcessor();

		/** 
		* @brief destructor
		*/
		~CGUIInputProcessor();

		/** 
		 * @brief reset the input processor
		 */
		void	Reset();

		/** 
		 * @brief set system pointer
		 */
		void	SetSystem(CGUIWidgetSystem* pSystem);
		
		/**
		* @brief process mouse and generate relative event
		* @return whether this mouse event has been processed
		*/
		bool			ProcessMouse(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);

		/**
		* @brief process keyboard and generate relative event
		* @return whether this keyboard event has been processed
		*/
		bool			ProcessKeyboard( const IGUIInterfaceKeyboard::SKeyEvent& rKeyEvent );

	protected:
		/**
		* @brief respond to mouse button down event.
		*/
		bool			OnMouseButtonDown(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);

		/**
		* @brief respond to mouse button up event.
		*/
		bool			OnMouseButtonUp(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);

		/**
		* @brief respond to mouse wheel changed event
		*/
		bool			OnMouseWheelChange(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);

		/**
		* @brief respond to mouse move event
		*/
		bool			OnMouseMove(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);

		/**
		* @brief begin to drag
		*/
		void			BeginDrag(CGUIWidget* pWidget, const CGUIVector2& rDeltaPos, uint32 nButton );

		/**
		* @brief end to drag
		*/
		void			EndDrag( );


	protected:
		CMouseTracker*	m_pMouseTracker;			///tracker used for mouse
		CDragTracker*	m_pDragTracker;				///tracker used for drag
		real			m_fDbClickTimeout;			///timeout of 

		CGUIWidgetSystem*	m_pSystem;				//pointer of widget system

	private:

	};
}//namespace guiex

#endif //__GUI_WIDGETINPUTPROCESSOR_20070524_H__
