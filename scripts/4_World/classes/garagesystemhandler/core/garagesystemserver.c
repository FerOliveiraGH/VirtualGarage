modded class GarageServer
{
    override void ParkInRequestHandler(PlayerBase player, int id, CarScript car, vector pos)
    {
        CarScript CarToRemove;
        if(!car)
            CarToRemove = GarageHelpers.CheckForCarAtPos(pos);
        else
            CarToRemove = car;

        if(!car)
        {
            return;
        }

        if(GetGarageConfig().VehicleMustHaveLock)
        {
        #ifndef CARLOCKDISABLE
            if(car.m_CarLockOwner == -1)
            {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().NotRightToPark, "Garage/image/CarLogo.paa");
            return;
            }
        #else
            #ifdef MuchCarKey
            if(!car.m_HasCKAssigned)
            {
                NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().NotRightToPark, "Garage/image/CarLogo.paa");
                return;
            }
            #else
            #ifdef TRADER
            if(!car.m_Trader_HasKey)
            {
                NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().NotRightToPark, "Garage/image/CarLogo.paa");
                return;
            }
            #endif
            #endif
        #endif
        }

        #ifdef CARLOCKDISABLE
        if( (!player.GetAdminStatus() == SZ_IS_ADMIN && car.m_TraderPlus_LastDriverID != player.GetIdentity().GetPlainId()) || car.m_TraderPlus_LastDriverID == string.Empty)
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().NotRightToPark, "Garage/image/CarLogo.paa");
            return;
        }
        #endif

        if(GarageHelpers.CarHasMember(car))
        {
        NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().CarHasMember, "Garage/image/CarLogo.paa");
        //car has member
        return;
        }

        bool state = GarageHelpers.RemoveMoneyFromPlayer(player, GetGarageConfig().ParkInCost);
        if(!state)
        {
        return;
        }

        string VehicleName = CarToRemove.GetType();
        int    vehicleData = -1;
        float  VehicleHealth= CarToRemove.GetHealth();
        float  VehicleFuelAmount= CarToRemove.GarageGetFuelAmmount();

        if(GetGarageConfig().VehicleMustHaveLock || GetGarageConfig().SaveVehicleLock)
        {
        #ifndef CARLOCKDISABLE
            if(CarToRemove.m_CarLockPassword != -1)
            {
            vehicleData = CarToRemove.m_CarLockPassword;
            }
        #else
            #ifdef MuchCarKey
            if(CarToRemove.m_HasCKAssigned)
            {
                vehicleData = CarToRemove.m_CarKeyId;
            }
            #else
            #ifdef TRADER
                vehicleData = CarToRemove.m_Trader_VehicleKeyHash;
            #endif
            #endif
        #endif
        }

        GarageItemData cargo = GarageHelpers.GetGarageItemData(car);

        array<EntityAI> VehiclesAttachments = new array<EntityAI>;
        array<ref ItemsCargo> itemsArr = new array<ref ItemsCargo>;
        //VehiclesAttachments = GarageHelpers.GetAttachmentsFromEntityAI(CarToRemove);
        //itemsArr = GarageHelpers.ConvertEntitiesToItemsCargo(VehiclesAttachments, GARAGEVEHICLEATTACHMENT);

        int cargoId = 0;
        int timeStamp = 0;

        cargoId = TraderPlusHelper.GetTimeStamp();
        cargo.Save(id, cargoId);
        /*TraderPlusVehicleCargo vehicleCargo = TraderPlusVehicleCargo.Cast(GetGame().CreateObject("TraderPlusVehicleCargo", CarToRemove.GetPosition()));
        vehicleCargo = TraderPlusVehicleCargo.Cast(TraderPlusHelper.TransferInventoryVehicle(CarToRemove, vehicleCargo));
        vehicleCargo.SetCargoId(cargoId);
        vehicleCargo.SetPosition("0 0 0");*/

        if(GetGarageConfig().UseGarageOnlyToTrade)
        timeStamp = TraderPlusHelper.GetTimeStamp();

        int collateralMoney = 0;
        string TypeAndcollateralMoney = "";
        TraderPlusBankingData account = player.GetBankAccount();
        if(CarToRemove.GetCarUniqueId() != 0 && account.Insurances.Find(CarToRemove.GetCarUniqueId(), TypeAndcollateralMoney))
        {
            collateralMoney = TraderPlusHelper.GetCollateralPriceInsuranceData(TypeAndcollateralMoney);
            account.Insurances.Remove(CarToRemove.GetCarUniqueId());
            account.UpdateAccount(player);
        }

        GarageData garageData = GarageData.Load(id.ToString(), player.GetIdentity().GetName());
        if(garageData)
        {
        garageData.AddVehicle(VehicleName, cargoId, collateralMoney, vehicleData, VehicleHealth, VehicleFuelAmount, itemsArr, timeStamp);
        garageData.Save(id.ToString());

        if(GetGarageConfig().UseGarageOnlyToTrade){
            NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", string.Format(GetGarageConfig().TradeVehicleWarning, GetGarageConfig().SavedVehicleInGarageForTradeInHour), "Garage/image/CarLogo.paa");
        }
        else{
            NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().ParkInSuccess, "Garage/image/CarLogo.paa");
        }
        }
        else
        {
        NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().ParkInFail, "Garage/image/CarLogo.paa");
        }

        GetGame().ObjectDelete(CarToRemove);

        TStringArray vehiclesName = new TStringArray;
        TIntArray vehiclesHealth = new TIntArray;

        foreach(VehiculeData vehicle: garageData.Vehicles)
        {
        vehiclesName.Insert(vehicle.VehicleName);
        vehiclesHealth.Insert(GarageHelpers.ConvertHealthToHealthLevel(vehicle.VehicleName, vehicle.EngineHealth));
        }

        GetRPCManager().SendRPC("Garage", "GarageResponse",  new Param6<ref TStringArray, ref TIntArray, vector, int, vector, vector>(vehiclesName, vehiclesHealth, pos, TraderPlusResponse.BUY_SUCCESS, "0 0 0", "0 0 0"),true, player.GetIdentity());
    }

    void ParkOutRequestHandler(PlayerBase player, int id, int listpos,string carname, vector pos, vector dir, vector ori)
    {
        GarageData garageData = GarageData.Load(id.ToString(), player.GetIdentity().GetName());
        if(garageData)
        {
            if (dir != Vector(0, 0, 0))
            {
                pos = getObjectPosition(pos, dir);
            }
            else
            {
                // return;
                if(pos == garageData.ParkingPosition)
                {
                    ori = garageData.ParkingOrientation;
                }
                else
                {
                    for(int i=0;i<ObjectNPCs.Count();i++)
                    {
                        float distance = vector.Distance(GetGarageConfig().NPCs[i].ParkingPosition,pos);
                        if(distance < 3)
                        {
                            pos = GetGarageConfig().NPCs[i].ParkingPosition;
                            ori = GetGarageConfig().NPCs[i].ParkingOrientation;
                        }
                    }
                }
            }
            if(!GarageHelpers.IsParkingAvailable(pos,ori))
            {
                NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().ParkingNotAvailable, "Garage/image/CarLogo.paa");
                //park not available
                return;
            }

            if(garageData.Vehicles.Count() > listpos)
            {
                if(garageData.Vehicles[listpos].VehicleName == carname)
                {
                    bool state = GarageHelpers.CreateVehicle(player, carname, garageData.Vehicles[listpos].VehicleCargoId, garageData.Vehicles[listpos].CollateralMoney, garageData.Vehicles[listpos].VehicleID, garageData.Vehicles[listpos].EngineHealth, garageData.Vehicles[listpos].FuelAmmount, pos, ori, garageData.Vehicles[listpos].ItemsInCargo);
                    if(state)
                    {
                        state = GarageHelpers.RemoveMoneyFromPlayer(player, GetGarageConfig().ParkOutCost);
                        NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().ParkOutSuccess, "Garage/image/CarLogo.paa");
                        garageData.Vehicles.RemoveOrdered(listpos);
                        garageData.Save(id.ToString());
                    }
                }
            }

            if(!state)
            {
                for(int j=0;j<garageData.Vehicles.Count();j++)
                {
                    if(garageData.Vehicles[j].VehicleName == carname)
                    {
                        int collateralMoney = garageData.Vehicles[j].CollateralMoney;
                        int vehicleCargoId = garageData.Vehicles[j].VehicleCargoId;
                        state = GarageHelpers.CreateVehicle(player, carname,vehicleCargoId, collateralMoney, garageData.Vehicles[j].VehicleID, garageData.Vehicles[j].EngineHealth, garageData.Vehicles[j].FuelAmmount, pos, ori,garageData.Vehicles[j].ItemsInCargo);
                        if(state)
                        {
                            state = GarageHelpers.RemoveMoneyFromPlayer(player, GetGarageConfig().ParkOutCost);
                            NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().ParkOutSuccess, "Garage/image/CarLogo.paa");
                            garageData.Vehicles.RemoveOrdered(j);
                            garageData.Save(id.ToString());
                        }
                        else
                        {
                            NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().ParkOutFail, "Garage/image/CarLogo.paa");
                        }
                    }
                }
            }

            TStringArray vehiclesName = new TStringArray;
            TIntArray vehiclesHealth = new TIntArray;

            foreach(VehiculeData vehicle: garageData.Vehicles)
            {
                vehiclesName.Insert(vehicle.VehicleName);
                vehiclesHealth.Insert(GarageHelpers.ConvertHealthToHealthLevel(vehicle.VehicleName, vehicle.EngineHealth));
            }

            GetRPCManager().SendRPC("Garage", "GarageResponse",  new Param6<ref TStringArray, ref TIntArray, vector, int, vector, vector>(vehiclesName, vehiclesHealth, pos, TraderPlusResponse.BUY_SUCCESS, dir, ori),true, player.GetIdentity());
        }
    }

    vector getObjectPosition(vector pos, vector dir)
    {
        float meters = -4;

        vector objectPosition = pos + dir * meters;
        objectPosition[1] = GetGame().SurfaceY(objectPosition[0],objectPosition[2]);

        return objectPosition;
    }

    vector getObjectOrientation(PlayerBase player)
    {
        float graus = -90;

        vector objectOrientation = player.GetOrientation();
        objectOrientation[0] = objectOrientation[0] + graus;

        return objectOrientation;
    }

    void GarageRequestHandler(PlayerBase player, int id, vector pos,bool isFlag, vector dir, vector ori)
    {
        int start = TickCount(0);
        GarageData garageData = GarageData.Load(id.ToString(), player.GetIdentity().GetName());
        int time = TickCount(start);
        if(!garageData)
            return;

        TStringArray vehiclesName = new TStringArray;
        TIntArray vehiclesHealth = new TIntArray;

        if(GetGarageConfig().UseGarageOnlyToTrade)
        {
            for(int k = garageData.Vehicles.Count() - 1; k >= 0; k--)
            {
            if( (garageData.Vehicles[k].Timestamp + GetGarageConfig().SavedVehicleInGarageForTradeInHour*3600)  < TraderPlusHelper.GetTimeStamp() )
            {
                int playerId = GarageHelpers.GetLowSteamID(player.GetIdentity().GetPlainId());
                garageData.Vehicles[k].DeleteCargoIfExist(playerId);
                garageData.Vehicles.Remove(k);
                garageData.Save(playerId.ToString());
                NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 2, "Garage", GetGarageConfig().TradeVehicleHasBeenDeleted , "Garage/image/CarLogo.paa");
            }
            }
        }

        foreach(VehiculeData vehicle: garageData.Vehicles)
        {
            vehiclesName.Insert(vehicle.VehicleName);
            vehiclesHealth.Insert(GarageHelpers.ConvertHealthToHealthLevel(vehicle.VehicleName, vehicle.EngineHealth));
        }

        vector parkingpos;
        if(isFlag)
        {
            parkingpos = pos;
        else
        {
            for(int i=0;i<ObjectNPCs.Count();i++)
            {
                Object npc = ObjectNPCs[i];
                float distance = vector.Distance(npc.GetPosition(),pos);
                if(distance < 1)
                {
                    parkingpos = GetGarageConfig().NPCs[i].ParkingPosition;
                }
            }
        }

        GetRPCManager().SendRPC("Garage", "GarageResponse",  new Param6<ref TStringArray, ref TIntArray, vector, int, vector, vector>(vehiclesName, vehiclesHealth, parkingpos, TraderPlusResponse.NO_TRADE, dir, ori), true, player.GetIdentity());
    }

    void ParkInRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (!GetGame().IsServer())
            return;

        Param3<int, CarScript, vector> data;

        if (!ctx.Read(data))
            return;

        PlayerBase player = GMGetPlayerByIdentity(sender);

        if(player)
            ParkInRequestHandler(player, data.param1, data.param2, data.param3);
    }

    void ParkOutRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (!GetGame().IsServer())
            return;


        Param6<int, int, string, vector, vector, vector> data;

        if (!ctx.Read(data))
            return;

        PlayerBase player = GMGetPlayerByIdentity(sender);

        if(player)
            ParkOutRequestHandler(player, data.param1, data.param2, data.param3, data.param4, data.param5, data.param6);
    }

    void GarageRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (!GetGame().IsServer())
            return;

        Param5<int, vector, bool, vector, vector> data;

        if (!ctx.Read(data))
            return;

        PlayerBase player = GMGetPlayerByIdentity(sender);

        if(player)
            GarageRequestHandler(player, data.param1, data.param2, data.param3, data.param4, data.param5);
    }
};
