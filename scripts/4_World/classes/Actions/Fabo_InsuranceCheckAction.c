class Fabo_InsuranceCheckAction: ActionInteractBase
{
    void Fabo_InsuranceCheckAction()
    {
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
    }

    override string GetText()
	{
		return "#fabo_insurance_check";
	}

    override void CreateConditionComponents()  
	{
		m_ConditionItem 	= new CCINone;
		m_ConditionTarget 	= new CCTCursor;
	}
    
    override bool HasProgress()
	{
		return false;
	}

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (GetGame().IsServer())
            return true;

		CarScript vehicle = CarScript.Cast(target.GetObject());

		if (!vehicle || !vehicle.HasInsuranceId())
			return false;

        #ifdef FaboMod
        if (vehicle.GetOwnerCar() != player.GetIdentity().GetId())
            return false;
        #endif

        #ifdef ClaimVehicles_Mod
        if (!vehicle.IsOwned() || !vehicle.IsOwner())
             return false;
        #endif

        return true;
    }

    override void OnEnd( ActionData action_data )
	{
	    if (GetGame().IsClient())
	        return;

	    CarScript vehicle = CarScript.Cast(action_data.m_Target.GetObject());

	    if (!vehicle)
	        return;

        Fabo_VehicleInsurance insurance = new Fabo_VehicleInsurance(vehicle.GetInsuranceId());
        insurance.Load();

        if (!insurance.HasInsuranceId())
            return;

        PlayerIdentity player = action_data.m_Player.GetIdentity();
        string textMessage = "#fabo_insurance_check_date: " + insurance.GetExpiration() + " - #fabo_insurance_check_status: ";

        if (insurance.CurrentInsuranceExpired())
            textMessage = textMessage + "#fabo_insurance_check_expired";
        else
            textMessage = textMessage + "#fabo_insurance_check_activate";

        NotificationSystem.SendNotificationToPlayerIdentityExtended(player, 2, "#fabo_virtual_garage_title", textMessage, "VirtualGarage/data/images/vglogo.paa");
	}
}