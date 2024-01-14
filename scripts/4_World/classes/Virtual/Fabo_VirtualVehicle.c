class Fabo_VirtualVehicle
{
    private const string Root = "$profile:VirtualGarage\\";
    private const string Database = "$profile:VirtualGarage\\database\\";
    private const string Path = "$profile:VirtualGarage\\database\\vehicles\\";
    private int UniqueID;
    private string Type;
    private int OwnerID;
    private string Owner;
    private float Fuel;
    private int Password;
    private ref array<ref Fabo_VirtualVehicleAttachment> Attachments = new array<ref Fabo_VirtualVehicleAttachment>();

    void Fabo_VirtualVehicle(int ownerID, int uniqueID)
    {
        this.OwnerID = ownerID;
        this.UniqueID = uniqueID;

        if (!FileExist(Root))
        {
            MakeDirectory(Root);
        }

        if (!FileExist(Database))
        {
            MakeDirectory(Database);
        }

        if (!FileExist(Path))
        {
            MakeDirectory(Path);
        }

        if (!FileExist(Path + ownerID + "\\"))
        {
            MakeDirectory(Path + ownerID + "\\");
        }
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

    string GetOwner()
    {
        return this.Owner;
    }

    int GetPassword()
    {
        return this.Password;
    }

    array<ref Fabo_VirtualVehicleAttachment> GetAttachments()
    {
        return this.Attachments;
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

    void SetPassword(int password)
    {
        this.Password = password;
    }

    void SetAttachments(EntityAI entity)
    {
        array<EntityAI> attachments = new array<EntityAI>;
        entity.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, attachments);
        foreach (EntityAI attachment: attachments)
        {
            if (attachment)
            {
                this.Attachments.Insert(new Fabo_VirtualVehicleAttachment(attachment));
            }
        }
    }

    void Store()
    {
        JsonFileLoader<Fabo_VirtualVehicle>.JsonSaveFile(Path + OwnerID + "\\" + this.GetUniqueID() + ".json", this);
    }

    void Load()
    {
        JsonFileLoader<Fabo_VirtualVehicle>.JsonLoadFile(Path + OwnerID + "\\" + this.GetUniqueID() + ".json", this);
    }

    void Delete()
    {
        DeleteFile(Path + OwnerID + "\\" + this.GetUniqueID() + ".json");
    }
}