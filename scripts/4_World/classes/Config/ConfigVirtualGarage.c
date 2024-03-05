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

    void ConfigVirtualGarage()
    {
        if (GetGame().IsServer())
        {
            if (!FileExist(fabo_Patch))
                MakeDirectory(fabo_Patch);

            if (!FileExist(fabo_File))
            {
                version = "1.2.0";
                Store();
            }

            Load();

            if (version != "1.2.0")
            {
                version = "1.2.0";
                Store();
            }
        }
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

    private void Store()
    {
        JsonFileLoader<ConfigVirtualGarage>.JsonSaveFile(fabo_File, this);
    }

    private void Load()
    {
        JsonFileLoader<ConfigVirtualGarage>.JsonLoadFile(fabo_File, this);
    }

    private static ref ConfigVirtualGarage s_Instance;
    static ConfigVirtualGarage GetInstance()
    {
        if (!s_Instance) {
            s_Instance = new ConfigVirtualGarage();
        }

        return s_Instance;
    }

    void SendRPC(PlayerIdentity identity)
    {
        GetRPCManager().SendRPC("VirtualGarage", "LoadSettings", new Param1<bool>(attachDetachWithTool), true, identity);
    }

    void AddRPC()
    {
        GetRPCManager().AddRPC("VirtualGarage", "LoadSettings", this, SingleplayerExecutionType.Client);
    }

    void LoadSettings(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if(type != CallType.Client)
			return;

		Param1<bool> data;
		if(!ctx.Read(data))
			return;

        attachDetachWithTool = data.param1;
	}
}