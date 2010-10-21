namespace guiex
{
	class IGUIInterface
	{
		virtual void	DeleteSelf() = 0;

	};

	int	RegisterInterface(const CGUIString& rInterface, const CGUIString& rModuleName );

	int UnregisterInterface(const CGUIString& rInterface );

	IGUIInterface* GetInterface(const CGUIString& rInterface );
}


