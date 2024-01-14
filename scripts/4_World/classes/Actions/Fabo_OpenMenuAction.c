class Fabo_OpenMenuAction: ActionInteractBase
{
    private int    TraderID;
	private vector Pos;
	private vector Dir;
	private vector Ori;
	private bool   IsFlag;
	private int    GarageID;
	ref TStringArray m_VehiclesName;
	ref TIntArray m_VehiclesHealth;

    void Fabo_OpenMenuAction()
    {
        TraderID = -1;
		IsFlag = true;
		Pos = Vector(0,0,0);
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;

        if (GetGame().IsClient())
            GetRPCManager().AddRPC("VirtualGarage", "SetListVehicleRPC", this, SingleplayerExecutionType.Server);
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
		OpenGarageMenu(action_data.m_Target.GetObject());
    }

	void InitGarageMenu(PlayerBase player)
	{
		player.m_Fabo_VirtualGarageMenu = new Fabo_VirtualGarageMenu;
		player.m_Fabo_VirtualGarageMenu.Init();
//		player.m_Fabo_VirtualGarageMenu.m_IsFlag = IsFlag;
//		player.m_Fabo_VirtualGarageMenu.m_Position = Pos;
		player.m_Fabo_VirtualGarageMenu.m_ParkingPos = Pos;
		player.m_Fabo_VirtualGarageMenu.m_ParkingDir = Dir;
		player.m_Fabo_VirtualGarageMenu.m_ParkingOri = Ori;
//		int LowUID = GarageHelpers.GetLowSteamID(GetGame().GetUserManager().GetTitleInitiator().GetUid());
//		player.m_Fabo_VirtualGarageMenu.m_LowUID = LowUID;

//		GetRPCManager().SendRPC("VirtualGarage", "GarageRequest",  new Param4<int, vector, vector, vector>(LowUID, Pos, Dir, Ori), true, NULL);
        int LowUID = player.m_Fabo_VirtualGarageMenu.GetLowSteamID(GetGame().GetUserManager().GetTitleInitiator().GetUid());
        GetRPCManager().SendRPC("VirtualGarage", "GetListVehicleRPC",  new Param1<int>(LowUID), true, NULL);
	}

	void OpenGarageMenu(Object obj)
	{
      	PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if ( g_Game.GetUIManager().GetMenu() == NULL )
		{
			InitGarageMenu(player);
			GetGame().GetUIManager().ShowScriptedMenu( player.m_Fabo_VirtualGarageMenu, NULL );
		}
	}

	 void SetListVehicleRPC( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {
        if(!GetGame().IsClient())return;

        Param1<array<string>> data;

        if (!ctx.Read(data))
            return;

        GetGame().ChatPlayer("FaboMod :: data.param1 " + data.param1.Count());

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        player.m_Fabo_VirtualGarageMenu.SetResponseData(data.param1, "0 0 0");
        player.m_Fabo_VirtualGarageMenu.UIHandle();
//        SetResponseData(TStringArray vehicles, vector parkpos)

//        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
//
//        m_VehiclesName.Clear();
//        m_VehiclesHealth.Clear();
//
//        m_VehiclesName = data.param1;
//        m_VehiclesHealth = data.param2;
//
//        if(player && player.m_Fabo_VirtualGarageMenu)
//        {
//            player.m_Fabo_VirtualGarageMenu.SetResponseData(data.param1,data.param3);
//            player.m_Fabo_VirtualGarageMenu.SetResponseDataDirOri(data.param4,data.param5);
//            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.m_Fabo_VirtualGarageMenu.UIHandle, 50);
//        }
    }

//    void GarageRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
//    {
//        if (!GetGame().IsServer())
//            return;
//
//        Param4<int, vector, vector, vector> data;
//
//        if (!ctx.Read(data))
//            return;
//
//        PlayerBase player = FaboGetPlayerByIdentity(sender);
//
//        if (player)
//            GarageRequestHandler(player, data.param1, data.param2, data.param3, data.param4);
//    }
//
//    void GarageRequestHandler(PlayerBase player, int id, vector pos, vector dir, vector ori)
//    {
//        int start = TickCount(0);
//        GarageData garageData = GarageData.Load(id.ToString(), player.GetIdentity().GetName());
//        int time = TickCount(start);
//        if(!garageData)
//            return;
//
//        TStringArray vehiclesName = new TStringArray;
//        TIntArray vehiclesHealth = new TIntArray;
//
//        foreach(VehiculeData vehicle: garageData.Vehicles)
//        {
//            vehiclesName.Insert(vehicle.VehicleName);
////            vehiclesHealth.Insert(GarageHelpers.ConvertHealthToHealthLevel(vehicle.VehicleName, vehicle.EngineHealth));
//        }
//
//        GetRPCManager().SendRPC("VirtualGarage", "GarageResponse", new Param5<ref TStringArray, ref TIntArray, vector, vector, vector>(vehiclesName, vehiclesHealth, pos, dir, ori), true, player.GetIdentity());
//    }
//
//    void GarageResponse(CallType type, ParamsReadContext ctx,  PlayerIdentity sender,  Object target)
//    {
//        if(!GetGame().IsClient())return;
//
//        Param5<ref TStringArray, ref TIntArray ,vector, vector, vector> data;
//
//        if (!ctx.Read(data))
//            return;
//
//        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
//
//        m_VehiclesName.Clear();
//        m_VehiclesHealth.Clear();
//
//        m_VehiclesName = data.param1;
//        m_VehiclesHealth = data.param2;
//
//        if(player && player.m_Fabo_VirtualGarageMenu)
//        {
//            player.m_Fabo_VirtualGarageMenu.SetResponseData(data.param1,data.param3);
//            player.m_Fabo_VirtualGarageMenu.SetResponseDataDirOri(data.param4,data.param5);
//            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.m_Fabo_VirtualGarageMenu.UIHandle, 50);
//        }
//    }

//	int GetLowSteamID(string SteamID64)
//	{
//		string temp_ID="";
//		for(int j = 8; j<17; j++)
//		{
//			temp_ID+=SteamID64.Get(j);
//		}
//		return temp_ID.ToInt();
//	}

//	static PlayerBase FaboGetPlayerByIdentity(PlayerIdentity sender)
//    {
//        int	low	 =	0;
//        int	high =	0;
//        GetGame().GetPlayerNetworkIDByIdentityID( sender.GetPlayerId(), low, high );
//        return PlayerBase.Cast( GetGame().GetObjectByNetworkId(low, high ));
//    }
}