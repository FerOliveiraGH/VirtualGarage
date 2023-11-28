class ConfigVirtualGarage
{
    private const string Patch = "$profile:VirtualGarage\\";
    private const string File = "$profile:VirtualGarage\\settings.json";
    private int attachDetachWithTool = 1;

    void ConfigVirtualGarage()
    {
        if (GetGame().IsServer())
        {
            if (!FileExist(Patch))
                MakeDirectory(Patch);

            if (!FileExist(File))
                Store();

            Load();
        }
    }

    int AttachDetachWithTool()
    {
        return attachDetachWithTool;
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

    void SendRPC()
    {
        GetRPCManager().SendRPC("VirtualGarage", "LoadSettings", new Param1<int>(attachDetachWithTool), true, GetGame().GetPlayer().GetIdentity());
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