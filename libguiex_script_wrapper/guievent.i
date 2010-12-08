namespace guiex
{
	class CGUIWidget;

	class CGUIEvent
	{
	public:
		uint32 GetEventId() const;
		const CGUIString& GetEventType() const;
		CGUIWidget* GetReceiver();
		bool IsConsumed();
		void Consume(bool bConsumed);
	};


	class CGUIEventScrollbar : public CGUIEvent
	{
	public:
		void SetScrollbarType( bool bVertical );
		bool IsVertical() const;
		bool IsHorizontal() const;
		
		void SetScrollbar(CGUIWidget * pScrollbar);
		CGUIWidget*	GetScrollbar() const;

		void SetCurrentPos( uint32 nPos );
		uint32 GetCurrentPos() const;

		void SetRange( uint32 nRange );
		uint32 GetRange() const;
	};


	class CGUIEventTimer : public CGUIEvent
	{
	public:
		void SetTimerName(const CGUIString& rTimerName);
		const CGUIString& GetTimerName();
	};



	class CGUIEventNotification : public CGUIEvent
	{
	public:
	};


	class CGUIEventSize : public CGUIEvent
	{
	public:
		const CGUISize& GetSize() const;
		void SetSize(const CGUISize& rSize);
	};



	class CGUIEventAlpha : public CGUIEvent
	{
	public:
		real GetAlpha() const;
		void SetAlpha(real fAlpha);
	};



	class CGUIEventMouse : public CGUIEvent
	{
	public:
		const CGUIVector2& GetGlobalPosition() const;
		CGUIVector2 GetLocalPosition() const;
		
		uint32 GetSysKeys() const;

		real GetWheelChange( );

		uint32 GetMultiClickCount( ) const;

		int32 GetButton() const;
	};



	class CGUIEventKeyboard: public CGUIEvent
	{
	public:
		int32 GetKeyCode() const;
	};



	class CGUIEventDrag : public CGUIEvent
	{
	public:
		CGUIVector2 GetWidgetGlobalPos() const;
		const CGUIVector2& GetWidgetLocalPos() const;

		const CGUIVector2& GetMouseGlobalPos() const;
		CGUIVector2 GetMouseLocalPos() const;

		const int32 GetButton() const;
		bool IsExpired();
	};


	class CGUIEventUI: public CGUIEvent
	{
	public:
		const CGUIString& GetUIName( ) const;

		const CGUIString& GetArg(int nIndex) const;
	};
}

