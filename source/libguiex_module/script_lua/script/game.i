#define NICK_MAXLEN	16
#define GENDER_MALE		0
#define GENDER_FEMALE	1


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

	void	EnterGameMode( ETDGGameMode nGameMode );
	void	EnterUIMode( ETDGUIMode nUIMode );

	void	SelectRace( int nRaceID, int nRaceGender );

	int		GetNumLoginCharInfo( );
	SPlayerBaseInfo	GetNumLoginCharInfo( int nIndex );
	
	void	SelectChar( int nIndex);
	void	CreateChar(const guiex::CGUIString& rModelPrefix, int nGender, const guiex::CGUIString& rNickName);
	void	DeleteChar(int nIndex);
	
	guiex::CGUIString GetRaceName( int nRaceID );
	int					GetRaceID( const guiex::CGUIString& rModelPrefix );

	int		GetTarget( );
	void		DoSkill( int nSkillID );
}