function Button_TowerUpgrade_OnClick(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	tower:CloseTowerUpgradePanel();
	tower:UpgradeTower( );
end

function Button_TowerSell_OnClick(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	tower:CloseTowerUpgradePanel();
	tower:SellTower( );
end