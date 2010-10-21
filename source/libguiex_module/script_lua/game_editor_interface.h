/** 
 * @file game_editor_interface.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-11-11
 */






#ifndef __KEN_GAME_EDITOR_INTERFACE_20091111_H__
#define __KEN_GAME_EDITOR_INTERFACE_20091111_H__





//============================================================================//
// include
//============================================================================//
#include <libguiex_core\guiex.h>




//============================================================================//
// declare
//============================================================================//

enum	ETDGGameMode
{
	eGM_None,
	eGM_Title,
	eGM_World,
};

enum	ETDGUIMode
{
	eUI_None,
	eUI_Loading,
	eUI_Login,
	eUI_SelectPlayer,
	eUI_CreatePlayer,
	eUI_World,
};


#define NICK_MAXLEN	16

#define GENDER_MALE		0
#define GENDER_FEMALE	1

//============================================================================//
// class
//============================================================================//


//============================================================================//
// function
//============================================================================//
namespace game
{
	struct SPlayerBaseInfo {
		char nick[(NICK_MAXLEN)+1];
		int		race;
		int		gender;
		int		career;
		int		shape;
	} ;



	void	Login(const guiex::CGUIString& rName, const guiex::CGUIString& rPassword );
	void	EnterGameMode( int nGameMode );
	void	EnterUIMode( int nUIMode );

	void	SelectRace( int nRaceID, int nRaceGender );

	int		GetNumLoginCharInfo( );
	SPlayerBaseInfo	GetNumLoginCharInfo( int nIndex );

	void	SelectChar( int nIndex);
	void	CreateChar(const guiex::CGUIString& rModelPrefix, int nGender, const guiex::CGUIString& rNickName);
	void	DeleteChar(int nIndex);


	guiex::CGUIString GetRaceName( int nRaceID );
	int				GetRaceID( const guiex::CGUIString& rModelPrefix );

	int		GetTarget( );
	
	void		DoSkill( int nSkillID );
}


#endif __KEN_GAME_EDITOR_INTERFACE_20091111_H__
