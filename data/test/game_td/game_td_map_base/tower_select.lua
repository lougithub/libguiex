function Button_ArcherTower_OnClick(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	tower:CloseTowerSelectPanel();
	tower:BuildTower( tdgame.eTowerType_ArcherTower );
end

function Button_Mages_OnClick(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	tower:CloseTowerSelectPanel();
	tower:BuildTower( tdgame.eTowerType_Mages );
end

function Button_Barracks_OnClick(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	tower:CloseTowerSelectPanel();
	tower:BuildTower( tdgame.eTowerType_Barracks );
end

function Button_DwarvenBoombard_OnClick(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	tower:CloseTowerSelectPanel();
	tower:BuildTower( tdgame.eTowerType_Bombard );
end


