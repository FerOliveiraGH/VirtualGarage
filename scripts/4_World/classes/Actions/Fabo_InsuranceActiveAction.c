class Fabo_InsuranceActiveActionCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( UATimeSpent.DEFAULT );
	}
};

class Fabo_InsuranceActiveAction: ActionContinuousBase
{
    void Fabo_InsuranceActiveAction()
    {
        m_CallbackClass = Fabo_InsuranceActiveActionCB;
        m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;

        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
        m_FullBody = true;
        m_Text = "#STR_Action_fabo_virtual_garage_enable";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTCursorNoRuinCheck(UAMaxDistances.DEFAULT);
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (GetGame().IsServer())
            return true;

        if (!target)
            return false;

        CarScript vehicle = CarScript.Cast(target.GetObject());
        if (!vehicle || vehicle.GetHealthLevel("") == GameConstants.STATE_RUINED)
            return false;

        #ifdef FaboMod
        if (vehicle.GetOwnerCar() != player.GetIdentity().GetId())
            return false;
        #endif

        #ifdef ClaimVehicles_Mod
        if (!vehicle.IsOwned() || !vehicle.IsOwner())
             return false;
        #endif

        #ifdef TraderPlus
        if (vehicle.m_CarLockPassword <= 0)
            return false;
        #endif

        m_Text = "#STR_Action_fabo_virtual_garage_enable";

        if (vehicle.HasInsuranceId())
            m_Text = "#STR_Action_fabo_virtual_garage_renew";

        return true;
    }

    override void OnFinishProgressServer( ActionData action_data )
    {
        if (!action_data.m_MainItem || action_data.m_MainItem.GetHierarchyRootPlayer() != action_data.m_Player)
            return;

        CarScript vehicle = CarScript.Cast(action_data.m_Target.GetObject());

        Fabo_InsurancePaper insurance = Fabo_InsurancePaper.Cast(action_data.m_Player.GetItemInHands());

        PlayerIdentity player = action_data.m_Player.GetIdentity();

        if (vehicle.HasInsuranceId())
        {
            Fabo_VehicleInsurance insuranceOld = new Fabo_VehicleInsurance(vehicle.GetInsuranceId());
            insuranceOld.Load();

            Fabo_VirtualVehicle(insuranceOld.GetUserId(), insuranceOld.GetInsuranceId(), 1).Delete();

            insuranceOld.Delete();
        }

        int uniqueId = CreateUniqueID();
        int userId = GetPlayerUniqueId(player.GetPlainId());

        vehicle.SetInsuranceId(uniqueId);

        Fabo_VehicleInsurance vehicleInsurance = new Fabo_VehicleInsurance(uniqueId);
        vehicleInsurance.SetUserId(userId);
        vehicleInsurance.Store();

        Fabo_VirtualVehicle virtualVehicle = new Fabo_VirtualVehicle(userId, uniqueId, 1);

        virtualVehicle.SetType(vehicle.GetType());
        virtualVehicle.SetFuel(vehicle.GetFluidFraction(CarFluid.FUEL));
        virtualVehicle.SetOwner(player.GetName());
        virtualVehicle.SetOwnerCarID(player.GetId());
        virtualVehicle.SetInsuranceId(uniqueId);

        #ifdef TraderPlus
        virtualVehicle.SetPassword(vehicle.m_CarLockPassword);
        #endif

        virtualVehicle.SetOnlyAttachments(vehicle);
        virtualVehicle.Store();

        insurance.Delete();
        insurance.SetSynchDirty();

        NotificationSystem.SendNotificationToPlayerIdentityExtended(player, 2, "#fabo_virtual_garage_title", "#fabo_insurance_activated", "VirtualGarage/data/images/vglogo.paa");
    }

    int CreateUniqueID()
    {
        return Math.RandomInt(1000000000, 9999999999);
    }

    int GetPlayerUniqueId(string FullIdSteam)
	{
		string uniqueId = "";

		for (int i = 8; i < 17; i++)
			uniqueId += FullIdSteam.Get(i);

		return uniqueId.ToInt();
	}
}