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

		void GenerateUICanvas();
		void DestroyUICanvas();
		CGUIUICanvasLayer* GetUICanvas();

		//********************************************************
		//	screen size
		//********************************************************
		const CGUISize& GetScreenSize( ) const;
		uint32 GetScreenWidth( ) const;
		uint32 GetScreenHeight( ) const;


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

	protected:
		CGUISystem();
		~CGUISystem();
	};


	CGUISystem* GetSystem();

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
