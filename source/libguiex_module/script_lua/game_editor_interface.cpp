/** 
 * @file game_editor_interface.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-11-11
 */






//============================================================================//
// include
//============================================================================//
#include "game_editor_interface.h"




//============================================================================//
// function
//============================================================================//




namespace game
{
	//------------------------------------------------------------------------------
	void	Login(const guiex::CGUIString& rName, const guiex::CGUIString& rPassword )
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.Login]:	name is <%s>, password is <%s>", rName.c_str(), rPassword.c_str() );
	}
	//------------------------------------------------------------------------------
	void	EnterGameMode( int nGameMode )
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.EnterGameMode]: enter game mode <%d>", nGameMode );
	}
	//------------------------------------------------------------------------------
	void	EnterUIMode( int nUIMode )
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.ETDGUIMode]: enter ui mode <%d>", nUIMode );
	}
	//------------------------------------------------------------------------------
	void	SelectRace( int nRaceID, int nRaceGender )
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.SelectRace]: select race <%d>", nRaceID );
	}
	//------------------------------------------------------------------------------
	int		GetNumLoginCharInfo( )
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.GetNumLoginCharInfo]" );
		return 4;
	}
	//------------------------------------------------------------------------------
	SPlayerBaseInfo	GetNumLoginCharInfo( int nIndex )
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.GetNumLoginCharInfo]" );

		SPlayerBaseInfo aBaseInfo;
		aBaseInfo.career = 0;
		aBaseInfo.gender = 0;
		aBaseInfo.shape = 0;
		aBaseInfo.race = 0;
		strcpy(aBaseInfo. nick, "ken");
		return aBaseInfo;
	}
	//------------------------------------------------------------------------------
	void	SelectChar( int)
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.SelectChar]" );
	}
	//------------------------------------------------------------------------------
	void	CreateChar(const guiex::CGUIString& rModelPrefix, int nGender, const guiex::CGUIString& rNickName)
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.CreateChar]" );
	}
	//------------------------------------------------------------------------------
	guiex::CGUIString GetRaceName( int nRaceID )
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.GetRaceName]" );

		return "ken";
	}
	//------------------------------------------------------------------------------
	int					GetRaceID( const guiex::CGUIString& rModelPrefix )
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.GetRaceID]" );
		return 0;
	}
	//------------------------------------------------------------------------------
	int		GetTarget( )
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.GetTarget]" );
		return 100;
	}
	//------------------------------------------------------------------------------
	void	DeleteChar(int nIndex)
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.DeleteChar]" );
	}
	//------------------------------------------------------------------------------
	void		DoSkill( int nSkillID )
	{
		GUI_LOG->Log( guiex::GUI_LM_DEBUG, "[game.DoSkill]" );
	}
	//------------------------------------------------------------------------------

}


//------------------------------------------------------------------------------



