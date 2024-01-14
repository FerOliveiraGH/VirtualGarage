class Fabo_DeployVehicleCore
{
    void Fabo_DeployVehicleCore()
	{
	}

	private static ref Fabo_DeployVehicleCore s_Instance;
    static Fabo_DeployVehicleCore GetInstance()
    {
        if (!s_Instance) {
            s_Instance = new Fabo_DeployVehicleCore();
        }

        return s_Instance;
    }

    void InitRPC()
    {
        GetRPCManager().AddRPC("VirtualGarage", "DeployVehicleRPC", this, SingleplayerExecutionType.Client);
    }

    void DeployVehicleRPC( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {

        Param4<vector, vector, int, int> data;

        if (!ctx.Read(data))
            return;

        Print("FaboMod :: DeployVehicleRPC " + data.param3);

        Fabo_CreateVehicle(data.param1, data.param2, data.param3, data.param4);
    }

//    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
//    {
//        if (target.GetObject())
//            return false;
//
//        if (item.GetQuantity() > 0)
//            return false;
//
//        CarReceipt receipt = CarReceipt.Cast(player.GetItemInHands());
//        if (receipt.GetInventory().IsInventoryUnlocked())
//            return false;
//
//        vector position = getObjectPosition(player);
//        vector orientation = getObjectOrientation(player);
//
//        ref array<ref AreaSpawnVehicleBlocked> areasSpawnVehicleBlocked = ConfigFaboMod.GetInstance().getAreasSpawnVehicleBlocked();
//
//        foreach(ref AreaSpawnVehicleBlocked areaSpawnVehicleBlocked: areasSpawnVehicleBlocked)
//        {
//            vector vectorBlocked = areaSpawnVehicleBlocked.getLocation();
//
//            float distance = vector.Distance(vectorBlocked, position);
//
//            if (distance < areaSpawnVehicleBlocked.getRadius())
//                return false;
//        }
//
//        if (IsColliding(position, orientation))
//            return false;
//
//        if (IsSurfaceWater(position))
//            return false;
//
//        return true;
//    }

//    override bool ActionConditionContinue(ActionData action_data)
//    {
//         return true;
//    }

//    override void OnFinishProgressServer(ActionData action_data)
//    {
//        if (!action_data.m_MainItem || action_data.m_MainItem.GetHierarchyRootPlayer() != action_data.m_Player)
//            return;
//
//        PlayerBase player = PlayerBase.Cast(action_data.m_Player);
//        CarReceipt receipt = CarReceipt.Cast(player.GetItemInHands());
//
//        CreateVehicle(getObjectPosition(player), getObjectOrientation(player), receipt);
//    }
//
//    bool IsColliding(vector position, vector orientation)
//    {
//        return GetGame().IsBoxCollidingGeometry(position, orientation, Vector(2, 4, 2), ObjIntersect.View, ObjIntersect.Geom, new array<Object>);
//    }
//
//    bool IsSurfaceWater(vector position)
//    {
//        return GetGame().SurfaceIsSea(position[0], position[2]) || GetGame().SurfaceIsPond(position[0], position[2]);
//    }
//
//    vector getObjectPosition(PlayerBase player)
//    {
//        float meters = 5;
//
//        vector objectPosition = player.GetPosition() + player.GetDirection() * meters;
//        objectPosition[1] = GetGame().SurfaceY(objectPosition[0],objectPosition[2]);
//
//        return objectPosition;
//    }
//
//    vector getObjectOrientation(PlayerBase player)
//    {
//        float graus = -90;
//
//        vector objectOrientation = player.GetOrientation();
//        objectOrientation[0] = objectOrientation[0] + graus;
//
//        return objectOrientation;
//    }
}