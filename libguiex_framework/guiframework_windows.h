/** 
* @file guiframework_windows.h
* @brief framework to use this engine
* @author ken
* @date 2010-12-10
*/

#ifndef __GUI_FRAMEWORKWINDOWS_20101210_H__
#define __GUI_FRAMEWORKWINDOWS_20101210_H__

//============================================================================//
// include
//============================================================================// 
#include "guiframeworkbase.h"


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIFramework_Windows : public CGUIFrameworkBase
	{
	public:
		CGUIFramework_Windows( );
		virtual ~CGUIFramework_Windows();


	protected:
		virtual int32 InitializeGame( );
		virtual void ReleaseGame( );

		virtual int32 InitializeSystem( const CGUISize& rScreenSize, const char* pDataPath );
		virtual void ReleaseSystem( );

		virtual void SetupLogSystem( );
		virtual void RegisterInterfaces( );
		virtual void RegisterWidgetGenerators( );

		virtual void PreUpdate( real fDeltaTime );
		virtual void PostUpdate( real fDeltaTime );

		virtual void OutputFatalError( const char* pErrorDesc );

	private:
	};

}//namespace guiex


#endif	//__GUI_FRAMEWORKWINDOWS_20101210_H__

