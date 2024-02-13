modded class CarScript extends Car
{
    private int FaboInsuranceCarID = 0;
    private int FaboStoreVehicle = 0;

    void CarScript()
    {
        RegisterNetSyncVariableInt("FaboInsuranceCarID");
        RegisterNetSyncVariableInt("FaboStoreVehicle");
    }

    int GetInsuranceId()
    {
        return FaboInsuranceCarID;
    }

    bool HasInsuranceId()
    {
        string insuranceId = FaboInsuranceCarID.ToString();
        return FaboInsuranceCarID > 0 && insuranceId.Length() > 1;
    }

    void SetInsuranceId(int insuranceId)
    {
        FaboInsuranceCarID = insuranceId;

        SetSynchDirty();
    }

    void SetStoreVehicle(int storeVehicle)
    {
        FaboStoreVehicle = storeVehicle;

        SetSynchDirty();
    }

    void ~CarScript()
	{
	    RescueVehicleInsurance();

		#ifndef SERVER
		CleanupEffects();
		#endif
	}

	void RescueVehicleInsurance()
	{
	    Print("VirtualGarage :: Vehicle Deleted");

        if (GetGame().IsClient() || !HasInsuranceId() || FaboStoreVehicle == 1)
            return;

        if (GetHealthLevel("") != GameConstants.STATE_RUINED)
            return;

        Fabo_VehicleInsurance vehicleInsurance = new Fabo_VehicleInsurance(FaboInsuranceCarID);
        vehicleInsurance.Load();

        Print("VirtualGarage :: Vehicle Has Insurance? " + vehicleInsurance.HasInsuranceId());

        if (!vehicleInsurance.HasInsuranceId() || vehicleInsurance.CurrentInsuranceExpired())
            return;

        Print("VirtualGarage :: Vehicle Rescued: " + GetType() + " - Insurance ID: " + FaboInsuranceCarID);

        Fabo_VirtualVehicle virtualVehicle = new Fabo_VirtualVehicle(vehicleInsurance.GetUserId(), FaboInsuranceCarID, 1);
        virtualVehicle.Load();
        virtualVehicle.SetInsurance(0);
        virtualVehicle.Store();

        Fabo_ListGarage fabo_ListGarage = new Fabo_ListGarage(vehicleInsurance.GetUserId());
        fabo_ListGarage.addVehicleId(FaboInsuranceCarID);
        fabo_ListGarage.Store();
	}

	override void OnStoreSave( ParamsWriteContext ctx )
    {
        super.OnStoreSave(ctx);

        ctx.Write(FaboInsuranceCarID);
        ctx.Write(FaboStoreVehicle);
    }

    override bool OnStoreLoad( ParamsReadContext ctx, int version )
    {
        if ( !super.OnStoreLoad( ctx, version ) )
            return false;

        ctx.Read(FaboInsuranceCarID);
        ctx.Read(FaboStoreVehicle);

        return true;
    }

    override void SetActions()
    {
        super.SetActions();

        AddAction(Fabo_InsuranceCheckAction);
    }
}