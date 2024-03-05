class Fabo_StoreVehicleCore
{
    void Fabo_StoreVehicleCore()
    {
        InitRPC();
    }

    private static ref Fabo_StoreVehicleCore s_Instance;
    static Fabo_StoreVehicleCore GetInstance()
    {
        if (!s_Instance) {
            s_Instance = new Fabo_StoreVehicleCore();
        }

        return s_Instance;
    }

    void InitRPC()
    {
        GetRPCManager().AddRPC("VirtualGarage", "StoreVehicleRPC", this, SingleplayerExecutionType.Client);
    }

    void StoreVehicleRPC( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {
        Param2<int, CarScript> data;

        if (!ctx.Read(data))
            return;

        CarScript vehicle = data.param2;

        if (!vehicle)
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_vehicle_not_found", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        if (vehicle.GetHealthLevel("") == GameConstants.STATE_RUINED)
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_vehicle_destroyed", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        #ifdef FaboMod
        if (vehicle.GetOwnerCar() != sender.GetId())
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#STR_Action_fabo_virtual_invalid_owner", "VirtualGarage/data/images/vglogo.paa");
            FaboLogger.GetInstance().Log("Player: " + sender.GetName() + " (" + sender.GetPlainId() + ") - Vehicle: " + vehicle.GetDisplayName() + " (" + vehicle.GetType() + ") - Action: " + "INVALID OWNER");
            return;
        }
        #endif

        #ifdef ClaimVehicles_Mod
        if (!vehicle.IsOwned() || vehicle.GetOwnerID() != sender.GetPlainId())
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#STR_Action_fabo_virtual_invalid_owner", "VirtualGarage/data/images/vglogo.paa");
            FaboLogger.GetInstance().Log("Player: " + sender.GetName() + " (" + sender.GetPlainId() + ") - Vehicle: " + vehicle.GetDisplayName() + " (" + vehicle.GetType() + ") - Action: " + "INVALID OWNER");
            return;
        }
        #endif

        #ifdef TraderPlus
        if (vehicle.m_CarLockPassword <= 0)
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#STR_Action_fabo_virtual_invalid_owner", "VirtualGarage/data/images/vglogo.paa");
            return;
        }
        #endif

        Fabo_ListGarage fabo_ListGarage = new Fabo_ListGarage(data.param1);
        int limit = ConfigVirtualGarage.GetInstance().Limit();

        if (limit != -1 && fabo_ListGarage.getVehiclesIds().Count() >= limit)
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_vehicle_max_limit", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        int uniqueId = CreateUniqueID();

        Fabo_VirtualVehicle virtualVehicle = new Fabo_VirtualVehicle(data.param1, uniqueId);

        virtualVehicle.SetType(vehicle.GetType());
        virtualVehicle.SetFuel(vehicle.GetFluidFraction(CarFluid.FUEL));
        virtualVehicle.SetOwner(sender.GetName());
        virtualVehicle.SetOwnerCarID(sender.GetId());
        virtualVehicle.SetInsuranceId(vehicle.GetInsuranceId());
        virtualVehicle.SetPosition(vehicle.GetPosition());

        #ifdef TraderPlus
        virtualVehicle.SetPassword(vehicle.m_CarLockPassword);
        #endif

        virtualVehicle.SetAttachments(vehicle);
        virtualVehicle.Store();

        fabo_ListGarage.addVehicleId(uniqueId);
        fabo_ListGarage.Store();

        vehicle.SetStoreVehicle(1);
        vehicle.Delete();
        vehicle.SetSynchDirty();

        GetRPCManager().SendRPC("VirtualGarage", "UpdateListVehicleRPC",  new Param1<int>(data.param1), true, sender);

        NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_success_storage", "VirtualGarage/data/images/vglogo.paa");

        FaboLogger.GetInstance().Log("Player: " + sender.GetName() + " (" + sender.GetPlainId() + ") - Vehicle: " + vehicle.GetDisplayName() + " (" + vehicle.GetType() + ") - Action: " + "STORE");
    }

    int CreateUniqueID()
    {
        return Math.RandomInt(1000000000, 9999999999);
    }
}