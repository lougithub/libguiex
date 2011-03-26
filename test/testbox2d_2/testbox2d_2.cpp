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
	CGUISceneManager::Instance()->LoadResources( "box2d.uip" );	
	CGUISceneManager::Instance()->LoadWidgets( "box2d.uip" );
	CGUISceneManager::Instance()->LoadResources( "common.uip" );
	CGUISceneManager::Instance()->LoadWidgets( "common.uip" );
	CGUIWidget* pWidget = NULL;
	pWidget = CGUIWidgetManager::Instance()->GetPage( "sample_1.xml", "box2d.uip" );
	GSystem->GetUICanvas()->OpenUIPage(pWidget);		
	pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common.uip" );
	GSystem->GetUICanvas()->OpenUIPage(pWidget);		
	return 0;
}


