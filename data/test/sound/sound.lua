function panel_onopen(self, event)
	guiex.CGUIInterfaceManager_Instance():GetInterfaceSound():PlayMusic( "music.ogg", true );
end

function btn_playeffect_se_yes_onclick(self, event)
	guiex.CGUIInterfaceManager_Instance():GetInterfaceSound():PlayEffect( "se_yes.ogg" );
end

function btn_playeffect_se_warning_onclick(self, event)
	guiex.CGUIInterfaceManager_Instance():GetInterfaceSound():PlayEffect( "se_warning.ogg" );
end

function btn_playeffect_stereo_onclick(self, event)
	guiex.CGUIInterfaceManager_Instance():GetInterfaceSound():PlayEffect( "stereo.ogg" );
end

function btn_playmusic_onclick(self, event)
	guiex.CGUIInterfaceManager_Instance():GetInterfaceSound():PlayMusic( "music.ogg", true );
end

function btn_pausemusic_onclick(self, event)
	if guiex.CGUIInterfaceManager_Instance():GetInterfaceSound():IsPlayingMusic() == true then
		guiex.CGUIInterfaceManager_Instance():GetInterfaceSound():PauseMusic( );
	else
		guiex.CGUIInterfaceManager_Instance():GetInterfaceSound():ResumeMusic( );
	end
end

function btn_stopmusic_onclick(self, event)
	guiex.CGUIInterfaceManager_Instance():GetInterfaceSound():StopMusic( );
end


