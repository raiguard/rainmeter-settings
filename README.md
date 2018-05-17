# Dynamic Rainmeter Settings System
A general-use Rainmeter settings system. Includes toggle buttons, radio buttons, and input boxes.

## Introduction
Rainmeter skins are meant to be tinkered with - it is the central purpose and theme of Rainmeter itself. People are encouraged to delve deep into the code and figure out how things work, and modify them to suit their needs. However, most people are not willing to do this, and some just don't have the knowledge or patience to learn.

Most skin authors will include some sort of "settings" skin, with a UI to allow the users to change settings without needing to so much as glance at the code. However, creating an in-depth and engaging settings skin can be complicated and time-consuming, and when the settings themselves get complicated, it it difficult to create a good interface for them.

That is where this system comes in. The dynamic Rainmeter settings system is meant to ease the process of creating complex settings skins for use by end-users of a suite. It allows skin authors to create more expressive and more interactive user interfaces for changing these settings. It removes the need for authors to manually code every settings interaction, instead only needing to provide some arguments for a function and specify what bangs need to be executed along with the setting.

## Usage
These instructions assume that you have intermediate knowledge of how to create Rainmeter skins. The [Rainmeter documentation](https://docs.rainmeter.net) includes several tutorials on the basics of skin editing.

### The Script Measure
Copy the Settings.lua script into your suite and create a script measure for it. There are several options you will need to add to the measure beyond the usual:

`SettingsPath`: Path to the file where the settings variables are contained. Will default to the current skin.  
`ConfigPath`: The config of the skin where the settings will take effect. Will default to the current config.  
`ToggleOn`: The text or image path for toggle buttons in the "On" state.  
`ToggleOff`: The text or image path for toggle buttons in the "Off" state.  
`RadioOn`: The text or image path for radio buttons in the "On" state.  
`RadioOff`: The text or image path for radio buttons in the "Off" state.  
`DefaultAction` (required): The default ActionSet to use if one is not specified or is missing.  

#### ActionSets
`ActionSets` are collections of bangs to execute when that option is changed. The recommended naming syntax is `(variable name)Action`. For example, if I'm changing the `#showCpuName#` variable, the ActionSet name would be `CpuNameAction`. ActionSets are defined in the script measure as any other option would be.

The purpose of ActionSets is to avoid refreshing the skin. Refreshing a skin is the single most resource-intensive activity in Rainmeter, and in very large and complicated skins, can hang Rainmeter for several moments. As a rule of thumb, you should avoid refreshing a skin as much as humanly possible. ActionSets allow you to use `!SetOption` bangs to invoke the changes of the setting you changed, without needing to use DynamicVariables and without refreshing the skin.

When using ActionSets, keep in mind that any variables you use will automatically be dynamic. So if you use the `#showCpuName#` variable in the `CpuNameAction` ActionSet, the variable's value will be the new state of the variable after it was changed.

#### IfLogic
`IfLogic` is an optional behavior that you can use with `ActionSets` to invoke different sets of bangs depending on the new state of the variable that was changed. For ActionSets using IfLogic, use the same naming syntax with the new state of the variable added onto the end. For example, instead of `CpuNameAction`, you would use `CpuNameAction0` and `CpuNameAction1`.

#### Implementation
Here is an example of what the script measure may look like when populated with options:

```
[MeasureSettingsScript]
Measure=Script
ScriptFile=#@#Scripts\Settings.lua
SettingsPath=#skinSettingsPath#
ConfigPath=#skinConfigPath#
ToggleOn=[\x5a]
ToggleOff=[\x56]
RadioOn=[\x5c]
RadioOff=[\x5b]
DefaultAction=[!Update "#skinConfigPath#"][!Redraw "#skinConfigPath#"]
CpuNameAction=[!SetOption CpuNameString Hidden "(#showCPuName# = 0)" "#skinConfigPath#"][!UpdateMeter CpuNameString "#skinConfigPath#"][!Redraw "#skinConfigPath#"]
```

### Skin path variables
Most of the time, you will only need to specify the skin config and settings path in the script measure. However, if you have options for multiple skins / settings files in one settings skin, you will need to override the global _SettingsPath_ and _ConfigPath_ options that you set in the script measure.

The `Toggle()` and `Set()` functions have optional arguments that allow for this. However, unlike simply specifying the path in the script measure, if you do the same in the function arguments, LUA will get confused and throw an error. This is because Windows uses backslashes `\` in file paths, and in LUA the backslash `\` is an escape character. Rainmeter will account for this most of the time, but in this specific circumstance, you must manually escape the file paths with double backslashes `\\`.

Here is an example of what needs to be done:
```
[MeasureSkinSettingsPath]
Measure=String
String=#@#SkinSettings.inc
Substitute="\":"\\"

[MeasureSkinConfig]
Measure=String
String=RainmeterSettings\ExampleSkin
Substitute="\":"\\"
```
This measure will replace the backslashes `\` in the paths with double backslashes `\\`, allowing the paths to be used in LUA without conflicts. When using the global overrides in a script argument, use these measures as sections variables.

`[!CommandMeasure MeasureSettingsScript "Set('exampleRadio1', 'on', 'ExampleRadio1Action', false, '[MeasureSkinSettingsPath]', '[MeasureSkinConfig]')"]`

### GetIcon() function
The Settings.lua script includes the `GetIcon(variable, onState, offState)` function. This function is used through inline LUA on the button meter.

`GetIcon(value, onState, offState)`
*value* (string) - The value of the variable that the button represents
*onState* (string) - The variable value which the button will consider the _on_ state
*offState* (string) - The variable value which the button will consider the _off_ state

There are several ways in which this can be used:  
- If you are using a toggle button that is toggling between 0 and 1, you can simply provide the variable value as a number, without needing to supply _onState_ and _offState_ (e.g. `GetIcon([#exampleValue1])`)
- For radio buttons, only provide the first two values (e.g. `GetIcon('[#exampleRadio1]', 'on')`)

### Toggle Button
[TODO: Radio button image]
A toggle button is the simplest way to change an option. When the button is pressed, it toggles the value of the given variable between two given states.

#### Implementation
```
[MeterCpuNameToggleButton]
Meter=String
FontFace=Elegant Icons
FontSize=10
FontColor=128,210,255
X=5
Y=2R
Antialias=1
Text=[&MeasureSettingsScript:GetIcon([#showCpuName])]
LeftMouseUpAction=[!CommandMeasure MeasureSettingsScript "Toggle('showCpuName', '1', '0', 'CpuNameAction')"]
DynamicVariables=1
Group=ToggleButtons
```
(NOTE: The above example is a string meter using character reference variables, but you could also use image meters if desired.)

The following are required options on a toggle button meter:
- `DynamicVariables=1`
- `Group=ToggleButtons`

The rest of the options are able to be changed as desired.

#### Script function
`Toggle(variable, onState, offState, settingsPath, configPath, actionSet, ifLogic)`
*variable* - The variable you wish to toggle
*onState* - The value of the 'on' state of the variable
*offState* - The value of the 'off' state of the variable
*actionSet* - The name of the action set to execute
*ifLogic* - If set to true, will use the script's ifLogic functionality
*settingsPath* - Overrides the `SettingsPath` option in the script measure
*configPath* - Overrides the `ConfigPath` option in the script measure

#### Other notes
- The toggled values do not necessarily need to be 0 and 1. They can be any two strings or numbers that you desire
- Even if you use numbers, the offState and onState arguments must be passed as strings
- The variable argument takes in the name of the variable as a string, not the value of the variable itself

### Radio Button
[TODO: Radio button image]
Radio buttons allow a variable to be changed between multiple predefined values. Each button will set the variable to its given value. Multiple radio buttons can be used with the same variable to enable having more than two states for one variable.

#### Implementation
```
[MeterModeRadioButton]
Meter=String
FontFace=Elegant Icons
FontSize=10
FontColor=128,210,255
X=5
Y=2R
Antialias=1
Text=[&MeasureSettingsScript:GetIcon('[#mode]', 'off')]
LeftMouseUpAction=[!CommandMeasure MeasureSettingsScript "Set('mode', 'off', 'ModeAction', true)"]
DynamicVariables=1
Group=ToggleButtons
```
The `Set()` function is used to _set_ the variable to the given value, rather than toggling between just two values. As with toggle buttons, you must use `DynamicVariables=1` on the implementation meter.

#### Script function
`Set(variable, state, actionSet, ifLogic, settingsPath, configPath)`
*variable* - The variable you wish to toggle
*state* - The state that the button will set the variable to
*actionSet* - The name of the action set to execute
*ifLogic* - If set to true, will use the script's ifLogic functionality
*settingsPath* - Overrides the `SettingsPath` option in the script measure
*configPath* - Overrides the `ConfigPath` option in the script measure

#### Other notes
- The `state` argument must be a string (but can be a number inside a string)
- IfLogic is most likely to be used with a set of radio buttons

### Input Box
Input Boxes are a method with which to set a variable to any value the user desires. Input boxes differ fundamentally from toggle and radio buttons in that the script function is not invoked in the meter itself - it is invoked from the InputText measure. Input boxes use the `Set()` function, the same as radio buttons.

Input boxes require knowledge of how to use the [InputText plugin](https://docs.rainmeter.net/manual-beta/plugins/inputtext). Click the link to view the plugin documentation.

#### Implementation
```
[MeasureInputText]
Measure=Plugin
Plugin=InputText
(other inputtext options here)
Command1=[!CommandMeasure MeasureSettingsScript "Set('customText', '$UserInput$', 'CustomTextAction')"] Default="#customText#" X=[MeterCustomTextInputBox:X] Y=[MeterCustomTextInputBox:Y] W=[MeterCustomTextInputBox:W] H=[MeterCustomTextInputBox:H]

[MeterCustomTextInputBox]
Meter=String
FontFace=Roboto Lt
FontColor=250,250,250
FontSize=10
SolidColor=25,25,25
X=5
Y=3R
W=80
H=16
Padding=2,0,2,0
Text="#customText#"
LeftMouseUpAction=[!CommandMeasure MeasureInputText "Executebatch 1"]
DynamicVariables=1
Group=ToggleButtons
```