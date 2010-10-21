namespace guiex
{
	class CGUITimer
	{
	public:
		CGUITimer();
		CGUITimer(const CGUITimer& rTimer);
		~CGUITimer();

		void	UpdateTime();
		void	Assign( const CGUITimer& rTimer );
		int32	operator-(const CGUITimer& rTimer) const;
	};
}
