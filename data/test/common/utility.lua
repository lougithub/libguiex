function showfps_onload( self, event)
	guiex.PrintDebug( "showfps_onload" )
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
