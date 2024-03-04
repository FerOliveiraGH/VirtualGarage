class ConfigVirtualGarage
{
    private const string fabo_Patch = "$profile:VirtualGarage\\";
    private const string fabo_File = "$profile:VirtualGarage\\settings.json";
    private string version;
    private int logs = 1;
    private int attachDetachWithTool = 1;
    private int limit = -1;
    private int spawnInWater = 0;

    void ConfigVirtualGarage()
    {
        if (GetGame().IsServer())
        {
            if (!FileExist(fabo_Patch))
                MakeDirectory(fabo_Patch);

            if (!FileExist(fabo_File))
            {
                version = "1.1.0";
                Store();
            }

            Load();

            if (version != "1.1.0")
            {
                version = "1.1.0";
                Store();
            }
        }
    }

    int AttachDetachWithTool()
    {
        return attachDetachWithTool;
    }

    int Logs()
    {
        return logs;
    }

    int Limit()
    {
        return limit;
    }

    int SpawnInWater()
    {
        return spawnInWater;
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
        GetRPCManager().SendRPC("VirtualGarage", "LoadSettings", new Param1<int>(attachDetachWithTool), true, identity);
    }

    void AddRPC()
    {
        GetRPCManager().AddRPC("VirtualGarage", "LoadSettings", this, SingleplayerExecutionType.Client);
    }

    void LoadSettings(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if(type != CallType.Client)
			return;

		Param1<int> data;
		if(!ctx.Read(data))
			return;

        attachDetachWithTool = data.param1;
	}
}