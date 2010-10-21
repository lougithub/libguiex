namespace guiex
{
	class CGUIMouseCursor
	{
	public:
		void	SetVisible( bool bVisible);
		bool	IsVisible( ) const;

		void	SetCursor(const CGUIString& rCursorName);
		CGUIString	GetCursor() const;

		void	RemoveCursor(const CGUIString& rCursorName);
		void	RemoveAllCursor();

	};


	CGUIMouseCursor*	GetMouseCursor();

}
