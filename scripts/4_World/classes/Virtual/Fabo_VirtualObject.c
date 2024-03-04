class Fabo_VirtualObject
{
	string Type;
	string Parent;
	float Health;
	float Quantity;
	float Energy = 0.0;
	int AmmoCount = 0;
	int LiquidType = 0;
	int SlotType;
	int SlotIdx;
	int AttSlot;
	int AttRow;
	int AttCol;
	float AttFlip;
	ref array<ref Fabo_VirtualCartridge> Cartridges = new array<ref Fabo_VirtualCartridge>;
	ref array<ref Fabo_VirtualObject> Children = new array<ref Fabo_VirtualObject>;

	void Fabo_VirtualObject(EntityAI attachment, int onlyAttachments = 0)
	{
	    Type = attachment.GetType();
        Health = attachment.GetHealth();
        Quantity = attachment.GetQuantity();

        Magazine mag = Magazine.Cast(attachment);
        if (mag)
            AmmoCount = mag.GetAmmoCount();

        Weapon weapon = Weapon.Cast(attachment);
        if (weapon)
           SetWeaponCartridges(weapon);

        ItemBase itemBase = ItemBase.Cast(attachment);
        if (itemBase)
            LiquidType = itemBase.GetLiquidType();

        ComponentEnergyManager em = itemBase.GetCompEM();
        if (em)
            Energy = em.GetEnergy();

        InventoryLocation loc = new InventoryLocation;
        attachment.GetInventory().GetCurrentInventoryLocation( loc );
        SlotType = loc.GetType();
        SlotIdx = loc.GetIdx();
        AttSlot = loc.GetSlot();
        AttRow = loc.GetRow();
        AttCol = loc.GetCol();
        AttFlip = loc.GetFlip();

        EntityAI parent = loc.GetParent();
        Parent = parent.GetType();

        if (onlyAttachments == 1)
            return;

        array<EntityAI> children = new array<EntityAI>;
        attachment.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, children);

        foreach (EntityAI child: children)
        {
            if (child)
            {
                Children.Insert(new Fabo_VirtualObject(child));
            }
        }
	}

	void SetWeaponCartridges(Weapon weapon)
	{
	    for (int i = 0; i < weapon.GetMuzzleCount(); i++)
        {
            float ammoDamage;
            string AmmoTypeName;

            if (weapon.HasInternalMagazine(i) && weapon.GetInternalMagazineCartridgeCount(i) > 1)
            {
                weapon.GetInternalMagazineCartridgeInfo(i, 1, ammoDamage, AmmoTypeName);

                if (!weapon.IsChamberEmpty(i))
                    Cartridges.Insert(Fabo_VirtualCartridge(i, ammoDamage, AmmoTypeName));

                Cartridges.Insert(Fabo_VirtualCartridge(i, ammoDamage, AmmoTypeName, 1, weapon.GetInternalMagazineCartridgeCount(i)));
            }
            else
            {
                if (weapon.IsChamberEmpty(i))
                    continue;

                weapon.GetCartridgeInfo(i, ammoDamage, AmmoTypeName);

                Cartridges.Insert(Fabo_VirtualCartridge(i, ammoDamage, AmmoTypeName));
            }
        }
	}

	string GetType()
    {
        return this.Type;
    }

    string GetParent()
    {
        return this.Parent;
    }

    float GetHealth()
    {
        return this.Health;
    }

    float GetQuantity()
    {
        return this.Quantity;
    }

    float GetEnergy()
    {
        return this.Energy;
    }

    float GetAmmoCount()
    {
        return this.AmmoCount;
    }

    int GetLiquidType()
    {
        return this.LiquidType;
    }

    int GetSlotType()
    {
        return this.SlotType;
    }

    int GetSlotIdx()
    {
        return this.SlotIdx;
    }

    int GetSlot()
    {
        return this.AttSlot;
    }

    int GetRow()
    {
        return this.AttRow;
    }

    int GetCol()
    {
        return this.AttCol;
    }

    int GetFlip()
    {
        return this.AttFlip;
    }

    array<ref Fabo_VirtualObject> GetChildren()
    {
        return this.Children;
    }
}