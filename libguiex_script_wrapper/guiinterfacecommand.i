namespace guiex
{
	class IGUIInterfaceCommand : public IGUIInterface
	{
	public:
		virtual	void Send( const CGUIString& rOutput ) = 0;
	};

}

