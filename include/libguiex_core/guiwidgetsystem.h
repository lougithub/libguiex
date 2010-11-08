/** 
* @file guiwidgetsystem.h
* @brief widget system
* @author ken
* @date 2006-06-21
*/


#ifndef __GUI_WIDGETSYSTEM_20060621_H__
#define __GUI_WIDGETSYSTEM_20060621_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guisingleton.h"
#include "guiinterfacemouse.h"
#include "guitimer.h"
#include "guistring.h"
#include "guirect.h"
#include "guievent.h"
#include "guiinputprocessor.h"
#include <map>
#include <set>
#include <list>
#include <vector>



//============================================================================//
// declare
//============================================================================// 
namespace guiex
{	
	class CGUIWidget;
	class CGUIEventKeyboard;
}

//============================================================================//
// enum
//============================================================================// 

//============================================================================//
// define
//============================================================================// 
#define GUI_CREATE_WIDGET(type, widgetname, projectname)		\
	::guiex::CGUIWidgetSystem::Instance()->CreateWidget(type,widgetname, projectname)

#define GUI_DESTROY_WIDGET(widget)		\
	::guiex::CGUIWidgetSystem::Instance()->DestroyWidget(widget)

#define GUI_GET_WIDGET(widgetname, projectname)		\
	::guiex::CGUIWidgetSystem::Instance()->GetWidget(widgetname, projectname)


#define GUI_WIDGET_SYSTEM( )		\
	::guiex::CGUIWidgetSystem::Instance()




