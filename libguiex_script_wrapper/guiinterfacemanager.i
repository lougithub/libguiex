namespace guiex
{
	class IGUIInterface
	{
		virtual void DeleteSelf() = 0;
	};
	class CGUIInterfaceManager
	{
	public:
		CGUIInterfaceManager();
		~CGUIInterfaceManager();

		static CGUIInterfaceManager* Instance();

		int	RegisterInterface(const CGUIString& rInterface, const CGUIString& rModuleName, void* pUserData = NULL);
		int	RegisterInterface(const CGUIString& rInterface, IGUIInterface* pInterface );
		int UnregisterInterface(const CGUIString& rInterface );
		void UnregisterAllInterface();
		IGUIInterface* GetInterface(const CGUIString& rInterface );
		bool HasInterface(const CGUIString& rInterface );

	public:
		/*
		IGUIInterfaceRender* GetInterfaceRender();
		IGUIInterfaceFont* GetInterfaceFont();
		IGUIInterfaceScript* GetInterfaceScript();
		IGUIInterfaceConfigFile* GetInterfaceConfigFile();
		IGUIInterfaceFileSys* GetInterfaceFileSys();
		IGUIInterfaceMouse* GetInterfaceMouse();
		IGUIInterfaceKeyboard* GetInterfaceKeyboard();
		IGUIInterfaceSound* GetInterfaceSound();
		IGUIInterfaceCommand* GetInterfaceCommand();
		IGUIInterfaceIme* GetInterfaceIme();
		IGUIInterfaceStringConv* GetInterfaceStringConv();
		*/
	};
		
	int	RegisterInterface(const CGUIString& rInterface, const CGUIString& rModuleName );
	int UnregisterInterface(const CGUIString& rInterface );
	IGUIInterface* GetInterface(const CGUIString& rInterface );
}


