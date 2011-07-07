/** 
* @file guiframework_ios.h
* @brief framework to use this engine
* @author ken
* @date 2011-04-08
*/

#ifndef __GUI_FRAMEWORKIOS_20110408_H__
#define __GUI_FRAMEWORKIOS_20110408_H__

//============================================================================//
// include
//============================================================================// 
#include "guiframeworkbase.h"


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIFramework_IOS : public CGUIFrameworkBase
	{
	public:
		CGUIFramework_IOS( );
		virtual ~CGUIFramework_IOS();


	protected:
		virtual int32 InitializeGame( );
		virtual void ReleaseGame( );

		virtual int32 InitializeSystem( const CGUIIntSize& rScreenSize, const char* pDataPath );
		virtual void ReleaseSystem( );

		virtual void SetupLogSystem( );
		virtual void RegisterWidgetGenerators( );
		
		virtual void RegisterInterfaces_Render( );
		virtual void RegisterInterfaces_ImageLoader( );
		virtual void RegisterInterfaces_FileSys( );
		virtual void RegisterInterfaces_Mouse( );
		virtual void RegisterInterfaces_Keyboard( );
		virtual void RegisterInterfaces_Font( );
		virtual void RegisterInterfaces_ConfigFile( );
		virtual void RegisterInterfaces_StringConv( );
		virtual void RegisterInterfaces_Script( );
		virtual void RegisterInterfaces_Physics( );
		virtual void RegisterInterfaces_Sound( );
		virtual void RegisterInterfaces_Ime( );
		virtual void RegisterInterfaces_LocalizationLoader( );

		virtual void PreUpdate( real fDeltaTime );
		virtual void PostUpdate( real fDeltaTime );

		virtual void OutputFatalError( const char* pErrorDesc );

	private:
	};

}//namespace guiex


#endif	//__GUI_FRAMEWORKIOS_20110408_H__

