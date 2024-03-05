class Fabo_CreateVehicle
{
    CarScript Vehicle;

    void Fabo_CreateVehicle(vector position, vector orientation, Fabo_VirtualVehicle virtualVehicle)
    {
        Car obj = Car.Cast(GetGame().CreateObjectEx(virtualVehicle.GetType(), position, ECE_PLACE_ON_SURFACE));
        obj.SetOrientation(orientation);

        CarScript vehicle = CarScript.Cast(obj);

        if (!vehicle)
        {
            Print("VirtualGarage :: Create Vehicle Failed: " + virtualVehicle.GetType());
            return;
        }

        CreateAttachments(vehicle, virtualVehicle.GetAttachments());

        obj.Fill(CarFluid.FUEL, vehicle.GetFluidCapacity(CarFluid.FUEL) * virtualVehicle.GetFuel());
        obj.Fill(CarFluid.OIL, vehicle.GetFluidCapacity(CarFluid.OIL));
        obj.Fill(CarFluid.COOLANT, vehicle.GetFluidCapacity(CarFluid.COOLANT));
        obj.Fill(CarFluid.BRAKE, vehicle.GetFluidCapacity(CarFluid.BRAKE));

        SetCarLock(vehicle, virtualVehicle);
        SetOwnerVehicle(vehicle, virtualVehicle);
        vehicle.SetInsuranceId(virtualVehicle.GetInsuranceId());

        Vehicle = vehicle;

        virtualVehicle.Delete();
    }

    void CreateAttachments(EntityAI obj, array<ref Fabo_VirtualObject> attachments)
    {
        foreach(Fabo_VirtualObject line: attachments)
        {
            if (!line)
                continue;

            EntityAI att;

            if (line.GetSlotType() == InventoryLocationType.CARGO)
            {
                if (line.GetSlotIdx() == -1 || line.GetCol() == -1 || line.GetRow() == -1)
                    att = obj.GetInventory().CreateInInventory(line.GetType());
                else
                    att = obj.GetInventory().CreateEntityInCargoEx(line.GetType(), line.GetSlotIdx(), line.GetRow(), line.GetCol(), line.GetFlip());
            }
            else if (line.GetSlotType() == InventoryLocationType.ATTACHMENT)
            {
                att = obj.GetInventory().CreateAttachmentEx(line.GetType(), line.GetSlot());
            }

            if (!att)
                att = obj.GetInventory().CreateInInventory(line.GetType());

            if (!att)
            {
                Print("VirtualGarage :: Create Object Failed: " + line.GetType());
                return;
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

    void CreateWeaponCartridges(Weapon weapon, Fabo_VirtualObject line)
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

        Weapon_Base weaponBase = Weapon_Base.Cast(weapon);
        weaponBase.RandomizeFSMState();
        weaponBase.Synchronize();
    }

    void SetCarLock(CarScript vehicle, Fabo_VirtualVehicle virtualVehicle)
    {
        #ifdef TraderPlus
        if (virtualVehicle.GetPassword() > 0)
        {
            vehicle.SetCarLockOwner(virtualVehicle.GetOwnerID());
            vehicle.SetCarLock(true);
            vehicle.SetCarLockPassword(virtualVehicle.GetPassword());
        }
        vehicle.SetSynchDirty();
        #endif
    }

    void SetOwnerVehicle(CarScript vehicle, Fabo_VirtualVehicle virtualVehicle)
    {
        #ifdef FaboMod
        if (virtualVehicle.GetOwnerCarID())
        {
            vehicle.SetOwnerCarServer(virtualVehicle.GetOwnerCarID(), virtualVehicle.GetOwner());
            vehicle.SetSynchDirty();
        }
        #endif
    }
}