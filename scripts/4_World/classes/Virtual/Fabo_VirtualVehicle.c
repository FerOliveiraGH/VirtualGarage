class Fabo_VirtualVehicle
{
    private const string Root = "$profile:VirtualGarage\\";
    private const string Database = "$profile:VirtualGarage\\database\\";
    private const string Path = "$profile:VirtualGarage\\database\\vehicles\\";
    private const string PathInsurance = "$profile:VirtualGarage\\database\\insurance\\";
    private int UniqueID;
    private string Type;
    private int OwnerID;
    private string OwnerCarID;
    private string Owner;
    private float Fuel;
    private int Password;
    private int Insurance;
    private int InsuranceId;
    private vector Position;
    private Fabo_VirtualVehicleKey VirtualKey;
    private ref array<ref Fabo_VirtualObject> Attachments = new array<ref Fabo_VirtualObject>();

    void Fabo_VirtualVehicle(int ownerID, int uniqueID, int insurance = 0)
    {
        this.OwnerID = ownerID;
        this.UniqueID = uniqueID;
        this.Insurance = insurance;

        if (!FileExist(Root))
            MakeDirectory(Root);

        if (!FileExist(Database))
            MakeDirectory(Database);

        if (!FileExist(GetPathFile()))
            MakeDirectory(GetPathFile());

        if (!FileExist(GetPathFile() + ownerID + "\\"))
            MakeDirectory(GetPathFile() + ownerID + "\\");
    }

    int GetUniqueID()
    {
        return this.UniqueID;
    }

    string GetType()
    {
        return this.Type;
    }
    
    float GetFuel()
    {
        return this.Fuel;
    }

    int GetOwnerID()
    {
        return this.OwnerID;
    }

    string GetOwnerCarID()
    {
        return this.OwnerCarID;
    }

    string GetOwner()
    {
        return this.Owner;
    }

    int GetPassword()
    {
        return this.Password;
    }

    int GetInsuranceId()
    {
        return this.InsuranceId;
    }

    vector GetPosition()
    {
        return this.Position;
    }

    bool HasInsuranceId()
    {
        string insuranceId = InsuranceId.ToString();
        return InsuranceId > 0 && insuranceId.Length() > 1;
    }

    array<ref Fabo_VirtualObject> GetAttachments()
    {
        return this.Attachments;
    }

    Fabo_VirtualVehicleKey GetVirtualKey()
    {
        return this.VirtualKey;
    }

    void SetVirtualKey(Fabo_VirtualVehicleKey virtualKey)
    {
        this.VirtualKey = virtualKey;
    }

    void SetType(string type)
    {
        this.Type = type;
    }
    
    void SetFuel(float fuel)
    {
        this.Fuel = fuel;
    }

    void SetOwner(string owner)
    {
        this.Owner = owner;
    }

    void SetOwnerCarID(string ownerCarID)
    {
        this.OwnerCarID = ownerCarID;
    }

    void SetPassword(int password)
    {
        this.Password = password;
    }

    void SetInsurance(int insurance)
    {
        this.Insurance = insurance;
    }

    void SetInsuranceId(int insuranceId)
    {
        this.InsuranceId = insuranceId;
    }

    void SetPosition(vector position)
    {
        this.Position = position;
    }

    void SetAttachments(EntityAI entity)
    {
        array<EntityAI> attachments = new array<EntityAI>;
        entity.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, attachments);
        foreach (EntityAI attachment: attachments)
        {
            if (attachment)
            {
                this.Attachments.Insert(new Fabo_VirtualObject(attachment));
            }
        }
    }

    void SetOnlyAttachments(EntityAI entity)
    {
        array<EntityAI> attachments = GetOnlyVehicleAttachments(entity);
        foreach (EntityAI attachment: attachments)
        {
            if (attachment)
            {
                this.Attachments.Insert(new Fabo_VirtualObject(attachment, 1));
            }
        }
    }

    private array<EntityAI> GetOnlyVehicleAttachments(EntityAI entity)
	{
		array<EntityAI> ret = new array<EntityAI>;

		for (int i = 0; i < entity.GetInventory().AttachmentCount(); ++i)
		{
            EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex(i);

		    bool insert = false;

            if (CarWheel.Cast(attachment))
                insert = true;

            if (CarDoor.Cast(attachment))
                insert = true;

            if (CarRadiator.Cast(attachment))
                insert = true;

            if (SparkPlug.Cast(attachment))
                insert = true;

            if (GlowPlug.Cast(attachment))
                insert = true;

            if (HeadlightH7.Cast(attachment))
                insert = true;

            if (VehicleBattery.Cast(attachment))
                insert = true;

            if (insert)
			    ret.Insert(attachment);
		}

		return ret;
	}

    void Store()
    {
        JsonFileLoader<Fabo_VirtualVehicle>.JsonSaveFile(GetPathFile() + OwnerID + "\\" + this.GetUniqueID() + ".json", this);
    }

    void Load()
    {
        JsonFileLoader<Fabo_VirtualVehicle>.JsonLoadFile(GetPathFile() + OwnerID + "\\" + this.GetUniqueID() + ".json", this);
    }

    void Delete()
    {
        DeleteFile(GetPathFile() + OwnerID + "\\" + this.GetUniqueID() + ".json");
    }

    private string GetPathFile()
    {
        string PathFile = Path;

        if (Insurance == 1)
            PathFile = PathInsurance;

        return PathFile;
    }
}

class Fabo_VirtualVehicleKey
{
    string type;
    int vehicleId;
    int keyId;
    string ownerName;
    string ownerID;
}