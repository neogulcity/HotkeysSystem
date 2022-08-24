Scriptname _HotkeysSystem_MCM extends SKI_ConfigBase  

; Global Native Functions
Function UIHS_Exec(int _code, bool _modifier1, bool _modifier2, bool _modifier3) Global Native
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
Function UIHS_SendSettingData(string[] _data) Global Native
Function UIHS_SendWidgetData(string[] _data) Global Native
Function UIHS_SaveSetting(string[] _data) Global Native
string[] Function UIHS_LoadSetting() Global Native
Function UIHS_CalculateKeydown(int _code, bool _modifier1, bool _modifier2, bool modifier3, float _time) Global Native
Function UIHS_InitWidget() Global Native
Function UIHS_ClearWidget() Global Native
Function UIHS_InitWidgetNext() Global Native
Function UIHS_SaveEquipsetData() Global Native
Function UIHS_LoadEquipsetData() Global Native
Function UIHS_RemoveAllEquipset() Global Native


; Number of items Equipset can have & Cycle Equipset can have / Number of displaying Overview page Equipset
int MAX_ITEMS = 15
int MAX_CYCLEITEMS = 5
int MAX_OVERVIEWLIST = 60

; Properties
bool bGenWidget
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
int[]	optManaging_CycleItems

int[]	optOverview_Equipset
int 	optOverview_Edit
int 	optOverview_Remove

int 	optWidget_Font
int 	optWidget_FontSize
int 	optWidget_Size
int 	optWidget_Alpha
int 	optWidget_Display
int 	optWidget_Delay

int		optSetting_Modifier1
int		optSetting_Modifier2
int		optSetting_Modifier3
int		optSetting_Sort
int		optSetting_Gamepad
int		optSetting_Favor

int		optMaintenance_Save
int		optMaintenance_Load
int		optMaintenance_RSetting
int		optMaintenance_REquipsets
int		optMaintenance_Mod
int		optMaintenance_Widget

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
int 	sWidget_FontSize = 100
int 	sWidget_Size = 100
int 	sWidget_Alpha = 100
int 	sWidget_Display = 0
string[] sWidget_DisplayString
float 	sWidget_Delay = 5.0

int 	sSetting_Modifier1 = 42
int 	sSetting_Modifier2 = 29
int 	sSetting_Modifier3 = 56
int		sSetting_Sort = 0
string[] sSetting_SortString
bool 	sSetting_Gamepad = false
bool	sSetting_Favor = false

bool	sMaintenance_Mod = true
bool 	sMaintenance_Widget = true


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
string eColor_Red = "#FF6961"
string eColor_Yellow = "#F4EEB1"

Event OnConfigInit()
	ModName = "Hotkeys System"
	Pages = new string[5]
	Pages[0] = "$UIHS_Page_Managing"
	Pages[1] = "$UIHS_Page_Overview"
	Pages[2] = "$UIHS_Page_Widget"
	Pages[3] = "$UIHS_Page_Setting"
	Pages[4] = "$UIHS_Page_Maintenance"

	Load()
	UIHS_SendSettingData(ZipSettingdata())
	UIHS_Init()
	UIHS_SendWidgetData(ZipWidgetData())
EndEvent

Event OnConfigOpen()
	if sMaintenance_Widget && bGenWidget 
		ShowMessage("$UIHS_MSG_PREFIX9", false, "$OK", "")
		return
	endif

	ClearProperties()
	UIHS_Clear()
	UIHS_SendSettingData(ZipSettingData())
	UIHS_Init()
	Init_MCMOpt()
	Init_StoredOpt()
EndEvent

Event OnConfigClose()
	if sMaintenance_Widget
		bGenWidget = true
	endif
	UIHS_SendWidgetData(ZipWidgetData())

	; ClearProperties()
	; UIHS_Clear()
	UnregisterForAllKeys()
	if sMaintenance_Mod
		RegisterForAllKeys()
	endif
	Save()

	UIHS_ClearWidget()
	if sMaintenance_Widget 
		Utility.Wait(0.1)
		UIHS_InitWidget()
		Utility.Wait(0.1)
		UIHS_InitWidgetNext()
		bGenWidget = false
	endif
EndEvent

Event OnGameReload()
	parent.OnGameReload()
	Load()
	UIHS_SendSettingData(ZipSettingdata())
	UIHS_Init()
	UIHS_SendWidgetData(ZipWidgetData())

	UIHS_ClearWidget()
	if sMaintenance_Widget 
		Utility.Wait(0.1)
		UIHS_InitWidget()
		Utility.Wait(0.1)
		UIHS_InitWidgetNext()
	endif
EndEvent

Event OnPageReset(string _page)	
	if _page == "$UIHS_Page_Managing"
		if CurPage != ePage_Managing
			UIHS_Clear()
			UIHS_SendSettingData(ZipSettingData())
			UIHS_Init()
			Init_MCMOpt()
			Init_StoredOpt()
			ClearProperties()
		endif
		OnPage_Managing()
	elseif _page == "$UIHS_Page_Overview"
		if CurPage != ePage_Overview
			UIHS_Clear()
			UIHS_SendSettingData(ZipSettingData())
			UIHS_Init()
			Init_MCMOpt()
			Init_StoredOpt()
			ClearProperties()
		endif
		OnPage_Overview()
	elseif _page == "$UIHS_Page_Widget"
		OnPage_Widget()
	elseif _page == "$UIHS_Page_Setting"
		OnPage_Setting()
	elseif _page == "$UIHS_Page_Maintenance"
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
	sWidget_FontSize	= data[1] as int
	sWidget_Size		= data[2] as int
	sWidget_Alpha		= data[3] as int
	sWidget_Display		= data[4] as int
	sWidget_Delay		= data[5] as float
	sSetting_Modifier1	= data[6] as int
	sSetting_Modifier2	= data[7] as int
	sSetting_Modifier3	= data[8] as int
	sSetting_Sort		= data[9] as int
	sSetting_Gamepad	= to_bool(data[10])
	sSetting_Favor		= to_bool(data[11])
	sMaintenance_Mod	= to_bool(data[12])
	sMaintenance_Widget	= to_bool(data[13])
