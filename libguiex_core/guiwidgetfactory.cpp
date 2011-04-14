/** 
* @file guiwidgetfactory.cpp
* @brief factory of widget, used to 
* generate and destory widget
* @author ken
* @date 2006-06-07
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guiwidgetfactory.h>
#include <libguiex_core/guiwidgetgenerator.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guilogmsgmanager.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUIEXPORT CGUIWidgetFactory* GetWidgetFactory()
	{
		return CGUIWidgetFactory::Instance();
	}
	//------------------------------------------------------------------------------
	GUIEXPORT int32 RegisterWidgetGenerator( const CGUIString& rWidgetType, const CGUIString& rModuleName)
	{
		return CGUIWidgetFactory::Instance()->RegisterGenerator( rWidgetType, rModuleName);
	}
	//------------------------------------------------------------------------------
	GUIEXPORT int32 UnregisterWidgetGenerator(
		const CGUIString& rWidgetType)
	{
		return CGUIWidgetFactory::Instance()->UnregisterGenerator( rWidgetType);
	}
	//------------------------------------------------------------------------------
	
	//------------------------------------------------------------------------------
	CGUIWidgetFactory * CGUIWidgetFactory::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIWidgetFactory::CGUIWidgetFactory()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIWidgetFactory::CGUIWidgetFactory]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIWidgetFactory::~CGUIWidgetFactory()
	{
		//unregister all generator
		UnregisterAllGenerator();

		//unregister all module
		UnregisterAllModule();

		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIWidgetFactory* CGUIWidgetFactory::Instance()
	{
		return m_pSingleton;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register module
	* @return 
	*		- 0 for success
	*		- -1 for failed
	* @exception CGUIException
	*/
	int32	CGUIWidgetFactory::RegisterModule( const CGUIString& rModuleName )
	{
		GUI_TRACE( GUI_FORMAT( "[CGUIWidgetFactory::RegisterModule]:	Register widget module <%s> \n",
			rModuleName.c_str()));

		if( m_mapModule.find( rModuleName) == m_mapModule.end())
		{
			GUI_DYNLIB_HANDLE d_handle = GUI_DYNLIB_LOAD(rModuleName.c_str());
			if (d_handle == NULL)
			{
				throw CGUIException(
					"[CGUIWidgetFactory::RegisterModule] failed to load module <%s>",
					rModuleName.c_str());
				return -1;
			}
			else
			{
				m_mapModule.insert(std::make_pair( rModuleName, d_handle ));
				return 0;
			}
		}
		else
		{
			throw CGUIException(
				"[CGUIWidgetFactory::RegisterModule] the module <%s> has been registed",
				rModuleName.c_str());
			return -1;
		}		
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register module
	* @return 
	*		- 0 for success
	*		- -1 for failed
	* @exception CGUIException
	*/
	int32	CGUIWidgetFactory::UnregisterModule( const CGUIString& rModuleName )
	{
		GUI_TRACE( GUI_FORMAT( "[CGUIWidgetFactory::UnregisterModule]:    Unregister widget module <%s> \n",
			rModuleName.c_str()));

		TMapModule::iterator itor = m_mapModule.find( rModuleName );
		if( itor == m_mapModule.end() )
		{
			throw CGUIException(
				"[CGUIWidgetFactory::UnregisterModule] the module <%s> has been unregisted",
				rModuleName.c_str());
			return -1;
		}
		else
		{
			GUI_DYNLIB_UNLOAD((*itor).second);
			m_mapModule.erase( itor );
			return 0;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register all widget generator in this module
	* @param pFunName string name of function which create all generator,
	* this function is located in the dll,if the value is NULL,the default
	* name is GetAllGenerator_<rModuleName>
	* @return 
	*		- 0 for success
	*		- -1 for failed
	* @exception CGUIException
	*/
	int32	CGUIWidgetFactory::RegisterAllGenerator(const CGUIString& rModuleName,const char* pFunName)
	{
		GUI_TRACE( GUI_FORMAT( "[CGUIWidgetFactory::RegisterAllGenerator]:    Register all widget generators from module <%s>\n",
			rModuleName.c_str()));

		//get module
		TMapModule::iterator itor = m_mapModule.find( rModuleName );
		if( itor == m_mapModule.end() )
		{
			if( 0 != RegisterModule( rModuleName ))
			{
				return -1;
			}
			itor = m_mapModule.find( rModuleName );
		}

		//get function name
		CGUIString strFunName;
		if( pFunName )
		{
			strFunName = pFunName;
		}
		else
		{
			strFunName = "GetAllGenerators";
		}

		//get function
		FunRegisterAllWidgets pFunc = (FunRegisterAllWidgets)GUI_DYNLIB_GETSYM((*itor).second, strFunName.c_str());
		if( !pFunc )
		{
			throw CGUIException(
				"[CGUIWidgetFactory::RegisterAllGenerator] failed to get function <%s> from module <%s>",
				strFunName.c_str(),
				rModuleName.c_str());
			return -1;
		}

		//register generator
		CGUIWidgetGenerator** pGenerator = pFunc();
		while(*pGenerator)
		{
			RegisterGenerator( *pGenerator);
			pGenerator ++;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register generator
	* @param pFunName string name of function which create the generator,
	* this function is located in the dll,if the value is NULL,the default
	* name is GetGenerator_<rWidgetType>
	* @return 
	*		- 0 for success
	*		- -1 for failed
	* @exception CGUIException
	*/
	int32	CGUIWidgetFactory::RegisterGenerator(const CGUIString& rWidgetType, 
		const CGUIString& rModuleName, 
		const char* pFunName )
	{
		GUI_TRACE( GUI_FORMAT( "[CGUIWidgetFactory::RegisterGenerator]:    Register widget generator <%s> from module <%s>\n",
			rWidgetType.c_str(), rModuleName.c_str()));

		//get module
		TMapModule::iterator itor = m_mapModule.find( rModuleName );
		if( itor == m_mapModule.end() )
		{
			if( 0 != RegisterModule( rModuleName ))
			{
				return -1;
			}
			itor = m_mapModule.find( rModuleName );
		}


		//get function name
		CGUIString strFunName;
		if( pFunName )
		{
			strFunName = pFunName;
		}
		else
		{
			strFunName = "GetGenerator_"+rWidgetType;
		}

		//get function
		FunCreateGenerator pFunc = (FunCreateGenerator)GUI_DYNLIB_GETSYM((*itor).second, strFunName.c_str());
		if( !pFunc )
		{
			throw CGUIException(
				"[CGUIWidgetFactory::RegisterGenerator] failed to get function <%s> for widget <%s> from module <%s>",
				strFunName.c_str(),
				rWidgetType.c_str(),
				rModuleName.c_str());
			return -1;
		}

		//register generator
		return RegisterGenerator( pFunc());
	}
	//------------------------------------------------------------------------------
	/**
	* @brief unregister all module
	*/ 
	void CGUIWidgetFactory::UnregisterAllModule()
	{
		TMapModule::iterator itorEnd2 = m_mapModule.end();
		for( TMapModule::iterator itor = m_mapModule.begin();
			itor != itorEnd2;
			itor++)
		{
			GUI_DYNLIB_UNLOAD((*itor).second);
		}
		m_mapModule.clear();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register generator
	* @return 
	*		- 0 for success
	*		- -1 for failed
	* @exception CGUIException
	*/
	int32	CGUIWidgetFactory::RegisterGenerator(CGUIWidgetGenerator* pGenerator)
	{
		GUI_ASSERT(pGenerator, "wrong parameter");
		if( m_mapGenerator.find( pGenerator->GetWidgetType()) == m_mapGenerator.end())
		{
			m_mapGenerator.insert(std::make_pair(pGenerator->GetWidgetType(), pGenerator));
			return 0;
		}
		else
		{
			throw CGUIException(
				"[CGUIWidgetFactory::RegisterGenerator] the generator <%s> has been registed",
				pGenerator->GetWidgetType().c_str());
			return -1;
		}		
	}
	//------------------------------------------------------------------------------
	/**
	* @brief unregister a generator
	* @param pGenerator the pointer of a generator
	* @return 
	*		- 0 for success
	*		- -1 for failed
	* @exception CGUIException
	*/
	int32	CGUIWidgetFactory::UnregisterGenerator(CGUIWidgetGenerator* pGenerator)
	{
		GUI_ASSERT(pGenerator, "wrong parameter");
		TMapGenerator::iterator itor = m_mapGenerator.find( pGenerator->GetWidgetType());
		if( itor == m_mapGenerator.end() )
		{
			throw CGUIException(
				"[CGUIWidgetFactory::UnregisterGenerator] the generator <%s> has been unregisted",
				pGenerator->GetWidgetType().c_str());
			return -1;
		}
		else
		{
			m_mapGenerator.erase( itor );
			return 0;
		}		
	}
	//------------------------------------------------------------------------------
	/**
	* @brief ungister a generator
	* @param rType the type of a generator
	* @return 
	*		- 0 for success
	*		- -1 for failed
	* @exception CGUIException
	*/
	int32 CGUIWidgetFactory::UnregisterGenerator(const CGUIString& rType )
	{
		GUI_TRACE( GUI_FORMAT( "[CGUIWidgetFactory::UnregisterGenerator]:    Unregister widget generator <%s>\n",
			rType.c_str() ));

		TMapGenerator::iterator itor = m_mapGenerator.find( rType );
		if( itor == m_mapGenerator.end() )
		{
			throw CGUIException(
				"[CGUIWidgetFactory::UnregisterGenerator] the generator <%s> has been unregisted",
				rType.c_str());
			return -1;
		}
		else
		{
			(*itor).second->DestroySelf();
			m_mapGenerator.erase( itor );
			return 0;
		}	
	}
	//------------------------------------------------------------------------------

	/**
	* @brief unregister all generator
	*/
	void CGUIWidgetFactory::UnregisterAllGenerator()
	{
		TMapGenerator::iterator itorEnd = m_mapGenerator.end();
		for( TMapGenerator::iterator itor = m_mapGenerator.begin();
			itor != itorEnd;
			itor++)
		{
			CGUIWidgetGenerator* pGenerator = itor->second;
			pGenerator->DestroySelf();
		}
		m_mapGenerator.clear();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create a widget by widget name
	* @return pointer of widget, NULL for failed to find this widget
	* @exception CGUIException
	*/
	CGUIWidget* CGUIWidgetFactory::CreateWidget(const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
	{
		GUI_TRACE( GUI_FORMAT( "[CGUIWidgetFactory::CreateWidget]:    Create widget <%s> from generator <%s>\n",
			rName.c_str(), rType.c_str()));

		TMapGenerator::iterator itor = m_mapGenerator.find( rType );
		if( itor == m_mapGenerator.end())
		{
			throw CGUIException(
				"[CGUIWidgetFactory::CreateWidget] failed to find widget generator <%s>",
				rType.c_str());
			return NULL;			
		}
		else
		{
			CGUIWidget* pWidget = (*itor).second->GenerateWidget( rName, rSceneName);
			if( !pWidget )
			{
				throw CGUIException(
					"[CGUIWidgetFactory::CreateWidget] failed to create widget <%s>",
					rType.c_str());
				return NULL;
			}
			else
			{
				pWidget->SetGenerator((*itor).second);
				return pWidget;
			}
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destroy widget
	* @return 
	*	- 0 for success
	*	- -1 for failed
	*/
	int32 CGUIWidgetFactory::DestoryWidget( CGUIWidget* pWidget)
	{
		GUI_ASSERT(pWidget, "wrong parameter");
		const CGUIWidgetGenerator* pGenerator = pWidget->GetGenerator();
		pWidget->OnDestroy();
		if( !pGenerator )
		{
			delete pWidget;
		}
		else
		{
			pGenerator->DestroyWidget(pWidget);
		}
		return 0;
	}
	//------------------------------------------------------------------------------

}//namespace guiex


