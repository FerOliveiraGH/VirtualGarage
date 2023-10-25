// class VirtualVehicle
// {
//     private const string Patch = "$profile:FaboMod\\database\\vehicles\\";
//     private int UniqueID;
//     private string Type;
//     private string Owner;
//     private int Password;
//     private ref array<ref VirtualVehicleAttachment> Attachments = new array<ref VirtualVehicleAttachment>();

//     void VirtualVehicle(int uniqueID)
//     {
//         this.UniqueID = uniqueID;

//         if (!FileExist(Patch))
//         {
//             MakeDirectory(Patch);
//         }
//     }

//     int GetUniqueID()
//     {
//         return this.UniqueID;
//     }

//     string GetType()
//     {
//         return this.Type;
//     }

//     string GetOwner()
//     {
//         return this.Owner;
//     }

//     int GetPassword()
//     {
//         return this.Password;
//     }

//     array<ref VirtualVehicleAttachment> GetAttachments()
//     {
//         return this.Attachments;
//     }

//     void SetType(string type)
//     {
//         this.Type = type;
//     }

//     void SetOwner(string owner)
//     {
//         this.Owner = owner;
//     }

//     void SetPassword(int password)
//     {
//         this.Password = password;
//     }

//     void SetAttachments(EntityAI entity)
//     {
//         array<EntityAI> attachments = new array<EntityAI>;
//         entity.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, attachments);
//         foreach (EntityAI attachment: attachments)
//         {
//             if (attachment)
//             {
//                 this.Attachments.Insert(new VirtualVehicleAttachment(attachment));
//             }
//         }
//     }

//     void Store()
//     {
//         JsonFileLoader<VirtualVehicle>.JsonSaveFile(Patch + this.GetUniqueID() + ".json", this);
//     }

//     void Load()
//     {
//         JsonFileLoader<VirtualVehicle>.JsonLoadFile(Patch + this.GetUniqueID() + ".json", this);
//     }
// }