//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWidgetSystem
	* @brief widget system
	*/
	class GUIEXPORT CGUIWidgetSystem
	{
	public:

		/**
		* @brief initialize system
		* @return 0 for success, vice versa
		*/
		int		Initialize();

		/**
		* @brief release system
		*/
		void	Release();

		/** 
		 * @brief has system been initialized
		 */
		bool	IsInitialized() const;

		/**
		* @brief Update system
		*/
		void	Update( real fDeltaTime );

		/**
		* @brief render system
		*/
		void	Render();

		void	SetDrawExtraInfo( bool bDraw );
		bool	IsDrawExtraInfo() const;

		/**
		* @brief get global time
		*/
		real GetGlobalTimer() const;

		/** 
		* @brief set data path of the ui system, the path should end by "/" or "\"
		*/
		void SetDataPath(const CGUIString& rDataPath);

		/** 
		* @brief get data path of the ui system, the path will end by "/" or "\"
		*/
		const CGUIString&	GetDataPath() const;


		/**
		* @brief create a widget by type and name
		* @return pointer of created widget
		*/
		CGUIWidget*		CreateWidget( const CGUIString& rType, const CGUIString& rWidgetName, const CGUIString& rProjectName );

		/**
		* @brief destroy widget
		*/
		void			DestroyWidget(CGUIWidget* pWidget);

		/**
		* @brief get widget by given name
		* @return return NULL for error
		*/
		CGUIWidget*		GetWidget(  const CGUIString& rWidgetName, const CGUIString& rProjectName );

		/**
		* @brief has widget which has given name
		* @return return true for has, vice versa
		*/
		bool			HasWidget(  const CGUIString& rWidgetName, const CGUIString& rProjectName );

		/**
		* @brief generate a anonymous name
		* @return return a string
		*/
		const char*		GenerateAnonymousName();

		/**
		* @brief set widget which has focus
		*/
		void			SetFocusWidget(CGUIWidget* pWidget);

		/** 
		* @brief get widget which has focus
		*/
		CGUIWidget*		GetFocusWidget();



		/** 
		* @brief set default multi-byte's code
		*/
		void			SetDefaultCode(const char* pDataPath);

		/** 
		* @brief get default multi-byte's code
		*/
		const char*		GetDefaultCode() const;

		//********************************************************
		//	page and dialog
		//********************************************************

		/**
		* @brief open a page
		*/
		void			OpenPage(CGUIWidget* pPage);

		/**
		* @brief close a page
		*/
		void			ClosePage(CGUIWidget* pPage);

		///< get opened page num, which should be opened by method OpenPage()
		uint32			GetOpenedPageNum() const;

		///< get opened page by index
		CGUIWidget*		GetOpenedPageByIndex( uint32 nIdx );

		/**
		* @brief add a page to map
		*/
		void			AddPage(  CGUIWidget* pPage, const CGUIString& rProjectName = CGUIString());

		/**
		* @brief get a page by name
		*/
		CGUIWidget*		GetPage( const CGUIString& rWidgetName, const CGUIString& rProjectName );

		/**
		* @brief get a page page's configure file name
		*/
		CGUIWidget*		GetPageByFilename( const CGUIString& rFilename, const CGUIString& rProjectName );

		/**
		* @brief is there a page whose name is given name
		*/
		bool			HasPage(const CGUIString& rWidgetName, const CGUIString& rProjectName) const;

		/**
		* @brief is there a page whose ptr is same as given page
		*/
		bool			HasPage( CGUIWidget* pPage) const;

		/**
		* @brief delete specify page
		*/
		void			DeletePage( CGUIWidget* pPage);


		/** 
		* @brief open modal dialog
		*/
		void			OpenDialog(CGUIWidget* pDlg);

		/** 
		* @brief get current modal dialog
		*/
		CGUIWidget*		GetTopestDialog( ) const;

		/** 
		* @brief close modal dialog
		*/
		void			CloseDialog(CGUIWidget* pDlg);

		/** 
		* @brief open popup widget
		*/
		void			OpenPopupWidget(CGUIWidget* pWidget);

		/** 
		* @brief get current modal dialog
		*/
		CGUIWidget*		GetCurrentPopupWidget( ) const;

		/** 
		* @brief close modal dialog
		*/
		void			ClosePopupWidget(CGUIWidget* pWidget);

		/** 
		* @brief get current root widget, will be a dialog if there has or a page
		*/
		CGUIWidget*		GetCurrentRootWidget( ) const;

		void			CloseAll();

		//********************************************************
		//	screen size
		//********************************************************

		/**
		* @brief set the size of screen
		*/
		void	SetScreenSize( uint32 width, uint32 height);

		/**
		* @brief get the size of screen, in default
		*/
		const CGUISize& GetScreenSize( ) const;
		uint32 GetScreenWidth( ) const;
		uint32 GetScreenHeight( ) const;

		/**
		* @brief get the position of screen, in default
		*/
		const CGUIVector2& GetScreenPos( ) const;

		/**
		* @brief get the rect of screen, in default
		*/
		const CGUIRect& GetScreenRect( ) const;

		/**
		* @brief get fps of system
		*/
		int32			GetFPS() const;

		/**
		* @brief get widget by position.
		* if the widget is disabled or invisible, this widget will be skipped
		* @return return the widget where the point hits,
		* NULL if there hasn't any point.
		*/
		CGUIWidget*		GetWidgetUnderPoint(const CGUIVector2& rPos);



		//********************************************************
		//	ui event
		//********************************************************

		/**
		* @brief register a event for this Widget.this widget should unregister
		* the event manually.
		*/
		void	RegisterUIEvent( const CGUIString& rEventName, CGUIWidget* pWidget);

		/**
		* @brief unregister a event for this Widget.
		*/
		void	UnregisterUIEvent( const CGUIString& rEventName, CGUIWidget* pWidget);

		/**
		* @brief unregister a event for this Widget.
		*/
		void	UnregisterUIEvent( CGUIWidget* pWidget);

		/**
		* @brief send a ui event.if there is no widget registered for this event, it
		* will be ignored.
		*/
		void	SendUIEvent(CGUIEventUI* pEvent );

		void	SendEvent(CGUIEvent* pEvent );

		/**
		* @brief unregister all ui event
		*/
		void	UnregisterAllUIEvent( );

		//********************************************************
		//	global key event
		//********************************************************
		/**
		* @brief register global key
		* @param pReceiver the widget which will receive the global key event
		* @pRoot the root widget of this receiver, is a root of page or a root of dialog
		*/
		void	RegisterGlobalKeyReceiver( CGUIWidget* pReceiver, CGUIWidget* pRoot);

		/** 
		* @brief remove key event by root widget
		*/
		void	UngisterGlobalKeyByRoot( CGUIWidget* pRoot);	

		/** 
		* @brief remove all key event
		*/
		void	UngisterAllGlobalKey( );

		/**
		* @brief read config file
		*/
		CGUIWidget*	LoadDynamicPage( const CGUIString& rPageFileName, const CGUIString& rPageProjectName, const CGUIString& rWorkingProjectName );

		/**
		* @brief read config file
		*/
		CGUIWidget*	LoadPage( const CGUIString& rFileName, const CGUIString& rProjectName);

		/**
		* @brief read resource config file
		*/
		int32	LoadResource( const CGUIString& rFileName, const CGUIString& rProjectName);

		/**
		* @brief free resource by project name, 
		* only free named image now.
		*/
		void	FreeResource( const CGUIString& rProjectName );

		/**
		*/
		void	FreeAllWidgets(  );

		void	UnloadAllResource();

		/**
		* @brief free resource by project name, 
		* only free named image now.
		*/
		void	FreeAllResources(  );

		//!< set whether should system run script
		void	SetRunScript( bool bShouldRun );

		//!< return should system run script
		bool	ShouldRunScript() const;

		/**
		* @brief process mouse and generate relative event
		* @return whether this mouse event has been processed
		*/
		bool	ProcessMouseInput(const IGUIInterfaceMouse::SMouseEvent& rMouseEvent);

		/**
		* @brief process keyboard and generate relative event
		* @return whether this keyboard event has been processed
		*/
		bool	ProcessKeyboardInput( const IGUIInterfaceKeyboard::SKeyEvent& rKeyEvent );

	protected:
		friend class CGUIWidget;
		friend class CGUIInputProcessor;


		/**
		* @brief get widget which has captured input
		*/
		CGUIWidget*		GetCapturedInput() const;

		/// update time and frame
		void			UpdateTime(real fDeltaTime);

		void			AddToGarbage( CGUIWidget* pWidget );
		void			RefreshGarbage( );


	protected:

		///process command
		void			ProcessCommand();

		/** 
		* @brief process the global key event
		* @return whether this event is comsumed
		*/
		bool			ProcessGlobalKeyEvent(CGUIEventKeyboard* pEvent);

	public:
		static uint32					GUI_SCREEN_WIDTH;		//!< size of screen, in pixel
		static uint32					GUI_SCREEN_HEIGHT;		//!< size of screen, in pixel

	protected:
		///constructor and destructor, for singleton use.
		CGUIWidgetSystem();
		~CGUIWidgetSystem();

		/**
		* @brief reset system
		*/
		void	Reset();

	private:
		//----------------------------------------------------------------------
		/// the root widget of widget tree
		CGUIWidget*		m_pWgtRoot;					///widget root
		CGUIWidget*		m_pWgtFocus;				///widget which has focus
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		/// screen size
		CGUIRect		m_aScreenRect;				//!< rect of screen, in default
		CGUISize		m_aScreenSize;				//!< size of screen, in default
		CGUIVector2		m_aScreenPos;				//!< pos of screen, in default
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//time related
		real			m_fGlobalTimer;				///global time
		real			m_fTimerForFrame;			///time used to calculate frame
		int32			m_aFrame;					///frame of system
		int32			m_nFps;						///current fps
		//----------------------------------------------------------------------

		CGUIInputProcessor	m_aInputProcessor;		///input processor

		//----------------------------------------------------------------------
		//widget list
		typedef std::map<CGUIString , std::map<CGUIString, CGUIWidget*> >		TMapWidget;	//project name, widget name, widget
		TMapWidget		m_aMapWidget;				///contain all widget create by system which has name
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//page list
		typedef std::vector< std::pair< CGUIWidget*, CGUIString > >	TVecPage;	//<widget pointer, configure file
		TVecPage						m_vecPage;
		std::set<CGUIWidget*>			m_setDynamicPageList;
		std::vector<CGUIWidget*>		m_vOpenedPage;

		std::vector<CGUIWidget*>		m_vecPageGarbage;
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//widget modal dialog
		typedef std::list<CGUIWidget*>	TListDialog;	
		TListDialog	m_listOpenedDlg;		///contain modal dialog
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//popup widget
		CGUIWidget*	m_pPopupWidget;		///contain popup widget shown on top
		//----------------------------------------------------------------------


		//----------------------------------------------------------------------
		//global ui event
		typedef std::vector<CGUIWidget* >	TEventReceiver;
		typedef std::map<CGUIString, TEventReceiver> TMapUIEvent;
		TMapUIEvent			m_mapUIEvent;
		//global ui event
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		//global key event
		/*receiver*/	/*root of this receiver*/
		typedef std::vector<std::pair<CGUIWidget*, CGUIWidget*> >	TGlobalKeyObj;
		TGlobalKeyObj		m_vecGlobalKeyObj;
		//global key event
		//----------------------------------------------------------------------

		CGUIString			m_strDefaultCode;	//multi bytes code
		CGUIString			m_strDataPath;		//data path

		//----------------------------------------------------------------------
		//index for name generating
		uint32				m_nNameGenerateIdx;
		char				m_szAnonymousName[32];
		//index for name generating
		//----------------------------------------------------------------------

		bool				m_bDrawExtraInfo;

		bool				m_bShouldRunScript;	//!< should system run script

	private:
		bool				m_bInitialized;


	private:
		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUIWidgetSystem);	
	};

	//implement for singleton
	//GUI_SINGLETON_IMPLEMENT(CGUIWidgetSystem, GUI_WIDGET_SYSTEM );

}//namespace guiex



