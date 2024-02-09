class FaboLogger
{
    private static string Path = "$profile:VirtualGarage\\";
    private static string PathLog = "$profile:VirtualGarage\\logs\\";
    private static string FileName = "VirtualGarage-%1.log";
    private static ref FaboLogger faboLogger;
    private FileHandle fileHandle;

    void FaboLogger()
    {
        if (ConfigVirtualGarage.GetInstance().Logs() == 0)
            return;

        if (!FileExist(Path))
            MakeDirectory(Path);

        if (!FileExist(PathLog))
            MakeDirectory(PathLog);

        string fileName = string.Format(FileName, GetDate());
        string fullPath = PathLog + fileName;

        if (!FileExist(fullPath))
        {
            fileHandle = OpenFile(fullPath, FileMode.WRITE);
            FPrintln(fileHandle, "Start Log :: " + GetFullDate());
        }
        else
            fileHandle = OpenFile(fullPath, FileMode.APPEND);
    }

    static FaboLogger GetInstance()
    {
        if (!faboLogger)
            faboLogger = new FaboLogger();

        return faboLogger;
    }

    void Log(string log)
    {
        if (ConfigVirtualGarage.GetInstance().Logs() == 0)
            return;

        FPrintln(fileHandle, GetFullDate() + " :: " + log);
    }

    private string GetFullDate()
	{
		return GetDate() + " " + GetTime();
	}

	private string GetDate()
	{
	    int year, month, day;

		GetYearMonthDay(year, month, day);

		return "" + year + "-" + month + "-" + day;
	}

	private string GetTime()
	{
		int hour, minute, second;

		GetHourMinuteSecond(hour, minute, second);

		return "" + hour + ":" + minute + ":" + second;
	}
}