Scriptname _HotkeysSystem_MCM extends SKI_ConfigBase  

; Global Native Functions
Function UIHS_Exec(int _code, bool _modifier1, bool _modifier2, bool _modifier3) Global Native
Function Exec2() Global Native
Function UIHS_Init() Global Native
Function UIHS_Clear() Global Native
string Function UIHS_GetNamePrefix() Global Native
string Function UIHS_GetStringFromKeycode(int _key) Global Native
string[] Function UIHS_GetList(int _type) Global Native
string Function UIHS_GetStringFromList(int _index, int _type) Global Native
bool Function UIHS_NewEquipset(string[] _data) Global Native
bool Function UIHS_NewCycleEquipset(string[] _data) Global Native
bool Function UIHS_EditEquipset(string _name, string[] _data) Global Native
bool Function UIHS_RemoveEquipset(string _data) Global Native
string[] Function UIHS_GetEquipsetData(string _name) Global Native
string Function UIHS_GetKeyConflict(int _key, bool[] _modifier, bool _beast) Global Native
bool Function UIHS_IsNameConflict(string _name) Global Native
bool Function UIHS_IsCycleEquipset(string _name) Global Native
int[] Function UIHS_GetKeycodeList() Global Native
Function UIHS_SendSettingData(int[] _data) Global Native
Function UIHS_SaveSetting(string[] _data) Global Native
string[] Function UIHS_LoadSetting() Global Native
Function UIHS_CalculateKeydown(int _code, bool _modifier1, bool _modifier2, bool modifier3, float _time) Global Native

; Number of items Equipset can have & Cycle Equipset can have / Number of displaying Overview page Equipset
int MAX_ITEMS = 15
int MAX_CYCLEITEMS = 5
int MAX_OVERVIEWLIST = 20

; Properties
bool bModActive
bool bWidgetActive
int CurPage
int CurOpt

; Gamepad Press Check
bool Gamepad_Modifier1 = false
bool Gamepad_Modifier2 = false
bool Gamepad_Modifier3 = false

; MCM Option Properties
int 	optManaging_New
int 	optManaging_NewCycle
int 	optManaging_Edit
int 	optManaging_Remove
int 	optManaging_Name
int 	optManaging_Hotkey
int 	optManaging_Modifier1
int 	optManaging_Modifier2
int 	optManaging_Modifier3
int 	optManaging_Sound
int 	optManaging_Toggle
int 	optManaging_Reequip
int 	optManaging_Beast
int 	optManaging_Widget
int 	optManaging_Hpos
int 	optManaging_Vpos
int 	optManaging_DWidget
int 	optManaging_DName
int 	optManaging_DHotkey
int 	optManaging_Confirm
int 	optManaging_Cancel
int 	optManaging_Left
int 	optManaging_Right
int 	optManaging_Shout
int[]	optManaging_Items
int 	optManaging_CycleName
int 	optManaging_CycleHotkey
int 	optManaging_CycleModifier1
int 	optManaging_CycleModifier2
int 	optManaging_CycleModifier3
int 	optManaging_CyclePersist
int 	optManaging_CycleExpire
int 	optManaging_CycleReset
int 	optManaging_CycleBeast
int 	optManaging_CycleHpos
int 	optManaging_CycleVpos
int 	optManaging_CycleDWidget
int 	optManaging_CycleDName
int 	optManaging_CycleDHotkey
int 	optManaging_CycleConfirm
int		optManaging_CycleCancel
int[]	optManaging_CycleItems

int[]	optOverview_Equipset
int 	optOverview_Edit
int 	optOverview_Remove

int 	optWidget_Font
int 	optWidget_Size
int 	optWidget_Alpha
int 	optWidget_Display
int 	optWidget_Delay
int 	optWidget_EFont
int 	optWidget_ESize
int 	optWidget_EAlpha
int 	optWidget_EDisplay
int 	optWidget_EDelay
int 	optWidget_LDisplay
int 	optWidget_LName
int 	optWidget_LHpos
int 	optWidget_LVpos
int 	optWidget_RDisplay
int 	optWidget_RName
int 	optWidget_RHpos
int 	optWidget_RVpos
int 	optWidget_SDisplay
int 	optWidget_SName
int 	optWidget_SHpos
int 	optWidget_SVpos

int		optSetting_Modifier1
int		optSetting_Modifier2
int		optSetting_Modifier3
int		optSetting_Sort
int		optSetting_Gamepad
int		optSetting_Favor

; MCM Stored option variables
string  sManaging_Name
string	sManaging_OName
int 	sManaging_Hotkey
bool 	sManaging_Modifier1
bool 	sManaging_Modifier2
bool 	sManaging_Modifier3
bool 	sManaging_Sound
bool 	sManaging_Toggle
bool 	sManaging_Reequip
bool 	sManaging_Beast
int 	sManaging_Widget
int 	sManaging_Hpos
int 	sManaging_Vpos
bool 	sManaging_DWidget
bool 	sManaging_DName
bool 	sManaging_DHotkey
int 	sManaging_Left
int 	sManaging_Right
int 	sManaging_Shout
int 	sManaging_numItems
int[] 	sManaging_Items
string 	sManaging_CycleName
string 	sManaging_CycleOName
int 	sManaging_CycleHotkey
bool 	sManaging_CycleModifier1
bool 	sManaging_CycleModifier2
bool 	sManaging_CycleModifier3
bool 	sManaging_CyclePersist
float 	sManaging_CycleExpire
float 	sManaging_CycleReset
bool 	sManaging_CycleBeast
int 	sManaging_CycleHpos
int 	sManaging_CycleVpos
bool 	sManaging_CycleDWidget
bool 	sManaging_CycleDName
bool 	sManaging_CycleDHotkey
int 	sManaging_CyclenumItems
int[] 	sManaging_CycleItems

int		sOverview_Equipset

int 	sWidget_Font = 0
int 	sWidget_Size = 100
int 	sWidget_Alpha = 100
int 	sWidget_Display = 0
string[] sWidget_DisplayString
float 	sWidget_Delay = 3.0
int 	sWidget_EFont = 0
int 	sWidget_ESize = 100
int 	sWidget_EAlpha = 100
int 	sWidget_EDisplay = 0
float 	sWidget_EDelay = 3.0
bool 	sWidget_LDisplay = false
bool 	sWidget_LName = false
int 	sWidget_LHpos = 0
int 	sWidget_LVpos = 0
bool 	sWidget_RDisplay = false
bool 	sWidget_RName = false
int 	sWidget_RHpos = 0
int 	sWidget_RVpos = 0
bool 	sWidget_SDisplay = false
bool 	sWidget_SName = false
int 	sWidget_SHpos = 0
int 	sWidget_SVpos = 0

int 	sSetting_Modifier1 = 42
int 	sSetting_Modifier2 = 29
int 	sSetting_Modifier3 = 56
int		sSetting_Sort = 0
string[] sSetting_SortString
bool 	sSetting_Gamepad = false
bool	sSetting_Favor = false

; Enum List
int ePage_Managing = 1
int ePage_Overview = 2
int ePage_Widget = 3
int ePage_Setting = 4
int ePage_Maintenance = 5

int eOpt_None = 0
int eOpt_New = 1
int eOpt_NewCycle = 2
int eOpt_Edit = 3
int eOpt_Remove = 4

int eType_Widget = 0
int eType_Weapon = 1
int eType_Shout = 2
int eType_Items = 3
int eType_CycleItems = 4
int eType_SelectList = 5
int eType_Equipset = 6
int eType_Font = 7

int eWidget_Disable = 0
int eWidget_Combat = 1
int eWidget_Enable = 2

string eColor_Green = "#4FE0A7"
string eColor_Red = "#7257CA"
string eColor_Yellow = "#F4EEB1"

Event OnConfigInit()
	ModName = "Hotkeys System"
	Pages = new string[5]
	Pages[0] = "$Managing"
	Pages[1] = "$Overview"
	Pages[2] = "$Widget"
	Pages[3] = "$Setting"
	Pages[4] = "$Maintenance"
	RegisterForKey(21) ; Y
	RegisterForKey(22) ; U

	Load()
EndEvent

Event OnConfigOpen()
	UIHS_Clear()
	UIHS_SendSettingData(ZipSettingData())
	UIHS_Init()
	Init_MCMOpt()
	Init_StoredOpt()
EndEvent

Event OnConfigClose()
	ClearProperties()
	UIHS_Clear()
	UnregisterForAllKeys()
	RegisterForAllKeys()
	Save()
EndEvent

Event OnGameReload()
	parent.OnGameReload()
	Load()
EndEvent

Event OnPageReset(string _page)	
	if _page == "$Managing"
		if CurPage != ePage_Managing
			UIHS_Clear()
			UIHS_SendSettingData(ZipSettingData())
			UIHS_Init()
			Init_MCMOpt()
			Init_StoredOpt()
			ClearProperties()
		endif
		OnPage_Managing()
	elseif _page == "$Overview"
		if CurPage != ePage_Overview
			UIHS_Clear()
			UIHS_SendSettingData(ZipSettingData())
			UIHS_Init()
			Init_MCMOpt()
			Init_StoredOpt()
			ClearProperties()
		endif
		OnPage_Overview()
	elseif _page == "$Widget"
		OnPage_Widget()
	elseif _page == "$Setting"
		OnPage_Setting()
	elseif _page == "$Maintenance"
		OnPage_Maintenance()
	endif
EndEvent

