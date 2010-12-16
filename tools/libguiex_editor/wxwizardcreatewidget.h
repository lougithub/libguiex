/** 
* @file wizardcreatewidget.h
* @brief a wizard dialog to create widget
* @author ken
* @date 2007-09-27
*/

#ifndef __KEN_WIZARD_CREATE_WIDGET_20070927_H__
#define __KEN_WIZARD_CREATE_WIDGET_20070927_H__

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIWidget;
}


//============================================================================//
// class
//============================================================================// 
class WxWizardCreateWidget : public wxWizard
{
public:
	WxWizardCreateWidget(wxFrame *frame, CGUIWidget* pParent = NULL);

	//get first page
	wxWizardPage *GetFirstPage() const 
	{ 
		return m_pPage1; 
	}

	//get created widget
	CGUIWidget* GetWidget()
	{
		return m_pWidget;
	}

protected:
	wxWizardPageSimple* InitPage1();
	wxWizardPageSimple* InitPage2();


	//event
	void OnWizardPageChanging(wxWizardEvent& event);
	void OnWizardFinished(wxWizardEvent& event);
	
private:
	wxWizardPageSimple* m_pPage1;
	wxWizardPageSimple* m_pPage2;

	wxString m_strWidgetType;	//store the type of widget

	wxComboBox* m_pComboBoxType;
	wxTextCtrl* m_pEditName;
	wxPropertyGridManager* m_pPropGridMgr;

	CGUIWidget* m_pWidget;
	CGUIWidget* m_pParent;

	DECLARE_EVENT_TABLE()
};

#endif //__KEN_WIZARD_CREATE_WIDGET_20070927_H__
