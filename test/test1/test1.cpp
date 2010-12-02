/** 
 * @file sample_opengl.cpp
 * @brief test and show how to use opengl as render

 * @author ken
 * @date 2006-07-11
 */

#include <libguiex_core/guiex.h>
#include <libguiex_widget/guiwgt.h>

const char* GetSampleSceneName()
{
	return "common.uip";
}

guiex::CGUIWidget* SampleInitialize()
{
	guiex::CGUIWgtEmptyNode* pWidgetRoot = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtEmptyNode>( "page", "testscene" );
	pWidgetRoot->SetAnchorPoint( 0.5f, 0.5f );
	pWidgetRoot->SetPositionType( guiex::eScreenValue_Percentage );
	pWidgetRoot->SetPosition( 0.5f,0.5f );
	pWidgetRoot->Create();

	guiex::CGUIWgtStaticImage* pWidget_staticimage =  
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtStaticImage>( "staticimage_0", "testscene");
	pWidget_staticimage->SetParent( pWidgetRoot );
	pWidget_staticimage->SetImage( "BGIMAGE", "color_white" );
	pWidget_staticimage->SetSize( 50, 50 );
	pWidget_staticimage->SetPosition( 100, 100 );
	pWidget_staticimage->SetAnchorPoint( 0.5, 0.5 );
	pWidget_staticimage->SetClipChildren( true );
	pWidget_staticimage->Create();

	guiex::CGUIWgtStaticImage* pWidget_staticimage2 = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtStaticImage>( "staticimage_1", "testscene");
	pWidget_staticimage2->SetParent( pWidget_staticimage );
	pWidget_staticimage2->SetImage( "BGIMAGE", "checkbutton_glow_checked" );
	pWidget_staticimage2->SetAnchorPoint( 0.5, 0.5 );
	pWidget_staticimage2->SetSize( 25, 25 );
	pWidget_staticimage2->SetPosition( 0, 0 );
	pWidget_staticimage2->Create();

	guiex::CGUIWgtStaticText* pWidget_statictext = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtStaticText>( "staticText_1", "testscene");
	pWidget_statictext->SetParent( pWidgetRoot );
	pWidget_statictext->SetAnchorPoint( 0.5, 0.5 );
	pWidget_statictext->SetSize( 250, 25 );
	pWidget_statictext->SetPosition( 300, 300 );
	pWidget_statictext->SetTextContentUTF8("hello libguiex");
	pWidget_statictext->SetTextColor(guiex::CGUIColor( 1,0,0,1));
	pWidget_statictext->Create();

	guiex::CGUIWgtButton* pWidget_button = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtButton>( "button", "testscene");
	pWidget_button->SetParent( pWidgetRoot );
	pWidget_button->SetImage( "BTN_NORMAL", "button_blue_normal" );
	pWidget_button->SetImage( "BTN_HOVER", "button_blue_hover" );
	pWidget_button->SetImage( "BTN_PUSH", "button_blue_push" );
	pWidget_button->SetImage( "BTN_DISABLE", "button_blue_disable" );
	pWidget_button->SetAnchorPoint( 0.5, 0.5 );
	pWidget_button->SetPosition( 400, 200 );
	pWidget_button->SetTextContentUTF8("ok");
	pWidget_button->SetTextColor(guiex::CGUIColor( 1,1,1,1));
	pWidget_button->Create();

	guiex::CGUIWgtButton* pWidget_button2 = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtButton>( "button2", "testscene");
	pWidget_button2->SetParent( pWidgetRoot );
	pWidget_button2->SetImage( "BTN_NORMAL", "button_blue_normal" );
	pWidget_button2->SetImage( "BTN_HOVER", "button_blue_hover" );
	pWidget_button2->SetImage( "BTN_PUSH", "button_blue_push" );
	pWidget_button2->SetImage( "BTN_DISABLE", "button_blue_disable" );
	pWidget_button2->SetAnchorPoint( 0.5, 0.5 );
	pWidget_button2->SetPosition( 600, 200 );
	pWidget_button2->SetTextContentUTF8("cancel");
	pWidget_button2->SetTextColor(guiex::CGUIColor( 1,1,1,1));
	pWidget_button2->SetRotation( 0,0,45 );
	pWidget_button2->Create();

	guiex::CGUIWgtCheckButton* pWidget_checkbutton = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtCheckButton>( "checkbutton", "testscene");
	pWidget_checkbutton->SetParent( pWidgetRoot );
	pWidget_checkbutton->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_checkbutton->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_checkbutton->SetSize( 20, 20 );
	pWidget_checkbutton->SetAnchorPoint( 0.5, 0.5 );
	pWidget_checkbutton->SetPosition( 400, 300 );
	pWidget_checkbutton->Create();

	guiex::CGUIWgtCheckButton* pWidget_checkbutton2 = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtCheckButton>( "checkbutton2", "testscene");
	pWidget_checkbutton2->SetParent( pWidgetRoot );
	pWidget_checkbutton2->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_checkbutton2->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_checkbutton2->SetSize( 20, 20 );
	pWidget_checkbutton2->SetAnchorPoint( 0.5, 0.5 );
	pWidget_checkbutton2->SetPosition( 450, 300 );
	pWidget_checkbutton2->Create();

	guiex::CGUIWgtEmptyNode* pWidgetRadioGroup = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtEmptyNode>( "radiogroup", "testscene");
	pWidgetRadioGroup->SetParent( pWidgetRoot );
	pWidgetRadioGroup->Create();

	guiex::CGUIWgtRadioButton* pWidget_radiobutton = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtRadioButton>( "radiobutton", "testscene");
	pWidget_radiobutton->SetParent( pWidgetRadioGroup );
	pWidget_radiobutton->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_radiobutton->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_radiobutton->SetSize( 20, 20 );
	pWidget_radiobutton->SetAnchorPoint( 0.5, 0.5 );
	pWidget_radiobutton->SetPosition( 400, 350 );
	pWidget_radiobutton->Create();

	guiex::CGUIWgtRadioButton* pWidget_radiobutton2 =
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtRadioButton>( "radiobutton2", "testscene");
	pWidget_radiobutton2->SetParent( pWidgetRadioGroup );
	pWidget_radiobutton2->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_radiobutton2->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_radiobutton2->SetSize( 20, 20 );
	pWidget_radiobutton2->SetAnchorPoint( 0.5, 0.5 );
	pWidget_radiobutton2->SetPosition( 450, 350 );
	pWidget_radiobutton2->Create();


	guiex::CGUIWgtEditBox* pWidget_editbox = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtEditBox>( "editbox", "testscene");
	pWidget_editbox->SetParent( pWidgetRoot );
	pWidget_editbox->SetImage( "EDIT_BG", "edit_bg" );
	pWidget_editbox->SetImage( "EDIT_CURSOR", "color_white" );
	pWidget_editbox->SetSize( 150, 25 );
	pWidget_editbox->SetAnchorPoint( 0.5, 0.5 );
	pWidget_editbox->SetPosition( 400, 400 );
	pWidget_editbox->SetCursorSize( guiex::CGUISize(2,20));
	pWidget_editbox->Create();


	guiex::CGUIWgtMultiEditBox* pWidget_multieditbox = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtMultiEditBox>( "multieditbox", "testscene");
	pWidget_multieditbox->SetParent( pWidgetRoot );
	pWidget_multieditbox->SetImage( "EDIT_BG", "edit_bg" );
	pWidget_multieditbox->SetImage( "EDIT_CURSOR", "color_white" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_ARROW_NORMAL_0", "scrollbar_downbutton_down" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_ARROW_HOVER_0", "scrollbar_downbutton_highlight" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_ARROW_DISABLE_0", "scrollbar_downbutton_disable" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_ARROW_PUSH_0", "scrollbar_downbutton_up" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_ARROW_NORMAL_1", "scrollbar_downbutton_down" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_ARROW_HOVER_1", "scrollbar_downbutton_highlight" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_ARROW_DISABLE_1", "scrollbar_downbutton_disable" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_ARROW_PUSH_1", "scrollbar_downbutton_up" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_SLIDE_NORMAL", "scrollbar_downbutton_down" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_SLIDE_HOVER", "scrollbar_downbutton_highlight" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_SLIDE_DISABLE", "scrollbar_downbutton_disable" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_VERTIC_SLIDE_PUSH", "scrollbar_downbutton_up" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_HORIZON_ARROW_NORMAL_0", "scrollbar_downbutton_down" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_HORIZON_ARROW_HOVER_0", "scrollbar_downbutton_highlight" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_HORIZON_ARROW_DISABLE_0", "scrollbar_downbutton_disable" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_HORIZON_ARROW_PUSH_0", "scrollbar_downbutton_up" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_HORIZON_ARROW_NORMAL_1", "scrollbar_downbutton_down" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_HORIZON_ARROW_HOVER_1", "scrollbar_downbutton_highlight" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_HORIZON_ARROW_DISABLE_1", "scrollbar_downbutton_disable" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_HORIZON_ARROW_PUSH_1", "scrollbar_downbutton_up" );
	pWidget_multieditbox->SetImage( "SCROLLBAR_HORIZON_SLIDE_NORMAL", "scrollbar_knob" );
	//pWidget_multieditbox->ForceVertScrollbar( true );
	//pWidget_multieditbox->ForceHorzScrollbar( true );
	pWidget_multieditbox->SetSize( 150, 50 );
	pWidget_multieditbox->SetAnchorPoint( 0.5, 0.5 );
	pWidget_multieditbox->SetPosition( 400, 500 );
	pWidget_multieditbox->SetCursorSize( guiex::CGUISize(2,20));
	pWidget_multieditbox->Create();


	guiex::CGUIWgtPanel* pWidget_panel= 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtPanel>( "panel", "testscene");
	pWidget_panel->SetParent( pWidgetRoot );
	pWidget_panel->SetImage( "BGIMAGE", "panel_bg_grade_blue" );
	pWidget_panel->SetImage( "PANEL_BORDER_TOP", "panel_border_top" );
	pWidget_panel->SetImage( "PANEL_BORDER_BOTTOM", "panel_border_bottom" );
	pWidget_panel->SetImage( "PANEL_BORDER_LEFT", "panel_border_left" );
	pWidget_panel->SetImage( "PANEL_BORDER_RIGHT", "panel_border_right" );
	pWidget_panel->SetImage( "PANEL_BORDER_TOPLEFT", "panel_border_topleft" );
	pWidget_panel->SetImage( "PANEL_BORDER_TOPRIGHT", "panel_border_topright" );
	pWidget_panel->SetImage( "PANEL_BORDER_BOTTOMLEFT", "panel_border_bottomleft" );
	pWidget_panel->SetImage( "PANEL_BORDER_BOTTOMRIGHT", "panel_border_bottomright" );
	pWidget_panel->SetSize( 150, 200 );
	pWidget_panel->SetPosition( 700, 200 );
	pWidget_panel->SetAnchorPoint( 0.5f, 0.5f );
	pWidget_panel->SetRotation( 0,0,70 );
	pWidget_panel->Create();


	guiex::CGUIWgtTabControl* pWidget_tabcontroll= 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtTabControl>( "tabcontrol", "testscene");
	pWidget_tabcontroll->SetParent( pWidget_panel );
	pWidget_tabcontroll->Create();


	guiex::CGUIWgtTabButton* pWidget_tabbutton = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtTabButton>( "tabbutton", "testscene");
	pWidget_tabbutton->SetParent( pWidget_tabcontroll );
	pWidget_tabbutton->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_tabbutton->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_tabbutton->SetSizeType( guiex::eScreenValue_Percentage );
	pWidget_tabbutton->SetPositionType( guiex::eScreenValue_Percentage );
	pWidget_tabbutton->SetSize( 0.33f,0.1f );
	pWidget_tabbutton->SetPosition( 0, 0 );
	pWidget_tabbutton->Create();

	guiex::CGUIWgtPanel* pWidget_panel1= 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtPanel>( "panel1", "testscene");
	pWidget_panel1->SetParent( pWidget_tabbutton );
	pWidget_panel1->SetImage( "BGIMAGE", "color_white" );
	pWidget_panel1->SetSizeType( guiex::eScreenValue_Percentage );
	pWidget_panel1->SetSize( 1.0f, 0.5f);
	pWidget_panel1->SetColor(guiex::CGUIColor(1,0,0,1));
	pWidget_panel1->Create();

	guiex::CGUIWgtTabButton* pWidget_tabbutton2 = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtTabButton>( "tabbutton2", "testscene");
	pWidget_tabbutton2->SetParent( pWidget_tabcontroll );
	pWidget_tabbutton2->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_tabbutton2->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_tabbutton2->SetSizeType( guiex::eScreenValue_Percentage );
	pWidget_tabbutton2->SetPositionType( guiex::eScreenValue_Percentage );
	pWidget_tabbutton2->SetSize( 0.33f, 0.1f );
	pWidget_tabbutton2->SetPosition( 0.33f, 0 );
	pWidget_tabbutton2->Create();

	guiex::CGUIWgtPanel* pWidget_panel2=
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtPanel>( "panel2", "testscene");
	pWidget_panel2->SetParent( pWidget_tabbutton2 );
	pWidget_panel2->SetImage( "BGIMAGE", "color_white" );
	pWidget_panel2->SetSizeType( guiex::eScreenValue_Percentage );
	pWidget_panel2->SetSize( 1.0f, 0.5f);
	pWidget_panel2->SetColor(guiex::CGUIColor(0,1,0,1));
	pWidget_panel2->Create();

	guiex::CGUIWgtTabButton* pWidget_tabbutton3 = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtTabButton>( "tabbutton3", "testscene");
	pWidget_tabbutton3->SetParent( pWidget_tabcontroll );
	pWidget_tabbutton3->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_tabbutton3->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_tabbutton3->SetSizeType( guiex::eScreenValue_Percentage );
	pWidget_tabbutton3->SetPositionType( guiex::eScreenValue_Percentage );
	pWidget_tabbutton3->SetSize( 0.33f, 0.1f );
	pWidget_tabbutton3->SetPosition( 0.66f, 0 );
	pWidget_tabbutton3->Create();

	guiex::CGUIWgtPanel* pWidget_panel3= 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtPanel>( "panel3", "testscene");
	pWidget_panel3->SetParent( pWidget_tabbutton3 );
	pWidget_panel3->SetImage( "BGIMAGE", "color_white" );
	pWidget_panel3->SetSizeType( guiex::eScreenValue_Percentage );
	pWidget_panel3->SetSize( 1.0f, 0.5f);
	pWidget_panel3->SetColor(guiex::CGUIColor(0,0,1,1));
	pWidget_panel3->Create();


	guiex::CGUIWgtProgress* pWidget_progress= 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtProgress>( "progress", "testscene");
	pWidget_progress->SetParent( pWidgetRoot );
	pWidget_progress->SetImage( "BACKGROUND_IMG", "color_white" );
	pWidget_progress->SetImage( "FOREGROUND_IMG", "panel_bg_grade_green" );
	pWidget_progress->SetSize( 200, 25);
	pWidget_progress->SetMaximumProgress( 100 );
	pWidget_progress->SetCurrentProgress( 50 );
	pWidget_progress->SetPosition( 500,500 );
	pWidget_progress->Create();

	guiex::CGUIWgtStaticText* pWidget_frame = 
		guiex::CGUIWidgetManager::Instance()->CreateWidget<guiex::CGUIWgtStaticText>( "frame", "testscene");
	pWidget_frame->SetParent( pWidgetRoot );
	pWidget_frame->SetAnchorPoint( 1, 0 );
	pWidget_frame->SetSize( 250, 25 );
	pWidget_frame->SetPositionType( guiex::eScreenValue_Percentage );
	pWidget_frame->SetPosition( 1, 0.1f );
	pWidget_frame->SetTextContentUTF8("frame = ???");
	pWidget_frame->SetTextColor(guiex::CGUIColor( 1,1,0,1));
	pWidget_frame->Create();

	guiex::CGUIAsManager* pAsManager = guiex::CGUIAsManager::Instance();
	{
		guiex::CGUIAsAlpha* pAsAlpha = pAsManager->AllocateResource<guiex::CGUIAsAlpha>( );
		pAsAlpha->SetInterpolationValue( 0, 1, 2 );
		pAsAlpha->SetReceiver( pWidgetRoot );
		pWidgetRoot->PlayAs( pAsAlpha );
		pAsAlpha->RefRelease();
	}

	{
		guiex::CGUIAsScale* pAsScale = pAsManager->AllocateResource<guiex::CGUIAsScale>( );
		pAsScale->SetInterpolationValue( guiex::CGUISize(0.2f,0.2f), guiex::CGUISize(1,1), 4 );
		pAsScale->SetReceiver( pWidget_panel );
		pAsScale->SetInterpolationType( guiex::eInterpolationType_EaseInOut );
		pWidget_panel->PlayAs( pAsScale );
		pAsScale->RefRelease();

		guiex::CGUIAsRotation* pAsRotation = pAsManager->AllocateResource<guiex::CGUIAsRotation>( );
		pAsRotation->SetInterpolationValue( guiex::CGUIVector3(0,0,360*5), guiex::CGUIVector3(), 4 );
		pAsRotation->SetReceiver( pWidget_panel );
		pWidget_panel->PlayAs( pAsRotation );
		pAsRotation->RefRelease();
	}

	{
		guiex::CGUIAsPosition* pAsPos1 = pAsManager->AllocateResource<guiex::CGUIAsPosition>( );
		pAsPos1->SetInterpolationValue( guiex::CGUIVector2(0,0), guiex::CGUIVector2(500,500), 3 );
		pAsPos1->SetReceiver( pWidget_staticimage );
		pWidget_staticimage->PlayAs( pAsPos1 );
		pAsPos1->RefRelease();

		guiex::CGUIAsPosition* pAsPos2 = pAsManager->AllocateResource<guiex::CGUIAsPosition>( );
		pAsPos2->SetInterpolationValue( guiex::CGUIVector2(500,500), guiex::CGUIVector2(100,500), 2 );
		pAsPos2->SetReceiver( pWidget_staticimage );
		pAsPos1->PushSuccessor(pAsPos2);
		pAsPos2->RefRelease();


	}

	{
		guiex::CGUIAsContainer* pAsContainer = pAsManager->AllocateResource<guiex::CGUIAsContainer>( );
		guiex::CGUIAsColor* pAsColor = NULL;
		guiex::CGUIAsContainItemInfo aItem;

		pAsColor = pAsManager->AllocateResource<guiex::CGUIAsColor>( );
		pAsColor->SetInterpolationValue( guiex::CGUIColor(1,0,0, 1), guiex::CGUIColor(0,1,0,1), 2 );
		aItem.m_pAs = pAsColor;
		aItem.m_fBeginTime = 0;
		pAsContainer->AddItem( aItem );
		pAsColor->RefRelease();

		pAsColor = pAsManager->AllocateResource<guiex::CGUIAsColor>( );
		pAsColor->SetInterpolationValue( guiex::CGUIColor(0,1,0, 1), guiex::CGUIColor(0,0,1,1), 2 );
		aItem.m_pAs = pAsColor;
		aItem.m_fBeginTime = 2;
		pAsContainer->AddItem( aItem );
		pAsColor->RefRelease();

		pAsColor = pAsManager->AllocateResource<guiex::CGUIAsColor>( );
		pAsColor->SetInterpolationValue( guiex::CGUIColor(0,0,1, 1), guiex::CGUIColor(1,0,0,1), 2 );
		aItem.m_pAs = pAsColor;
		aItem.m_fBeginTime = 4;
		pAsContainer->AddItem( aItem );
		pAsColor->RefRelease();

		pAsContainer->SetReceiver( pWidget_staticimage );
		pAsContainer->SetLooping( true );
		pWidget_staticimage->PlayAs( pAsContainer );
		pAsContainer->RefRelease();
	}

	{
		guiex::CGUIAsRotation* pAsRotation = pAsManager->AllocateResource<guiex::CGUIAsRotation>( );
		pAsRotation->SetInterpolationValue( guiex::CGUIVector3(0,0,360), guiex::CGUIVector3(), 4 );
		pAsRotation->SetReceiver( pWidget_staticimage );
		pAsRotation->SetLooping( true );
		pWidget_staticimage->PlayAs( pAsRotation );
		pAsRotation->RefRelease();
	}

	{
		guiex::CGUIAsScale* pAsScale = pAsManager->AllocateResource<guiex::CGUIAsScale>( );
		pAsScale->SetInterpolationValue( guiex::CGUISize(5,5), guiex::CGUISize(1,1), 6 );
		pAsScale->SetReceiver( pWidget_staticimage2 );
		pWidget_staticimage2->PlayAs( pAsScale );
		pAsScale->RefRelease();

		guiex::CGUIAsAlpha* pAsAlpha = pAsManager->AllocateResource<guiex::CGUIAsAlpha>( );
		pAsAlpha->SetInterpolationValue( 0, 1, 3 );
		pAsAlpha->SetReceiver( pWidget_staticimage2 );
		pWidget_staticimage2->PlayAs( pAsAlpha );
		pAsAlpha->RefRelease();

		guiex::CGUIAsRotation* pAsRotation = pAsManager->AllocateResource<guiex::CGUIAsRotation>( );
		pAsRotation->SetInterpolationValue( guiex::CGUIVector3(0,0,360*2), guiex::CGUIVector3(), 4 );
		pAsRotation->SetReceiver( pWidget_staticimage2 );
		pWidget_staticimage2->PlayAs( pAsRotation );
		pAsRotation->RefRelease();
	}

	{
		guiex::CGUIAsRotation* pAsRotation = pAsManager->AllocateResource<guiex::CGUIAsRotation>( );
		pAsRotation->SetInterpolationValue( guiex::CGUIVector3(0,0,360*5), guiex::CGUIVector3(), 1 );
		pAsRotation->SetReceiver( pWidget_statictext );
		pWidget_statictext->PlayAs( pAsRotation );
		pAsRotation->RefRelease();

		guiex::CGUIAsAlpha* pAsAlpha = pAsManager->AllocateResource<guiex::CGUIAsAlpha>( );
		pAsAlpha->SetInterpolationValue( 0, 1, 1 );
		pAsAlpha->SetReceiver( pWidget_statictext );
		pWidget_statictext->PlayAs( pAsAlpha );
		pAsAlpha->RefRelease();

		guiex::CGUIAsScale* pAsScale = pAsManager->AllocateResource<guiex::CGUIAsScale>( );
		pAsScale->SetInterpolationValue( guiex::CGUISize(0,0), guiex::CGUISize(2,2), 1 );
		pAsScale->SetReceiver( pWidget_statictext );
		pWidget_statictext->PlayAs( pAsScale );
		pAsScale->RefRelease();
	}

	return pWidgetRoot;
}

void SampleUpdate( float fDeltaTime)
{
	guiex::CGUIWgtStaticText* pWidget = 
		guiex::CGUIWidgetManager::Instance()->GetWidgetWithTypeCheck<guiex::CGUIWgtStaticText>( "frame", "testscene" );
	char buf[512];
	snprintf( buf, 512, "frame = %d", guiex::GSystem->GetFPS() );
	pWidget->SetTextContentUTF8( buf );
}

void SampleDestroy()
{

}
