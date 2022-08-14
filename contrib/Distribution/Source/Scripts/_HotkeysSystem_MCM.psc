Scriptname _HotkeysSystem_MCM extends SKI_ConfigBase  

; Global Functions
Function Exec() Global Native
Function Exec2() Global Native

; Properties
int CurPage
int CurOpt

; MCM Option Properties
int optManaging_New
int optManaging_NewCycle
int optManaging_Edit
int optManaging_Remove
int optManaging_Name
int optManaging_Hotkey
int optManaging_Modifier1
int optManaging_Modifier2
int optManaging_Modifier3
int optManaging_Sound
int optManaging_Toggle
int optManaging_Reequip
int optManaging_Widget
int optManaging_Hpos
int optManaging_Vpos
int optManaging_DWidget
int optManaging_DName
int optManaging_DHotkey
int optManaging_Confirm
int optManaging_Cancel
int optManaging_Left
int optManaging_Right
int optManaging_Shout

; Enum List
int ePage_Manaing = 1
int ePage_Overview = 2
int ePage_Widget = 3
int ePage_Settings = 4
int ePage_Maintenance = 5

int eOpt_None = 0
int eOpt_New = 1
int eOpt_NewCycle = 2
int eOpt_Edit = 3
int eOpt_Remove = 4

Event OnConfigInit()
	ModName = "Hotkeys System"
	Pages = new string[5]
	Pages[0] = "$Managing"
	Pages[1] = "$Overview"
	Pages[2] = "$Widget"
	Pages[3] = "$Settings"
	Pages[4] = "$Maintenance"
	RegisterForKey(21) ; Y
	RegisterForKey(22) ; U
EndEvent

Event OnConfigClose()
	ClearProperties()
EndEvent

Event OnGameReload()
	parent.OnGameReload()
	Load()
EndEvent

Event OnPageReset(string _page)	
	if _page == "$Managing"
		OnPage_Managing()
	elseif _page == "$Overview"
		OnPage_Overview()
	elseif _page == "$Widget"
		OnPage_Widget()
	elseif _page == "$Settings"
		OnPage_Settings()
	elseif _page == "$Maintenance"
		OnPage_Maintenance()
	endif
EndEvent

Event OnKeyDown(int _code)
	if _code == 21
		Exec()
	elseif _code == 22
		Exec2()
	endif
EndEvent

Function Load()

EndFunction

; Start of writting MCM pages functions.

