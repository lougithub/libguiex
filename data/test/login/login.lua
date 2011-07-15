function node_main_onload( self, event)
end

function node_main_onopen(self, event)
    guiex.PrintDebug("opopen");
    input_username:SetFocus(true);
end

function dlg_btn_ok_onclick(self, event)
    guiex.GetSystem():GetUICanvas():CloseDialog( self:GetParent());
end

function btn_login_onclick(self, event)
    if input_username:IsTextContentEmpty() == true then
        local dlg = guiex.LoadDynamicPage( "dialog_ok.xml", "common", guiex.GetSceneName() );
        dlg:FindWidgetByName("dlg_ok_button_ok"):RegisterScriptCallbackFunc("OnMouseLeftClick", "dlg_btn_ok_onclick");
        dlg:FindWidgetByName("dlg_ok_content"):SetTextContentUTF8("please input username!");
        guiex.GetSystem():GetUICanvas():OpenDialog(dlg);
    elseif input_password:IsTextContentEmpty() == true then
        local dlg = guiex.LoadDynamicPage( "dialog_ok.xml", "common", guiex.GetSceneName() );
        dlg:FindWidgetByName("dlg_ok_button_ok"):RegisterScriptCallbackFunc("OnMouseLeftClick", "dlg_btn_ok_onclick");
        dlg:FindWidgetByName("dlg_ok_content"):SetTextContentUTF8("please input password!");
        guiex.GetSystem():GetUICanvas():OpenDialog(dlg);
    else
    end
end


