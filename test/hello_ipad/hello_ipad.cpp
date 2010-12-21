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
	CGUISceneManager::Instance()->LoadResources( "common.uip" );
	CGUISceneManager::Instance()->LoadWidgets( "common.uip" );
	CGUIWidget* pWidget = NULL;
	pWidget = CGUIWidgetManager::Instance()->GetPage( "dialog_ok.xml", "common.uip" );
	CGUISystem::Instance()->OpenPage(pWidget);	
	pWidget = CGUIWidgetManager::Instance()->GetPage( "dialog_okcancel.xml", "common.uip" );
	CGUISystem::Instance()->OpenPage(pWidget);	
	pWidget = CGUIWidgetManager::Instance()->GetPage( "showfps.xml", "common.uip" );
	CGUISystem::Instance()->OpenPage(pWidget);		
	return 0;
}


