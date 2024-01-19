class Fabo_OpenMenuAction: ActionInteractBase
{
	private vector Pos;
	private vector Dir;
	private vector Ori;

    void Fabo_OpenMenuAction()
    {
		Pos = Vector(0,0,0);
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;

        if (GetGame().IsClient())
        {
            GetRPCManager().AddRPC("VirtualGarage", "SetListVehicleRPC", this, SingleplayerExecutionType.Server);
            GetRPCManager().AddRPC("VirtualGarage", "UpdateListVehicleRPC", this, SingleplayerExecutionType.Server);
        }
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

		Fabo_VirtualGarage virtualGarage = Fabo_VirtualGarage.Cast(target.GetObject());

		if (virtualGarage.Deployed == 0)
			return false;

        ItemBase nObject = ItemBase.Cast( target.GetObject() );
        Pos = nObject.GetPosition();
        Dir = nObject.GetDirection();
        Ori = nObject.GetOrientation();

        return true;
    }

    override void OnStartClient(ActionData action_data)
    {
		OpenVirtualGarageMenu(action_data.m_Target.GetObject());
    }

	void StartGarageMenu(PlayerBase player)
	{
		player.m_Fabo_VirtualGarageMenu = new Fabo_VirtualGarageMenu;
		player.m_Fabo_VirtualGarageMenu.Init();
		player.m_Fabo_VirtualGarageMenu.m_ParkingPosition = Pos;
		player.m_Fabo_VirtualGarageMenu.m_ParkingDirection = Dir;
		player.m_Fabo_VirtualGarageMenu.m_ParkingOrientation = Ori;

        string uid = GetGame().GetUserManager().GetTitleInitiator().GetUid();
        int uniquePlayerId = player.m_Fabo_VirtualGarageMenu.GetPlayerUniqueId(uid);

        GetRPCManager().SendRPC("VirtualGarage", "GetListVehicleRPC",  new Param1<int>(uniquePlayerId), true, player.GetIdentity());
	}

	void OpenVirtualGarageMenu(Object obj)
	{
      	PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (g_Game.GetUIManager().GetMenu() != NULL)
            return;

        StartGarageMenu(player);
        GetGame().GetUIManager().ShowScriptedMenu(player.m_Fabo_VirtualGarageMenu, NULL);
	}

	void SetListVehicleRPC( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {
        if(!GetGame().IsClient())return;

        Param1<array<string>> data;

        if (!ctx.Read(data))
            return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

        if (player.GetIdentity().GetId() != sender.GetId())
            return;

        player.m_Fabo_VirtualGarageMenu.SetDataVehicle(data.param1, getObjectPosition(Pos, Dir), Ori);
        player.m_Fabo_VirtualGarageMenu.GetListVehicles();
    }

    void UpdateListVehicleRPC( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {
        if(!GetGame().IsClient())return;

        Param1<int> data;

        if (!ctx.Read(data))
            return;

        GetRPCManager().SendRPC("VirtualGarage", "GetListVehicleRPC",  new Param1<int>(data.param1), true, sender);
    }

    vector getObjectPosition(vector pos, vector dir)
    {
        float meters = -4;

        vector objectPosition = pos + dir * meters;
//        objectPosition[1] = GetGame().SurfaceY(objectPosition[0],objectPosition[2]);

        return objectPosition;
    }
}