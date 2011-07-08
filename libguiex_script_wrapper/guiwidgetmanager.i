namespace guiex
{
	class CGUIWidgetManager
	{
	public:
		CGUIWidget*	LoadDynamicPage( const CGUIString& rPageName, const CGUIString& rSceneName, const CGUIString& rWorkingSceneName );
		CGUIWidget* GetWidget( const CGUIString& rWidgetName, const CGUIString& rSceneName );

	protected:
		CGUIWidgetManager();
		virtual ~CGUIWidgetManager();
	};
}
