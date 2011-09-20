namespace guiex
{
	class CGUIWidgetUtility
	{
	public:
		static CGUIString GetWidgetInfo( const CGUIWidget* pWidget );
		static CGUIString GetWidgetInfo( const CGUIString& rWidgetName, const CGUIString& rProjectName );
		static CGUIString GetWidgetTreeInfo( const CGUIWidget* pWidget );
		static CGUIString GetWidgetTreeInfo( const CGUIString& rWidgetName, const CGUIString& rProjectName );
	};
}
