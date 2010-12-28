function node_main_onload( self, event)
end

function node_main_onopen(self, event)
    guiex.PrintDebug("opopen");
    --input_username:SetFocus(true);
end

function dlg_btn_ok_onclick(self, event)
    --guiex.CloseDialog( self:GetRoot());
end

function btn_login_onclick(self, event)
    --if input_username:IsTextContentEmpty() == true then
        --local dlg = guiex.LoadDynamicPage( "dialog_ok.xml", "common.uip", self:GetProjectName() );
        --dlg:GetChild("dlg_button_ok"):RegisterScriptCallbackFunc("OnMouseLeftClick", "dlg_btn_ok_onclick");
        --dlg:GetChild("dlg_text_content"):SetTextContentUTF8("please input username!");
        --guiex.OpenDialog(dlg);
    --elseif input_password:IsTextContentEmpty() == true then
        --local dlg = guiex.LoadDynamicPage( "dialog_ok.xml", "common.uip", self:GetProjectName() );
        --dlg:GetChild("dlg_button_ok"):RegisterScriptCallbackFunc("OnMouseLeftClick", "dlg_btn_ok_onclick");
        --dlg:GetChild("dlg_text_content"):SetTextContentUTF8("please input password!");
        --guiex.OpenDialog(dlg);
    --else
    --end
end


