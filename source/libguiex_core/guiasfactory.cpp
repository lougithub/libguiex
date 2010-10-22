/** 
* @file guiasfactory.cpp
* @brief the factory which is used to create as(action sequence)
* @author ken
* @date 2007-11-21
*/



//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guiasfactory.h>
#include <libguiex_core\guias.h>
#include <libguiex_core\guiasgenerator.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guistring.h>


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	GUI_SINGLETON_IMPLEMENT_EX(CGUIAsFactory);
	//------------------------------------------------------------------------------
	CGUIAsFactory::CGUIAsFactory()
	{
	}
	//------------------------------------------------------------------------------
	CGUIAsFactory::~CGUIAsFactory()
	{ 
	}
	//------------------------------------------------------------------------------
	int		CGUIAsFactory::RegisterGenerator(const CGUIString& rName, CGUIAsGenerator* pGenerator )
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
	int		CGUIAsFactory::UnregisterGenerator(const CGUIString& rName)
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
	void	CGUIAsFactory::UnregisterAllGenerator()
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
	CGUIAs*	CGUIAsFactory::GenerateAs(const CGUIString& rName, CGUIWidget* pReceiver)
	{
		TMapGenerator::iterator itor = m_mapGenerator.find( rName );
		if( itor == m_mapGenerator.end())
		{
			throw CGUIException(
				"[CGUIAsFactory::GenerateAs] failed to find as generator <%s>",
				rName.c_str());
			return NULL;			
		}
		else
		{
			CGUIAs* pAs = (*itor).second->GenerateAs(pReceiver);
			if( !pAs )
			{
				throw CGUIException(
					"[CGUIAsFactory::GenerateAs] failed to create as <%s>",
					rName.c_str());
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
	int	CGUIAsFactory::DestroyAs(CGUIAs*	pAs)
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
