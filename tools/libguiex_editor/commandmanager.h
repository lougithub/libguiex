/** 
* @file commandmanager.h
* @brief command manager
* @author ken
* @date 2007-11-30
*/

#ifndef __KEN_COMMAND_MANAGER_20071130_H__
#define __KEN_COMMAND_MANAGER_20071130_H__


//============================================================================//
// include
//============================================================================// 
#include <map>
#include <vector>


//============================================================================//
// declare
//============================================================================// 

class CCommand;


//============================================================================//
// class
//============================================================================// 
class CCommandManager
{
public:
	~CCommandManager();

	/** 
	* @brief get singleton of project config object
	*/
	static CCommandManager* Instance();

	bool	HasUndoCommand() const;
	bool	HasRedoCommand() const;

	void StoreCommand(CCommand* cmd);
	void ClearAllCommand();

	void Undo();
	void Redo();

protected:
	CCommandManager();

protected:
	typedef std::vector<CCommand*>	TCommandList;
	TCommandList	m_listUndo;
	TCommandList	m_listRedo;
};


#endif //__KEN_COMMAND_MANAGER_20071130_H__
