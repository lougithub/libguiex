/** 
* @file commandmanager.cpp
* @brief command manager
* @author ken
* @date 2007-11-30
*/

//============================================================================//
// include
//============================================================================// 
#include "commandmanager.h"
#include "command.h"

//============================================================================//
// function
//============================================================================// 
CCommandManager::CCommandManager()
{
}
//------------------------------------------------------------------------------
CCommandManager::~CCommandManager()
{

}
//------------------------------------------------------------------------------
CCommandManager* CCommandManager::Instance()
{
	static CCommandManager s_cmdmanager;
	return &s_cmdmanager;
}
//------------------------------------------------------------------------------
void CCommandManager::StoreCommand(CCommand* cmd)
{
	m_listUndo.push_back(cmd);
	ClearRedoCommand();
}
//------------------------------------------------------------------------------
void CCommandManager::ClearAllCommand()
{
	ClearUndoCommand();
	ClearRedoCommand();
}
//------------------------------------------------------------------------------
void CCommandManager::ClearUndoCommand()
{
	for( TCommandList::iterator itor = m_listUndo.begin();
		itor != m_listUndo.end();
		++itor )
	{
		delete (*itor);
	}
	m_listUndo.clear();
}
//------------------------------------------------------------------------------
void CCommandManager::ClearRedoCommand()
{
	for( TCommandList::iterator itor = m_listRedo.begin();
		itor != m_listRedo.end();
		++itor )
	{
		delete (*itor);
	}
	m_listRedo.clear();
}
//------------------------------------------------------------------------------
void CCommandManager::Undo()
{
	if ( m_listUndo.empty() ) 
	{
		return;
	}
	CCommand* cmd = m_listUndo.back();
	cmd->Undo();
	m_listUndo.pop_back( );
	m_listRedo.push_back(cmd);
}
//------------------------------------------------------------------------------
void CCommandManager::Redo()
{
	if ( m_listRedo.empty() )
	{
		return;
	}
	CCommand* cmd = m_listRedo.back();
	cmd->Execute();
	m_listRedo.pop_back( );
	m_listUndo.push_back(cmd);
}
//------------------------------------------------------------------------------
bool CCommandManager::HasUndoCommand() const
{
	return m_listUndo.empty()==false;
}
//------------------------------------------------------------------------------
bool CCommandManager::HasRedoCommand() const
{
	return m_listRedo.empty()==false;
}
//------------------------------------------------------------------------------


