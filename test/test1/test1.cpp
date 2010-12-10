/** 
 * @file test.cpp
 * @author ken
 * @date 2010-12-10
 */

#include <libguiex_framework/guiframework.h>
using namespace guiex;

class CGUIFrameworkTest : public CGUIFramework
{
public:
	CGUIFrameworkTest( const CGUISize& rScreenSize, const char* pDataPath )
		:CGUIFramework( rScreenSize, pDataPath )
	{
	}
protected:
	virtual int32 InitializeGame( );
	virtual void ReleaseGame( );

	virtual void PreUpdate( real fDeltaTime );
	virtual void PostUpdate( real fDeltaTime );
};

CGUIFrameworkBase* CreateFramework( const CGUISize& rScreenSize, const char* pDataPath )
{
	return new CGUIFrameworkTest( rScreenSize, pDataPath );
}

const char* GetSampleSceneName()
{
	return "common.uip";
}

int32 CGUIFrameworkTest::InitializeGame( )
{
	CGUISceneInfoManager::Instance()->LoadScenes( "/", ".uip" );
	CGUISceneUtility::LoadResource( "common.uip" );

	CGUIWgtEmptyNode* pWidgetRoot = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtEmptyNode>( "page", "testscene" );
	pWidgetRoot->SetAnchorPoint( 0.5f, 0.5f );
	pWidgetRoot->SetPositionType( eScreenValue_Percentage );
	pWidgetRoot->SetPosition( 0.5f,0.5f );
	pWidgetRoot->Create();

	CGUIWgtStaticImage* pWidget_staticimage =  
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticImage>( "staticimage_0", "testscene");
	pWidget_staticimage->SetParent( pWidgetRoot );
	pWidget_staticimage->SetImage( "BGIMAGE", "color_white" );
	pWidget_staticimage->SetSize( 50, 50 );
	pWidget_staticimage->SetPosition( 100, 100 );
	pWidget_staticimage->SetAnchorPoint( 0.5, 0.5 );
	pWidget_staticimage->SetClipChildren( true );
	pWidget_staticimage->Create();

	CGUIWgtStaticImage* pWidget_staticimage2 = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticImage>( "staticimage_1", "testscene");
	pWidget_staticimage2->SetParent( pWidget_staticimage );
	pWidget_staticimage2->SetImage( "BGIMAGE", "checkbutton_glow_checked" );
	pWidget_staticimage2->SetAnchorPoint( 0.5, 0.5 );
	pWidget_staticimage2->SetSize( 25, 25 );
	pWidget_staticimage2->SetPosition( 0, 0 );
	pWidget_staticimage2->Create();

	CGUIWgtStaticText* pWidget_statictext = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticText>( "staticText_1", "testscene");
	pWidget_statictext->SetParent( pWidgetRoot );
	pWidget_statictext->SetAnchorPoint( 0.5, 0.5 );
	pWidget_statictext->SetSize( 250, 25 );
	pWidget_statictext->SetPosition( 300, 300 );
	pWidget_statictext->SetTextContentUTF8("hello libguiex");
	pWidget_statictext->SetTextColor(CGUIColor( 1,0,0,1));
	pWidget_statictext->Create();

	CGUIWgtButton* pWidget_button = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtButton>( "button", "testscene");
	pWidget_button->SetParent( pWidgetRoot );
	pWidget_button->SetImage( "BTN_NORMAL", "button_blue_normal" );
	pWidget_button->SetImage( "BTN_HOVER", "button_blue_hover" );
	pWidget_button->SetImage( "BTN_PUSH", "button_blue_push" );
	pWidget_button->SetImage( "BTN_DISABLE", "button_blue_disable" );
	pWidget_button->SetAnchorPoint( 0.5, 0.5 );
	pWidget_button->SetPosition( 400, 200 );
	pWidget_button->SetTextContentUTF8("ok");
	pWidget_button->SetTextColor(CGUIColor( 1,1,1,1));
	pWidget_button->Create();

	CGUIWgtButton* pWidget_button2 = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtButton>( "button2", "testscene");
	pWidget_button2->SetParent( pWidgetRoot );
	pWidget_button2->SetImage( "BTN_NORMAL", "button_blue_normal" );
	pWidget_button2->SetImage( "BTN_HOVER", "button_blue_hover" );
	pWidget_button2->SetImage( "BTN_PUSH", "button_blue_push" );
	pWidget_button2->SetImage( "BTN_DISABLE", "button_blue_disable" );
	pWidget_button2->SetAnchorPoint( 0.5, 0.5 );
	pWidget_button2->SetPosition( 600, 200 );
	pWidget_button2->SetTextContentUTF8("cancel");
	pWidget_button2->SetTextColor(CGUIColor( 1,1,1,1));
	pWidget_button2->SetRotation( 0,0,45 );
	pWidget_button2->Create();

	CGUIWgtCheckButton* pWidget_checkbutton = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtCheckButton>( "checkbutton", "testscene");
	pWidget_checkbutton->SetParent( pWidgetRoot );
	pWidget_checkbutton->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_checkbutton->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_checkbutton->SetSize( 20, 20 );
	pWidget_checkbutton->SetAnchorPoint( 0.5, 0.5 );
	pWidget_checkbutton->SetPosition( 400, 300 );
	pWidget_checkbutton->Create();

	CGUIWgtCheckButton* pWidget_checkbutton2 = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtCheckButton>( "checkbutton2", "testscene");
	pWidget_checkbutton2->SetParent( pWidgetRoot );
	pWidget_checkbutton2->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_checkbutton2->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_checkbutton2->SetSize( 20, 20 );
	pWidget_checkbutton2->SetAnchorPoint( 0.5, 0.5 );
	pWidget_checkbutton2->SetPosition( 450, 300 );
	pWidget_checkbutton2->Create();

	CGUIWgtEmptyNode* pWidgetRadioGroup = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtEmptyNode>( "radiogroup", "testscene");
	pWidgetRadioGroup->SetParent( pWidgetRoot );
	pWidgetRadioGroup->Create();

	CGUIWgtRadioButton* pWidget_radiobutton = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtRadioButton>( "radiobutton", "testscene");
	pWidget_radiobutton->SetParent( pWidgetRadioGroup );
	pWidget_radiobutton->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_radiobutton->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_radiobutton->SetSize( 20, 20 );
	pWidget_radiobutton->SetAnchorPoint( 0.5, 0.5 );
	pWidget_radiobutton->SetPosition( 400, 350 );
	pWidget_radiobutton->Create();

	CGUIWgtRadioButton* pWidget_radiobutton2 =
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtRadioButton>( "radiobutton2", "testscene");
	pWidget_radiobutton2->SetParent( pWidgetRadioGroup );
	pWidget_radiobutton2->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_radiobutton2->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_radiobutton2->SetSize( 20, 20 );
	pWidget_radiobutton2->SetAnchorPoint( 0.5, 0.5 );
	pWidget_radiobutton2->SetPosition( 450, 350 );
	pWidget_radiobutton2->Create();


	CGUIWgtEditBox* pWidget_editbox = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtEditBox>( "editbox", "testscene");
	pWidget_editbox->SetParent( pWidgetRoot );
	pWidget_editbox->SetImage( "EDIT_BG", "edit_bg" );
	pWidget_editbox->SetImage( "EDIT_CURSOR", "color_white" );
	pWidget_editbox->SetSize( 150, 25 );
	pWidget_editbox->SetAnchorPoint( 0.5, 0.5 );
	pWidget_editbox->SetPosition( 400, 400 );
	pWidget_editbox->SetCursorSize( CGUISize(2,20));
	pWidget_editbox->Create();


	CGUIWgtMultiEditBox* pWidget_multieditbox = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtMultiEditBox>( "multieditbox", "testscene");
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
	pWidget_multieditbox->SetCursorSize( CGUISize(2,20));
	pWidget_multieditbox->Create();


	CGUIWgtPanel* pWidget_panel= 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtPanel>( "panel", "testscene");
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


	CGUIWgtTabControl* pWidget_tabcontroll= 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtTabControl>( "tabcontrol", "testscene");
	pWidget_tabcontroll->SetParent( pWidget_panel );
	pWidget_tabcontroll->Create();


	CGUIWgtTabButton* pWidget_tabbutton = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtTabButton>( "tabbutton", "testscene");
	pWidget_tabbutton->SetParent( pWidget_tabcontroll );
	pWidget_tabbutton->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_tabbutton->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_tabbutton->SetSizeType( eScreenValue_Percentage );
	pWidget_tabbutton->SetPositionType( eScreenValue_Percentage );
	pWidget_tabbutton->SetSize( 0.33f,0.1f );
	pWidget_tabbutton->SetPosition( 0, 0 );
	pWidget_tabbutton->Create();

	CGUIWgtPanel* pWidget_panel1= 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtPanel>( "panel1", "testscene");
	pWidget_panel1->SetParent( pWidget_tabbutton );
	pWidget_panel1->SetImage( "BGIMAGE", "color_white" );
	pWidget_panel1->SetSizeType( eScreenValue_Percentage );
	pWidget_panel1->SetSize( 1.0f, 0.5f);
	pWidget_panel1->SetColor(CGUIColor(1,0,0,1));
	pWidget_panel1->Create();

	CGUIWgtTabButton* pWidget_tabbutton2 = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtTabButton>( "tabbutton2", "testscene");
	pWidget_tabbutton2->SetParent( pWidget_tabcontroll );
	pWidget_tabbutton2->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_tabbutton2->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_tabbutton2->SetSizeType( eScreenValue_Percentage );
	pWidget_tabbutton2->SetPositionType( eScreenValue_Percentage );
	pWidget_tabbutton2->SetSize( 0.33f, 0.1f );
	pWidget_tabbutton2->SetPosition( 0.33f, 0 );
	pWidget_tabbutton2->Create();

	CGUIWgtPanel* pWidget_panel2=
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtPanel>( "panel2", "testscene");
	pWidget_panel2->SetParent( pWidget_tabbutton2 );
	pWidget_panel2->SetImage( "BGIMAGE", "color_white" );
	pWidget_panel2->SetSizeType( eScreenValue_Percentage );
	pWidget_panel2->SetSize( 1.0f, 0.5f);
	pWidget_panel2->SetColor(CGUIColor(0,1,0,1));
	pWidget_panel2->Create();

	CGUIWgtTabButton* pWidget_tabbutton3 = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtTabButton>( "tabbutton3", "testscene");
	pWidget_tabbutton3->SetParent( pWidget_tabcontroll );
	pWidget_tabbutton3->SetImage( "BTN_NORMAL", "button_hilight_square" );
	pWidget_tabbutton3->SetImage( "BTN_NORMAL_CHECKED", "checkbutton_glow_checked" );
	pWidget_tabbutton3->SetSizeType( eScreenValue_Percentage );
	pWidget_tabbutton3->SetPositionType( eScreenValue_Percentage );
	pWidget_tabbutton3->SetSize( 0.33f, 0.1f );
	pWidget_tabbutton3->SetPosition( 0.66f, 0 );
	pWidget_tabbutton3->Create();

	CGUIWgtPanel* pWidget_panel3= 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtPanel>( "panel3", "testscene");
	pWidget_panel3->SetParent( pWidget_tabbutton3 );
	pWidget_panel3->SetImage( "BGIMAGE", "color_white" );
	pWidget_panel3->SetSizeType( eScreenValue_Percentage );
	pWidget_panel3->SetSize( 1.0f, 0.5f);
	pWidget_panel3->SetColor(CGUIColor(0,0,1,1));
	pWidget_panel3->Create();


	CGUIWgtProgress* pWidget_progress= 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtProgress>( "progress", "testscene");
	pWidget_progress->SetParent( pWidgetRoot );
	pWidget_progress->SetImage( "BACKGROUND_IMG", "color_white" );
	pWidget_progress->SetImage( "FOREGROUND_IMG", "panel_bg_grade_green" );
	pWidget_progress->SetSize( 200, 25);
	pWidget_progress->SetMaximumProgress( 100 );
	pWidget_progress->SetCurrentProgress( 50 );
	pWidget_progress->SetPosition( 500,500 );
	pWidget_progress->Create();

	CGUIWgtStaticText* pWidget_frame = 
		CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticText>( "frame", "testscene");
	pWidget_frame->SetParent( pWidgetRoot );
	pWidget_frame->SetAnchorPoint( 1, 0 );
	pWidget_frame->SetSize( 250, 25 );
	pWidget_frame->SetPositionType( eScreenValue_Percentage );
	pWidget_frame->SetPosition( 1, 0.1f );
	pWidget_frame->SetTextContentUTF8("frame = ???");
	pWidget_frame->SetTextColor(CGUIColor( 1,1,0,1));
	pWidget_frame->Create();

	CGUIAsManager* pAsManager = CGUIAsManager::Instance();
	{
		CGUIAsAlpha* pAsAlpha = pAsManager->AllocateResource<CGUIAsAlpha>( );
		pAsAlpha->SetInterpolationValue( 0, 1, 2 );
		pAsAlpha->SetReceiver( pWidgetRoot );
		pWidgetRoot->PlayAs( pAsAlpha );
		pAsAlpha->RefRelease();
	}

	{
		CGUIAsScale* pAsScale = pAsManager->AllocateResource<CGUIAsScale>( );
		pAsScale->SetInterpolationValue( CGUISize(0.2f,0.2f), CGUISize(1,1), 4 );
		pAsScale->SetReceiver( pWidget_panel );
		pAsScale->SetInterpolationType( eInterpolationType_EaseInOut );
		pWidget_panel->PlayAs( pAsScale );
		pAsScale->RefRelease();

		CGUIAsRotation* pAsRotation = pAsManager->AllocateResource<CGUIAsRotation>( );
		pAsRotation->SetInterpolationValue( CGUIVector3(0,0,360*5), CGUIVector3(), 4 );
		pAsRotation->SetReceiver( pWidget_panel );
		pWidget_panel->PlayAs( pAsRotation );
		pAsRotation->RefRelease();
	}

	{
		CGUIAsPosition* pAsPos1 = pAsManager->AllocateResource<CGUIAsPosition>( );
		pAsPos1->SetInterpolationValue( CGUIVector2(0,0), CGUIVector2(500,500), 3 );
		pAsPos1->SetReceiver( pWidget_staticimage );
		pWidget_staticimage->PlayAs( pAsPos1 );
		pAsPos1->RefRelease();

		CGUIAsPosition* pAsPos2 = pAsManager->AllocateResource<CGUIAsPosition>( );
		pAsPos2->SetInterpolationValue( CGUIVector2(500,500), CGUIVector2(100,500), 2 );
		pAsPos2->SetReceiver( pWidget_staticimage );
		pAsPos1->PushSuccessor(pAsPos2);
		pAsPos2->RefRelease();
	}

	{
		CGUIAsContainer* pAsContainer = pAsManager->AllocateResource<CGUIAsContainer>( );
		CGUIAsColor* pAsColor = NULL;
		CGUIAsContainItemInfo aItem;

		pAsColor = pAsManager->AllocateResource<CGUIAsColor>( );
		pAsColor->SetInterpolationValue( CGUIColor(1,0,0, 1), CGUIColor(0,1,0,1), 2 );
		aItem.m_pAs = pAsColor;
		aItem.m_fBeginTime = 0;
		pAsContainer->AddItem( aItem );
		pAsColor->RefRelease();

		pAsColor = pAsManager->AllocateResource<CGUIAsColor>( );
		pAsColor->SetInterpolationValue( CGUIColor(0,1,0, 1), CGUIColor(0,0,1,1), 2 );
		aItem.m_pAs = pAsColor;
		aItem.m_fBeginTime = 2;
		pAsContainer->AddItem( aItem );
		pAsColor->RefRelease();

		pAsColor = pAsManager->AllocateResource<CGUIAsColor>( );
		pAsColor->SetInterpolationValue( CGUIColor(0,0,1, 1), CGUIColor(1,0,0,1), 2 );
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
		CGUIAsRotation* pAsRotation = pAsManager->AllocateResource<CGUIAsRotation>( );
		pAsRotation->SetInterpolationValue( CGUIVector3(0,0,360), CGUIVector3(), 4 );
		pAsRotation->SetReceiver( pWidget_staticimage );
		pAsRotation->SetLooping( true );
		pWidget_staticimage->PlayAs( pAsRotation );
		pAsRotation->RefRelease();
	}

	{
		CGUIAsScale* pAsScale = pAsManager->AllocateResource<CGUIAsScale>( );
		pAsScale->SetInterpolationValue( CGUISize(5,5), CGUISize(1,1), 6 );
		pAsScale->SetReceiver( pWidget_staticimage2 );
		pWidget_staticimage2->PlayAs( pAsScale );
		pAsScale->RefRelease();

		CGUIAsAlpha* pAsAlpha = pAsManager->AllocateResource<CGUIAsAlpha>( );
		pAsAlpha->SetInterpolationValue( 0, 1, 3 );
		pAsAlpha->SetReceiver( pWidget_staticimage2 );
		pWidget_staticimage2->PlayAs( pAsAlpha );
		pAsAlpha->RefRelease();

		CGUIAsRotation* pAsRotation = pAsManager->AllocateResource<CGUIAsRotation>( );
		pAsRotation->SetInterpolationValue( CGUIVector3(0,0,360*2), CGUIVector3(), 4 );
		pAsRotation->SetReceiver( pWidget_staticimage2 );
		pWidget_staticimage2->PlayAs( pAsRotation );
		pAsRotation->RefRelease();
	}

	{
		CGUIAsRotation* pAsRotation = pAsManager->AllocateResource<CGUIAsRotation>( );
		pAsRotation->SetInterpolationValue( CGUIVector3(0,0,360*5), CGUIVector3(), 1 );
		pAsRotation->SetReceiver( pWidget_statictext );
		pWidget_statictext->PlayAs( pAsRotation );
		pAsRotation->RefRelease();

		CGUIAsAlpha* pAsAlpha = pAsManager->AllocateResource<CGUIAsAlpha>( );
		pAsAlpha->SetInterpolationValue( 0, 1, 1 );
		pAsAlpha->SetReceiver( pWidget_statictext );
		pWidget_statictext->PlayAs( pAsAlpha );
		pAsAlpha->RefRelease();

		CGUIAsScale* pAsScale = pAsManager->AllocateResource<CGUIAsScale>( );
		pAsScale->SetInterpolationValue( CGUISize(0,0), CGUISize(2,2), 1 );
		pAsScale->SetReceiver( pWidget_statictext );
		pWidget_statictext->PlayAs( pAsScale );
		pAsScale->RefRelease();
	}

	CGUIWidgetManager::Instance()->AddPage( pWidgetRoot );
	GSystem->OpenPage( pWidgetRoot );

	return 0;
}

void CGUIFrameworkTest::ReleaseGame( )
{

}

void CGUIFrameworkTest::PreUpdate( real fDeltaTime )
{
	CGUIWgtStaticText* pWidget = 
		CGUIWidgetManager::Instance()->GetWidgetWithTypeCheck<CGUIWgtStaticText>( "frame", "testscene" );
	char buf[512];
	snprintf( buf, 512, "frame = %d", GSystem->GetFPS() );
	pWidget->SetTextContentUTF8( buf );
}

void CGUIFrameworkTest::PostUpdate( real fDeltaTime )
{

}