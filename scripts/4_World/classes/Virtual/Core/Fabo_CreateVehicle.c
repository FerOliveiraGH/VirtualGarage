class Fabo_CreateVehicle
{
    void Fabo_CreateVehicle(vector position, vector orientation, int userId, int carId)
    {
        Fabo_VirtualVehicle virtualVehicle = new Fabo_VirtualVehicle( userId, carId );
        virtualVehicle.Load();

        Car obj = Car.Cast(GetGame().CreateObjectEx(virtualVehicle.GetType(), position, ECE_PLACE_ON_SURFACE));
        obj.SetOrientation(orientation);

        CarScript vehicle = CarScript.Cast(obj);

        CreateAttachments(vehicle, virtualVehicle.GetAttachments());

        obj.Fill(CarFluid.FUEL, vehicle.GetFluidCapacity(CarFluid.FUEL) * virtualVehicle.GetFuel());
        obj.Fill(CarFluid.OIL, vehicle.GetFluidCapacity(CarFluid.OIL));
        obj.Fill(CarFluid.COOLANT, vehicle.GetFluidCapacity(CarFluid.COOLANT));
        obj.Fill(CarFluid.BRAKE, vehicle.GetFluidCapacity(CarFluid.BRAKE));

        SetCarLock(vehicle, virtualVehicle);

//        virtualVehicle.Delete();
    }

    void CreateAttachments(EntityAI obj, array<ref Fabo_VirtualVehicleAttachment> attachments)
    {
        foreach(Fabo_VirtualVehicleAttachment line: attachments)
        {
            EntityAI att;

            if (line.GetSlot() == -1) {
                InventoryLocation loc = new InventoryLocation;
                obj.GetInventory().GetCurrentInventoryLocation( loc );

                att = obj.GetInventory().CreateEntityInCargoEx(line.GetType(), loc.GetIdx(), line.GetRow(), line.GetCol(), line.GetFlip());
            } else {
                att = obj.GetInventory().CreateAttachmentEx(line.GetType(), line.GetSlot());
            }

            att.SetHealth("", "", line.GetHealth());

            ItemBase itemBase = ItemBase.Cast(att);
            if (itemBase) {
                if (itemBase.HasQuantity())
                    itemBase.SetQuantity(line.GetQuantity());

                itemBase.SetLiquidType(line.GetLiquidType());

                ComponentEnergyManager em = itemBase.GetCompEM();
                if (em)
                    em.SetEnergy(line.GetEnergy());
            }

            Magazine mag = Magazine.Cast(att);
            if (mag) {
                mag.ServerSetAmmoCount(line.GetAmmoCount());
                mag.LocalSetAmmoCount(line.GetAmmoCount());
            }

            if (line.GetChildren().Count() > 0)
                CreateAttachments(att, line.GetChildren());

            Weapon weapon = Weapon.Cast(att);
            if (weapon)
                CreateWeaponCartridges(weapon, line);
        }
    }

    void CreateWeaponCartridges(Weapon weapon, Fabo_VirtualVehicleAttachment line)
    {
        array<ref Fabo_VirtualCartridge> cartridges = line.Cartridges;
        for (int i = 0; i < cartridges.Count(); i++)
        {
            if (cartridges[i].Internal == 1)
            {
                for (int x = 0; x < cartridges[i].AmmoCount; x++)
                    weapon.PushCartridgeToInternalMagazine(cartridges[i].IndexMuzzle, cartridges[i].Damage, cartridges[i].TypeName);
            }
            else
            {
                weapon.PushCartridgeToChamber(cartridges[i].IndexMuzzle, cartridges[i].Damage, cartridges[i].TypeName);
            }
        }

        weapon.Synchronize();
    }

    void SetCarLock(CarScript vehicle, Fabo_VirtualVehicle virtualVehicle)
    {
        #ifdef TraderPlus
        if (virtualVehicle.GetPassword() > 0)
        {
            vehicle.SetCarLockOwner(GetLowSteamUID(virtualVehicle.GetOwner()));
            vehicle.SetCarLock(true);
            vehicle.SetCarLockPassword(virtualVehicle.GetPassword());
        }
        vehicle.SetSynchDirty();
        #endif
    }

    int GetLowSteamUID(string playerUID)
    {
        string temp_ID="";
        for(int j = 8; j<17; j++)
        {
            temp_ID+=playerUID.Get(j);
        }
        return temp_ID.ToInt();
    }
}