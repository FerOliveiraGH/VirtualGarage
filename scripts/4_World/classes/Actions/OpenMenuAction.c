class OpenMenuAction: ActionInteractBase
{
    private int    TraderID;
	private vector Pos;
	private bool   IsFlag;
	private int    GarageID;

    void OpenMenuAction()
    {
        TraderID = -1;
		IsFlag = false;
		Pos = Vector(0,0,0);
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
    }

    override string GetText()
	{
		return "#open";
	}

    override void CreateConditionComponents()  
	{
		m_ConditionItem 	= new CCINone();
		m_ConditionTarget 	= new CCTObject(UAMaxDistances.DEFAULT);
	}
    
    override bool HasProgress()
	{
		return false;
	}

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if ( GetGame().IsServer() )
            return true;

        BuildingBase nObject = BuildingBase.Cast( target.GetObject() );
        Pos = nObject.GetPosition();
        
        return true;
    }

    override void OnStartClient(ActionData action_data)
    {
        OpenGarageMenu(action_data.m_Target.GetObject());
    }

	void InitGarageMenu(PlayerBase player)
	{
		player.m_GarageMenu = new GarageMenu;
		player.m_GarageMenu.Init();
		player.m_GarageMenu.m_IsFlag = IsFlag;
		player.m_GarageMenu.m_Position = Pos;
		int LowUID = GarageHelpers.GetLowSteamID(GetGame().GetUserManager().GetTitleInitiator().GetUid());
		player.m_GarageMenu.m_LowUID = LowUID;
		GetRPCManager().SendRPC("Garage", "GarageRequest",  new Param3<int, vector,bool>(LowUID, Pos, IsFlag), true, NULL);
	}

	void OpenGarageMenu(Object obj)
	{
      	PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if ( g_Game.GetUIManager().GetMenu() == NULL )
		{
			InitGarageMenu(player);
			GetGame().GetUIManager().ShowScriptedMenu( player.m_GarageMenu, NULL );
		}
	}

	int GetLowSteamID(string SteamID64)
	{
		string temp_ID="";
		for(int j = 8; j<17; j++)
		{
			temp_ID+=SteamID64.Get(j);
		}
		return temp_ID.ToInt();
	}
}