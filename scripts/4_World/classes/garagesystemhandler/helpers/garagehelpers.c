//modded class GarageHelpers
//{
//	override static CarScript CheckForCarAtPos(vector position)
//	{
//		array<Object> objects = new array<Object>;
//		array<CargoBase> proxyCargos = new array<CargoBase>;
//		GetGame().GetObjectsAtPosition(position, 6, objects, proxyCargos);
//		CarScript car;
//		if (objects)
//		{
//			for (int i = 0; i < objects.Count(); i++ )
//			{
//				if (Class.CastTo( car, objects.Get(i) ) )
//						return car;
//			}
//		}
//		return NULL;
//	}
//
//	override static bool CreateVehicle(PlayerBase player, string classname, int cargoId, int collateralMoney, int id, float health, float fuel, vector position, vector orientation, array<ref ItemsCargo> items)
//	{
//		#ifdef GMDEBUG
//		GetGMLogger().LogInfo("CreateVehicle");
//		#endif
//		Object obj = GetGame().CreateObject(classname, position, false, false, true);
//		obj.SetOrientation(orientation);
//
//		EntityAI ent = EntityAI.Cast(obj);
//		if (ent)
//		{
//			//compatibility SIB Helis
//			if(ent.IsKindOf("HeliTest_SIB"))
//			{
//				for(int indexH = 0; indexH < 4; indexH++)
//				{
//					EntityAI fakeWheel = ent.GetInventory().CreateAttachment("HatchbackWheel");
//				}
//			}
//			for (int i = 0; i < items.Count(); i++)
//			{
//					if (items[i].ExtraData == GARAGEVEHICLEATTACHMENT)
//					{
//						#ifdef GMDEBUG
//						GetGMLogger().LogInfo("Create attach:"+items[i].ItemName);
//						#endif
//						EntityAI att = ent.GetInventory().CreateAttachment(items[i].ItemName);
//						if(!att)att = EntityAI.Cast( GetGame().CreateObjectEx(items[i].ItemName, vector.Zero, ECE_PLACE_ON_SURFACE) );
//						if(att)
//						{
//							ItemBase item = ItemBase.Cast(att);
//							if(item)
//							{
//								item = TraderPlusHelper.SetQuantity(item, items[i].Amount);
//								item.SetHealth(items[i].Health);
//							}
//						}
//					}
//			}
//
//			if(cargoId != 0)
//			{
//				TraderPlusVehicleCargo vehicleCargo;
//				if(GetGarageConfig().SaveVehicleCargo && TraderPlusVehicleCargo.GetMapAll().Find(cargoId, vehicleCargo))
//				{
//					ent = TraderPlusHelper.TransferInventoryVehicle(vehicleCargo, ent);
//					GetGame().ObjectDelete(vehicleCargo);
//				}
//				else
//				{
//					int playerId = GetLowSteamID(player.GetIdentity().GetPlainId());
//					GarageItemData garageItemData = GarageItemData.Load(playerId,cargoId);
//					if(garageItemData)
//					{
//						RestoreCargo(ent, garageItemData.childrens);
//						garageItemData.DeleteCargo(playerId, cargoId);
//					}
//				}
//			}
//
//			CarScript vehicle = CarScript.Cast(ent);
//			if(vehicle)
//			{
//				vehicle.DifferOrientation(orientation);
//				if(collateralMoney != 0)
//				{
//					int insuranceId = TraderPlusHelper.GetTimeStamp();
//					TraderPlusBankingData account = player.GetBankAccount();
//					if(account)
//					{
//						account.Insurances.Set(insuranceId, vehicle.GetType() + "," + collateralMoney.ToString());
//						vehicle.SetCarUniqueId(insuranceId);
//						account.UpdateAccount(player);
//					}
//				}
//
//				if(GetGarageConfig().SaveVehicleHealth){
//					#ifdef GMDEBUG
//					GetGMLogger().LogInfo("Set Health:"+health);
//					#endif
//					vehicle.SetHealth(health);
//				}
//
//				float fuelReq = vehicle.GetFluidCapacity(CarFluid.FUEL);
//				float oilReq = vehicle.GetFluidCapacity(CarFluid.OIL);
//				float coolantReq = vehicle.GetFluidCapacity(CarFluid.COOLANT);
//				float brakeReq = vehicle.GetFluidCapacity(CarFluid.BRAKE);
//
//				if(GetGarageConfig().SaveVehicleFuel){
//					#ifdef GMDEBUG
//					GetGMLogger().LogInfo("Set Fuel:"+fuel);
//					#endif
//					fuelReq = fuel;
//				}
//				vehicle.Fill(CarFluid.FUEL, fuelReq);
//				vehicle.Fill(CarFluid.OIL, oilReq);
//				vehicle.Fill(CarFluid.COOLANT, coolantReq);
//				vehicle.Fill(CarFluid.BRAKE, brakeReq);
//
//				//LOCK SCRIPT
//				if(GetGarageConfig().VehicleMustHaveLock || GetGarageConfig().SaveVehicleLock)
//				{
//					#ifdef GMDEBUG
//					GetGMLogger().LogInfo("Set Lock:"+id.ToString());
//					#endif
//					#ifndef CARLOCKDISABLE
//						if(id != -1 && id != 0){
//							vehicle.SetCarLockOwner(player.CLSteamlowID);
//							vehicle.SetCarLockPassword(id);
//							vehicle.SetCarLock(true);
//						}
//					#else
//						#ifdef MuchCarKey
//							if(id != 0)
//							{
//								vehicle.m_HasCKAssigned = true;
//								vehicle.m_CarKeyId = id;
//								vehicle.SynchronizeValues();
//							}
//						#else
//						 #ifdef TRADER
//							 if(id != 0){
//								 vehicle.m_Trader_HasKey = true;
//								 vehicle.m_Trader_VehicleKeyHash = id;
//								 vehicle.SynchronizeValues();
//							 }
//						 #endif
//						#endif
//					#endif
//				}
//
//                obj.Update();
//
//				return true;
//	        }
//		}
//
//		GetGame().ObjectDelete(obj);
//		#ifdef GMDEBUG
//		GetGMLogger().LogInfo("return false");
//		#endif
//        return false;
//	}
//}
