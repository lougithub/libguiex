
--button and dialog test
function dlg_btn_ok_onclick(self, event)
    guiex.GetSystem():GetUICanvas():CloseDialog( self:GetParent());
end

function OnBtnClick_test1(self, event)
	local dlg = guiex.LoadDynamicPage( "dialog_ok.xml", "common", guiex.GetSceneName() );
	dlg:FindWidgetByName("dlg_ok_button_ok"):RegisterScriptCallbackFunc("OnMouseLeftClick", "dlg_btn_ok_onclick");
	dlg:FindWidgetByName("dlg_ok_content"):SetTextContentUTF8("hello dialog!");
	guiex.GetSystem():GetUICanvas():OpenDialog(dlg);
end


--progress test
function OnLoad_Progress1( self, event )
	self:RegisterTimer( "Timer_Update", 0.033 );
end

function OnTimer_Progress1( self, event )
	local curValue = self:GetCurrentValue();
	local maxValue = self:GetMaxValue();
	curValue = curValue + 1;
	if( curValue > maxValue ) then
		curValue = 0;
	end
	self:SetCurrentValue( curValue );
end

--scrollbar test
function OnLoad_ScrollbarBar_1( self, event )
	local label = guiex.GetWidget( "label_scrollbar_1", guiex.GetSceneName() );
	label:SetTextContentUTF8( ""..self:GetCurrentPos() );
end
function OnScrollbarScroll_1( self, event )
	local label = guiex.GetWidget( "label_scrollbar_1", guiex.GetSceneName() );
	label:SetTextContentUTF8( ""..event:GetCurrentPos() );
end

function OnLoad_ScrollbarBar_2( self, event )
	local label = guiex.GetWidget( "label_scrollbar_2", guiex.GetSceneName() );
	label:SetTextContentUTF8( ""..self:GetCurrentPos() );
end
function OnScrollbarScroll_2( self, event )
	local label = guiex.GetWidget( "label_scrollbar_2", guiex.GetSceneName() );
	label:SetTextContentUTF8( ""..event:GetCurrentPos() );
end


--listbox test
function OnListBoxSelected_1( self, event )
	local label = guiex.GetWidget( "label_listboxoutput", guiex.GetSceneName() );
	label:SetTextContentUTF8( "item "..event:GetSelectedItemIdx().." selected" );
end


--listbox test 2
function OnBtnClick_additem( self, event )
	local listbox = guiex.GetWidget( "listbox_2", guiex.GetSceneName() );
	listbox = guiex.ExactType( listbox );
	local item = guiex.LoadDynamicPage( "listitem.xml", "gui", guiex.GetSceneName() );
	listbox:AddItem( item );
end

function OnBtnClick_removeitem( self, event )
	local listbox = guiex.GetWidget( "listbox_2", guiex.GetSceneName() );
	listbox = guiex.ExactType( listbox );
	local itemcount = listbox:GetItemCount();
	if( itemcount > 0 ) then
		listbox:RemoveItem( itemcount - 1 );
	end
end


--combobox test
function OnLoad_ComboBox( self, event )
	local i;
	for i=1,10,1 do
		local content = "item"..i;
		local item = guiex.LoadDynamicPage( "comboboxitem.xml", "gui", guiex.GetSceneName() );
		item = guiex.ExactType( item );
		item:SetItemContentUTF8( content );
		local label = item:FindWidgetByName( "label_text" );
		label = guiex.ExactType( label );
		label:SetTextContentUTF8(content);
		self:AddItem( item );
	end
end