Event OnKeyDown(int _code)
	if !_code
		return
	endif

	if !sSetting_Gamepad
		bool modifier1 = false
		bool modifier2 = false
		bool modifier3 = false

		if Input.IsKeyPressed(sSetting_Modifier1)
			modifier1 = true
		endif
		
		if Input.IsKeyPressed(sSetting_Modifier2)
			modifier2 = true
		endif
		 
		if Input.IsKeyPressed(sSetting_Modifier3)
			modifier3 = true
		endif

		UIHS_Exec(_code, modifier1, modifier2, modifier3)

		return

	 ; for GamePad
	else
		if _code != sSetting_Modifier1 && _code != sSetting_Modifier2 && _code != sSetting_Modifier3
			bool modifier1 = false
			bool modifier2 = false
			bool modifier3 = false

			if Gamepad_Modifier1
				modifier1 = true
			endif
			if Gamepad_Modifier2
				modifier2 = true
			endif
			if Gamepad_Modifier3
				modifier3 = true
			endif
			
			UIHS_Exec(_code, modifier1, modifier2, modifier3)
			
			return
		endif
		if _code == sSetting_Modifier1
			Gamepad_Modifier1 = true
		
		elseif _code == sSetting_Modifier2
			Gamepad_Modifier2 = true
		
		elseif _code == sSetting_Modifier3
			Gamepad_Modifier3 = true
		endif
	endif
EndEvent

Event OnKeyUp(Int _code, Float _holdTime)
	if !_code
		return
	endif

	if !sSetting_Gamepad
		bool modifier1 = false
		bool modifier2 = false
		bool modifier3 = false

		if Input.IsKeyPressed(sSetting_Modifier1)
			modifier1 = true
		endif
		
		if Input.IsKeyPressed(sSetting_Modifier2)
			modifier2 = true
		endif
		 
		if Input.IsKeyPressed(sSetting_Modifier3)
			modifier3 = true
		endif

		UIHS_CalculateKeydown(_code, modifier1, modifier2, modifier3, _holdTime)

		return

	; for GamePad
	elseif sSetting_Gamepad
		if _code != sSetting_Modifier1 && _code != sSetting_Modifier2 && _code != sSetting_Modifier3
			bool modifier1 = false
			bool modifier2 = false
			bool modifier3 = false

			if Gamepad_Modifier1
				modifier1 = true
			endif
			if Gamepad_Modifier2
				modifier2 = true
			endif
			if Gamepad_Modifier3
				modifier3 = true
			endif
			
			UIHS_CalculateKeydown(_code, modifier1, modifier2, modifier3, _holdTime)
			
			return
		endif

		if _code == sSetting_Modifier1
			Gamepad_Modifier1 = false
		
		elseif _code == sSetting_Modifier2
			Gamepad_Modifier2 = false
		
		elseif _code == sSetting_Modifier3
			Gamepad_Modifier3 = false
		endif
	endif
EndEvent

Function Load()
	string[] data = UIHS_LoadSetting()

	if data.Length == 0
		return
	endif

	sWidget_Font		= data[0] as int
	sWidget_Size		= data[1] as int
	sWidget_Alpha		= data[2] as int
	sWidget_Display		= data[3] as int
	sWidget_Delay		= data[4] as float
	sWidget_EFont		= data[5] as int
	sWidget_ESize		= data[6] as int
	sWidget_EAlpha		= data[7] as int
	sWidget_EDisplay	= data[8] as int
	sWidget_EDelay		= data[9] as float
	sWidget_LDisplay	= to_bool(data[10])
	sWidget_LName		= to_bool(data[11])
	sWidget_LHpos		= data[12] as int
	sWidget_LVpos		= data[13] as int
	sWidget_RDisplay	= to_bool(data[14])
	sWidget_RName		= to_bool(data[15])
	sWidget_RHpos		= data[16] as int
	sWidget_RVpos		= data[17] as int
	sWidget_SDisplay	= to_bool(data[18])
	sWidget_SName		= to_bool(data[19])
	sWidget_SHpos		= data[20] as int
	sWidget_SVpos		= data[21] as int
	sSetting_Modifier1	= data[22] as int
	sSetting_Modifier2	= data[23] as int
	sSetting_Modifier3	= data[24] as int
	sSetting_Sort		= data[25] as int
	sSetting_Gamepad	= to_bool(data[26])
	sSetting_Favor		= to_bool(data[27])
EndFunction

Function Save()
; sWidget_Font
; sWidget_Size
; sWidget_Alpha
; sWidget_Display
; sWidget_Delay
; sWidget_EFont
; sWidget_ESize
; sWidget_EAlpha
; sWidget_EDisplay
; sWidget_EDelay
; sWidget_LDisplay
; sWidget_LName
; sWidget_LHpos
; sWidget_LVpos
; sWidget_RDisplay
; sWidget_RName
; sWidget_RHpos
; sWidget_RVpos
; sWidget_SDisplay
; sWidget_SName
; sWidget_SHpos
; sWidget_SVpos
; sSetting_Modifier1
; sSetting_Modifier2
; sSetting_Modifier3
; sSetting_Sort
; sSetting_Gamepad
; sSetting_Favor
	UIHS_SaveSetting(ZipSaveData())
EndFunction

Function RegisterForAllKeys()
	int[] list = UIHS_GetKeycodeList();

	int i = 0
	while i < list.Length
		RegisterForKey(list[i])
		i += 1
	endwhile

	if sSetting_Gamepad
		RegisterForkey(sSetting_Modifier1)
		RegisterForkey(sSetting_Modifier2)
		RegisterForkey(sSetting_Modifier3)
	endif
EndFunction

; Start of writting MCM pages functions.

Function Init_StoredOpt()
	sManaging_Name = UIHS_GetNamePrefix()
	sManaging_OName = sManaging_Name
	sManaging_Hotkey = -1
	sManaging_Modifier1 = false
	sManaging_Modifier2 = false
	sManaging_Modifier3 = false
	sManaging_Sound = true
	sManaging_Toggle = false
	sManaging_Reequip = false
	sManaging_Beast = false
	sManaging_Widget = 0
	sManaging_Hpos = 0
	sManaging_Vpos = 0
	sManaging_DWidget = false
	sManaging_DName = false
	sManaging_DHotkey = false
	sManaging_Left = 0
	sManaging_Right = 0
	sManaging_Shout = 0
	sManaging_numItems = 0
	sManaging_Items = new int[15] ; Need MAX_ITEMS must matched

	sManaging_CycleName = UIHS_GetNamePrefix()
	sManaging_CycleOName = sManaging_CycleName
	sManaging_CycleHotkey = -1
	sManaging_CycleModifier1 = false
	sManaging_CycleModifier2 = false
	sManaging_CycleModifier3 = false
	sManaging_CyclePersist = false
	sManaging_CycleExpire = 0.0
	sManaging_CycleReset = 0.0
	sManaging_CycleBeast = false
	sManaging_CycleHpos = 0
	sManaging_CycleVpos = 0
	sManaging_CycleDWidget = false
	sManaging_CycleDName = false
	sManaging_CycleDHotkey = false
	sManaging_CyclenumItems = 0
	sManaging_CycleItems = new int[5] ; Need MAX_CYCLEITEMS must matched

	sOverview_Equipset = -1

	sWidget_DisplayString = new String[3]
	sWidget_DisplayString[0] = "$Disable"
	sWidget_DisplayString[1] = "$In Combat"
	sWidget_DisplayString[2] = "$Enable"

	sSetting_SortString = new String[4]
	sSetting_SortString[0] = "$Create Ascending"
	sSetting_SortString[1] = "$Create Descending"
	sSetting_SortString[2] = "$Name Ascending"
	sSetting_SortString[3] = "$Name Descending"
EndFunction

Function Load_StoredOpt(string _name)
	string[] Data = UIHS_GetEquipsetData(_name)

	bool isCycle 				= to_bool(Data[0])
	if !isCycle
		sManaging_Name 			= Data[1]
		sManaging_OName			= Data[1]
		sManaging_Hotkey 		= Data[2] as int
		sManaging_Modifier1 	= to_bool(Data[3])
		sManaging_Modifier2 	= to_bool(Data[4])
		sManaging_Modifier3 	= to_bool(Data[5])
		sManaging_Sound 		= to_bool(Data[6])
		sManaging_Toggle 		= to_bool(Data[7])
		sManaging_Reequip 		= to_bool(Data[8])
		sManaging_Beast 		= to_bool(Data[9])
		sManaging_Widget 		= Data[10] as int
		sManaging_Hpos 			= Data[11] as int
		sManaging_Vpos 			= Data[12] as int
		sManaging_DWidget 		= to_bool(Data[13])
		sManaging_DName 		= to_bool(Data[14])
		sManaging_DHotkey 		= to_bool(Data[15])
		sManaging_Left 			= Data[16] as int
		sManaging_Right 		= Data[17] as int
		sManaging_Shout 		= Data[18] as int
		sManaging_numItems 		= Data[19] as int
		
		int i = 0
		while i < sManaging_numItems
			sManaging_Items[i] 	= Data[20 + i] as int
			i += 1
		endwhile
	else
		sManaging_CycleName 		= Data[1]
		sManaging_CycleOName 		= Data[1]
		sManaging_CycleHotkey 		= Data[2] as int
		sManaging_CycleModifier1 	= to_bool(Data[3])
		sManaging_CycleModifier2 	= to_bool(Data[4])
		sManaging_CycleModifier3 	= to_bool(Data[5])
		sManaging_CyclePersist 		= to_bool(Data[6])
		sManaging_CycleExpire 		= Data[7] as float
		sManaging_CycleReset 		= Data[8] as float
		sManaging_CycleBeast 		= to_bool(Data[9])
		sManaging_CycleHpos 		= Data[10] as int
		sManaging_CycleVpos 		= Data[11] as int
		sManaging_CycleDWidget 		= to_bool(Data[12])
		sManaging_CycleDName 		= to_bool(Data[13])
		sManaging_CycleDHotkey 		= to_bool(Data[14])
		sManaging_CyclenumItems 	= Data[15] as int

		int i = 0
		while i < sManaging_CyclenumItems
			sManaging_CycleItems[i] = Data[16 + i] as int
			i += 1
		endwhile
	endif
