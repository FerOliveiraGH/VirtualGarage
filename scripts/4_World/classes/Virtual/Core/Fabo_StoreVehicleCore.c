class Fabo_StoreVehicleCore
{
    void Fabo_StoreVehicleCore()
    {
        Print("FaboMod :: InitRPC");
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
        Param3<int, CarScript, vector> data;

        if (!ctx.Read(data))
            return;

        CarScript vehicle = data.param2;

        Fabo_VirtualVehicle virtualVehicle = new Fabo_VirtualVehicle(data.param1, CreateUniqueID());

        virtualVehicle.SetType(vehicle.GetType());
        virtualVehicle.SetFuel(vehicle.GetFluidFraction(CarFluid.FUEL));
        virtualVehicle.SetOwner("" + data.param1);
        #ifdef TraderPlus
        virtualVehicle.SetPassword(vehicle.m_CarLockPassword);
        #endif
        virtualVehicle.SetAttachments(vehicle);

        virtualVehicle.Store();

//        vehicle.Delete();
    }

    int CreateUniqueID()
    {
        return Math.RandomInt(1000000000, 9999999999);
    }
}