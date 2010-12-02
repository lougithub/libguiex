namespace guiex
{
	class CGUIWidget
	{
	public:
		virtual int32 Create();
		virtual void	Open();
		virtual void	Close();

		bool	IsOpen() const;

		void	MoveToTop();

		CGUIWidget *		GetParent()  const;
		CGUIWidget*			GetRoot() ;
		CGUIWidget*			GetChild( ) const;
		CGUIWidget*			GetChild( const CGUIString& rChildName ) const;
		CGUIWidget*			GetNextSibling( ) const;

		const CGUIString& GetType() const;
		const CGUIString& GetName() const;	
		const CGUIString&	GetProjectName( ) const;

		void	SetUserData(void*	pData);
		void*	GetUserData() const;

		void	SetTextColor(const CGUIColor& rColor);

		virtual bool	IsTextContentEmpty( ) const;
		void	SetTextContentUTF8( const CGUIString& rString);
		CGUIString	GetTextContentUTF8() const;

		virtual void	SetValue(const CGUIString& rName, const CGUIString& rValue);
		virtual CGUIString GetValue(const CGUIString& rName) const;


		void			SetTagPoint( real fX, real fY );
		const CGUIVector2& 		GetTagPoint( );
		void			SetTagPoint( const CGUIVector2& rPoint);


		void	SetLocalPosition(real x, real y );
		void	SetLocalPosition(const CGUIVector2& rPoint  );
		void	SetPositionRatio(real x, real y  );
		void	SetPositionRatio(const CGUIVector2& rPoint  );
		const CGUIVector2&	GetLocalPosition(  );

		void	SetSize(real width, real height);
		void	SetSize(const CGUISize& rSize);
		const CGUISize&		GetSize();

		void	UpdateSize();

		void	SetFocusable(bool bFocusable);
		bool	IsFocusable() const;
		void	SetFocus(bool bFocus);
		bool	IsFocus() const;
		void	SetSelfVisible(bool bVisible);
		bool	IsSelfVisible( ) const;
		bool	IsVisible( );
		void	Show( );
		void	Hide( );
		void	SetSelfDisable(bool bDisable);
		bool	IsSelfDisable( ) const;
		bool	IsDisable( );
		void	SetSelfAlpha(real fAlpha);
		real	GetSelfAlpha()  const;
		real	GetAlpha();

		CGUIWidget*		GetWidgetAtPoint(const CGUIVector2& rPos);
		virtual bool	HitTest( const CGUIVector2& rPos);

		virtual void	Update();

		virtual void	SetImage( const CGUIString& rName, CGUIImage* pImage );

		CGUIImage*		SetImage( const CGUIString& rName, const CGUIString& rStringName );

		void			SetRectDirty();		//set dirty flag for rect size and position

		void	RegisterUIEvent( const CGUIString& rUIEventName );
		void	UnregisterUIEvent( const CGUIString& rUIEventName );

		void	RegisterScriptCallbackFunc( const CGUIString& strEventName, const CGUIString& strFunc );
		void	RegisterSound( const CGUIString& strEventName, int32 nSoundIdx );

		void	AddAs(CGUIAs* pAs);

	};



	//============================================================================//
	// CGUIWgtScrollbar
	//============================================================================// 
	class CGUIWgtScrollbar : public CGUIWidget
	{
	public:

		static CGUIWgtScrollbar*	FromWidget( CGUIWidget* pWidget );

		void			SetRange( uint32 nRange );
		void			SetCurrentPos(uint32 nPos);

	};


	//============================================================================//
	// CGUIWgtScrollPanel
	//============================================================================// 
	class CGUIWgtScrollPanel : public CGUIWidget
	{
	public:

		static CGUIWgtScrollPanel*	FromWidget( CGUIWidget* pWidget );

		//get actual page size
		void				UpdateScrollbars(void);

	};


	//============================================================================//
	// CGUIWgtCheckButton
	//============================================================================// 
	class CGUIWgtCheckButton : public CGUIWidget
	{
	public:
		virtual void	SetCheck(bool bChecked);
		bool	IsCheck() const;

		static CGUIWgtCheckButton*	FromWidget( CGUIWidget* pWidget );

	};


	//============================================================================//
	// CGUIWgtRadioButton
	//============================================================================// 
	class CGUIWgtRadioButton : public CGUIWgtCheckButton
	{
	public:
		virtual void	SetCheck(bool bChecked);
		bool	IsCheck() const;

		static CGUIWgtRadioButton*	FromWidget( CGUIWidget* pWidget );
	};


	//============================================================================//
	// CGUIWgtRadioButton
	//============================================================================// 
	class CGUIWgtButton : public CGUIWidget
	{
	public:
	};

	//============================================================================//
	// CGUIWgtEditBox
	//============================================================================// 
	class CGUIWgtEditBox : public CGUIWidget
	{
	};

	//============================================================================//
	// CGUIWgtEmptyNode
	//============================================================================// 
	class CGUIWgtEmptyNode : public CGUIWidget
	{
	};


	//============================================================================//
	// CGUIWgtPanel
	//============================================================================// 
	class CGUIWgtPanel : public CGUIWidget
	{
	};


	//============================================================================//
	// CGUIWgtStaticImage
	//============================================================================// 
	class CGUIWgtStatic : public CGUIWidget
	{
	};


	//============================================================================//
	// CGUIWgtStaticImage
	//============================================================================// 
	class CGUIWgtStaticImage : public CGUIWgtStatic
	{
	};
	
	//============================================================================//
	// CGUIWgtStaticText
	//============================================================================// 
	class CGUIWgtStaticText : public CGUIWgtStatic
	{
	};
}
