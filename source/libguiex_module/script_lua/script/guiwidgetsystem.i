namespace guiex
{
	class CGUIWidgetSystem
	{
	public:
		int					Initialize();
		void				Release();

		void				Update();
		void				Render();

		void				FreeResource( const CGUIString& rSource );

		const	CGUITimer&	GetGlobalTimer() const;

		//void			OpenPage(CGUIWidget* pPage);
		//void			ClosePage(CGUIWidget* pPage);
		//void			AddPage(  CGUIWidget* pPage);
		//CGUIWidget*		GetPage( const CGUIString& rPageName );
		//bool			HasPage(const CGUIString& rPageName) const;
		//bool			HasPage( CGUIWidget* pPage) const;
		//void			DeletePage( CGUIWidget* pPage);

		CGUIWidget*			CreateWidget( const CGUIString& rType, const CGUIString& rWidgetName, const CGUIString& rProjectName );
		void				DestroyWidget(CGUIWidget* pWidget);
		CGUIWidget*			GetWidget(  const CGUIString& rName, const CGUIString& rProjectName );
		void				SetScreenSize( uint32 width, uint32 height);
		const CGUIRect&		GetScreenRect( ) const;

		void				SetDataPath(const const CGUIString& rDataPath);
		const const CGUIString&	GetDataPath() const;

	protected:
		///constructor and destructor, for singleton use.
		CGUIWidgetSystem();
		~CGUIWidgetSystem();
	};


	CGUIWidgetSystem* GetWidgetSystem();
	CGUIWidget* GetWidget(const CGUIString& rName, const CGUIString& rProjectName);
	CGUIWidget*	LoadDynamicPage( const CGUIString& rPageFileName, const CGUIString& rPageProjectName, const CGUIString& rWorkingProjectName );
	void			OpenDialog(CGUIWidget* pDlg);
	void			CloseDialog(CGUIWidget* pDlg);
	void			SendUIEvent(const CGUIString& rUIEventName,
		const CGUIString& rArg1 = CGUIString(),
		const CGUIString& rArg2 = CGUIString(),
		const CGUIString& rArg3 = CGUIString(),
		const CGUIString& rArg4 = CGUIString(),
		const CGUIString& rArg5 = CGUIString(),
		const CGUIString& rArg6 = CGUIString(),
		const CGUIString& rArg7 = CGUIString(),
		const CGUIString& rArg8 = CGUIString(),
		const CGUIString& rArg9 = CGUIString());
	void			OpenPage( CGUIWidget* pPage );
	void			ClosePage( CGUIWidget* pPage );

}
