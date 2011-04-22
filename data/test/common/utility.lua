function showfps_onload( self, event)
	guiex.PrintDebug( "showfps_onload" )
	local curSize = self:GetPixelSize();
	self:SetPixelSize( curSize:GetWidth(), 44 );
end

function showfps_update( self, event )
	local fps = guiex.GetSystem():GetFPS();
	self:SetTextContentUTF8( ""..fps );
end

function toggle_wireframe( self, event )
	local  render = guiex.GetSystem():GetInterfaceManager():GetInterfaceRender();
	render:SetWireFrame( not render:IsWireFrame() );
end

function toggle_extrainfo( self, event )
	guiex.GetSystem():SetDrawExtraInfo( not guiex.GetSystem():IsDrawExtraInfo() );
end

function toggle_rotate( self, event )
	local orientation = guiex.GetSystem():GetScreenOrientation();
	orientation = (orientation+1) % (guiex.eScreenOrientation_LandscapeRight+1);
	guiex.GetSystem():SetScreenOrientation( orientation );
end

function toggle_clip( self, event )
	guiex.GetSystem():EnableClip( not guiex.GetSystem():IsEnableClip() );
end


function panel_run_as( widget, reverse )
	local as = widget:GetAs("default_as");
	as:SetReverse(reverse);
	as:Reset();
	widget:PlayAs(as);
end

function max_panel( self, event )
	local parent = self:GetParent();
	panel_run_as( parent, false );
	--local curSize = parent:GetPixelSize();
	--parent:SetPixelSize( curSize:GetWidth(), 136 );
	--parent:Refresh();
end

function min_panel( self, event )
	local parent = self:GetParent();
	panel_run_as( parent, true );
	--local curSize = parent:GetPixelSize();
	--parent:SetPixelSize( curSize:GetWidth(), 44 );
	--parent:Refresh();
end