Function OnPage_Managing()
	CurPage = ePage_Manaing;

	SetCursorFillMode(TOP_TO_BOTTOM)

	if CurOpt == eOpt_None
		optManaging_New = 		AddTextOption("", "$Add New Equipset", OPTION_FLAG_NONE)
		optManaging_NewCycle = 	AddTextOption("", "$Add New Cycle Equipset", OPTION_FLAG_NONE)
		optManaging_Edit = 		AddTextOption("", "$Edit Equipset Settings", OPTION_FLAG_NONE)
		optManaging_Remove = 	AddTextOption("", "$Remove Equipset", OPTION_FLAG_NONE)
	elseif CurOpt == eOpt_New
		optManaging_Name = 		AddInputOption("$Equipset Name:", Colorize("$Sample", "#7257CA"), OPTION_FLAG_NONE)
		optManaging_Hotkey = 	AddKeyMapOption("$Hotkey", -1, OPTION_FLAG_WITH_UNMAP)
		optManaging_Modifier1 = AddToggleOption("$Left Shift", false, OPTION_FLAG_NONE)
		optManaging_Modifier2 = AddToggleOption("$Left Control", false, OPTION_FLAG_NONE)
		optManaging_Modifier3 = AddToggleOption("$Left Alt", false, OPTION_FLAG_NONE)
		AddHeaderOption("", OPTION_FLAG_NONE)
		optManaging_Sound = 	AddToggleOption("$Equip Sound", false, OPTION_FLAG_NONE)
		optManaging_Toggle = 	AddToggleOption("$Toggle Equip/Unequip", false, OPTION_FLAG_NONE)
		optManaging_Reequip = 	AddToggleOption("$Re Equip", false, OPTION_FLAG_NONE)
		AddHeaderOption("", OPTION_FLAG_NONE)
		optManaging_Widget = 	AddMenuOption("$Widget Type", "", OPTION_FLAG_NONE)
		optManaging_Hpos = 		AddSliderOption("$Horizontal Position", 1.0, "{0}", OPTION_FLAG_NONE)
		optManaging_Vpos = 		AddSliderOption("$Vertical Position", 1.0, "{0}", OPTION_FLAG_NONE)
		optManaging_DWidget = 	AddToggleOption("$Display Equipset Widget", false, OPTION_FLAG_NONE)
		optManaging_DName = 	AddToggleOption("$Display Equipset Name", false, OPTION_FLAG_NONE)
		optManaging_DHotkey = 	AddToggleOption("$Display Equipset Hotkey", false, OPTION_FLAG_NONE)
		AddHeaderOption("", OPTION_FLAG_NONE)
		optManaging_Confirm = 	AddTextOption("", "$Confirm Adding New Equipset", OPTION_FLAG_NONE)
		optManaging_Cancel = 	AddTextOption("", "$Cancel and Back", OPTION_FLAG_NONE)

		SetCursorPosition(1)

		AddHeaderOption("$Weapons", OPTION_FLAG_NONE)
		optManaging_Left = AddTextOption("$Lefthand", "", OPTION_FLAG_NONE)
		optManaging_Right = AddTextOption("$Righthand", "", OPTION_FLAG_NONE)
		optManaging_Shout = AddTextOption("$Shout", "", OPTION_FLAG_NONE)
		AddEmptyOption()
		AddHeaderOption("$Items", OPTION_FLAG_NONE)

	elseif CurOpt == eOpt_NewCycle
		AddInputOption("$Equipset Name:", Colorize("$Sample", "#7257CA"), OPTION_FLAG_NONE)
		AddKeyMapOption("$Hotkey", -1, OPTION_FLAG_WITH_UNMAP)
		AddToggleOption("$Left Shift", false, OPTION_FLAG_NONE)
		AddToggleOption("$Left Control", false, OPTION_FLAG_NONE)
		AddToggleOption("$Left Alt", false, OPTION_FLAG_NONE)
		AddHeaderOption("", OPTION_FLAG_NONE)
		AddToggleOption("$Cycle Persist", false, OPTION_FLAG_NONE)
		AddSliderOption("$Cycle Expire", 1.0, "{0}", OPTION_FLAG_NONE)
		AddSliderOption("$Cycle Reset", 1.0, "{0}", OPTION_FLAG_NONE)
		AddHeaderOption("", OPTION_FLAG_NONE)
		AddMenuOption("$Widget Type", "", OPTION_FLAG_NONE)
		AddSliderOption("$Horizontal Position", 1.0, "{0}", OPTION_FLAG_NONE)
		AddSliderOption("$Vertical Position", 1.0, "{0}", OPTION_FLAG_NONE)
		AddToggleOption("$Display Equipset Widget", false, OPTION_FLAG_NONE)
		AddToggleOption("$Display Equipset Name", false, OPTION_FLAG_NONE)
		AddToggleOption("$Display Equipset Hotkey", false, OPTION_FLAG_NONE)
		AddHeaderOption("", OPTION_FLAG_NONE)
		AddTextOption("", "$Confirm Adding New Equipset", OPTION_FLAG_NONE)
		AddTextOption("", "$Cancel and Back", OPTION_FLAG_NONE)

		SetCursorPosition(1)

		AddHeaderOption("$Equipsets", OPTION_FLAG_NONE)
	elseif CurOpt == eOpt_Edit
		AddTextOption("$This is Edit Equipset", "", OPTION_FLAG_NONE)
		; Open select Equipset menu code;
	elseif CurOpt == eOpt_Remove
		AddTextOption("$This is Remove Equipset", "", OPTION_FLAG_NONE)
		; Open select Equipset menu code;
	endif
