namespace guiex
{
	class CGUIUICanvasLayer
	{
	public:
		//********************************************************
		//	page and dialog
		//********************************************************
		void OpenUIPage(CGUIWidget* pPage);
		void CloseUIPage(CGUIWidget* pPage);
		uint32 GetOpenedPageNum() const;
		CGUIWidget* GetOpenedPageByIndex( uint32 nIdx );

		void OpenDialog(CGUIWidget* pDlg);
		CGUIWidget* GetTopestDialog( ) const;
		void CloseDialog(CGUIWidget* pDlg);

		void OpenPopupWidget(CGUIWidget* pWidget);
		CGUIWidget* GetCurrentPopupWidget( ) const;
		void ClosePopupWidget(CGUIWidget* pWidget);
		CGUIWidget* GetCurrentRootWidget( ) const;
	};
}

