// class VirtualVehicleAttachment
// {
// 	string Type;
// 	string Parent;
// 	float Health;
// 	float Quantity;
// 	int AmmoCount = 0;
// 	int LiquidType = 0;
// 	int AttSlot;
// 	int AttRow;
// 	int AttCol;
// 	float AttFlip;
// 	ref array<ref VirtualCartridge> Cartridges = new array<ref VirtualCartridge>;
// 	ref array<ref VirtualVehicleAttachment> Children = new array<ref VirtualVehicleAttachment>;

// 	void VirtualVehicleAttachment(EntityAI attachment)
// 	{
// 	    Type = attachment.GetType();
//         Health = attachment.GetHealth();
//         Quantity = attachment.GetQuantity();

//         Magazine mag = Magazine.Cast(attachment);
//         if (mag)
//             AmmoCount = mag.GetAmmoCount();

//         Weapon weapon = Weapon.Cast(attachment);
//         if (weapon)
//            SetWeaponCartridges(weapon);

//         ItemBase itemBase = ItemBase.Cast(attachment);
//         if (itemBase) {
//             LiquidType = itemBase.GetLiquidType();
//         }

//         InventoryLocation loc = new InventoryLocation;
//         attachment.GetInventory().GetCurrentInventoryLocation( loc );
//         AttSlot = loc.GetSlot();
//         AttRow = loc.GetRow();
//         AttCol = loc.GetCol();
//         AttFlip = loc.GetFlip();

//         EntityAI parent = loc.GetParent();
//         Parent = parent.GetType();

//         array<EntityAI> children = new array<EntityAI>;
//         attachment.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, children);

//         foreach (EntityAI child: children)
//         {
//             if (child)
//             {
//                 Children.Insert(new VirtualVehicleAttachment(child));
//             }
//         }
// 	}

// 	void SetWeaponCartridges(Weapon weapon)
// 	{
// 	    for (int i = 0; i < weapon.GetMuzzleCount(); i++)
//         {
//             float ammoDamage;
//             string AmmoTypeName;

//             if (weapon.HasInternalMagazine(i) && weapon.GetInternalMagazineCartridgeCount(i) > 1)
//             {
//                 weapon.GetInternalMagazineCartridgeInfo(i, 1, ammoDamage, AmmoTypeName);

//                 if (!weapon.IsChamberEmpty(i))
//                     Cartridges.Insert(VirtualCartridge(i, ammoDamage, AmmoTypeName));

//                 Cartridges.Insert(VirtualCartridge(i, ammoDamage, AmmoTypeName, 1, weapon.GetInternalMagazineCartridgeCount(i)));
//             }
//             else
//             {
//                 if (weapon.IsChamberEmpty(i))
//                     continue;

//                 weapon.GetCartridgeInfo(i, ammoDamage, AmmoTypeName);

//                 Cartridges.Insert(VirtualCartridge(i, ammoDamage, AmmoTypeName));
//             }
//         }
// 	}

// 	string GetType()
//     {
//         return this.Type;
//     }

//     string GetParent()
//     {
//         return this.Parent;
//     }

//     float GetHealth()
//     {
//         return this.Health;
//     }

//     float GetQuantity()
//     {
//         return this.Quantity;
//     }

//     float GetAmmoCount()
//     {
//         return this.AmmoCount;
//     }

//     int GetLiquidType()
//     {
//         return this.LiquidType;
//     }

//     int GetSlot()
//     {
//         return this.AttSlot;
//     }

//     int GetRow()
//     {
//         return this.AttRow;
//     }

//     int GetCol()
//     {
//         return this.AttCol;
//     }

//     int GetFlip()
//     {
//         return this.AttFlip;
//     }

//     array<ref VirtualVehicleAttachment> GetChildren()
//     {
//         return this.Children;
//     }
// }