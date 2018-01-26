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
; Colors
colorBg=15,15,15
colorMain=240,240,240
colorDim=150,150,150
colorAccent=138,210,250
colorButtonPress=240,30,30
; File paths
skinSettingsPath=#@#SkinSettings.inc
skinConfigPath=RainmeterSettings\ExampleSkin

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
Y=1R

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
Group=ToggleButtons