namespace guiex
{
	class CGUISystem
	{
	public:
		static CGUISystem* Instance();

		void SetDrawExtraInfo( bool bDraw );
		bool IsDrawExtraInfo() const;

		const CGUIString& GetDataPath() const;

		const char* GenerateAnonymousName();

		CGUIWidget*	GetFocusWidget();

		CGUIWidget* GetWidgetUnderPoint(const CGUIVector2& rPos);

		//********************************************************
		//	page and dialog
		//********************************************************
		void OpenPage(CGUIWidget* pPage);
		void ClosePage(CGUIWidget* pPage);
		uint32 GetOpenedPageNum() const;
		CGUIWidget* GetOpenedPageByIndex( uint32 nIdx );

		void OpenDialog(CGUIWidget* pDlg);
		CGUIWidget* GetTopestDialog( ) const;
		void CloseDialog(CGUIWidget* pDlg);

		void OpenPopupWidget(CGUIWidget* pWidget);
		CGUIWidget* GetCurrentPopupWidget( ) const;
		void ClosePopupWidget(CGUIWidget* pWidget);
		CGUIWidget* GetCurrentRootWidget( ) const;

		void CloseAll();

		//********************************************************
		//	screen size
		//********************************************************
		const CGUISize& GetScreenSize( ) const;
		uint32 GetScreenWidth( ) const;
		uint32 GetScreenHeight( ) const;
		const CGUIVector2& GetScreenPos( ) const;
		const CGUIRect& GetScreenRect( ) const;


		//********************************************************
		//	ui event
		//********************************************************
		void SendUIEvent(CGUIEventUI* pEvent );
		void SendEvent(CGUIEvent* pEvent );

		//********************************************************
		//	global key event
		//********************************************************
		real GetSystemTime() const;
		int32 GetFPS() const;
	};


	CGUISystem* GetSystem();

	void OpenDialog(CGUIWidget* pDlg);
	void CloseDialog(CGUIWidget* pDlg);

	void OpenPage( CGUIWidget* pPage );
	void ClosePage( CGUIWidget* pPage );

	void SendUIEvent(const CGUIString& rUIEventName,
		const CGUIString& rArg1 = CGUIString(),
		const CGUIString& rArg2 = CGUIString(),
		const CGUIString& rArg3 = CGUIString(),
		const CGUIString& rArg4 = CGUIString(),
		const CGUIString& rArg5 = CGUIString(),
		const CGUIString& rArg6 = CGUIString(),
		const CGUIString& rArg7 = CGUIString(),
		const CGUIString& rArg8 = CGUIString(),
		const CGUIString& rArg9 = CGUIString());
}
