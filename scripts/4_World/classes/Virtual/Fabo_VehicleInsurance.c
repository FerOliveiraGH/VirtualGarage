class Fabo_VehicleInsurance
{
    private const string Root = "$profile:VirtualGarage\\";
    private const string PathDatabase = "$profile:VirtualGarage\\database\\";
    private const string Path = "$profile:VirtualGarage\\database\\insurance\\";
    private int InsuranceId;
    private int UserId;
    private string Date;
    private string Expiration;

    void Fabo_VehicleInsurance(int insuranceId)
    {
        this.InsuranceId = insuranceId;
        this.Date = GetDate();
        this.Expiration = GetExpirationDate();

        if (!FileExist(Root))
            MakeDirectory(Root);

        if (!FileExist(PathDatabase))
            MakeDirectory(PathDatabase);

        if (!FileExist(Path))
            MakeDirectory(Path);
    }

    bool HasInsuranceId()
    {
        string insuranceId = InsuranceId.ToString();
        return InsuranceId > 0 && insuranceId.Length() > 1;
    }

    int GetInsuranceId()
    {
        return InsuranceId;
    }

    int GetUserId()
    {
        return UserId;
    }

    string GetExpiration()
    {
        return Expiration;
    }

    void SetUserId(int userId)
    {
        UserId = userId;
    }

    private string GetDate()
	{
	    int year, month, day;

		GetYearMonthDay(year, month, day);

		return day.ToString() + "/" + month.ToString() + "/" + year.ToString();
	}

	private string GetExpirationDate()
    {
        int year, month, day;
        GetYearMonthDay(year, month, day);

        day += 30;

        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        {
            month++;
            day -= 30;
        }
        else if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) && day > 29)
        {
            month++;
            day -= 29;
        }
        else if (month == 2 && day > 28)
        {
            month++;
            day -= 28;
        }
        else if (day > 31)
        {
            month++;
            day -= 31;

            if (month > 12)
            {
                month = 1;
                year++;
            }
        }

        return day.ToString() + "/" + month.ToString() + "/" + year.ToString();
    }

    bool CurrentInsuranceExpired()
    {
        return GetCurrentTimeStamp() > ConvertDateToTimestamp(Expiration);
    }

    int GetCurrentTimeStamp() {
        int year, month, day;
        GetYearMonthDay(year, month, day);

        string dateText = day.ToString() + "/" + month.ToString() + "/" + year.ToString();

        int timeStamp = ConvertDateToTimestamp(dateText);
        return timeStamp;
    }

    int ConvertDateToTimestamp(string data)
    {
        array<string> dateSeparator = new array<string>;
        data.Split("/", dateSeparator);

        int dia = dateSeparator[0].ToInt();
        int mes = dateSeparator[1].ToInt();
        int ano = dateSeparator[2].ToInt();

        int monthDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        int totalDays = (ano - 1970) * 365 + dia;

        for (int i = 0; i < mes - 1; ++i) {
            totalDays += monthDay[i];
        }

        if (mes > 2 && ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0)) {
            totalDays += 1;
        }

        int seconds = totalDays * 86400;  // 86400 seconds por dia
        return seconds;
    }

    void Store()
    {
        JsonFileLoader<Fabo_VehicleInsurance>.JsonSaveFile(Path + InsuranceId + ".json", this);
    }

    void Load()
    {
        JsonFileLoader<Fabo_VehicleInsurance>.JsonLoadFile(Path + InsuranceId + ".json", this);
    }

    void Delete()
    {
        DeleteFile(Path + InsuranceId + ".json");
    }
}