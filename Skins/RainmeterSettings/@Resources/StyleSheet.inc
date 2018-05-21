[Variables]
; Positioning
bgOffset=5
bgWidth=200
bgWidthAbs=(#bgWidth# + (#bgOffset# * 2))
contentMarginAbs=5
contentMargin=(#bgOffset# + #contentMarginAbs#)
contentMarginCenter=((#bgWidth# / 2) + #bgOffset#)
contentMarginRight=((#bgWidth# + #bgOffset#) - #contentMarginAbs# - 1)
contentWidth=(#bgWidth# - (#contentMarginAbs# * 2))
rowSpacing=3
; Colors
colorBg=15,15,15
colorInputBg=40,40,40
colorMain=240,240,240
colorDim=150,150,150
colorAccent=138,210,250
colorButtonPress=200,70,70
; File paths
skinSettingsPath=#@#SkinSettings.inc
skinConfigPath=RainmeterSettings\ExampleSkin
; Character reference variables
deg=[\176]

[StyleBackground]
SolidColor=#colorBg#
X=#bgOffset#
Y=#bgOffset#
W=#bgWidth#
H=([MeterBackgroundHeight:Y] - (#bgOffset# * 2))
DynamicVariables=1
Group=Background

[StyleBackgroundHeight]
SolidColor=255,255,255,0
X=0
Y=(#contentMarginAbs# + #bgOffset#)R
W=#bgWidthAbs#
H=0
Group=Background

[StyleString]
FontFace=Roboto
FontSize=10
FontColor=#fontColor#
Antialias=1
X=#contentMargin#
Y=#rowSpacing#R

[StyleStringRightAlign]
StringAlign=Right
X=#contentMarginRight#
Y=r

[StyleStringTitle]
FontFace=Roboto Cn
FontColor=#colorDim#
StringCase=Upper
Y=(#contentMargin# - 3)

[StyleStringParagraph]
FontSize=9
W=#contentWidth#
ClipString=2

[StyleStringToggleLabel]
X=3R
Y=-1r

[StyleStringToggleButton]
FontFace=ElegantIcons
FontColor=#colorAccent#
FontSize=12
X=#contentMargin#
Y=4R
DynamicVariables=1
Group=Settings
MouseOverAction=[!SetOption #CURRENTSECTION# FontColor "#colorButtonPress#"][!UpdateMeter #CURRENTSECTION#][!Redraw]
MouseLeaveAction=[!SetOption #CURRENTSECTION# FontColor "#colorAccent#"][!UpdateMeter #CURRENTSECTION#][!Redraw]

[StyleStringRadioButton]
X=5R
Y=1r

[StyleStringInputBox]
FontFace=Roboto Lt
SolidColor=#colorInputBg#
Padding=2,-1,2,0
W=(#contentWidth# - 4)
ClipString=1
DynamicVariables=1
Group=Settings