EndFunction

Function Init_MCMOpt()
	optManaging_Items = new int[15] ; Need MAX_ITEMS must matched
	optManaging_CycleItems = new int[5] ; Need MAX_CYCLEITEMS must matched
	optOverview_Equipset = new int[20] ; Need MAX_OVERVIEWLIST must matched
EndFunction

Function OnPage_Managing()
	CurPage = ePage_Managing;

	SetCursorFillMode(TOP_TO_BOTTOM)

	if CurOpt == eOpt_None
		optManaging_New		  = AddTextOption("", "$Add New Equipset", OPTION_FLAG_NONE)
		optManaging_NewCycle  = AddTextOption("", "$Add New Cycle Equipset", OPTION_FLAG_NONE)
		optManaging_Edit	  = AddMenuOption("", "$Edit Equipset", OPTION_FLAG_NONE)
		optManaging_Remove	  = AddMenuOption("", "$Remove Equipset", OPTION_FLAG_NONE)
	elseif CurOpt == eOpt_New
		OnPage_Managing_New()

	elseif CurOpt == eOpt_NewCycle
		OnPage_Managing_NewCycle()

	elseif CurOpt == eOpt_Edit
		if UIHS_IsCycleEquipset(sManaging_CycleOName)
			OnPage_Managing_NewCycle()
		elseif !UIHS_IsCycleEquipset(sManaging_OName)
			OnPage_Managing_New()
		endif
	endif
EndFunction

