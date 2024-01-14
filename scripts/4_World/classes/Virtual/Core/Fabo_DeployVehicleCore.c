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
        fabo_ListGarage.removeVehicleId(data.param4);
        fabo_ListGarage.Store();

        Fabo_CreateVehicle(data.param1, data.param2, data.param3, vehicleId);

        GetRPCManager().SendRPC("VirtualGarage", "UpdateListVehicleRPC",  new Param1<int>(data.param3), true, NULL);
    }
}