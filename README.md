# Dynamic Rainmeter Settings System
A general-use Rainmeter settings system. Includes toggle buttons, radio buttons, and input boxes.

The following is work-in-progress documentation:

## The Script Measure
Create a script measure in the skin you are using for the settings. This measure serves not only as the invocation of the settings script, but as the location to place all of the action bangs that will need to be invoked when a setting is changed. It also contains a few common settings that will be needed for the system to function.
```
[MeasureSettingsScript]
Measure=Script
Script=#@#Scripts\Settings.lua
ToggleOn=[\x5a]
ToggleOff=[\x56]
RadioOn=[\x5c]
RadioOff=[\x5b]
DefaultAction=[!Update "#skinConfigPath#"][!Redraw "#skinConfigPath#"]
ShowTextAction=[!SetOption MeterText Hidden "(1 - (#showText#))" "#skinConfigPath#"][!UpdateMeter MeterText "#skinConfigPath#"][!Redraw "#skinConfigPath#"]
```
*toggleOn* and *toggleOff* define the image names or strings that correspond to a toggle button's state. The same is true of radio buttons. All of the included examples use strings with character reference variables, but you can change these settings to your liking.

### ActionSets
The majority of Rainmeter skin authors will simply refresh the skin when they change a variable. However, for large complex skins (such as the skins in ModernGadgets), this can take up tons of system resources and lag all other Rainmeter skins. Thus, we want to be able to change settings and update the meters without refreshing the entire skin. You could add `DynamicVariables` to every meter that will be affected by the change, but that will cause a large performance drop in complex skins.

Because of this, you will need to execute a series of custom bangs whenever you change a setting, to avoid refreshing and using DynamicVariables. These series of bangs are called *ActionSets*. Every setting you change should usually include an ActionSet, unless the meters being updated are dynamic. If you do not include an ActionSet, the script will update and redraw the entire skin (which is usually fine, unless the skin being updated is very large and complicated).

Every settings function has an *actionSet* argument. Set it to the name of the ActionSet you wish to execute when that setting is changed. If you do not include an ActionSet argument, the script will automatically check for an ActionSet with the name `(variable name)Action`. If that doesn't exist, it will use the `DefaultAction` ActionSet. If *that* does not exist, it will throw an error.

#### IfLogic
For more complex settings, it is possible that you will want to execute different sets of actions depending on the new state of the variable. The script allows this by setting the *ifLogic* argument in a settings function to true. If set to true, the script will check for an ActionSet that is appended by the new state of the variable that was changed. For example:
```
Column1ActionOff=[!SetOptionGroup ColumnMeters Hidden "1" "#skinConfigPath"][!UpdateMeterGroup ColumnMeters "#skinConfigPath#"][!Redraw "#skinConfigPath#"]
Column1ActionTemps=[!SetOptionGroup Voltages Hidden "1" "#skinConfigPath"][!SetOptionGroup Temps Hidden "0" "#skinConfigPath"][!UpdateMeterGroup ColumnMeters "#skinConfigPath#"][!Redraw "#skinConfigPath#"]
Column1ActionVoltages=[!SetOptionGroup Temps Hidden "1" "#skinConfigPath#"][!SetOptionGroup Voltages Hidden "0" "#skinConfigPath#"][!UpdateMeterGroup ColumnMeters "#skinConfigPath#"][!Redraw "#skinConfigPath#"]
```
This example shows one potential usecase for the ifLogic functionality. It is expected that ifLogic will be used for radio buttons, but there may be instances when it should be used for toggles or input boxes as well.

## Standard Toggle Button
Standard toggle buttons are the most commonly used, and the simplest, implementation of the settings system. Standard toggles will toggle the given variable between 0 and 1.

### Implementation Meters
```
[MeterShowTextButton]
Meter=String
MeterStyle=StyleString | StyleStringToggleButton
Text=[&MeasureSettingsScript:GetIconStandard(#showText#)]
LeftMouseUpAction=[!CommandMeasure MeasureSettingsScript ¨Toggle('showText', '#cpuSettingsPath#', '#cpuMeterConfig#', 'ShowTextAction')"]

[MeterShowTextLabel]
Meter=String
MeterStyle=StyleString | StyleStringToggleLabel
Text="Show Text"
```

### Arguments
`Toggle(variable, settingsPath, configPath, actionSet, ifLogic)`
*variable* - The variable you wish to toggle
*settingsPath* - The path to the file that the variable is contained in
*configPath* - The config of the skin that the setting will affect
*actionSet* - The name of the action set to execute
*ifLogic* - If set to true, will use the script's ifLogic functionality

