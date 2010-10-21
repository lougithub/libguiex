namespace guiex
{
	class CGUIWidget;

	class CGUIEvent
	{
	public:
		uint32					GetEventId() const;
		const CGUIString&		GetEventType() const;
		CGUIWidget*				GetReceiver();
		void					Consume(bool bConsumed);
	};


	class CGUIEventScrollbar : public CGUIEvent
	{
	public:
		void		SetScrollbarType( bool bVertical );
		bool		IsVertical() const;
		bool		IsHorizontal() const;
		void		SetCurrentPos( uint32 nPos);
		uint32		GetCurrentPos( ) const;
		void		SetRange( uint32 nRange );
		uint32		GetRange() const;
		CGUIWidget*	GetScrollbar() const;
	};


	class CGUIEventTimer : public CGUIEvent
	{
	public:
		void				SetTimerName(const CGUIString& rTimerName);
		const CGUIString&	GetTimerName();
	};



	class CGUIEventNotification : public CGUIEvent
	{
	public:
	};


	class CGUIEventSize : public CGUIEvent
	{
	public:
		const CGUISize& GetSize() const;
		void			SetSize(const CGUISize& rSize);
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
		void		SetPosition( const CGUIVector2& rPos );
		const CGUIVector2& GetPosition() const;
		void		SetSysKeys(uint32 uSyskeys);
		uint32		GetSysKeys() const;
		void		SetWheelChange(real fWheelChange);
		real		GetWheelChange( );
		void		SetMultiClickCount( uint32 nCount );
		uint32		GetMultiClickCount( ) const;
		void		SetButton(int32 nButton);
		int32		GetButton() const;
	};



	class CGUIEventKeyboard: public CGUIEvent
	{
	public:
		void	SetKeyCode( int32 nKeyCode);
		int32	GetKeyCode() const;
	};



	class CGUIEventDrag : public CGUIEvent
	{
	public:
		void		SetWidgetPos( const CGUIVector2& rPos );
		void		SetMousePos( const CGUIVector2& rPos );
		const CGUIVector2& GetWidgetPos() const;
		const CGUIVector2& GetMousePos() const;
		void		SetButton( int32 nButton);
		const int32 GetButton() const;

	};


	class CGUIEventUI: public CGUIEvent
	{
	public:
		/// get name of this ui event
		const CGUIString&	GetUIName( ) const;

		const CGUIString&	GetArg(int nIndex) const;

	};

}

