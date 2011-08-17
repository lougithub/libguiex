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

CGUIFrameworkBase* GUIEXCreateFramework( )
{
	return new CGUIFrameworkTest( );
}

const char* GUIEXGetDataDir()
{
	//should relative to dir "data"
	return "test";
}

int32 CGUIFrameworkTest::InitializeGame()
{
	CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");
	CGUISceneManager::Instance()->LoadResources( "common" );
	CGUISceneManager::Instance()->LoadWidgets( "common" );	
	CGUISceneManager::Instance()->LoadResources( "sound" );	
	CGUISceneManager::Instance()->LoadWidgets( "sound" );
	CGUIWidget* pWidget = NULL;
	pWidget = CGUIWidgetManager::Instance()->GetPage( "sound.xml", "sound" );
	GSystem->GetUICanvas()->OpenUIPage(pWidget);		
	pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common" );
	GSystem->GetUICanvas()->OpenUIPage( pWidget);
	return 0;
}