## Custom Toggle Button
Custom toggle buttons behave similarly to standard toggle buttons, but allow you to toggle between two custom values, rather than just 1 and 0. Aside from that change, they are exactly similar to standard toggles.

### Implementation Meters
```
[MeterTempUnitsButton]
Meter=String
MeterStyle=StyleString | StyleStringToggleButton
Text=[&MeasureSettingsScript:GetIcon('C', 'F')]
LeftMouseUpAction=[!CommandMeasure MeasureSettingsScript ¨CustomToggle('C', 'F', #tempUnits#, '#skinSettingsPath#', '#skinConfigPath#', 'TempUnitsAction')"]

[MeterTempUnitsLabel]
Meter=String
MeterStyle=StyleString | StyleStringToggleLabel
Text="Use Fahrenheit temperatures"
```

### Arguments
`CustomToggle(state0, state1, variable, filePath, configPath, actionSet, ifLogic)`
*state0* - The "off" setting
*state1* - The "on" setting
*variable* - The variable to be toggled
*settingsPath* - The path to the file that the variable is contained in
*configPath* - The config of the skin that the setting will affect
*actionSet* - The name of the action set to execute
*ifLogic* - If set to true, will use the script's ifLogic functionality

## Standard Radio Button
Radio buttons change a setting to a set value when clicked, and are off if the value is anything different. They are meant to be used in series, providing several options for one variable.

### Implementation Meters
```
[MeterColumn1OffButton]
Meter=String
MeterStyle=StyleString | StyleStringRadioButton
Text=[&MeasureSettingsScript:GetRadioIcon('Off')]
LeftMouseUpAction=[!CommandMeasure MeasureSettingsScript "Radio('Off', 'Column1Contents', '#skinSettingsPath#', '#skinConfigPath#', 'Column1Action', true)]

[MeterColumn1OffLabel]
Meter=String
MeterStyle=StyleString | StyleStringRadioLabel
Text="Off"

[MeterColumn1TempsButton]
Meter=String
MeterStyle=StyleString | StyleStringRadioButton
Text=[&MeasureSettingsScript:GetRadioIcon('Temps')]
LeftMouseUpAction=[!CommandMeasure MeasureSettingsScript "Radio('Temps', 'Column1Contents', '#skinSettingsPath#', '#skinConfigPath#', 'Column1Action', true)]

[MeterColumn1TempsLabel]
Meter=String
MeterStyle=StyleString | StyleStringRadioLabel
Text="Temperatures"

[MeterColumn1VoltagesButton]
Meter=String
MeterStyle=StyleString | StyleStringRadioButton
Text=[&MeasureSettingsScript:GetRadioIcon('Voltages')]
LeftMouseUpAction=[!CommandMeasure MeasureSettingsScript "Radio('Voltages', 'Column1Contents', '#skinSettingsPath#', '#skinConfigPath#', 'Column1Action', true)]

[MeterColumn1VoltagesLabel]
Meter=String
MeterStyle=StyleString | StyleStringRadioLabel
Text="Voltages"
```

### Arguments
`Radio(state, variable, filePath, configPath, actionSet, ifLogic)`
*state* - The value that the radio button will set the variable to
*variable* - The variable to be changed
*settingsPath* - The path to the file that the variable is contained in
*configPath* - The config of the skin that the setting will affect
*actionSet* - The name of the action set to execute
*ifLogic* - If set to true, will use the script's ifLogic functionality

## Input Box
Input boxes allow the user to change a variable to any value they wish. Because of the nature of the InputText plugin, there are a few key differences between how input boxes are implemented versus the other setting types.

Using input boxes requires knowledge of how to use the [InputText plugin](https://docs.rainmeter.net/manual/plugins/inputtext/). Click the link to read the documentation for that plugin.

### Implementation Meters
```
[MeasureInputText]
(inputtext options)
Command1=[!CommandMeasure MeasureSettingsScript "Input('$UserInput$', 'CustomText', '#skinSettingsPath#', '#skinConfigPath#', 'CustomTextAction', true)"]

[MeterCustomTextLabel]
Meter=String
MeterStyle=StyleString | StyleStringInputLabel
Text="Custom Text"

[MeterCustomTextInput]
Meter=String
MeterStyle=StyleString | StyleStringInputBox
Text="#customText#"
LeftMouseUpAction=[!CommandMeasure MeasureInputText "Executebatch 1"]
```

### Arguments
`Input(input, variable, settingsPath, configPath, actionSet, ifLogic)`
*input* - The input from the input box. This will always be `'$UserInput$'`
*variable* - The variable to be changed
*settingsPath* - The path to the file that the variable is contained in
*configPath* - The config of the skin that the setting will affect
*actionSet* - The name of the action set to execute
*ifLogic* - If set to true, will use the script's ifLogic functionality