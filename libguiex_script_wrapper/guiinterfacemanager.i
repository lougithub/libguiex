namespace guiex
{
	class CGUIInterfaceManager
	{
	protected:
		CGUIInterfaceManager();
		~CGUIInterfaceManager();

	public:
		static CGUIInterfaceManager* Instance();


	public:
		//IGUIInterfaceRender* GetInterfaceRender();
		//IGUIInterfaceFont* GetInterfaceFont();
		//IGUIInterfaceScript* GetInterfaceScript();
		//IGUIInterfaceConfigFile* GetInterfaceConfigFile();
		//IGUIInterfaceFileSys* GetInterfaceFileSys();
		//IGUIInterfaceMouse* GetInterfaceMouse();
		//IGUIInterfaceKeyboard* GetInterfaceKeyboard();
		//IGUIInterfaceSound* GetInterfaceSound();
		//IGUIInterfaceCommand* GetInterfaceCommand();
		//IGUIInterfaceIme* GetInterfaceIme();
		//IGUIInterfaceStringConv* GetInterfaceStringConv();
		IGUIInterfaceSound* GetInterfaceSound();
	};
}


