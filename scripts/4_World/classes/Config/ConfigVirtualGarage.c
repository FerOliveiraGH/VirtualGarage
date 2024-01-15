class ConfigVirtualGarage
{
    private const string Patch = "$profile:VirtualGarage\\";
    private const string File = "$profile:VirtualGarage\\settings.json";
    private string version;
    private int attachDetachWithTool = 1;
    private int limit = -1;
    private int spawnInWater = 0;

    void ConfigVirtualGarage()
    {
        if (GetGame().IsServer())
        {
            if (!FileExist(Patch))
                MakeDirectory(Patch);

            if (!FileExist(File))
            {
                version = "1.0.0";
                Store();
            }

            Load();

            if (version != "1.0.0")
            {
                version = "1.0.0";
                Store();
            }
        }
    }

    int AttachDetachWithTool()
    {
        return attachDetachWithTool;
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
        JsonFileLoader<ConfigVirtualGarage>.JsonSaveFile(File, this);
    }

    private void Load()
    {
        JsonFileLoader<ConfigVirtualGarage>.JsonLoadFile(File, this);
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