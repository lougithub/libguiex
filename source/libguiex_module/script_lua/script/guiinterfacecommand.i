namespace guiex
{
	class IGUIInterfaceCommand : public IGUIInterface
	{
	public:
		virtual	void	Send( const CGUIString& rOutput ) = 0;
	};


	IGUIInterfaceCommand*	GetInterface_Command( );
}

