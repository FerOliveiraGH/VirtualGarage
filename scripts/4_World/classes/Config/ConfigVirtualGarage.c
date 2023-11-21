class ConfigVirtualGarage
{
    private const string Patch = "$profile:VirtualGarage\\";
    private const string File = "$profile:VirtualGarage\\settings.json";
    private int attachDetachWithTool = 1;

    void ConfigVirtualGarage()
    {
        if (!FileExist(Patch))
            MakeDirectory(Patch);

        if (!FileExist(File))
            Store();

        Load();
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
}