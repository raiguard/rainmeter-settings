-- ----------------------------------------------------------------------
-- CUSTOM LUA ACTIONS
-- ----------------------------------------------------------------------
-- Even though the settings system uses bangs in the skin itself, there
-- are times where it is impossible to execute the needed actions
-- without using LUA logic. In this case, simply create a file like this
-- one and invoke a !CommandMeasure from the ActionSet or InputText
-- command. You could also just append the Settings.lua file if desired.

function Initialize()

	skinSettingsPath = SKIN:GetVariable('skinSettingsPath')
	skinConfigPath = SKIN:GetVariable('skinConfigPath')

	dofile(SKIN:GetVariable('@') .. 'Scripts\\Settings.lua')

end

function Update() end

function CustomText(input)

	if input == '' then
		Input('auto', 'customText', skinSettingsPath, skinConfigPath, 'CustomTextActionAuto')
	else
		Input(input, 'customText', skinSettingsPath, skinConfigPath, 'CustomTextAction')
	end

end