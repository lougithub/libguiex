%module tdgame

%{
#include "tdgameworld.h"

#include "libguiex_core/guiex.h"
#include "libguiex_widget/guiwgt.h"
#include "libguiex_widget_game/guiwgt_game.h"
%}




namespace guiex
{
	class CTDGameWorld
	{
	public:
		guiex::CGUIWidget* GetTowerSelectPanel() const;
		guiex::CGUIWidget* GetTowerUpgradePanel() const;
	
	protected:
		CTDGameWorld();
		virtual ~CTDGameWorld();
	};
	
	CTDGameWorld* GetGameWorld();
	
}	