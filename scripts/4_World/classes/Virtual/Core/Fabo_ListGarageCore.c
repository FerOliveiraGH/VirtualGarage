class Fabo_ListGarageCore
{
    void Fabo_ListGarageCore()
    {
        InitRPC();
    }

    private static ref Fabo_ListGarageCore s_Instance;
    static Fabo_ListGarageCore GetInstance()
    {
        if (!s_Instance) {
            s_Instance = new Fabo_ListGarageCore();
        }

        return s_Instance;
    }

    void InitRPC()
    {
        GetRPCManager().AddRPC("VirtualGarage", "GetListVehicleRPC", this, SingleplayerExecutionType.Client);
    }

    void GetListVehicleRPC( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {
         Param1<int> data;

        if (!ctx.Read(data))
            return;

        Fabo_ListGarage fabo_ListGarage = new Fabo_ListGarage(data.param1);

        ref array<string> typeVehicles = new array<string>;
        array<int> vehiclesIds = fabo_ListGarage.getVehiclesIds();

        foreach(int vehicleId : vehiclesIds)
        {
            Fabo_VirtualVehicle fabo_VirtualVehicle = new Fabo_VirtualVehicle(data.param1, vehicleId);
            fabo_VirtualVehicle.Load();
            typeVehicles.Insert(fabo_VirtualVehicle.GetType());
        }

        GetRPCManager().SendRPC("VirtualGarage", "SetListVehicleRPC",  new Param1<array<string>>(typeVehicles), true, sender);
    }
}

class Fabo_ListGarage
{
    private const string Path = "$profile:VirtualGarage\\database\\vehicles\\";
    private int UserId;
    private ref array<int> VehiclesIds = new array<int>;

    void Fabo_ListGarage(int userId)
    {
        this.UserId = userId;

        if (!FileExist(Path + UserId + ".json"))
            Store();
        else
            Load();
    }

    void Store()
    {
        JsonFileLoader<Fabo_ListGarage>.JsonSaveFile(Path + UserId + ".json", this);
    }

    void Load()
    {
        JsonFileLoader<Fabo_ListGarage>.JsonLoadFile(Path + UserId + ".json", this);
    }

    ref array<int> getVehiclesIds()
    {
        return VehiclesIds;
    }

    int getVehicleId(int index)
    {
        return VehiclesIds[index];
    }

    void addVehicleId(int vehicleId)
    {
        VehiclesIds.Insert(vehicleId);
    }

    void removeVehicleId(int index)
    {
        VehiclesIds.Remove(index);
    }
}