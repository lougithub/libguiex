%module tdgame

%{
#include "tdgametower.h"
#include "tdgameworld.h"
#include "tdgametype.h"

#include "libguiex_core/guiex.h"
#include "libguiex_widget/guiwgt.h"
#include "libguiex_widget_game/guiwgt_game.h"
%}




namespace guiex
{
	enum ETowerType
	{
		eTowerType_Basement = 0,
		eTowerType_ArcherTower,
		eTowerType_Mages,
		eTowerType_Bombard,
		eTowerType_Barracks,
	};

	//============================================================
	// CTDGameTower
	//============================================================
	class CTDGameTower
	{
	public:
		void CloseTowerSelectPanel();
		void CloseTowerUpgradePanel();
		void BuildTower(ETowerType eType);
		void UpgradeTower( );
		void SellTower( );
		
	protected:
		CTDGameTower();
		virtual ~CTDGameTower();
	};


	//============================================================
	// CTDGameWorld
	//============================================================
	class CTDGameWorld
	{
	public:
		guiex::CGUIWidget* GetTowerSelectPanel() const;
		guiex::CGUIWidget* GetTowerUpgradePanel() const;
	
		guiex::uint32 GetGold() const;
		guiex::uint32 GetHP() const;

		CTDGameTower* GetCurrentFocusTower() const;

	protected:
		CTDGameWorld();
		virtual ~CTDGameWorld();
	};
	CTDGameWorld* GetGameWorld();



}	