Function OnPage_Managing_New()
	optManaging_Name	  	= AddInputOption("$Equipset Name:", CString(sManaging_Name, eColor_Green), OPTION_FLAG_NONE)
	optManaging_Hotkey	  	= AddKeyMapOption("$Hotkey", sManaging_Hotkey, OPTION_FLAG_WITH_UNMAP)
	optManaging_Modifier1 	= AddToggleOption("$Left Shift", sManaging_Modifier1, OPTION_FLAG_NONE)
	optManaging_Modifier2 	= AddToggleOption("$Left Control", sManaging_Modifier2, OPTION_FLAG_NONE)
	optManaging_Modifier3 	= AddToggleOption("$Left Alt", sManaging_Modifier3, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	optManaging_Sound	  	= AddToggleOption("$Equip Sound", sManaging_Sound, OPTION_FLAG_NONE)
	optManaging_Toggle	  	= AddToggleOption("$Toggle Equip/Unequip", sManaging_Toggle, OPTION_FLAG_NONE)
	optManaging_Reequip	  	= AddToggleOption("$Re Equip", sManaging_Reequip, OPTION_FLAG_NONE)
	optManaging_Beast	  	= AddToggleOption("$Beast", sManaging_Beast, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	optManaging_Widget	  	= AddMenuOption("$Widget Type", UIHS_GetStringFromList(sManaging_Widget, eType_Widget), OPTION_FLAG_NONE)
	optManaging_Hpos	  	= AddSliderOption("$Horizontal Position", sManaging_Hpos as float, "{0}", OPTION_FLAG_NONE)
	optManaging_Vpos	  	= AddSliderOption("$Vertical Position", sManaging_Vpos as float, "{0}", OPTION_FLAG_NONE)
	optManaging_DWidget	  	= AddToggleOption("$Display Equipset Widget", sManaging_DWidget, OPTION_FLAG_NONE)
	optManaging_DName		= AddToggleOption("$Display Equipset Name", sManaging_DName, OPTION_FLAG_NONE)
	optManaging_DHotkey	  	= AddToggleOption("$Display Equipset Hotkey", sManaging_DHotkey, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	if CurOpt == eOpt_New
		optManaging_Confirm	= AddTextOption("", "$Confirm Adding New Equipset", OPTION_FLAG_NONE)
	elseif CurOpt == eOpt_Edit
		optManaging_Confirm	= AddTextOption("", "$Confirm Editing Equipset", OPTION_FLAG_NONE)
	endif
	optManaging_Cancel		= AddTextOption("", "$Cancel and Back", OPTION_FLAG_NONE)

	SetCursorPosition(1)

	AddHeaderOption("$Weapons", OPTION_FLAG_NONE)
	optManaging_Left	  	= AddMenuOption("$Lefthand", UIHS_GetStringFromList(sManaging_Left, eType_Weapon), OPTION_FLAG_NONE)
	optManaging_Right	  	= AddMenuOption("$Righthand", UIHS_GetStringFromList(sManaging_Right, eType_Weapon), OPTION_FLAG_NONE)
	optManaging_Shout	  	= AddMenuOption("$Shout", UIHS_GetStringFromList(sManaging_Shout, eType_Shout), OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Items", OPTION_FLAG_NONE)

	int i = 0
	while i < MAX_ITEMS
		if i < sManaging_numItems
			optManaging_Items[i] = AddMenuOption("", UIHS_GetStringFromList(sManaging_Items[i], eType_Items), OPTION_FLAG_NONE)
		elseif i == sManaging_numItems
			optManaging_Items[i] = AddMenuOption("", "$+", OPTION_FLAG_NONE)
		else
			optManaging_Items[i] = AddMenuOption("", "$+", OPTION_FLAG_HIDDEN)
		endif
		i += 1
	endwhile
EndFunction

Function OnPage_Managing_NewCycle()
	optManaging_CycleName			= AddInputOption("$Equipset Name:", CString(sManaging_CycleName, eColor_Green), OPTION_FLAG_NONE)
	optManaging_CycleHotkey			= AddKeyMapOption("$Hotkey", sManaging_CycleHotkey, OPTION_FLAG_WITH_UNMAP)
	optManaging_CycleModifier1  	= AddToggleOption("$Left Shift", sManaging_CycleModifier1, OPTION_FLAG_NONE)
	optManaging_CycleModifier2  	= AddToggleOption("$Left Control", sManaging_CycleModifier2, OPTION_FLAG_NONE)
	optManaging_CycleModifier3  	= AddToggleOption("$Left Alt", sManaging_CycleModifier3, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	optManaging_CyclePersist		= AddToggleOption("$Cycle Persist", sManaging_CyclePersist, OPTION_FLAG_NONE)
	optManaging_CycleExpire			= AddSliderOption("$Cycle Expire", sManaging_CycleExpire, "{1}sec", OPTION_FLAG_NONE)
	optManaging_CycleReset			= AddSliderOption("$Cycle Reset", sManaging_CycleReset, "{2}sec", OPTION_FLAG_NONE)
	optManaging_CycleBeast			= AddToggleOption("$Beast", sManaging_CycleBeast, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	optManaging_CycleHpos			= AddSliderOption("$Horizontal Position", sManaging_CycleHpos as float, "{0}", OPTION_FLAG_NONE)
	optManaging_CycleVpos			= AddSliderOption("$Vertical Position", sManaging_CycleVpos as float, "{0}", OPTION_FLAG_NONE)
	optManaging_CycleDWidget		= AddToggleOption("$Display Equipset Widget", sManaging_CycleDWidget, OPTION_FLAG_NONE)
	optManaging_CycleDName			= AddToggleOption("$Display Equipset Name", sManaging_CycleDName, OPTION_FLAG_NONE)
	optManaging_CycleDHotkey		= AddToggleOption("$Display Equipset Hotkey", sManaging_CycleDHotkey, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	if CurOpt == eOpt_NewCycle
		optManaging_CycleConfirm	= AddTextOption("", "$Confirm Adding New Equipset", OPTION_FLAG_NONE)
	elseif CurOpt == eOpt_Edit
		optManaging_CycleConfirm	= AddTextOption("", "$Confirm Editing Equipset", OPTION_FLAG_NONE)
	endif
	optManaging_CycleCancel			= AddTextOption("", "$Cancel and Back", OPTION_FLAG_NONE)

	SetCursorPosition(1)

	AddHeaderOption("$Equipsets", OPTION_FLAG_NONE)

	int i = 0
	while i < MAX_CYCLEITEMS
		if i < sManaging_CyclenumItems
			optManaging_CycleItems[i] = AddMenuOption("", UIHS_GetStringFromList(sManaging_CycleItems[i], eType_CycleItems), OPTION_FLAG_NONE)
		elseif i == sManaging_CyclenumItems
			optManaging_CycleItems[i] = AddMenuOption("", "$+", OPTION_FLAG_NONE)
		else
			optManaging_CycleItems[i] = AddMenuOption("", "$+", OPTION_FLAG_HIDDEN)
		endif
		i += 1
	endwhile
EndFunction

Function OnPage_Overview()
	CurPage = ePage_Overview;
	
	SetCursorFillMode(TOP_TO_BOTTOM)

	if CurOpt == eOpt_None
		string[] equipset = UIHS_GetList(eType_Equipset)

		if equipset.Length == 0
			AddTextOption("$No Equipset Found", "", OPTION_FLAG_NONE)
		endif

		int i = 0
		while i < equipset.Length && i < MAX_OVERVIEWLIST
			if i != sOverview_Equipset
				optOverview_Equipset[i] = AddTextOption(UIHS_GetStringFromList(i, eType_Equipset), "$Open", OPTION_FLAG_NONE)
			else
				optOverview_Equipset[i] = AddTextOption(CString(UIHS_GetStringFromList(i, eType_Equipset), eColor_Green), "", OPTION_FLAG_NONE)
			endif
			i += 1
		endwhile

		SetCursorPosition(1)

		if sOverview_Equipset != -1
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnPage_Overview_CycleItems()
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnPage_Overview_Items()
			endif
		endif
	elseif CurOpt == eOpt_Edit
		if UIHS_IsCycleEquipset(sManaging_CycleOName)
			OnPage_Managing_NewCycle()
		elseif !UIHS_IsCycleEquipset(sManaging_OName)
			OnPage_Managing_New()
		endif
	endif
EndFunction

Function OnPage_Overview_Items()
	AddHeaderOption(CString(UIHS_GetStringFromList(sOverview_Equipset, eType_Equipset), eColor_Green), OPTION_FLAG_NONE)
	AddTextOption("$Lefthand", UIHS_GetStringFromList(sManaging_Left, eType_Weapon), OPTION_FLAG_NONE)
	AddTextOption("$Righthand", UIHS_GetStringFromList(sManaging_Right, eType_Weapon), OPTION_FLAG_NONE)
	AddTextOption("$Shout", UIHS_GetStringFromList(sManaging_Shout, eType_Shout), OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Items", OPTION_FLAG_NONE)
	
	if sManaging_numItems == 0
		AddTextOption("", "$Nothing", OPTION_FLAG_NONE)
	else
		int j = 0
		while j < sManaging_numItems
			AddTextOption("", UIHS_GetStringFromList(sManaging_Items[j], eType_Items), OPTION_FLAG_NONE)
			j += 1
		endwhile
	endif

	AddHeaderOption("", OPTION_FLAG_NONE)
	optOverview_Edit = AddTextOption("", "Edit", OPTION_FLAG_NONE)
	optOverview_Remove = AddTextOption("", "Remove", OPTION_FLAG_NONE)
EndFunction

Function OnPage_Overview_CycleItems()
	AddHeaderOption(CString(UIHS_GetStringFromList(sOverview_Equipset, eType_Equipset), eColor_Green), OPTION_FLAG_NONE)
	
	if sManaging_CyclenumItems == 0
		AddTextOption("", "$Nothing", OPTION_FLAG_NONE)
	else
		int j = 0
		while j < sManaging_CyclenumItems
			AddTextOption("", UIHS_GetStringFromList(sManaging_CycleItems[j], eType_CycleItems), OPTION_FLAG_NONE)
			j += 1
		endwhile
	endif

	AddHeaderOption("", OPTION_FLAG_NONE)
	optOverview_Edit = AddTextOption("", "Edit", OPTION_FLAG_NONE)
	optOverview_Remove = AddTextOption("", "Remove", OPTION_FLAG_NONE)
EndFunction

Function OnPage_Widget()
	CurPage = ePage_Widget;
	
	SetCursorFillMode(TOP_TO_BOTTOM)
	AddHeaderOption("$Widget Setting", OPTION_FLAG_NONE)
	optWidget_Font = AddMenuOption("$Widget Font", UIHS_GetStringFromList(sWidget_Font, eType_Font), OPTION_FLAG_NONE)
	optWidget_Size = AddSliderOption("$Widget Size", sWidget_Size as float, "{0}%", OPTION_FLAG_NONE)
	optWidget_Alpha = AddSliderOption("$Widget Background Transparency", sWidget_Alpha as float, "{0}%", OPTION_FLAG_NONE)
	optWidget_Display = AddMenuOption("$Widget Display Mode", sWidget_DisplayString[sWidget_Display], OPTION_FLAG_NONE)
	optWidget_Delay = AddSliderOption("$Widget Dissolve Delay", sWidget_Delay, "{1}sec", OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Equipment Widget Setting", OPTION_FLAG_NONE)
	optWidget_EFont = AddMenuOption("$Widget Font", UIHS_GetStringFromList(sWidget_EFont, eType_Font), OPTION_FLAG_NONE)
	optWidget_ESize = AddSliderOption("$Widget Size", sWidget_ESize as float, "{0}%", OPTION_FLAG_NONE)
	optWidget_EAlpha = AddSliderOption("$Widget Background Transparency", sWidget_EAlpha as float, "{0}%", OPTION_FLAG_NONE)
	optWidget_EDisplay = AddMenuOption("$Widget Display Mode", sWidget_DisplayString[sWidget_EDisplay], OPTION_FLAG_NONE)
	optWidget_EDelay = AddSliderOption("$Widget Dissolve Delay", sWidget_EDelay, "{1}sec", OPTION_FLAG_NONE)

	SetCursorPosition(1)

	AddHeaderOption("$Lefthand", OPTION_FLAG_NONE)
	optWidget_LDisplay = AddToggleOption("$Display Widget", sWidget_LDisplay, OPTION_FLAG_NONE)
	optWidget_LName = AddToggleOption("$Display Name", sWidget_LName, OPTION_FLAG_NONE)
	optWidget_LHpos = AddSliderOption("$Horizontal Position", sWidget_LHpos as float, "{0}", OPTION_FLAG_NONE)
	optWidget_LVpos = AddSliderOption("$Vertical Position", sWidget_LVpos as float, "{0}", OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Righthand", OPTION_FLAG_NONE)
	optWidget_RDisplay = AddToggleOption("$Display Widget", sWidget_RDisplay, OPTION_FLAG_NONE)
	optWidget_RName = AddToggleOption("$Display Name", sWidget_RName, OPTION_FLAG_NONE)
	optWidget_RHpos = AddSliderOption("$Horizontal Position", sWidget_RHpos as float, "{0}", OPTION_FLAG_NONE)
	optWidget_RVpos = AddSliderOption("$Vertical Position", sWidget_RVpos as float, "{0}", OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Shout", OPTION_FLAG_NONE)
	optWidget_SDisplay = AddToggleOption("$Display Widget", sWidget_SDisplay, OPTION_FLAG_NONE)
	optWidget_SName = AddToggleOption("$Display Name", sWidget_SName, OPTION_FLAG_NONE)
	optWidget_SHpos = AddSliderOption("$Horizontal Position", sWidget_SHpos as float, "{0}", OPTION_FLAG_NONE)
	optWidget_SVpos = AddSliderOption("$Vertical Position", sWidget_SVpos as float, "{0}", OPTION_FLAG_NONE)
EndFunction

Function OnPage_Setting()
	CurPage = ePage_Setting;
	
	SetCursorFillMode(TOP_TO_BOTTOM)
	AddHeaderOption("$Modifier", OPTION_FLAG_NONE)
	optSetting_Modifier1 = AddKeyMapOption("$Modifier Key 1", sSetting_Modifier1, OPTION_FLAG_WITH_UNMAP)
	optSetting_Modifier2 = AddKeyMapOption("$Modifier Key 2", sSetting_Modifier2, OPTION_FLAG_WITH_UNMAP)
	optSetting_Modifier3 = AddKeyMapOption("$Modifier Key 3", sSetting_Modifier3, OPTION_FLAG_WITH_UNMAP)
	AddEmptyOption()
	AddHeaderOption("$Equipset", OPTION_FLAG_NONE)
	optSetting_Sort = AddMenuOption("$Sort order", sSetting_SortString[sSetting_Sort], OPTION_FLAG_NONE)
	optSetting_Favor = AddToggleOption("$Show Favorited Items Only", sSetting_Favor, OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Gamepad", OPTION_FLAG_NONE)
	optSetting_Gamepad = AddToggleOption("$Gamepad Support", sSetting_Gamepad, OPTION_FLAG_NONE)
EndFunction

Function OnPage_Maintenance()
	CurPage = ePage_Maintenance;
	
	SetCursorFillMode(TOP_TO_BOTTOM)
	AddHeaderOption("$Save", OPTION_FLAG_NONE)
	AddTextOption("$Save MCM Setting", "", OPTION_FLAG_NONE)
	AddTextOption("$Save EquipSets", "", OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$Load", OPTION_FLAG_NONE)
	AddTextOption("$Load MCM Setting", "", OPTION_FLAG_NONE)
	AddTextOption("$Load EquipSets", "", OPTION_FLAG_NONE)
	
	SetCursorPosition(1)

	AddHeaderOption("$Maintenance", OPTION_FLAG_NONE)
	AddTextOption("$Reset Setting", "Go", OPTION_FLAG_NONE)
	AddTextOption("$Reset Equipsets", "Go", OPTION_FLAG_NONE)
	AddTextOption("$Mod Status", CString("$Activated", "#4FE0A7"), OPTION_FLAG_NONE)
	AddTextOption("$Widget Status", CString("$Activated", "#4FE0A7"), OPTION_FLAG_NONE)
	AddTextOption("$Equipment Widget Status", CString("$Activated", "#4FE0A7"), OPTION_FLAG_NONE)
EndFunction

; End of writting MCM pages functions.


; Start of MCM option open functions.

Function OnOptionMenuOpen(int _opt)
	if CurPage == ePage_Managing
		if CurOpt == eOpt_None
			if _opt == optManaging_Edit
				SetMenuDialogOptions(UIHS_GetList(eType_SelectList))
				SetMenuDialogStartIndex(0)
				SetMenuDialogDefaultIndex(0)
			elseif _opt == optManaging_Remove
				SetMenuDialogOptions(UIHS_GetList(eType_SelectList))
				SetMenuDialogStartIndex(0)
				SetMenuDialogDefaultIndex(0)
			endif
		elseif CurOpt == eOpt_New
			OnOptionMenuOpen_New(_opt)
		elseif CurOpt == eOpt_NewCycle
			OnOptionMenuOpen_NewCycle(_opt)
		elseif CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionMenuOpen_NewCycle(_opt)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionMenuOpen_New(_opt)
			endif
		endif
	elseif CurPage == ePage_Overview
		if CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionMenuOpen_NewCycle(_opt)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionMenuOpen_New(_opt)
			endif
		endif
	elseif CurPage == ePage_Widget
		if _opt == optWidget_Font
			string[] list = UIHS_GetList(eType_Font)
			SetMenuDialogOptions(list)
			SetMenuDialogStartIndex(sWidget_Font)
			SetMenuDialogDefaultIndex(sWidget_Font)
		elseif _opt == optWidget_EFont
			string[] list = UIHS_GetList(eType_Font)
			SetMenuDialogOptions(list)
			SetMenuDialogStartIndex(sWidget_EFont)
			SetMenuDialogDefaultIndex(sWidget_EFont)
		elseif _opt == optWidget_Display
			SetMenuDialogOptions(sWidget_DisplayString)
			SetMenuDialogStartIndex(sWidget_Display)
			SetMenuDialogDefaultIndex(sWidget_Display)
		elseif _opt == optWidget_EDisplay
			SetMenuDialogOptions(sWidget_DisplayString)
			SetMenuDialogStartIndex(sWidget_EDisplay)
			SetMenuDialogDefaultIndex(sWidget_EDisplay)
		endif
	elseif CurPage == ePage_Setting
		if _opt == optSetting_Sort
			SetMenuDialogOptions(sSetting_SortString)
			SetMenuDialogStartIndex(sSetting_Sort)
			SetMenuDialogDefaultIndex(sSetting_Sort)
		endif
	endif
EndFunction

Function OnOptionMenuOpen_New(int _opt)
	if _opt == optManaging_Widget
		SetMenuDialogOptions(UIHS_GetList(eType_Widget))
		SetMenuDialogStartIndex(sManaging_Widget)
		SetMenuDialogDefaultIndex(sManaging_Widget)
	elseif _opt == optManaging_Left
		SetMenuDialogOptions(UIHS_GetList(eType_Weapon))
		SetMenuDialogStartIndex(sManaging_Left)
		SetMenuDialogDefaultIndex(sManaging_Left)
	elseif _opt == optManaging_Right
		SetMenuDialogOptions(UIHS_GetList(eType_Weapon))
		SetMenuDialogStartIndex(sManaging_Right)
		SetMenuDialogDefaultIndex(sManaging_Right)
	elseif _opt == optManaging_Shout
		SetMenuDialogOptions(UIHS_GetList(eType_Shout))
		SetMenuDialogStartIndex(sManaging_Shout)
		SetMenuDialogDefaultIndex(sManaging_Shout)
	endif

	int i = 0
	while i < MAX_ITEMS
		if _opt == optManaging_Items[i]
			SetMenuDialogOptions(UIHS_GetList(eType_Items))
			SetMenuDialogStartIndex(sManaging_Items[i])
			SetMenuDialogDefaultIndex(sManaging_Items[i])
		endif
		i += 1
	endwhile
EndFunction

Function OnOptionMenuOpen_NewCycle(int _opt)
	int i = 0
	while i < MAX_CYCLEITEMS
		if _opt == optManaging_CycleItems[i]
			SetMenuDialogOptions(UIHS_GetList(eType_CycleItems))
			SetMenuDialogStartIndex(sManaging_CycleItems[i])
			SetMenuDialogDefaultIndex(sManaging_CycleItems[i])
		endif
		i += 1
	endwhile
EndFunction

Function OnOptionSliderOpen(int _opt)
	if CurPage == ePage_Managing
		if CurOpt == eOpt_New
			OnOptionSliderOpen_New(_opt)
		elseif CurOpt == eOpt_NewCycle
			OnOptionSliderOpen_NewCycle(_opt)
		elseif CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionSliderOpen_NewCycle(_opt)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionSliderOpen_New(_opt)
			endif
		endif
	elseif CurPage == ePage_Overview
		if CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionSliderOpen_NewCycle(_opt)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionSliderOpen_New(_opt)
			endif
		endif
	elseif CurPage == ePage_Widget
		if _opt == optWidget_Size
			SetSliderDialogStartValue(sWidget_Size as float)
			SetSliderDialogDefaultValue(sWidget_Size as float)
			SetSliderDialogRange(0.0, 200.0)
			SetSliderDialogInterval(5.0)
		elseif _opt == optWidget_Alpha
			SetSliderDialogStartValue(sWidget_Alpha as float)
			SetSliderDialogDefaultValue(sWidget_Alpha as float)
			SetSliderDialogRange(0.0, 100.0)
			SetSliderDialogInterval(5.0)
		elseif _opt == optWidget_Delay
			SetSliderDialogStartValue(sWidget_Delay)
			SetSliderDialogDefaultValue(sWidget_Delay)
			SetSliderDialogRange(0.0, 10.0)
			SetSliderDialogInterval(0.5)
		elseif _opt == optWidget_ESize
			SetSliderDialogStartValue(sWidget_ESize as float)
			SetSliderDialogDefaultValue(sWidget_ESize as float)
			SetSliderDialogRange(0.0, 200.0)
			SetSliderDialogInterval(5.0)
		elseif _opt == optWidget_EAlpha
			SetSliderDialogStartValue(sWidget_EAlpha as float)
			SetSliderDialogDefaultValue(sWidget_EAlpha as float)
			SetSliderDialogRange(0.0, 100.0)
			SetSliderDialogInterval(5.0)
		elseif _opt == optWidget_EDelay
			SetSliderDialogStartValue(sWidget_EDelay)
			SetSliderDialogDefaultValue(sWidget_EDelay)
			SetSliderDialogRange(0.0, 10.0)
			SetSliderDialogInterval(0.5)
		elseif _opt == optWidget_LHPos
			SetSliderDialogStartValue(sWidget_LHPos as float)
			SetSliderDialogDefaultValue(sWidget_LHPos as float)
			SetSliderDialogRange(-100.0, 100.0)
			SetSliderDialogInterval(1.0)
		elseif _opt == optWidget_LVPos
			SetSliderDialogStartValue(sWidget_LVPos as float)
			SetSliderDialogDefaultValue(sWidget_LVPos as float)
			SetSliderDialogRange(-100.0, 100.0)
			SetSliderDialogInterval(1.0)
		elseif _opt == optWidget_RHPos
			SetSliderDialogStartValue(sWidget_RHPos as float)
			SetSliderDialogDefaultValue(sWidget_RHPos as float)
			SetSliderDialogRange(-100.0, 100.0)
			SetSliderDialogInterval(1.0)
		elseif _opt == optWidget_RVPos
			SetSliderDialogStartValue(sWidget_RVPos as float)
			SetSliderDialogDefaultValue(sWidget_RVPos as float)
			SetSliderDialogRange(-100.0, 100.0)
			SetSliderDialogInterval(1.0)
		elseif _opt == optWidget_SHPos
			SetSliderDialogStartValue(sWidget_SHPos as float)
			SetSliderDialogDefaultValue(sWidget_SHPos as float)
			SetSliderDialogRange(-100.0, 100.0)
			SetSliderDialogInterval(1.0)
		elseif _opt == optWidget_SVPos
			SetSliderDialogStartValue(sWidget_SVPos as float)
			SetSliderDialogDefaultValue(sWidget_SVPos as float)
			SetSliderDialogRange(-100.0, 100.0)
			SetSliderDialogInterval(1.0)
		endif
	endif
EndFunction

Function OnOptionSliderOpen_New(int _opt)
	if _opt == optManaging_Hpos
		SetSliderDialogStartValue(sManaging_Hpos as float)
		SetSliderDialogDefaultValue(sManaging_Hpos as float)
		SetSliderDialogRange(-100.0, 100.0)
		SetSliderDialogInterval(1.0)
	elseif _opt == optManaging_Vpos
		SetSliderDialogStartValue(sManaging_Vpos as float)
		SetSliderDialogDefaultValue(sManaging_Vpos as float)
		SetSliderDialogRange(-100.0, 100.0)
		SetSliderDialogInterval(1.0)
	endif
EndFunction

Function OnOptionSliderOpen_NewCycle(int _opt)
	if _opt == optManaging_CycleExpire
		SetSliderDialogStartValue(sManaging_CycleExpire)
		SetSliderDialogDefaultValue(sManaging_CycleExpire)
		SetSliderDialogRange(0.0, 20.0)
		SetSliderDialogInterval(0.5)
	elseif _opt == optManaging_CycleReset
		SetSliderDialogStartValue(sManaging_CycleReset)
		SetSliderDialogDefaultValue(sManaging_CycleReset)
		SetSliderDialogRange(0.0, 3.0)
		SetSliderDialogInterval(0.05)
	elseif _opt == optManaging_CycleHpos
		SetSliderDialogStartValue(sManaging_CycleHpos as float)
		SetSliderDialogDefaultValue(sManaging_CycleHpos as float)
		SetSliderDialogRange(-100.0, 100.0)
		SetSliderDialogInterval(1.0)
	elseif _opt == optManaging_CycleVpos
		SetSliderDialogStartValue(sManaging_CycleVpos as float)
		SetSliderDialogDefaultValue(sManaging_CycleVpos as float)
		SetSliderDialogRange(-100.0, 100.0)
		SetSliderDialogInterval(1.0)
	endif
EndFunction

; End of MCM option open functions.


; Start of MCM option select or accept functions.

Function OnOptionSelect(Int _opt)
	if CurPage == ePage_Managing
		if CurOpt == eOpt_None
			if _opt == optManaging_New
				CurOpt = eOpt_New
			elseif _opt == optManaging_NewCycle
				CurOpt = eOpt_NewCycle
			endif
			SetTextOptionValue(_opt, CString("$Loading...", eColor_Yellow), false)
			ForcePageReset()

		elseif CurOpt == eOpt_New
			OnOptionSelect_New(_opt)
		elseif CurOpt == eOpt_NewCycle
			OnOptionSelect_NewCycle(_opt)
		elseif CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionSelect_NewCycle(_opt)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionSelect_New(_opt)
			endif
		endif
	elseif CurPage == ePage_Overview
		if CurOpt == eOpt_None
			int i = 0
			while i < MAX_OVERVIEWLIST
				if _opt == optOverview_Equipset[i]
					SetTextOptionValue(_opt, CString("$Opening...", eColor_Yellow), false)
					Init_MCMOpt()
					Init_StoredOpt()
					sOverview_Equipset = i
					string name = UIHS_GetStringFromList(i, eType_Equipset)
					Load_StoredOpt(name)
					ForcePageReset()
				endif
				i += 1
			endwhile

			if _opt == optOverview_Edit
				CurOpt = eOpt_Edit
				SetTextOptionValue(_opt, CString("$Loading...", eColor_Yellow), false)
				ForcePageReset()
			elseif _opt == optOverview_Remove
				if UIHS_IsCycleEquipset(sManaging_CycleOName)
					UIHS_RemoveEquipset(sManaging_CycleOName)
				elseif !UIHS_IsCycleEquipset(sManaging_OName)
					UIHS_RemoveEquipset(sManaging_OName)
				endif
				SetTextOptionValue(_opt, CString("$Removing...", eColor_Yellow), false)
				UIHS_Clear()
				UIHS_SendSettingData(ZipSettingData())
				UIHS_Init()
				Init_MCMOpt()
				Init_StoredOpt()
				ClearProperties()
				ForcePageReset()
			endif
		elseif CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionSelect_NewCycle(_opt)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionSelect_New(_opt)
			endif
		endif
	elseif CurPage == ePage_Widget
		if _opt == optWidget_LDisplay
			sWidget_LDisplay = !sWidget_LDisplay
			SetToggleOptionValue(_opt, sWidget_LDisplay, false)
		elseif _opt == optWidget_LName
			sWidget_LName = !sWidget_LName
			SetToggleOptionValue(_opt, sWidget_LName, false)
		elseif _opt == optWidget_RDisplay
			sWidget_RDisplay = !sWidget_RDisplay
			SetToggleOptionValue(_opt, sWidget_RDisplay, false)
		elseif _opt == optWidget_RName
			sWidget_RName = !sWidget_RName
			SetToggleOptionValue(_opt, sWidget_RName, false)
		elseif _opt == optWidget_SDisplay
			sWidget_SDisplay = !sWidget_SDisplay
			SetToggleOptionValue(_opt, sWidget_SDisplay, false)
		elseif _opt == optWidget_SName
			sWidget_SName = !sWidget_SName
			SetToggleOptionValue(_opt, sWidget_SName, false)
		endif
	elseif CurPage == ePage_Setting
		if _opt == optSetting_Favor
			sSetting_Favor = !sSetting_Favor
			SetToggleOptionValue(_opt, sSetting_Favor, false)
		elseif _opt == optSetting_Gamepad
				sSetting_Gamepad = !sSetting_Gamepad
				SetToggleOptionValue(_opt, sSetting_Gamepad, false)
		endif
	endif
EndFunction

Function OnOptionSelect_New(Int _opt)
	if _opt == optManaging_Modifier1
		sManaging_Modifier1 = !sManaging_Modifier1
		SetToggleOptionValue(_opt, sManaging_Modifier1, false)
	elseif _opt == optManaging_Modifier2
		sManaging_Modifier2 = !sManaging_Modifier2
		SetToggleOptionValue(_opt, sManaging_Modifier2, false)
	elseif _opt == optManaging_Modifier3
		sManaging_Modifier3 = !sManaging_Modifier3
		SetToggleOptionValue(_opt, sManaging_Modifier3, false)
	elseif _opt == optManaging_Sound
		sManaging_Sound = !sManaging_Sound
		SetToggleOptionValue(_opt, sManaging_Sound, false)
	elseif _opt == optManaging_Toggle
		sManaging_Toggle = !sManaging_Toggle
		SetToggleOptionValue(_opt, sManaging_Toggle, false)
	elseif _opt == optManaging_Reequip
		sManaging_Reequip = !sManaging_Reequip
		SetToggleOptionValue(_opt, sManaging_Reequip, false)
	elseif _opt == optManaging_Beast
		sManaging_Beast = !sManaging_Beast
		SetToggleOptionValue(_opt, sManaging_Beast, false)
	elseif _opt == optManaging_DWidget
		sManaging_DWidget = !sManaging_DWidget
		SetToggleOptionValue(_opt, sManaging_DWidget, false)
	elseif _opt == optManaging_DName
		sManaging_DName = !sManaging_DName
		SetToggleOptionValue(_opt, sManaging_DName, false)
	elseif _opt == optManaging_DHotkey
		sManaging_DHotkey = !sManaging_DHotkey
		SetToggleOptionValue(_opt, sManaging_DHotkey, false)
	elseif _opt == optManaging_Confirm
		if CurOpt != eOpt_Edit
			bool[] modifier = new bool[3]
			modifier[0] = sManaging_Modifier1
			modifier[1] = sManaging_Modifier2
			modifier[2] = sManaging_Modifier3
			string conflictName = UIHS_GetKeyConflict(sManaging_Hotkey, modifier, sManaging_Beast);

			if UIHS_IsNameConflict(sManaging_Name)
				ShowMessage("$Name Conflict", false, "OK", "")
			elseif conflictName != "_NOTFOUND_"
				ShowMessage("$Hotkey Conflict", false, "OK", "")
			elseif UIHS_NewEquipset(ZipData(false))
				ShowMessage("$Successfully Equipset Added", false, "OK", "")
				UIHS_Clear()
				UIHS_SendSettingData(ZipSettingData())
				UIHS_Init()
				Init_MCMOpt()
				Init_StoredOpt()
				ClearProperties()
				ForcePageReset()
			else
				ShowMessage("$failed to add Equipset", false, "OK", "")
			endif
		else
			bool[] modifier = new bool[3]
			modifier[0] = sManaging_Modifier1
			modifier[1] = sManaging_Modifier2
			modifier[2] = sManaging_Modifier3
			string conflictName = UIHS_GetKeyConflict(sManaging_Hotkey, modifier, sManaging_Beast);

			if UIHS_IsNameConflict(sManaging_Name) && sManaging_Name != sManaging_OName
				ShowMessage("$Name Conflict", false, "OK", "")
			elseif conflictName != "_NOTFOUND_" && conflictName != sManaging_OName
				ShowMessage("$Hotkey Conflict", false, "OK", "")
			elseif UIHS_EditEquipset(sManaging_OName, ZipData(false))
				ShowMessage("$Successfully Equipset Edited", false, "OK", "")
				UIHS_Clear()
				UIHS_SendSettingData(ZipSettingData())
				UIHS_Init()
				Init_MCMOpt()
				Init_StoredOpt()
				ClearProperties()
				ForcePageReset()
			else
				ShowMessage("$failed to Edit Equipset", false, "OK", "")
			endif
		endif
	endif
EndFunction

Function OnOptionSelect_NewCycle(Int _opt)
	if _opt == optManaging_CycleModifier1
		sManaging_CycleModifier1 = !sManaging_CycleModifier1
		SetToggleOptionValue(_opt, sManaging_CycleModifier1, false)
	elseif _opt == optManaging_CycleModifier2
		sManaging_CycleModifier2 = !sManaging_CycleModifier2
		SetToggleOptionValue(_opt, sManaging_CycleModifier2, false)
	elseif _opt == optManaging_CycleModifier3
		sManaging_CycleModifier3 = !sManaging_CycleModifier3
		SetToggleOptionValue(_opt, sManaging_CycleModifier3, false)
	elseif _opt == optManaging_CyclePersist
		sManaging_CyclePersist = !sManaging_CyclePersist
		SetToggleOptionValue(_opt, sManaging_CyclePersist, false)
	elseif _opt == optManaging_CycleBeast
		sManaging_CycleBeast = !sManaging_CycleBeast
		SetToggleOptionValue(_opt, sManaging_CycleBeast, false)
	elseif _opt == optManaging_CycleDWidget
		sManaging_CycleDWidget = !sManaging_CycleDWidget
		SetToggleOptionValue(_opt, sManaging_CycleDWidget, false)
	elseif _opt == optManaging_CycleDName
		sManaging_CycleDName = !sManaging_CycleDName
		SetToggleOptionValue(_opt, sManaging_CycleDName, false)
	elseif _opt == optManaging_CycleDHotkey
		sManaging_CycleDHotkey = !sManaging_CycleDHotkey
		SetToggleOptionValue(_opt, sManaging_CycleDHotkey, false)
	elseif _opt == optManaging_CycleConfirm
		if CurOpt != eOpt_Edit
			bool[] modifier = new bool[3]
			modifier[0] = sManaging_CycleModifier1
			modifier[1] = sManaging_CycleModifier2
			modifier[2] = sManaging_CycleModifier3
			string conflictName = UIHS_GetKeyConflict(sManaging_CycleHotkey, modifier, sManaging_CycleBeast);

			if UIHS_IsNameConflict(sManaging_CycleName)
				ShowMessage("$Name Conflict", false, "OK", "")
			elseif conflictName != "_NOTFOUND_"
				ShowMessage("$Hotkey Conflict", false, "OK", "")
			elseif UIHS_NewCycleEquipset(ZipData(true))
				ShowMessage("$Successfully Equipset Added", false, "OK", "")
				Init_MCMOpt()
				Init_StoredOpt()
				ClearProperties()
				ForcePageReset()
			else
				ShowMessage("$failed to add Equipset", false, "OK", "")
			endif
		else
			bool[] modifier = new bool[3]
			modifier[0] = sManaging_CycleModifier1
			modifier[1] = sManaging_CycleModifier2
			modifier[2] = sManaging_CycleModifier3
			string conflictName = UIHS_GetKeyConflict(sManaging_CycleHotkey, modifier, sManaging_CycleBeast);

			if UIHS_IsNameConflict(sManaging_CycleName) && sManaging_CycleName != sManaging_CycleOName
				ShowMessage("$Name Conflict", false, "OK", "")
			elseif conflictName != "_NOTFOUND_" && conflictName != sManaging_CycleOName
				ShowMessage("$Hotkey Conflict", false, "OK", "")
			elseif UIHS_EditEquipset(sManaging_CycleOName, ZipData(true))
				ShowMessage("$Successfully Equipset Edited", false, "OK", "")
				Init_MCMOpt()
				Init_StoredOpt()
				ClearProperties()
				ForcePageReset()
			else
				ShowMessage("$failed to Edit Equipset", false, "OK", "")
			endif
		endif
	endif
EndFunction

Function OnOptionInputAccept(int _opt, string _input)
	if CurPage == ePage_Managing
		if CurOpt == eOpt_New
			OnOptionInputAccept_New(_opt, _input)
		elseif CurOpt == eOpt_NewCycle
			OnOptionInputAccept_NewCycle(_opt, _input)
		elseif CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionInputAccept_NewCycle(_opt, _input)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionInputAccept_New(_opt, _input)
			endif
		endif
	elseif CurPage == ePage_Overview
		if CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionInputAccept_NewCycle(_opt, _input)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionInputAccept_New(_opt, _input)
			endif
		endif
	endif
EndFunction

Function OnOptionInputAccept_New(int _opt, string _input)
	if _opt == optManaging_Name
		sManaging_Name = _input;
		SetInputOptionValue(_opt, CString(_input, eColor_Green), false)
	endif
EndFunction

Function OnOptionInputAccept_NewCycle(int _opt, string _input)
	if _opt == optManaging_CycleName
		sManaging_CycleName = _input;
		SetInputOptionValue(_opt, CString(_input, eColor_Green), false)
	endif
EndFunction

Function OnOptionKeyMapChange(int _opt, int _code, string _conflictCode, string _conflictMod)
	if CurPage == ePage_Managing
		if CurOpt == eOpt_New
			OnOptionKeyMapChange_New(_opt, _code, _conflictCode, _conflictMod)
		elseif CurOpt == eOpt_NewCycle
			OnOptionKeyMapChange_NewCycle(_opt, _code, _conflictCode, _conflictMod)
		elseif CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionKeyMapChange_NewCycle(_opt, _code, _conflictCode, _conflictMod)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionKeyMapChange_New(_opt, _code, _conflictCode, _conflictMod)
			endif
		endif
	elseif CurPage == ePage_Overview
		if CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionKeyMapChange_NewCycle(_opt, _code, _conflictCode, _conflictMod)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionKeyMapChange_New(_opt, _code, _conflictCode, _conflictMod)
			endif
		endif
	endif
EndFunction

Function OnOptionKeyMapChange_New(int _opt, int _code, string _conflictCode, string _conflictMod)
	if _opt == optManaging_Hotkey
		sManaging_Hotkey = _code
		SetKeyMapOptionValue(optManaging_Hotkey, _code, false)
	endif
EndFunction

Function OnOptionKeyMapChange_NewCycle(int _opt, int _code, string _conflictCode, string _conflictMod)
	if _opt == optManaging_CycleHotkey
		sManaging_CycleHotkey = _code
		SetKeyMapOptionValue(optManaging_CycleHotkey, _code, false)
	endif
EndFunction

Function OnOptionMenuAccept(int _opt, int _index)
	if CurPage == ePage_Managing
		if CurOpt == eOpt_None
			if _opt == optManaging_Edit
				if _index != 0
					CurOpt = eOpt_Edit
					string name = UIHS_GetStringFromList(_index, eType_SelectList)
					Load_StoredOpt(name)
					SetMenuOptionValue(_opt, CString("$Loading...", eColor_Yellow), false)
					ForcePageReset()
				endif
			elseif _opt == optManaging_Remove
				if _index != 0
					string name = UIHS_GetStringFromList(_index, eType_SelectList)
					UIHS_RemoveEquipset(name)
					UIHS_Clear()
					UIHS_SendSettingData(ZipSettingData())
					UIHS_Init()
					Init_MCMOpt()
					Init_StoredOpt()
					ClearProperties()
				endif
			endif
		elseif CurOpt == eOpt_New
			OnOptionMenuAccept_New(_opt, _index)
		elseif CurOpt == eOpt_NewCycle
			OnOptionMenuAccept_NewCycle(_opt, _index)
		elseif CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionMenuAccept_NewCycle(_opt, _index)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionMenuAccept_New(_opt, _index)
			endif
		endif
	elseif CurPage == ePage_Overview
		if CurOpt == eOpt_Edit
			if _opt == optOverview_Edit || _opt == optOverview_Remove
				if UIHS_IsCycleEquipset(sManaging_CycleOName)
					OnOptionMenuAccept_NewCycle(_opt, _index)
				elseif !UIHS_IsCycleEquipset(sManaging_OName)
					OnOptionMenuAccept_New(_opt, _index)
				endif
			endif
		endif
	elseif CurPAge == ePage_Widget
		if _opt == optWidget_Font
			sWidget_Font = _index
			SetMenuOptionValue(_opt, UIHS_GetStringFromList(_index, eType_Font), false)
		elseif _opt == optWidget_EFont
			sWidget_EFont = _index
			SetMenuOptionValue(_opt, UIHS_GetStringFromList(_index, eType_Font), false)
		elseif _opt == optWidget_Display
			sWidget_Display = _index
			SetMenuOptionValue(_opt, sWidget_DisplayString[_index], false)
		elseif _opt == optWidget_EDisplay
			sWidget_EDisplay = _index
			SetMenuOptionValue(_opt, sWidget_DisplayString[_index], false)
		endif
	elseif CurPage == ePage_Setting
		if _opt == optSetting_Sort
			sSetting_Sort = _index
			SetMenuOptionValue(_opt, sSetting_SortString[_index], false)
		endif
	endif
EndFunction

Function OnOptionMenuAccept_New(int _opt, int _index)
	if _opt == optManaging_Widget
		sManaging_Widget = _index
		SetMenuOptionValue(_opt, UIHS_GetStringFromList(_index, eType_Widget), false)
	elseif _opt == optManaging_Left
		sManaging_Left = _index
		SetMenuOptionValue(_opt, UIHS_GetStringFromList(_index, eType_Weapon), false)
	elseif _opt == optManaging_Right
		sManaging_Right = _index
		SetMenuOptionValue(_opt, UIHS_GetStringFromList(_index, eType_Weapon), false)
	elseif _opt == optManaging_Shout
		sManaging_Shout = _index
		SetMenuOptionValue(_opt, UIHS_GetStringFromList(_index, eType_Shout), false)
	endif

	int i = 0
	while i < MAX_ITEMS
		if _opt == optManaging_Items[i]
			if _index != 0
				sManaging_Items[i] = _index
				SetMenuOptionValue(_opt, UIHS_GetStringFromList(_index, eType_Items), false)
				if i == sManaging_numItems
					sManaging_numItems += 1
					if i + 1 < MAX_ITEMS
						SetOptionFlags(optManaging_Items[i+1], OPTION_FLAG_NONE, false)
					endif
				endif
			else
				if i < sManaging_numItems - 1
					int j = i
					while j < sManaging_numItems - 1
						sManaging_Items[j] = sManaging_Items[j+1]
						SetMenuOptionValue(optManaging_Items[j], UIHS_GetStringFromList(sManaging_Items[j], eType_Items), false)
						j += 1
					endwhile
					sManaging_Items[j] = 0
					SetMenuOptionValue(optManaging_Items[j], "$+", false)
					if j + 1 < MAX_ITEMS
						SetOptionFlags(optManaging_Items[j+1], OPTION_FLAG_HIDDEN, false)
					endif
					sManaging_numItems -= 1
				elseif i == sManaging_numItems - 1
					sManaging_Items[i] = _index
					SetMenuOptionValue(_opt, "$+", false)
					if i + 1 < MAX_ITEMS
						SetOptionFlags(optManaging_Items[i+1], OPTION_FLAG_HIDDEN, false)
					endif
					sManaging_numItems -= 1
				endif
			endif
		endif
		i += 1
	endwhile
EndFunction

Function OnOptionMenuAccept_NewCycle(int _opt, int _index)
	int i = 0
	while i < MAX_CYCLEITEMS
		if _opt == optManaging_CycleItems[i]
			if _index != 0
				sManaging_CycleItems[i] = _index
				SetMenuOptionValue(_opt, UIHS_GetStringFromList(_index, eType_CycleItems), false)
				if i == sManaging_CyclenumItems
					sManaging_CyclenumItems += 1
					if i + 1 < MAX_CYCLEITEMS
						SetOptionFlags(optManaging_CycleItems[i+1], OPTION_FLAG_NONE, false)
					endif
				endif
			else
				if i < sManaging_CyclenumItems - 1
					int j = i
					while j < sManaging_CyclenumItems - 1
						sManaging_CycleItems[j] = sManaging_CycleItems[j+1]
						SetMenuOptionValue(optManaging_CycleItems[j], UIHS_GetStringFromList(sManaging_CycleItems[j], eType_CycleItems), false)
						j += 1
					endwhile
					sManaging_CycleItems[j] = 0
					SetMenuOptionValue(optManaging_CycleItems[j], "$+", false)
					if j + 1 < MAX_CYCLEITEMS
						SetOptionFlags(optManaging_CycleItems[j+1], OPTION_FLAG_HIDDEN, false)
					endif
					sManaging_CyclenumItems -= 1
				elseif i == sManaging_CyclenumItems - 1
					sManaging_CycleItems[i] = _index
					SetMenuOptionValue(_opt, "$+", false)
					if i + 1 < MAX_CYCLEITEMS
						SetOptionFlags(optManaging_CycleItems[i+1], OPTION_FLAG_HIDDEN, false)
					endif
					sManaging_CyclenumItems -= 1
				endif
			endif
		endif
		i += 1
	endwhile
EndFunction

Function OnOptionSliderAccept(int _opt, float _value)
	if CurPage == ePage_Managing
		if CurOpt == eOpt_New
			OnOptionSliderAccept_New(_opt, _value)
		elseif CurOpt == eOpt_NewCycle
			OnOptionSliderAccept_NewCycle(_opt, _value)
		elseif CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionSliderAccept_NewCycle(_opt, _value)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionSliderAccept_New(_opt, _value)
			endif
		endif
	elseif CurPage == ePage_Overview
		if CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionSliderAccept_NewCycle(_opt, _value)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionSliderAccept_New(_opt, _value)
			endif
		endif
	elseif CurPage == ePage_Widget
		if _opt == optWidget_Size
			sWidget_Size = _value as int
			SetSliderOptionValue(_opt, _value, "{0}%", false)
		elseif _opt == optWidget_ESize
			sWidget_ESize = _value as int
			SetSliderOptionValue(_opt, _value, "{0}%", false)
		elseif _opt == optWidget_Alpha
			sWidget_Alpha = _value as int
			SetSliderOptionValue(_opt, _value, "{0}%", false)
		elseif _opt == optWidget_EAlpha
			sWidget_EAlpha = _value as int
			SetSliderOptionValue(_opt, _value, "{0}%", false)
		elseif _opt == optWidget_Delay
			sWidget_Delay = _value
			SetSliderOptionValue(_opt, _value, "{1}sec", false)
		elseif _opt == optWidget_EDelay
			sWidget_EDelay = _value
			SetSliderOptionValue(_opt, _value, "{1}sec", false)
		elseif _opt == optWidget_LHPos
			sWidget_LHpos = _value as int
			SetSliderOptionValue(_opt, _value, "{0}", false)
		elseif _opt == optWidget_LVPos
			sWidget_LVpos = _value as int
			SetSliderOptionValue(_opt, _value, "{0}", false)
		elseif _opt == optWidget_RHPos
			sWidget_RHpos = _value as int
			SetSliderOptionValue(_opt, _value, "{0}", false)
		elseif _opt == optWidget_RVPos
			sWidget_RVpos = _value as int
			SetSliderOptionValue(_opt, _value, "{0}", false)
		elseif _opt == optWidget_SHPos
			sWidget_SHpos = _value as int
			SetSliderOptionValue(_opt, _value, "{0}", false)
		elseif _opt == optWidget_SVPos
			sWidget_SVpos = _value as int
			SetSliderOptionValue(_opt, _value, "{0}", false)
		endif
	endif
EndFunction

Function OnOptionSliderAccept_New(int _opt, float _value)
	if _opt == optManaging_Hpos
		sManaging_Hpos = _value as int
		SetSliderOptionValue(_opt, _value, "{0}", false)
	elseif _opt == optManaging_Vpos
		sManaging_Vpos = _value as int
		SetSliderOptionValue(_opt, _value, "{0}", false)
	endif
EndFunction

Function OnOptionSliderAccept_NewCycle(int _opt, float _value)
	if _opt == optManaging_CycleExpire
		sManaging_CycleExpire = _value
		SetSliderOptionValue(_opt, _value, "{1}sec", false)
	elseif _opt == optManaging_CycleReset
		sManaging_CycleReset = _value
		SetSliderOptionValue(_opt, _value, "{2}sec", false)
	elseif _opt == optManaging_CycleHpos
		sManaging_CycleHpos = _value as int
		SetSliderOptionValue(_opt, _value, "{0}", false)
	elseif _opt == optManaging_CycleVpos
		sManaging_CycleVpos = _value as int
		SetSliderOptionValue(_opt, _value, "{0}", false)
	endif
EndFunction

; End of MCM option select functions.

Function ClearProperties()
	CurOpt = eOpt_None
EndFunction

string Function CString(String _text, String _code)
	return "<font color='" + _code + "''>" + _text + "</font>"
EndFunction

string[] Function ZipData(bool _isCycle)
	string[] result
	
	if !_isCycle
		result = new string[64] ; MAX_ITEMS size can be problem when it reach the limit.
		result[0] = sManaging_Name
		result[1] = sManaging_Hotkey as string
		result[2] = sManaging_Modifier1 as string
		result[3] = sManaging_Modifier2 as string
		result[4] = sManaging_Modifier3 as string
		result[5] = sManaging_Sound as string
		result[6] = sManaging_Toggle as string
		result[7] = sManaging_Reequip as string
		result[8] = sManaging_Beast as string
		result[9] = sManaging_Widget as string
		result[10] = sManaging_Hpos as string
		result[11] = sManaging_Vpos as string
		result[12] = sManaging_DWidget as string
		result[13] = sManaging_DName as string
		result[14] = sManaging_DHotkey as string
		result[15] = sManaging_Left as string
		result[16] = sManaging_Right as string
		result[17] = sManaging_Shout as string
		result[18] = sManaging_numItems as string

		int i = 0
		while i < sManaging_numItems
			result[19 + i] = sManaging_Items[i] as string
			i += 1
		endwhile
	else
		result = new string[64] ; MAX_CYCLEITEMS size can be problem when it reach the limit.
		result[0] = sManaging_CycleName
		result[1] = sManaging_CycleHotkey as string
		result[2] = sManaging_CycleModifier1 as string
		result[3] = sManaging_CycleModifier2 as string
		result[4] = sManaging_CycleModifier3 as string
		result[5] = sManaging_CyclePersist as string
		result[6] = sManaging_CycleExpire as string
		result[7] = sManaging_CycleReset as string
		result[8] = sManaging_CycleBeast as string
		result[9] = sManaging_CycleHpos as string
		result[10] = sManaging_CycleVpos as string
		result[11] = sManaging_CycleDWidget as string
		result[12] = sManaging_CycleDName as string
		result[13] = sManaging_CycleDHotkey as string
		result[14] = sManaging_CyclenumItems as string

		int i = 0
		while i < sManaging_CyclenumItems
			result[15 + i] = sManaging_CycleItems[i] as string
			i += 1
		endwhile
	endif

	return result
EndFunction

int[] Function ZipSettingData()
	int[] result = new int[2]

	result[0] = sSetting_Sort
	result[1] = sSetting_Favor as int

	return result
EndFunction

string[] Function ZipSaveData()
	string[] result = new string[28]
	result[0] = sWidget_Font as string
	result[1] = sWidget_Size as string
	result[2] = sWidget_Alpha as string
	result[3] = sWidget_Display as string
	result[4] = sWidget_Delay as string
	result[5] = sWidget_EFont as string
	result[6] = sWidget_ESize as string
	result[7] = sWidget_EAlpha as string
	result[8] = sWidget_EDisplay as string
	result[9] = sWidget_EDelay as string
	result[10] = sWidget_LDisplay as string
	result[11] = sWidget_LName as string
	result[12] = sWidget_LHpos as string
	result[13] = sWidget_LVpos as string
	result[14] = sWidget_RDisplay as string
	result[15] = sWidget_RName as string
	result[16] = sWidget_RHpos as string
	result[17] = sWidget_RVpos as string
	result[18] = sWidget_SDisplay as string
	result[19] = sWidget_SName as string
	result[20] = sWidget_SHpos as string
	result[21] = sWidget_SVpos as string
	result[22] = sSetting_Modifier1 as string
	result[23] = sSetting_Modifier2 as string
	result[24] = sSetting_Modifier3 as string
	result[25] = sSetting_Sort as string
	result[26] = sSetting_Gamepad as string
	result[27] = sSetting_Favor as string

	return result
EndFunction

bool Function to_bool(string _data)
	return _data == "True" || _data == "TRUE" || _data == "1"
EndFunction