//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	/**
	* @brief get widget system
	*/
	GUIEXPORT	CGUIWidgetSystem* GetWidgetSystem();

	GUIEXPORT CGUIWidget*		GetWidget(  const CGUIString& rWidgetName, const CGUIString& rProjectName );

	GUIEXPORT CGUIWidget*		LoadDynamicPage( const CGUIString& rPageFileName, const CGUIString& rPageProjectName, const CGUIString& rWorkingProjectName );
	GUIEXPORT	void			OpenDialog(CGUIWidget* pDlg);
	GUIEXPORT	void			CloseDialog(CGUIWidget* pDlg);

	GUIEXPORT	void			OpenPage( CGUIWidget* pPage );
	GUIEXPORT	void			ClosePage( CGUIWidget* pPage );
	//GUIEXPORT	void			OpenPageByFileName(const CGUIString& rFileName, const CGUIString& rPageProjectName );

	GUIEXPORT	void			SendUIEvent(const CGUIString& rUIEventName,
		const CGUIString& rArg1 = CGUIString(),
		const CGUIString& rArg2 = CGUIString(),
		const CGUIString& rArg3 = CGUIString(),
		const CGUIString& rArg4 = CGUIString(),
		const CGUIString& rArg5 = CGUIString(),
		const CGUIString& rArg6 = CGUIString(),
		const CGUIString& rArg7 = CGUIString(),
		const CGUIString& rArg8 = CGUIString(),
		const CGUIString& rArg9 = CGUIString());

}//namespace guiex

#endif //__GUI_WIDGETSYSTEM_20060621_H__
