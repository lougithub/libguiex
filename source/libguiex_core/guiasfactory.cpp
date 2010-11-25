/** 
* @file guiasfactory.cpp
* @brief the factory which is used to create as(action sequence)
* @author ken
* @date 2007-11-21
*/



//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiasfactory.h>
#include <libguiex_core/guias.h>
#include <libguiex_core/guiasgenerator.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guistring.h>


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIAsFactory * CGUIAsFactory::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIAsFactory::CGUIAsFactory()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIAsFactory::CGUIAsFactory]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIAsFactory::~CGUIAsFactory()
	{ 
		UnregisterAllGenerator();

		m_pSingleton = NULL;
	}
	//------------------------------------------------------------------------------
	CGUIAsFactory* CGUIAsFactory::Instance()
	{
		return m_pSingleton;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register generator
	* @return 0 for successful, vice versa
	*/
	int	CGUIAsFactory::RegisterGenerator(const CGUIString& rName, CGUIAsGenerator* pGenerator )
	{
		GUI_ASSERT(pGenerator,"havn't as generator");

		TMapGenerator::iterator itor = m_mapGenerator.find(rName);
		if( itor != m_mapGenerator.end())
		{
			//error
			throw CGUIException(
				"[CGUIAsFactory::RegisterGenerator] the generator <%s> has been registed",
				rName.c_str());
			return -1;
		}
		else
		{
			m_mapGenerator.insert(std::make_pair( rName, pGenerator ));
			return 0;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief unregister generator
	* @return 0 for successful, vice versa
	*/
	int CGUIAsFactory::UnregisterGenerator(const CGUIString& rName)
	{
		TMapGenerator::iterator itor = m_mapGenerator.find( rName );
		if( itor == m_mapGenerator.end() )
		{
			throw CGUIException(
				"[CGUIAsFactory::UnregisterGenerator] the generator <%s> has been unregisted",
				rName.c_str());
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
	* @brief unregister all generator
	*/
	void CGUIAsFactory::UnregisterAllGenerator()
	{
		TMapGenerator::iterator itorEnd2 = m_mapGenerator.end();
		for( TMapGenerator::iterator itor = m_mapGenerator.begin();
			itor != itorEnd2;
			itor++)
		{
			(*itor).second->DestroySelf();
		}
		m_mapGenerator.clear();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create a as by name
	*/
	CGUIAs*	CGUIAsFactory::GenerateAs(const CGUIString& rAsType, const CGUIString& rAsName, const CGUIString& rSceneName)
	{
		TMapGenerator::iterator itor = m_mapGenerator.find( rAsType );
		if( itor == m_mapGenerator.end())
		{
			throw CGUIException(
				"[CGUIAsFactory::GenerateAs] failed to find as generator <%s>",
				rAsType.c_str());
			return NULL;			
		}
		else
		{
			CGUIAs* pAs = (*itor).second->GenerateAs( rAsName, rSceneName );
			if( !pAs )
			{
				throw CGUIException(
					"[CGUIAsFactory::GenerateAs] failed to create as <%s>",
					rAsType.c_str());
				return NULL;
			}
			else
			{
				pAs->SetGenerator((*itor).second);
				return pAs;
			}
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destroy as
	*/
	int	CGUIAsFactory::DestroyAs(CGUIAs* pAs)
	{
		GUI_ASSERT(pAs,"wrong parameter");
		const CGUIAsGenerator* pGenerator = pAs->GetGenerator();
		if( !pGenerator )
		{
			throw CGUIException(
				"[CGUIAsFactory::DestroyAs] failed to delete as");
			return -1;
		}
		else
		{
			GUI_ASSERT(pAs->m_pAsGenerator, "this as isn't created by this factory");
			pGenerator->DestroyAs(pAs);
			return 0;
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex
