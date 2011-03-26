#include <libguiex_framework/guiframework.h>
using namespace guiex;

class CGUIFrameworkTest : public CGUIFramework
{
	public:
		CGUIFrameworkTest(  )
			:CGUIFramework(  )
		{
		}

	protected:
		virtual int32 InitializeGame( );
};

CGUIFrameworkBase* CreateFramework( )
{
	return new CGUIFrameworkTest( );
}

int32 CGUIFrameworkTest::InitializeGame()
{
	CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");
	CGUISceneManager::Instance()->LoadResources( "login.uip" );
	CGUISceneManager::Instance()->LoadResources( "sound.uip" );	
	CGUISceneManager::Instance()->LoadWidgets( "login.uip" );
	CGUISceneManager::Instance()->LoadWidgets( "common.uip" );
	CGUISceneManager::Instance()->LoadWidgets( "sound.uip" );
	CGUIWidget* pWidget = NULL;
	pWidget = CGUIWidgetManager::Instance()->GetPage( "login.xml", "login.uip" );
	GSystem->GetUICanvas()->OpenUIPage(pWidget);	
	//pWidget = CGUIWidgetManager::Instance()->GetPage( "dialog_okcancel.xml", "common.uip" );
	//CGUISystem::Instance()->OpenUIPage(pWidget);	
	pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common.uip" );
	GSystem->GetUICanvas()->OpenUIPage(pWidget);	
	//pWidget = CGUIWidgetManager::Instance()->GetPage( "sound.xml", "sound.uip" );
	//CGUISystem::Instance()->OpenUIPage(pWidget);		
	return 0;
}


