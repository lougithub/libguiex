------------------------------------------------------------------------------
function Button_ArcherTower_OnLoad(self, event)
	self:RegisterTimer( "check_gold", 1 );
end

function Button_ArcherTower_OnTimer(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	if( tower:GetBuildCost(tdgame.eTowerType_ArcherTower) > tdgame.GetGameWorld():GetGold() ) then
		self:SetDisable(true);
	else 
		self:SetDisable(false);
	end
end

function Button_ArcherTower_OnClick(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	tower:CloseTowerSelectPanel();
	tower:BuildTower( tdgame.eTowerType_ArcherTower );
end


------------------------------------------------------------------------------
function Button_Mages_OnLoad(self, event)
	self:RegisterTimer( "check_gold", 1 );
end

function Button_Mages_OnTimer(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	if( tower:GetBuildCost(tdgame.eTowerType_Mages) > tdgame.GetGameWorld():GetGold() ) then
		self:SetDisable(true);
	else 
		self:SetDisable(false);
	end
end

function Button_Mages_OnClick(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	tower:CloseTowerSelectPanel();
	tower:BuildTower( tdgame.eTowerType_Mages );
end


------------------------------------------------------------------------------
function Button_Barracks_OnLoad(self, event)
	self:RegisterTimer( "check_gold", 1 );
end

function Button_Barracks_OnTimer(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	if( tower:GetBuildCost(tdgame.eTowerType_Barracks) > tdgame.GetGameWorld():GetGold() ) then
		self:SetDisable(true);
	else 
		self:SetDisable(false);
	end
end

function Button_Barracks_OnClick(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	tower:CloseTowerSelectPanel();
	tower:BuildTower( tdgame.eTowerType_Barracks );
end


------------------------------------------------------------------------------
function Button_DwarvenBoombard_OnLoad(self, event)
	self:RegisterTimer( "check_gold", 1 );
end

function Button_DwarvenBoombard_OnTimer(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	if( tower:GetBuildCost(tdgame.eTowerType_Bombard) > tdgame.GetGameWorld():GetGold() ) then
		self:SetDisable(true);
	else
		self:SetDisable(false);
	end
end

function Button_DwarvenBoombard_OnClick(self, event)
	local tower = tdgame.GetGameWorld():GetCurrentFocusTower();
	tower:CloseTowerSelectPanel();
	tower:BuildTower( tdgame.eTowerType_Bombard );
end


