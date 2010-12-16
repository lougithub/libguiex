function showfps_onload( self, event)
	guiex.PrintDebug( "showfps_onload" )
end

function showfps_update( self, event )
	local fps = guiex.GetSystem():GetFPS();
	self:SetTextContentUTF8( ..fps );
end

