class Fabo_DeployVehicleCore
{
    void Fabo_DeployVehicleCore()
	{
	    InitRPC();
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

        Fabo_ListGarage fabo_ListGarage = new Fabo_ListGarage(data.param3);
        int vehicleId = fabo_ListGarage.getVehicleId(data.param4);

        if (!vehicleId)
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_garage_title", "#fabo_vehicle_not_found", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        if (IsColliding(data.param1, data.param2))
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_garage_title", "#fabo_block_area", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        if (ConfigVirtualGarage.GetInstance().SpawnInWater() == 0 && IsSurfaceWater(data.param1))
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_garage_title", "#fabo_block_area", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        fabo_ListGarage.removeVehicleId(data.param4);
        fabo_ListGarage.Store();

        Fabo_CreateVehicle(data.param1, data.param2, data.param3, vehicleId);

        GetRPCManager().SendRPC("VirtualGarage", "UpdateListVehicleRPC",  new Param1<int>(data.param3), true, sender);

        NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_garage_title", "#fabo_success_recovered", "VirtualGarage/data/images/vglogo.paa");
    }

    bool IsColliding(vector position, vector orientation)
    {
        return GetGame().IsBoxCollidingGeometry(position, orientation, Vector(2, 4, 2), ObjIntersect.View, ObjIntersect.Geom, new array<Object>);
    }

    bool IsSurfaceWater(vector position)
    {
        return GetGame().SurfaceIsSea(position[0], position[2]) || GetGame().SurfaceIsPond(position[0], position[2]);
    }
}