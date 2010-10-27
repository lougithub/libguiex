/** 
 * @file sample_opengl.cpp
 * @brief test and show how to use opengl as render

 * @author ken
 * @date 2006-07-11
 */

#include <libguiex_core\guiex.h>
#include <libguiex_widget\guiwgt.h>

guiex::CGUIWidget* SampleInit()
{
	guiex::CGUIWidget* pWidgetRoot = GUI_CREATE_WIDGET("CGUIWgtEmptyNode", "page", "testproject");
	pWidgetRoot->Create();

	guiex::CGUIWidget* pWidget = GUI_CREATE_WIDGET("CGUIWgtStaticImage", "staticimage_0", "testproject");
	pWidget->SetParent( pWidgetRoot );
	pWidget->SetImage( "color_white", "color_white" );
	pWidget->SetValue( "Image", "color_white" );
	pWidget->NEWSetSize( 50, 50 );
	pWidget->NEWSetPosition( 100, 100 );
	pWidget->SetTagPoint( 0.5, 0.5 );
	pWidget->Create();

	guiex::CGUIWidget* pWidget2 = GUI_CREATE_WIDGET("CGUIWgtStaticImage", "staticimage_1", "testproject");
	pWidget2->SetParent( pWidget );
	pWidget2->SetImage( "checkbutton_glow_checked", "checkbutton_glow_checked" );
	pWidget2->SetValue( "Image", "checkbutton_glow_checked" );
	pWidget2->SetTagPoint( 0.5, 0.5 );
	pWidget2->NEWSetSize( 25, 25 );
	pWidget2->NEWSetPosition( 25, 25 );
	pWidget2->Create();

	{
		guiex::CGUIAsPosition* pAsPos1 = static_cast<guiex::CGUIAsPosition*>(guiex::CGUIAsFactory::Instance()->GenerateAs("CGUIAsPosition", pWidget ));
		pAsPos1->SetPositionSequence( guiex::CGUIVector2(0,0), guiex::CGUIVector2(500,500), 3 );
		guiex::CGUIAsPosition* pAsPos2 = static_cast<guiex::CGUIAsPosition*>(guiex::CGUIAsFactory::Instance()->GenerateAs("CGUIAsPosition", pWidget ));
		pAsPos2->SetPositionSequence( guiex::CGUIVector2(500,500), guiex::CGUIVector2(100,500), 2 );
		pAsPos1->PushSuccessor(pAsPos2);
		pWidget->AddAs( pAsPos1 );
	}

	{
		guiex::CGUIAsRotation* pAsRotation = static_cast<guiex::CGUIAsRotation*>(guiex::CGUIAsFactory::Instance()->GenerateAs("CGUIAsRotation", pWidget ));
		pAsRotation->SetRotationSequence( 3.1415f, 0, 4 );
		pWidget->AddAs( pAsRotation );
	}

	{
		guiex::CGUIAsScale* pAsScale = static_cast<guiex::CGUIAsScale*>(guiex::CGUIAsFactory::Instance()->GenerateAs("CGUIAsScale", pWidget2 ));
		pAsScale->SetScaleSequence( guiex::CGUISize(5,5), guiex::CGUISize(1,1), 6 );
		pWidget2->AddAs( pAsScale );

		guiex::CGUIAsAlpha* pAsAlpha = static_cast<guiex::CGUIAsAlpha*>(guiex::CGUIAsFactory::Instance()->GenerateAs("CGUIAsAlpha", pWidget2 ));
		pAsAlpha->SetAlphaSequence( 0, 1, 3 );
		pWidget2->AddAs( pAsAlpha );

		guiex::CGUIAsRotation* pAsRotation = static_cast<guiex::CGUIAsRotation*>(guiex::CGUIAsFactory::Instance()->GenerateAs("CGUIAsRotation", pWidget2 ));
		pAsRotation->SetRotationSequence( 3.1415f*5.0f, 0, 4 );
		pWidget2->AddAs( pAsRotation );
	}

	return pWidgetRoot;
}
