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
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_vehicle_not_found", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        if (IsColliding(data.param1, data.param2))
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_block_area", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        if (!ConfigVirtualGarage.GetInstance().SpawnInWater() && IsSurfaceWater(data.param1))
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_block_area", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        Fabo_VirtualVehicle virtualVehicle = new Fabo_VirtualVehicle(data.param3, vehicleId);
        virtualVehicle.Load();

        if (ConfigVirtualGarage.GetInstance().BlockedDeployByGarage() && virtualVehicle.GetPosition())
        {
            float distance = vector.Distance(virtualVehicle.GetPosition(), data.param1);
            #ifdef FABODEBUG
            Print("VirtualGarage :: virtualVehicle.GetPosition() " + virtualVehicle.GetPosition());
            Print("VirtualGarage :: data.param1 " + data.param1);
            Print("VirtualGarage :: distance " + distance);
            #endif
            if (distance > 3)
            {
                NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_parking_blocked_garage", "VirtualGarage/data/images/vglogo.paa");
                return;
            }
        }

        Fabo_CreateVehicle fabo_CreateVehicle = Fabo_CreateVehicle(data.param1, data.param2, virtualVehicle);
        CarScript vehicle = fabo_CreateVehicle.Vehicle;

        if (!vehicle)
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_failed_recovered", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        fabo_ListGarage.removeVehicleId(data.param4);
        fabo_ListGarage.Store();

        GetRPCManager().SendRPC("VirtualGarage", "UpdateListVehicleRPC",  new Param1<int>(data.param3), true, sender);

        NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_success_recovered", "VirtualGarage/data/images/vglogo.paa");

        FaboLogger.GetInstance().Log("Player: " + sender.GetName() + " (" + sender.GetPlainId() + ") - Vehicle: " + vehicle.GetDisplayName() + " (" + vehicle.GetType() + ") - Action: " + "DEPLOY");
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