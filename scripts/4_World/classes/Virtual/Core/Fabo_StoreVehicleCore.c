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

        if (ConfigVirtualGarage.GetInstance().BlockedStorageWithoutOwner() && OwnerInvalid(vehicle, sender))
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#STR_Action_fabo_virtual_invalid_owner", "VirtualGarage/data/images/vglogo.paa");
            FaboLogger.GetInstance().Log("Player: " + sender.GetName() + " (" + sender.GetPlainId() + ") - Vehicle: " + vehicle.GetDisplayName() + " (" + vehicle.GetType() + ") - Action: " + "INVALID OWNER");
            return;
        }

        if (ConfigVirtualGarage.GetInstance().BlockedStorageWithoutKey() && KeyInvalid(vehicle))
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#STR_Action_fabo_virtual_invalid_owner", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        Fabo_ListGarage fabo_ListGarage = new Fabo_ListGarage(data.param1);
        int limit = ConfigVirtualGarage.GetInstance().Limit();

        if (limit != -1 && fabo_ListGarage.getVehiclesIds().Count() >= limit)
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_vehicle_max_limit", "VirtualGarage/data/images/vglogo.paa");
            return;
        }

        int uniqueId = CreateUniqueID();

        CreateVirtualVehicle(vehicle, sender, data.param1, uniqueId);

        fabo_ListGarage.addVehicleId(uniqueId);
        fabo_ListGarage.Store();

        vehicle.SetStoreVehicle(1);
        vehicle.Delete();
        vehicle.SetSynchDirty();

        GetRPCManager().SendRPC("VirtualGarage", "UpdateListVehicleRPC",  new Param1<int>(data.param1), true, sender);

        NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, 2, "#fabo_virtual_garage_title", "#fabo_success_storage", "VirtualGarage/data/images/vglogo.paa");

        FaboLogger.GetInstance().Log("Player: " + sender.GetName() + " (" + sender.GetPlainId() + ") - Vehicle: " + vehicle.GetDisplayName() + " (" + vehicle.GetType() + ") - Action: " + "STORE");
    }

    bool OwnerInvalid(CarScript vehicle, PlayerIdentity sender)
    {
        #ifdef FaboMod
        return vehicle.GetOwnerCar() != sender.GetId();
        #endif

        #ifdef ClaimVehicles_Mod
        return !vehicle.IsOwned() || vehicle.GetOwnerID() != sender.GetPlainId();
        #endif

        #ifdef MuchCarKey
        bool ownerInvalid = vehicle.m_OriginalOwnerId != sender.GetPlainId();
            #ifdef Trader
            if (!vehicle.m_HasCKAssigned)
                ownerInvalid = vehicle.m_Trader_LastDriverId != sender.GetId();
            #endif
        return ownerInvalid;
        #endif

        #ifdef Trader
        return vehicle.m_Trader_LastDriverId != sender.GetId();
        #endif

        return false;
    }

    bool KeyInvalid(CarScript vehicle)
    {
        #ifdef MuchCarKey
        bool keyInvalid = !vehicle.m_HasCKAssigned;
            #ifdef Trader
            if (keyInvalid)
                keyInvalid = !vehicle.m_Trader_HasKey;
            #endif
        return keyInvalid;
        #endif

        #ifdef TraderPlus
        return vehicle.m_CarLockPassword <= 0;
        #endif

        #ifdef Trader
        return !vehicle.m_Trader_HasKey;
        #endif

        return false;
    }

    void CreateVirtualVehicle(CarScript vehicle, PlayerIdentity sender, int ownerId, int uniqueId)
    {
        ref Fabo_VirtualVehicle virtualVehicle = new Fabo_VirtualVehicle(ownerId, uniqueId);

        virtualVehicle.SetType(vehicle.GetType());
        virtualVehicle.SetFuel(vehicle.GetFluidFraction(CarFluid.FUEL));
        virtualVehicle.SetOwner(sender.GetName());
        virtualVehicle.SetOwnerCarID(sender.GetId());
        virtualVehicle.SetInsuranceId(vehicle.GetInsuranceId());
        virtualVehicle.SetPosition(vehicle.GetPosition());

        #ifdef TraderPlus
        virtualVehicle.SetPassword(vehicle.m_CarLockPassword);
        #endif

        Fabo_VirtualVehicleKey virtualKey = MountVirtualKey(vehicle, sender, uniqueId);
        virtualVehicle.SetVirtualKey(virtualKey);

        virtualVehicle.SetAttachments(vehicle);
        virtualVehicle.Store();
    }

    Fabo_VirtualVehicleKey MountVirtualKey(CarScript vehicle, PlayerIdentity sender, int uniqueId)
    {
        Fabo_VirtualVehicleKey virtualKey = new Fabo_VirtualVehicleKey();

        #ifdef MuchCarKey
        if (vehicle.m_CarKeyId > 0)
        {
            virtualKey.type = "MuchCarKey";
            virtualKey.vehicleId = vehicle.m_CarScriptId;
            virtualKey.keyId = vehicle.m_CarKeyId;
            virtualKey.ownerName = vehicle.m_OriginalOwnerName;
            virtualKey.ownerID = vehicle.m_OriginalOwnerId;
        }
        #endif

        #ifdef TraderPlus
        if (!virtualKey.type && vehicle.m_CarLockPassword > 0)
        {
            virtualKey.type = "TraderPlus";
            virtualKey.vehicleId = uniqueId;
            virtualKey.keyId = vehicle.m_CarLockPassword;
            virtualKey.ownerName = sender.GetName();
            virtualKey.ownerID = sender.GetId();
        }
        #endif

        #ifdef Trader
        if (!virtualKey.type && vehicle.m_Trader_VehicleKeyHash > 0)
        {
            virtualKey.type = "Trader";
            virtualKey.vehicleId = uniqueId;
            virtualKey.keyId = vehicle.m_Trader_VehicleKeyHash;
            virtualKey.ownerName = sender.GetName();
            virtualKey.ownerID = vehicle.m_Trader_LastDriverId;
        }
        #endif

        return virtualKey;
    }

    int CreateUniqueID()
    {
        return Math.RandomInt(1000000000, 9999999999);
    }
}