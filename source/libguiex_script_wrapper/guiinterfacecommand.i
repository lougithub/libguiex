namespace guiex
{
	class IGUIInterfaceCommand
	{
	public:
		virtual	void Send( const CGUIString& rOutput ) = 0;

	protected:
		IGUIInterfaceCommand();
		~IGUIInterfaceCommand();
	};

}

