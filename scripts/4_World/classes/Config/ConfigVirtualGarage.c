class ConfigVirtualGarage
{
    private const string fabo_Patch = "$profile:VirtualGarage\\";
    private const string fabo_File = "$profile:VirtualGarage\\settings.json";
    private string version;
    private bool logs = true;
    private bool attachDetachWithTool = true;
    private int limit = -1;
    private bool spawnInWater = false;
    private bool blockedDeployByGarage = false;
    private bool blockedStorageWithoutOwner = true;
    private bool blockedStorageWithoutKey = false;
    private int distanceDeploy = 4;

    void ConfigVirtualGarage()
    {
        if (GetGame().IsServer())
        {
            if (!FileExist(fabo_Patch))
                MakeDirectory(fabo_Patch);

            if (!FileExist(fabo_File))
            {
                version = "1.4.0";
                Store();
            }

            Load();

            if (version != "1.4.0")
            {
                version = "1.4.0";
                Store();
            }
        }
    }

    private static ref ConfigVirtualGarage s_Instance;
    static ConfigVirtualGarage GetInstance()
    {
        if (!s_Instance) {
            s_Instance = new ConfigVirtualGarage();
        }

        return s_Instance;
    }

    bool AttachDetachWithTool()
    {
        return attachDetachWithTool;
    }

    bool Logs()
    {
        return logs;
    }

    int Limit()
    {
        return limit;
    }

    bool SpawnInWater()
    {
        return spawnInWater;
    }

    bool BlockedDeployByGarage()
    {
        return blockedDeployByGarage;
    }

    bool BlockedStorageWithoutOwner()
    {
        return blockedStorageWithoutOwner;
    }

    bool BlockedStorageWithoutKey()
    {
        return blockedStorageWithoutKey;
    }
    
    int DistanceDeploy()
    {
        return distanceDeploy;
    }

    private void Store()
    {
        JsonFileLoader<ConfigVirtualGarage>.JsonSaveFile(fabo_File, this);
    }

    private void Load()
    {
        JsonFileLoader<ConfigVirtualGarage>.JsonLoadFile(fabo_File, this);
    }

    void SendRPC(PlayerIdentity identity)
    {
        Param4<bool, bool, bool, int> data = new Param4
            <bool, bool, bool, int>
            (attachDetachWithTool, blockedStorageWithoutOwner, blockedStorageWithoutKey, distanceDeploy);

        GetRPCManager().SendRPC("VirtualGarage", "LoadSettings", data, true, identity);
    }

    void AddRPC()
    {
        GetRPCManager().AddRPC("VirtualGarage", "LoadSettings", this, SingleplayerExecutionType.Client);
    }

    void LoadSettings(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if(type != CallType.Client)
			return;

		Param4<bool, bool, bool, int> data;
		if(!ctx.Read(data))
			return;

        attachDetachWithTool = data.param1;
        blockedStorageWithoutOwner = data.param2;
        blockedStorageWithoutKey = data.param3;
        distanceDeploy = data.param4;
	}
}