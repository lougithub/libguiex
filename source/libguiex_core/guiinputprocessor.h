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
	class CGUISystem;
	class IGUIInterfaceKeyboard;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIMouseListener
	{
	public:
		CGUIMouseListener()
		{
		}
		virtual ~CGUIMouseListener()
		{
		}

		virtual void OnMouseButtonDown( const IGUIInterfaceMouse::SMouseEvent& rMouseEvent )
		{
		}
		virtual void OnMouseButtonUp( const IGUIInterfaceMouse::SMouseEvent& rMouseEvent )
		{
		}
		virtual void OnMouseMove(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent)
		{
		}
	};

	class GUIEXPORT CGUIInputProcessor
	{
	public:
		CGUIInputProcessor();
		~CGUIInputProcessor();

		void Reset();

		bool ProcessMouse(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);
		bool ProcessKeyboard( const IGUIInterfaceKeyboard::SKeyEvent& rKeyEvent );

		void AddMouseListener( CGUIMouseListener* pListener );
		void ClearMouseListener(CGUIMouseListener* pListener );
		void ClearAllMouseListener( );

	protected:
		bool OnMouseButtonDown(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);
		bool OnMouseButtonUp(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);
		bool OnMouseWheelChange(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);
		bool OnMouseMove(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);
		
		void BeginDrag(CGUIWidget* pWidget, const CGUIVector2& rDeltaPos, uint32 nButton );
		void EndDrag( );

	protected:
		CMouseTracker* m_pMouseTracker; ///tracker used for mouse
		CDragTracker* m_pDragTracker; ///tracker used for drag
		real m_fDbClickTimeout; ///timeout of 

		std::vector<CGUIMouseListener*> m_vecMouseListener;
	};
}//namespace guiex

#endif //__GUI_WIDGETINPUTPROCESSOR_20070524_H__
