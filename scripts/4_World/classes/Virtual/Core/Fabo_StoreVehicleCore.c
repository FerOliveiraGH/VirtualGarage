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
            return;

        int uniqueId = CreateUniqueID();

        Fabo_ListGarage fabo_ListGarage = new Fabo_ListGarage(data.param1);
        fabo_ListGarage.addVehicleId(uniqueId);
        fabo_ListGarage.Store();

        Fabo_VirtualVehicle virtualVehicle = new Fabo_VirtualVehicle(data.param1, uniqueId);

        virtualVehicle.SetType(vehicle.GetType());
        virtualVehicle.SetFuel(vehicle.GetFluidFraction(CarFluid.FUEL));
        virtualVehicle.SetOwner("" + data.param1);

        #ifdef TraderPlus
        virtualVehicle.SetPassword(vehicle.m_CarLockPassword);
        #endif

        virtualVehicle.SetAttachments(vehicle);
        virtualVehicle.Store();

        vehicle.Delete();
        vehicle.SetSynchDirty();

        GetRPCManager().SendRPC("VirtualGarage", "UpdateListVehicleRPC",  new Param1<int>(data.param1), true, NULL);
    }

    int CreateUniqueID()
    {
        return Math.RandomInt(1000000000, 9999999999);
    }
}