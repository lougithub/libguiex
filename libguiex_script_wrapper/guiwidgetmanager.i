namespace guiex
{
	class CGUIWidgetManager
	{
	public:
		CGUIWidget*	LoadDynamicPage( const CGUIString& rPageName, const CGUIString& rSceneName, const CGUIString& rWorkingSceneName );

	protected:
		CGUIWidgetManager();
		virtual ~CGUIWidgetManager();
	};
}