EndFunction

Function OnPage_Overview()
	CurPage = ePage_Overview;
	
	SetCursorFillMode(TOP_TO_BOTTOM)
	AddTextOption("$SampleName #1", "$Open", OPTION_FLAG_NONE)
	AddTextOption("$SampleName #2", "$Open", OPTION_FLAG_NONE)
	AddTextOption(Colorize("$SampleName #3", "#7257CA"), "", OPTION_FLAG_NONE)
	AddTextOption("$SampleName #4", "$Open", OPTION_FLAG_NONE)
	AddTextOption("$SampleName #5", "$Open", OPTION_FLAG_NONE)
	AddTextOption("$SampleName #6", "$Open", OPTION_FLAG_NONE)
	AddTextOption("$SampleName #7", "$Open", OPTION_FLAG_NONE)
	AddTextOption("$SampleName #8", "$Open", OPTION_FLAG_NONE)
	AddTextOption("$SampleName #9", "$Open", OPTION_FLAG_NONE)
	AddTextOption("$SampleName #10", "$Open", OPTION_FLAG_NONE)
	AddTextOption("$SampleName #11", "$Open", OPTION_FLAG_NONE)
	AddTextOption("$SampleName #12", "$Open", OPTION_FLAG_NONE)
	AddTextOption("$SampleName #13", "$Open", OPTION_FLAG_NONE)

	SetCursorPosition(1)

	AddHeaderOption(Colorize("$SampleName #3", "#7257CA"), OPTION_FLAG_NONE)
	AddTextOption("$Righthand", "Iron sword", OPTION_FLAG_NONE)
	AddTextOption("$Lefthand", "Wooden staff", OPTION_FLAG_NONE)
	AddTextOption("$Shout", "$None", OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Items", OPTION_FLAG_NONE)
	AddTextOption("", "Potion of Health", OPTION_FLAG_NONE)
	AddTextOption("", "Potion of Stamina", OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	AddTextOption("", "Edit", OPTION_FLAG_NONE)
	AddTextOption("", "Remove", OPTION_FLAG_NONE)

EndFunction

Function OnPage_Widget()
	CurPage = ePage_Widget;
	
	SetCursorFillMode(TOP_TO_BOTTOM)
	AddHeaderOption("$Widget Settings", OPTION_FLAG_NONE)
	AddMenuOption("$Widget Font", "", OPTION_FLAG_NONE)
	AddSliderOption("$Widget Size", 100.0, "{0}%", OPTION_FLAG_NONE)
	AddSliderOption("$Widget Background Transparency", 100.0, "{0}%", OPTION_FLAG_NONE)
	AddMenuOption("$Widget Display Mode", "", OPTION_FLAG_NONE)
	AddSliderOption("$Widget Dissolve Delay", 1.0, "{0}sec", OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Equipped Widget Settings", OPTION_FLAG_NONE)
	AddMenuOption("$Equipped Widget Font", "", OPTION_FLAG_NONE)
	AddSliderOption("$Equipped Widget Size", 100.0, "{0}%", OPTION_FLAG_NONE)
	AddSliderOption("$Equipped Widget Background Transparency", 100.0, "{0}%", OPTION_FLAG_NONE)
	AddMenuOption("$Equipped Widget Display Mode", "", OPTION_FLAG_NONE)
	AddSliderOption("$Equipped Widget Dissolve Delay", 1.0, "{0}sec", OPTION_FLAG_NONE)

	SetCursorPosition(1)

	AddHeaderOption("$Lefthand", OPTION_FLAG_NONE)
	AddToggleOption("$Display Widget", false, OPTION_FLAG_NONE)
	AddToggleOption("$Display Name", false, OPTION_FLAG_NONE)
	AddSliderOption("$Horizontal Position", 1.0, "{0}", OPTION_FLAG_NONE)
	AddSliderOption("$Vertical Position", 1.0, "{0}", OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Righthand", OPTION_FLAG_NONE)
	AddToggleOption("$Display Widget", false, OPTION_FLAG_NONE)
	AddToggleOption("$Display Name", false, OPTION_FLAG_NONE)
	AddSliderOption("$Horizontal Position", 1.0, "{0}", OPTION_FLAG_NONE)
	AddSliderOption("$Vertical Position", 1.0, "{0}", OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Shout", OPTION_FLAG_NONE)
	AddToggleOption("$Display Widget", false, OPTION_FLAG_NONE)
	AddToggleOption("$Display Name", false, OPTION_FLAG_NONE)
	AddSliderOption("$Horizontal Position", 1.0, "{0}", OPTION_FLAG_NONE)
	AddSliderOption("$Vertical Position", 1.0, "{0}", OPTION_FLAG_NONE)
EndFunction

Function OnPage_Settings()
	CurPage = ePage_Settings;
	
	SetCursorFillMode(TOP_TO_BOTTOM)
	AddHeaderOption("$Modifier", OPTION_FLAG_NONE)
	AddKeyMapOption("$Modifier Key 1", -1, OPTION_FLAG_WITH_UNMAP)
	AddKeyMapOption("$Modifier Key 2", -1, OPTION_FLAG_WITH_UNMAP)
	AddKeyMapOption("$Modifier Key 3", -1, OPTION_FLAG_WITH_UNMAP)
	AddEmptyOption()
	AddHeaderOption("$Equipset Sort", OPTION_FLAG_NONE)
	AddMenuOption("$Sort order", "", OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Gamepad", OPTION_FLAG_NONE)
	AddToggleOption("$Gamepad Support", false, OPTION_FLAG_NONE)
EndFunction

Function OnPage_Maintenance()
	CurPage = ePage_Maintenance;
	
	SetCursorFillMode(TOP_TO_BOTTOM)
	AddHeaderOption("$Save", OPTION_FLAG_NONE)
	AddTextOption("$Save MCM Settings", "", OPTION_FLAG_NONE)
	AddTextOption("$Save EquipSets", "", OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Load", OPTION_FLAG_NONE)
	AddTextOption("$Load MCM Settings", "", OPTION_FLAG_NONE)
	AddTextOption("$Load EquipSets", "", OPTION_FLAG_NONE)
	
	SetCursorPosition(1)

	AddHeaderOption("$Maintenance", OPTION_FLAG_NONE)
	AddTextOption("$Reset Settings", "Go", OPTION_FLAG_NONE)
	AddTextOption("$Reset Equipsets", "Go", OPTION_FLAG_NONE)
	AddTextOption("$Mod Status", Colorize("$Activated", "#4FE0A7"), OPTION_FLAG_NONE)
	AddTextOption("$Widget Status", Colorize("$Activated", "#4FE0A7"), OPTION_FLAG_NONE)
	AddTextOption("$Equipped Widget Status", Colorize("$Activated", "#4FE0A7"), OPTION_FLAG_NONE)
EndFunction

; End of writting MCM pages functions.


; Start of MCM option open functions.



; End of MCM option open functions.


; Start of MCM option select or accept functions.

Function OnOptionSelect(Int _opt)
	if CurPage == ePage_Manaing
		if _opt == optManaging_New
			CurOpt = eOpt_New
		elseif _opt == optManaging_NewCycle
			CurOpt = eOpt_NewCycle
		elseif _opt == optManaging_Edit
			CurOpt = eOpt_Edit
		elseif _opt == optManaging_Remove
			CurOpt = eOpt_Remove
		endif
		ForcePageReset()
	endif
EndFunction

; End of MCM option select functions.

Function ClearProperties()
	CurOpt = eOpt_None
EndFunction

string Function Colorize(String _text, String _code)
	return "<font color='" + _code + "''>" + _text + "</font>"
EndFunction