EndFunction

Function Save()
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

	sWidget_DisplayString = new String[2]
	sWidget_DisplayString[0] = "$UIHS_Always"
	sWidget_DisplayString[1] = "$UIHS_InCombat"

	sSetting_SortString = new String[4]
	sSetting_SortString[0] = "$UIHS_Order1"
	sSetting_SortString[1] = "$UIHS_Order2"
	sSetting_SortString[2] = "$UIHS_Order3"
	sSetting_SortString[3] = "$UIHS_Order4"
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
		optManaging_New		  = AddTextOption("", "$UIHS_Managing_New", OPTION_FLAG_NONE)
		optManaging_NewCycle  = AddTextOption("", "$UIHS_Managing_NewCycle", OPTION_FLAG_NONE)
		optManaging_Edit	  = AddMenuOption("", "$UIHS_Managing_Edit", OPTION_FLAG_NONE)
		optManaging_Remove	  = AddMenuOption("", "$UIHS_Managing_Remove", OPTION_FLAG_NONE)
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
	optManaging_Name	  	= AddInputOption("$UIHS_Managing_Name", CString(sManaging_Name, eColor_Green), OPTION_FLAG_NONE)
	optManaging_Hotkey	  	= AddKeyMapOption("$UIHS_Managing_Hotkey", sManaging_Hotkey, OPTION_FLAG_WITH_UNMAP)
	
	optManaging_Modifier1 	= AddToggleOption("+" + UIHS_GetStringFromKeycode(sSetting_Modifier1), sManaging_Modifier1, OPTION_FLAG_NONE)
	optManaging_Modifier2 	= AddToggleOption("+" + UIHS_GetStringFromKeycode(sSetting_Modifier2), sManaging_Modifier2, OPTION_FLAG_NONE)
	optManaging_Modifier3 	= AddToggleOption("+" + UIHS_GetStringFromKeycode(sSetting_Modifier3), sManaging_Modifier3, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	optManaging_Sound	  	= AddToggleOption("$UIHS_Managing_Sound", sManaging_Sound, OPTION_FLAG_NONE)
	optManaging_Toggle	  	= AddToggleOption("$UIHS_Managing_Toggle", sManaging_Toggle, OPTION_FLAG_NONE)
	optManaging_Reequip	  	= AddToggleOption("$UIHS_Managing_Reequip", sManaging_Reequip, OPTION_FLAG_NONE)
	optManaging_Beast	  	= AddToggleOption("$UIHS_Managing_Beast", sManaging_Beast, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	optManaging_Widget	  	= AddMenuOption("$UIHS_Managing_Widget", UIHS_GetStringFromList(sManaging_Widget, eType_Widget), OPTION_FLAG_NONE)
	optManaging_Hpos	  	= AddSliderOption("$UIHS_Managing_Hpos", sManaging_Hpos as float, "{0}", OPTION_FLAG_NONE)
	optManaging_Vpos	  	= AddSliderOption("$UIHS_Managing_Vpos", sManaging_Vpos as float, "{0}", OPTION_FLAG_NONE)
	optManaging_DWidget	  	= AddToggleOption("$UIHS_Managing_DWidget", sManaging_DWidget, OPTION_FLAG_NONE)
	optManaging_DName		= AddToggleOption("$UIHS_Managing_DName", sManaging_DName, OPTION_FLAG_NONE)
	optManaging_DHotkey	  	= AddToggleOption("$UIHS_Managing_DHotkey", sManaging_DHotkey, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	if CurOpt == eOpt_New
		optManaging_Confirm	= AddTextOption("", "$UIHS_Managing_Confirm1", OPTION_FLAG_NONE)
	elseif CurOpt == eOpt_Edit
		optManaging_Confirm	= AddTextOption("", "$UIHS_Managing_Confirm2", OPTION_FLAG_NONE)
	endif

	SetCursorPosition(1)

	AddHeaderOption("$UIHS_Managing_Weapons", OPTION_FLAG_NONE)
	optManaging_Left	  	= AddMenuOption("$UIHS_Managing_Lefthand", UIHS_GetStringFromList(sManaging_Left, eType_Weapon), OPTION_FLAG_NONE)
	optManaging_Right	  	= AddMenuOption("$UIHS_Managing_Righthand", UIHS_GetStringFromList(sManaging_Right, eType_Weapon), OPTION_FLAG_NONE)
	optManaging_Shout	  	= AddMenuOption("$UIHS_Managing_Shout", UIHS_GetStringFromList(sManaging_Shout, eType_Shout), OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$UIHS_Managing_Items", OPTION_FLAG_NONE)

	int i = 0
	while i < MAX_ITEMS
		if i < sManaging_numItems
			optManaging_Items[i] = AddMenuOption("", UIHS_GetStringFromList(sManaging_Items[i], eType_Items), OPTION_FLAG_NONE)
		elseif i == sManaging_numItems
			optManaging_Items[i] = AddMenuOption("", "$UIHS_Plus", OPTION_FLAG_NONE)
		else
			optManaging_Items[i] = AddMenuOption("", "$UIHS_Plus", OPTION_FLAG_HIDDEN)
		endif
		i += 1
	endwhile
EndFunction

Function OnPage_Managing_NewCycle()
	optManaging_CycleName			= AddInputOption("$UIHS_Managing_Name", CString(sManaging_CycleName, eColor_Green), OPTION_FLAG_NONE)
	optManaging_CycleHotkey			= AddKeyMapOption("$UIHS_Managing_Hotkey", sManaging_CycleHotkey, OPTION_FLAG_WITH_UNMAP)
	optManaging_CycleModifier1  	= AddToggleOption(UIHS_GetStringFromKeycode(sSetting_Modifier1), sManaging_CycleModifier1, OPTION_FLAG_NONE)
	optManaging_CycleModifier2  	= AddToggleOption(UIHS_GetStringFromKeycode(sSetting_Modifier2), sManaging_CycleModifier2, OPTION_FLAG_NONE)
	optManaging_CycleModifier3  	= AddToggleOption(UIHS_GetStringFromKeycode(sSetting_Modifier3), sManaging_CycleModifier3, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	optManaging_CyclePersist		= AddToggleOption("$UIHS_Managing_CyclePersist", sManaging_CyclePersist, OPTION_FLAG_NONE)
	optManaging_CycleExpire			= AddSliderOption("$UIHS_Managing_CycleExpire", sManaging_CycleExpire, "$UIHS_EXPREESION_1", OPTION_FLAG_NONE)
	optManaging_CycleReset			= AddSliderOption("$UIHS_Managing_CycleReset", sManaging_CycleReset, "$UIHS_EXPREESION_2", OPTION_FLAG_NONE)
	optManaging_CycleBeast			= AddToggleOption("$UIHS_Managing_Beast", sManaging_CycleBeast, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	optManaging_CycleHpos			= AddSliderOption("$UIHS_Managing_Hpos", sManaging_CycleHpos as float, "{0}", OPTION_FLAG_NONE)
	optManaging_CycleVpos			= AddSliderOption("$UIHS_Managing_Vpos", sManaging_CycleVpos as float, "{0}", OPTION_FLAG_NONE)
	optManaging_CycleDWidget		= AddToggleOption("$UIHS_Managing_DWidget", sManaging_CycleDWidget, OPTION_FLAG_NONE)
	optManaging_CycleDName			= AddToggleOption("$UIHS_Managing_DName", sManaging_CycleDName, OPTION_FLAG_NONE)
	optManaging_CycleDHotkey		= AddToggleOption("$UIHS_Managing_DHotkey", sManaging_CycleDHotkey, OPTION_FLAG_NONE)
	AddHeaderOption("", OPTION_FLAG_NONE)
	if CurOpt == eOpt_NewCycle
		optManaging_CycleConfirm	= AddTextOption("", "$UIHS_Managing_Confirm1", OPTION_FLAG_NONE)
	elseif CurOpt == eOpt_Edit
		optManaging_CycleConfirm	= AddTextOption("", "$UIHS_Managing_Confirm2", OPTION_FLAG_NONE)
	endif

	SetCursorPosition(1)

	AddHeaderOption("$UIHS_Managing_Equipsets", OPTION_FLAG_NONE)

	int i = 0
	while i < MAX_CYCLEITEMS
		if i < sManaging_CyclenumItems
			optManaging_CycleItems[i] = AddMenuOption("", UIHS_GetStringFromList(sManaging_CycleItems[i], eType_CycleItems), OPTION_FLAG_NONE)
		elseif i == sManaging_CyclenumItems
			optManaging_CycleItems[i] = AddMenuOption("", "$UIHS_Plus", OPTION_FLAG_NONE)
		else
			optManaging_CycleItems[i] = AddMenuOption("", "$UIHS_Plus", OPTION_FLAG_HIDDEN)
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
			AddTextOption("$UIHS_Overview_NotFound", "", OPTION_FLAG_NONE)
		endif

		int i = 0
		while i < equipset.Length && i < MAX_OVERVIEWLIST
			if i != sOverview_Equipset
				optOverview_Equipset[i] = AddTextOption(UIHS_GetStringFromList(i, eType_Equipset), "$UIHS_Overview_Open", OPTION_FLAG_NONE)
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
	AddKeyMapOption("$UIHS_Managing_Hotkey", sManaging_Hotkey, OPTION_FLAG_DISABLED)
	AddToggleOption(UIHS_GetStringFromKeycode(sSetting_Modifier1), sManaging_Modifier1, OPTION_FLAG_DISABLED)
	AddToggleOption(UIHS_GetStringFromKeycode(sSetting_Modifier2), sManaging_Modifier2, OPTION_FLAG_DISABLED)
	AddToggleOption(UIHS_GetStringFromKeycode(sSetting_Modifier3), sManaging_Modifier3, OPTION_FLAG_DISABLED)
	AddEmptyOption()
	AddHeaderOption("$UIHS_Managing_Weapons", OPTION_FLAG_NONE)
	AddTextOption("$UIHS_Managing_Lefthand", UIHS_GetStringFromList(sManaging_Left, eType_Weapon), OPTION_FLAG_NONE)
	AddTextOption("$UIHS_Managing_Righthand", UIHS_GetStringFromList(sManaging_Right, eType_Weapon), OPTION_FLAG_NONE)
	AddTextOption("$UIHS_Managing_Shout", UIHS_GetStringFromList(sManaging_Shout, eType_Shout), OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$UIHS_Managing_Items", OPTION_FLAG_NONE)
	
	if sManaging_numItems == 0
		AddTextOption("", "$UIHS_Nothing", OPTION_FLAG_NONE)
	else
		int j = 0
		while j < sManaging_numItems
			AddTextOption("", UIHS_GetStringFromList(sManaging_Items[j], eType_Items), OPTION_FLAG_NONE)
			j += 1
		endwhile
	endif

	AddHeaderOption("", OPTION_FLAG_NONE)
	optOverview_Edit = AddTextOption("", "$UIHS_Overview_Edit", OPTION_FLAG_NONE)
	optOverview_Remove = AddTextOption("", "$UIHS_Overview_Remove", OPTION_FLAG_NONE)
EndFunction

Function OnPage_Overview_CycleItems()
	AddHeaderOption(CString(UIHS_GetStringFromList(sOverview_Equipset, eType_Equipset), eColor_Green), OPTION_FLAG_NONE)
	AddKeyMapOption("$UIHS_Managing_Hotkey", sManaging_CycleHotkey, OPTION_FLAG_DISABLED)
	AddToggleOption(UIHS_GetStringFromKeycode(sSetting_Modifier1), sManaging_CycleModifier1, OPTION_FLAG_DISABLED)
	AddToggleOption(UIHS_GetStringFromKeycode(sSetting_Modifier2), sManaging_CycleModifier2, OPTION_FLAG_DISABLED)
	AddToggleOption(UIHS_GetStringFromKeycode(sSetting_Modifier3), sManaging_CycleModifier3, OPTION_FLAG_DISABLED)
	AddEmptyOption()
	AddHeaderOption("$UIHS_Managing_Equipsets", OPTION_FLAG_NONE)
	if sManaging_CyclenumItems == 0
		AddTextOption("", "$UIHS_Nothing", OPTION_FLAG_NONE)
	else
		int j = 0
		while j < sManaging_CyclenumItems
			AddTextOption("", UIHS_GetStringFromList(sManaging_CycleItems[j], eType_CycleItems), OPTION_FLAG_NONE)
			j += 1
		endwhile
	endif

	AddHeaderOption("", OPTION_FLAG_NONE)
	optOverview_Edit = AddTextOption("", "$UIHS_Overview_Edit", OPTION_FLAG_NONE)
	optOverview_Remove = AddTextOption("", "$UIHS_Overview_Remove", OPTION_FLAG_NONE)
EndFunction

Function OnPage_Widget()
	CurPage = ePage_Widget;
	
	SetCursorFillMode(TOP_TO_BOTTOM)
	AddHeaderOption("$UIHS_Widget_Setting", OPTION_FLAG_NONE)
	optWidget_Font = AddMenuOption("$UIHS_Widget_Font", UIHS_GetStringFromList(sWidget_Font, eType_Font), OPTION_FLAG_NONE)
	optWidget_FontSize = AddSliderOption("$UIHS_Widget_FontSize", sWidget_FontSize as float, "{0}%", OPTION_FLAG_NONE)
	optWidget_Size = AddSliderOption("$UIHS_Widget_Size", sWidget_Size as float, "{0}%", OPTION_FLAG_NONE)
	optWidget_Alpha = AddSliderOption("$UIHS_Widget_Alpha", sWidget_Alpha as float, "{0}%", OPTION_FLAG_NONE)
	optWidget_Display = AddMenuOption("$UIHS_Widget_Display", sWidget_DisplayString[sWidget_Display], OPTION_FLAG_NONE)
	optWidget_Delay = AddSliderOption("$UIHS_Widget_Delay", sWidget_Delay, "$UIHS_EXPREESION_1", OPTION_FLAG_NONE)

	; SetCursorPosition(1)

EndFunction

Function OnPage_Setting()
	CurPage = ePage_Setting;
	
	SetCursorFillMode(TOP_TO_BOTTOM)
	AddHeaderOption("$UIHS_Setting_Modifier", OPTION_FLAG_NONE)
	optSetting_Modifier1 = AddKeyMapOption("$UIHS_Setting_Modifier1", sSetting_Modifier1, OPTION_FLAG_WITH_UNMAP)
	optSetting_Modifier2 = AddKeyMapOption("$UIHS_Setting_Modifier2", sSetting_Modifier2, OPTION_FLAG_WITH_UNMAP)
	optSetting_Modifier3 = AddKeyMapOption("$UIHS_Setting_Modifier3", sSetting_Modifier3, OPTION_FLAG_WITH_UNMAP)
	AddEmptyOption()
	AddHeaderOption("$UIHS_Setting_Equipset", OPTION_FLAG_NONE)
	optSetting_Sort = AddMenuOption("$UIHS_Setting_Sort", sSetting_SortString[sSetting_Sort], OPTION_FLAG_NONE)
	optSetting_Favor = AddToggleOption("$UIHS_Setting_Favor", sSetting_Favor, OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$UIHS_Setting_HGamepad", OPTION_FLAG_NONE)
	optSetting_Gamepad = AddToggleOption("$UIHS_Setting_Gamepad", sSetting_Gamepad, OPTION_FLAG_NONE)
EndFunction

Function OnPage_Maintenance()
	CurPage = ePage_Maintenance;
	
	SetCursorFillMode(TOP_TO_BOTTOM)
	AddHeaderOption("$UIHS_Maintenance_HSave", OPTION_FLAG_NONE)
	optMaintenance_Save = AddTextOption("$UIHS_Maintenance_Save", "$UIHS_Go", OPTION_FLAG_NONE)
	AddEmptyOption()
	AddHeaderOption("$UIHS_Maintenance_HLoad", OPTION_FLAG_NONE)
	optMaintenance_Load = AddTextOption("$UIHS_Maintenance_Load", "$UIHS_Go", OPTION_FLAG_NONE)
	
	SetCursorPosition(1)

	AddHeaderOption("$UIHS_Maintenance_Header", OPTION_FLAG_NONE)
	optMaintenance_RSetting = AddTextOption("$UIHS_Maintenance_RSetting", "$UIHS_Go", OPTION_FLAG_NONE)
	optMaintenance_REquipsets = AddTextOption("$UIHS_Maintenance_REquipsets", "$UIHS_Go", OPTION_FLAG_NONE)
	if sMaintenance_Mod
		optMaintenance_Mod = AddTextOption("$UIHS_Maintenance_Mod", "$UIHS_Maintenance_Act", OPTION_FLAG_NONE)
	else
		optMaintenance_Mod = AddTextOption("$UIHS_Maintenance_Mod", "$UIHS_Maintenance_Deact", OPTION_FLAG_NONE)
	endif
	if sMaintenance_Widget
		optMaintenance_Widget = AddTextOption("$UIHS_Maintenance_Widget", "$UIHS_Maintenance_Act", OPTION_FLAG_NONE)
	else
		optMaintenance_Widget = AddTextOption("$UIHS_Maintenance_Widget", "$UIHS_Maintenance_Deact", OPTION_FLAG_NONE)
	endif
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
		elseif _opt == optWidget_Display
			SetMenuDialogOptions(sWidget_DisplayString)
			SetMenuDialogStartIndex(sWidget_Display)
			SetMenuDialogDefaultIndex(sWidget_Display)
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
		if _opt == optWidget_FontSize
			SetSliderDialogStartValue(sWidget_FontSize as float)
			SetSliderDialogDefaultValue(sWidget_FontSize as float)
			SetSliderDialogRange(0.0, 200.0)
			SetSliderDialogInterval(5.0)
		elseif _opt == optWidget_Size
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
		endif
	endif
EndFunction

Function OnOptionSliderOpen_New(int _opt)
	if _opt == optManaging_Hpos
		SetSliderDialogStartValue(sManaging_Hpos as float)
		SetSliderDialogDefaultValue(sManaging_Hpos as float)
		SetSliderDialogRange(-320.0, 1600.0)
		SetSliderDialogInterval(1.0)
	elseif _opt == optManaging_Vpos
		SetSliderDialogStartValue(sManaging_Vpos as float)
		SetSliderDialogDefaultValue(sManaging_Vpos as float)
		SetSliderDialogRange(0.0, 720.0)
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
		SetSliderDialogRange(-320.0, 1600.0)
		SetSliderDialogInterval(1.0)
	elseif _opt == optManaging_CycleVpos
		SetSliderDialogStartValue(sManaging_CycleVpos as float)
		SetSliderDialogDefaultValue(sManaging_CycleVpos as float)
		SetSliderDialogRange(0.0, 720.0)
		SetSliderDialogInterval(1.0)
	endif
EndFunction

Function OnOptionInputOpen(int _opt)
	if CurPage == ePage_Managing
		if CurOpt == eOpt_New
			OnOptionInputOpen_New(_opt)
		elseif CurOpt == eOpt_NewCycle
			OnOptionInputOpen_NewCycle(_opt)
		elseif CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionInputOpen_NewCycle(_opt)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionInputOpen_New(_opt)
			endif
		endif
	elseif CurPage == ePage_Overview
		if CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionInputOpen_NewCycle(_opt)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionInputOpen_New(_opt)
			endif
		endif
	endif
EndFunction

Function OnOptionInputOpen_New(int _opt)
	if _opt == optManaging_Name
		SetInputDialogStartText(sManaging_Name)
	endif
EndFunction

Function OnOptionInputOpen_NewCycle(int _opt)
	if _opt == optManaging_CycleName
		SetInputDialogStartText(sManaging_CycleName)
	endif
EndFunction

; End of MCM option open functions.


; Start of MCM option select or accept functions.

Function OnOptionSelect(int _opt)
	if CurPage == ePage_Managing
		if CurOpt == eOpt_None
			if _opt == optManaging_New
				CurOpt = eOpt_New
			elseif _opt == optManaging_NewCycle
				CurOpt = eOpt_NewCycle
			endif
			SetTextOptionValue(_opt, "$UIHS_Loading", false)
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
					SetTextOptionValue(_opt, "$UIHS_Opening", false)
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
				SetTextOptionValue(_opt, "$UIHS_Loading", false)
				ForcePageReset()
			elseif _opt == optOverview_Remove
				if UIHS_IsCycleEquipset(sManaging_CycleOName)
					UIHS_RemoveEquipset(sManaging_CycleOName)
				elseif !UIHS_IsCycleEquipset(sManaging_OName)
					UIHS_RemoveEquipset(sManaging_OName)
				endif
				SetTextOptionValue(_opt, "$UIHS_Removing", false)
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
	elseif CurPage == ePage_Setting
		if _opt == optSetting_Favor
			sSetting_Favor = !sSetting_Favor
			SetToggleOptionValue(_opt, sSetting_Favor, false)
		elseif _opt == optSetting_Gamepad
				sSetting_Gamepad = !sSetting_Gamepad
				SetToggleOptionValue(_opt, sSetting_Gamepad, false)
		endif
	elseif CurPage == ePage_Maintenance
		if _opt == optMaintenance_Save
			SetTextOptionValue(_opt, "$UIHS_Saving", false)
			UIHS_SaveEquipsetData()
			ForcePageReset()
			ShowMessage("$UIHS_MSG_PREFIX1", false, "$OK", "")
		elseif _opt == optMaintenance_Load
			bool result = ShowMessage("$UIHS_MSG_PREFIX10", true, "$OK", "$Cancel")
			if result
				SetTextOptionValue(_opt, "$UIHS_Loading", false)
				UIHS_LoadEquipsetData()
				ForcePageReset()
				ShowMessage("$UIHS_MSG_PREFIX2", false, "$OK", "")
			endif
		elseif _opt == optMaintenance_RSetting
			bool result = ShowMessage("$UIHS_MSG_PREFIX11", true, "$OK", "$Cancel")
			if result
				SetTextOptionValue(_opt, "$UIHS_Loading", false)
				ResetAllSetting()
				UIHS_SendSettingData(ZipSettingdata())
				UIHS_SendWidgetData(ZipWidgetData())
				ForcePageReset()
				ShowMessage("$UIHS_MSG_PREFIX3", false, "$OK", "")
			endif
		elseif _opt == optMaintenance_REquipsets
			bool result = ShowMessage("$UIHS_MSG_PREFIX12", true, "$OK", "$Cancel")
			if result
				SetTextOptionValue(_opt, "$UIHS_Loading", false)
				UIHS_RemoveAllEquipset()
				ForcePageReset()
				ShowMessage("$UIHS_MSG_PREFIX4", false, "$OK", "")
			endif
		elseif _opt == optMaintenance_Mod
			sMaintenance_Mod = !sMaintenance_Mod
			if sMaintenance_Mod
				SetTextOptionValue(_opt, "$UIHS_Maintenance_Act", false)
			else
				SetTextOptionValue(_opt, "$UIHS_Maintenance_Deact", false)
			endif
		elseif _opt == optMaintenance_Widget
			sMaintenance_Widget = !sMaintenance_Widget
			if sMaintenance_Widget
				SetTextOptionValue(_opt, "$UIHS_Maintenance_Act", false)
			else
				SetTextOptionValue(_opt, "$UIHS_Maintenance_Deact", false)
			endif
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
				ShowMessage("$UIHS_MSG_PREFIX5", false, "$OK", "")
			elseif conflictName != "_NOTFOUND_"
				ShowMessage("$UIHS_MSG_PREFIX6{" + conflictName + "}", false, "$OK", "")
			elseif UIHS_NewEquipset(ZipData(false))
				ShowMessage("$UIHS_MSG_PREFIX7", false, "$OK", "")
				UIHS_Clear()
				UIHS_SendSettingData(ZipSettingData())
				UIHS_Init()
				Init_MCMOpt()
				Init_StoredOpt()
				ClearProperties()
				ForcePageReset()
			else
				ShowMessage("$UIHS_MSG_PREFIX8", false, "$OK", "")
			endif
		else
			bool[] modifier = new bool[3]
			modifier[0] = sManaging_Modifier1
			modifier[1] = sManaging_Modifier2
			modifier[2] = sManaging_Modifier3
			string conflictName = UIHS_GetKeyConflict(sManaging_Hotkey, modifier, sManaging_Beast);

			if UIHS_IsNameConflict(sManaging_Name) && sManaging_Name != sManaging_OName
				ShowMessage("$UIHS_MSG_PREFIX5", false, "$OK", "")
			elseif conflictName != "_NOTFOUND_" && conflictName != sManaging_OName
				ShowMessage("$UIHS_MSG_PREFIX6{" + conflictName + "}", false, "$OK", "")
			elseif UIHS_EditEquipset(sManaging_OName, ZipData(false))
				ShowMessage("$UIHS_MSG_PREFIX7", false, "$OK", "")
				UIHS_Clear()
				UIHS_SendSettingData(ZipSettingData())
				UIHS_Init()
				Init_MCMOpt()
				Init_StoredOpt()
				ClearProperties()
				ForcePageReset()
			else
				ShowMessage("$UIHS_MSG_PREFIX8", false, "$OK", "")
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
				ShowMessage("$UIHS_MSG_PREFIX5", false, "$OK", "")
			elseif conflictName != "_NOTFOUND_"
				ShowMessage("$UIHS_MSG_PREFIX6{" + conflictName + "}", false, "$OK", "")
			elseif UIHS_NewCycleEquipset(ZipData(true))
				ShowMessage("$UIHS_MSG_PREFIX7", false, "$OK", "")
				Init_MCMOpt()
				Init_StoredOpt()
				ClearProperties()
				ForcePageReset()
			else
				ShowMessage("$UIHS_MSG_PREFIX8", false, "$OK", "")
			endif
		else
			bool[] modifier = new bool[3]
			modifier[0] = sManaging_CycleModifier1
			modifier[1] = sManaging_CycleModifier2
			modifier[2] = sManaging_CycleModifier3
			string conflictName = UIHS_GetKeyConflict(sManaging_CycleHotkey, modifier, sManaging_CycleBeast);

			if UIHS_IsNameConflict(sManaging_CycleName) && sManaging_CycleName != sManaging_CycleOName
				ShowMessage("$UIHS_MSG_PREFIX5", false, "$OK", "")
			elseif conflictName != "_NOTFOUND_" && conflictName != sManaging_CycleOName
				ShowMessage("$UIHS_MSG_PREFIX6{" + conflictName + "}", false, "$OK", "")
			elseif UIHS_EditEquipset(sManaging_CycleOName, ZipData(true))
				ShowMessage("$UIHS_MSG_PREFIX7", false, "$OK", "")
				Init_MCMOpt()
				Init_StoredOpt()
				ClearProperties()
				ForcePageReset()
			else
				ShowMessage("$UIHS_MSG_PREFIX8", false, "$OK", "")
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
	elseif CurPage == ePage_Setting
		if _opt == optSetting_Modifier1
			sSetting_Modifier1 = _code
			SetKeyMapOptionValue(_opt, _code, false)
		elseif _opt == optSetting_Modifier2
			sSetting_Modifier2 = _code
			SetKeyMapOptionValue(_opt, _code, false)
		elseif _opt == optSetting_Modifier3
			sSetting_Modifier3 = _code
			SetKeyMapOptionValue(_opt, _code, false)
		endif
	endif
EndFunction

Function OnOptionKeyMapChange_New(int _opt, int _code, string _conflictCode, string _conflictMod)
	if _opt == optManaging_Hotkey
		sManaging_Hotkey = _code
		SetKeyMapOptionValue(_opt, _code, false)
	endif
EndFunction

Function OnOptionKeyMapChange_NewCycle(int _opt, int _code, string _conflictCode, string _conflictMod)
	if _opt == optManaging_CycleHotkey
		sManaging_CycleHotkey = _code
		SetKeyMapOptionValue(_opt, _code, false)
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
					SetMenuOptionValue(_opt, "$UIHS_Loading", false)
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
					ShowMessage("$UIHS_MSG_PREFIX13{" + name + "}", false, "$OK", "")
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
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionMenuAccept_NewCycle(_opt, _index)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionMenuAccept_New(_opt, _index)
			endif
		endif
	elseif CurPAge == ePage_Widget
		if _opt == optWidget_Font
			sWidget_Font = _index
			SetMenuOptionValue(_opt, UIHS_GetStringFromList(_index, eType_Font), false)
		elseif _opt == optWidget_Display
			sWidget_Display = _index
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
					SetMenuOptionValue(optManaging_Items[j], "$UIHS_Plus", false)
					if j + 1 < MAX_ITEMS
						SetOptionFlags(optManaging_Items[j+1], OPTION_FLAG_HIDDEN, false)
					endif
					sManaging_numItems -= 1
				elseif i == sManaging_numItems - 1
					sManaging_Items[i] = _index
					SetMenuOptionValue(_opt, "$UIHS_Plus", false)
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
					SetMenuOptionValue(optManaging_CycleItems[j], "$UIHS_Plus", false)
					if j + 1 < MAX_CYCLEITEMS
						SetOptionFlags(optManaging_CycleItems[j+1], OPTION_FLAG_HIDDEN, false)
					endif
					sManaging_CyclenumItems -= 1
				elseif i == sManaging_CyclenumItems - 1
					sManaging_CycleItems[i] = _index
					SetMenuOptionValue(_opt, "$UIHS_Plus", false)
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
		if _opt == optWidget_FontSize
			sWidget_FontSize = _value as int
			SetSliderOptionValue(_opt, _value, "{0}%", false)
		elseif _opt == optWidget_Size
			sWidget_Size = _value as int
			SetSliderOptionValue(_opt, _value, "{0}%", false)
		elseif _opt == optWidget_Alpha
			sWidget_Alpha = _value as int
			SetSliderOptionValue(_opt, _value, "{0}%", false)
		elseif _opt == optWidget_Delay
			sWidget_Delay = _value
			SetSliderOptionValue(_opt, _value, "$UIHS_EXPREESION_1", false)
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
		SetSliderOptionValue(_opt, _value, "$UIHS_EXPREESION_1", false)
	elseif _opt == optManaging_CycleReset
		sManaging_CycleReset = _value
		SetSliderOptionValue(_opt, _value, "$UIHS_EXPREESION_2", false)
	elseif _opt == optManaging_CycleHpos
		sManaging_CycleHpos = _value as int
		SetSliderOptionValue(_opt, _value, "{0}", false)
	elseif _opt == optManaging_CycleVpos
		sManaging_CycleVpos = _value as int
		SetSliderOptionValue(_opt, _value, "{0}", false)
	endif
EndFunction

; End of MCM option select functions.

; Start of MCM option highlight functions.

Function OnOptionHighlight(int _opt)
	if CurPage == ePage_Managing
		if CurOpt == eOpt_New
			OnOptionHighlight_New(_opt)
		elseif CurOpt == eOpt_NewCycle
			OnOptionHighlight_NewCycle(_opt)
		elseif CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionHighlight_NewCycle(_opt)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionHighlight_New(_opt)
			endif
		endif
	elseif CurPage == ePage_Overview
		if CurOpt == eOpt_Edit
			if UIHS_IsCycleEquipset(sManaging_CycleOName)
				OnOptionHighlight_NewCycle(_opt)
			elseif !UIHS_IsCycleEquipset(sManaging_OName)
				OnOptionHighlight_New(_opt)
			endif
		endif
	elseif CurPage == ePage_Widget
		if _opt == optWidget_Display
			SetInfoText("$UIHS_Widget_Display_H")
		endif
	elseif CurPage == ePage_Setting
		if _opt == optSetting_Sort
			SetInfoText("$UIHS_Setting_Sort_H")
		elseif _opt == optSetting_Favor
			SetInfoText("$UIHS_Setting_Favor_H")
		elseif _opt == optSetting_Gamepad
			SetInfoText("$UIHS_Setting_Gamepad_H")
		endif
	elseif CurPage == ePage_Maintenance
		if _opt == optMaintenance_Save
			SetInfoText("$UIHS_Maintenance_Save_H")
		elseif _opt == optMaintenance_Load
			SetInfoText("$UIHS_Maintenance_Load_H")
		elseif _opt == optMaintenance_RSetting
			SetInfoText("$UIHS_Maintenance_RSetting_H")
		elseif _opt == optMaintenance_REquipsets
			SetInfoText("$UIHS_Maintenance_REquipsets_H")
		endif
	endif
EndFunction

Function OnOptionHighlight_New(int _opt)
	if _opt == optManaging_Sound
		SetInfoText("$UIHS_Managing_Sound_H")
	elseif _opt == optManaging_Toggle
		SetInfoText("$UIHS_Managing_Toggle_H")
	elseif _opt == optManaging_Reequip
		SetInfoText("$UIHS_Managing_Reequip_H")
	elseif _opt == optManaging_Beast
		SetInfoText("$UIHS_Managing_Beast_H")
	elseif _opt == optManaging_Hpos
		SetInfoText("$UIHS_Managing_Hpos_H")
	elseif _opt == optManaging_Vpos
		SetInfoText("$UIHS_Managing_Vpos_H")
	endif
EndFunction

Function OnOptionHighlight_NewCycle(int _opt)
	if _opt == optManaging_CyclePersist
		SetInfoText("$UIHS_Managing_CyclePersist_H")
	elseif _opt == optManaging_CycleExpire
		SetInfoText("$UIHS_Managing_CycleExpire_H")
	elseif _opt == optManaging_CycleReset
		SetInfoText("$UIHS_Managing_CycleReset_H")
	elseif _opt == optManaging_CycleBeast
		SetInfoText("$UIHS_Managing_Beast_H")
	elseif _opt == optManaging_CycleHpos
		SetInfoText("$UIHS_Managing_Hpos_H")
	elseif _opt == optManaging_CycleVpos
		SetInfoText("$UIHS_Managing_Vpos_H")
	endif
EndFunction

; End of MCM info text functions.

Function ClearProperties()
	CurOpt = eOpt_None
EndFunction

string Function CString(String _text, String _code)
	return "<font color='" + _code + "''>" + _text + "</font>"
EndFunction

Function ResetAllSetting()
	sWidget_Font = 0
	sWidget_FontSize = 100
	sWidget_Size = 100
	sWidget_Alpha = 100
	sWidget_Display = 0
	sWidget_Delay = 5.0
	sSetting_Modifier1 = 42
	sSetting_Modifier2 = 29
	sSetting_Modifier3 = 56
	sSetting_Sort = 0
	sSetting_SortString
	sSetting_Gamepad = false
	sSetting_Favor = false
	sMaintenance_Mod = true
	sMaintenance_Widget = true
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

string[] Function ZipSettingData()
	string[] result = new string[6]

	result[0] = sSetting_Modifier1 as string
	result[1] = sSetting_Modifier2 as string
	result[2] = sSetting_Modifier3 as string
	result[3] = sSetting_Sort as string
	result[4] = sSetting_Favor as string
	result[5] = sMaintenance_Widget as string

	return result
EndFunction

string[] Function ZipWidgetData()
	string[] result = new string[6]

	result[0] = sWidget_Font as string
	result[1] = sWidget_FontSize as string
	result[2] = sWidget_Size as string
	result[3] = sWidget_Alpha as string
	result[4] = sWidget_Display as string
	result[5] = sWidget_Delay as string

	return result
EndFunction

string[] Function ZipSaveData()
	string[] result = new string[14]
	result[0] = sWidget_Font as string
	result[1] = sWidget_FontSize as string
	result[2] = sWidget_Size as string
	result[3] = sWidget_Alpha as string
	result[4] = sWidget_Display as string
	result[5] = sWidget_Delay as string
	result[6] = sSetting_Modifier1 as string
	result[7] = sSetting_Modifier2 as string
	result[8] = sSetting_Modifier3 as string
	result[9] = sSetting_Sort as string
	result[10] = sSetting_Gamepad as string
	result[11] = sSetting_Favor as string
	result[12] = sMaintenance_Mod as string
	result[13] = sMaintenance_Widget as string

	return result
EndFunction

bool Function to_bool(string _data)
	return _data == "True" || _data == "TRUE" || _data == "1"
EndFunction