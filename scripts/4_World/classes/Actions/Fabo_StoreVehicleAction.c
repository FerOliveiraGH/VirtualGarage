class Fabo_StoreVehicleActionCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( UATimeSpent.DEFAULT );
	}
};

class Fabo_StoreVehicleAction: ActionContinuousBase
{
    void Fabo_StoreVehicleAction()
    {
        m_CallbackClass = Fabo_StoreVehicleActionCB;
        m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;

        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
        m_FullBody = true;
    }

    override void OnActionInfoUpdate( PlayerBase player, ActionTarget target, ItemBase item )
    {
        m_Text = "#STR_Action_StoreVehicle0";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTCursorNoRuinCheck(UAMaxDistances.SMALL);
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!target)
            return false;

        CarScript CarAction = CarScript.Cast(target.GetObject());
        if (!CarAction)
            return false;

        #ifdef ClaimVehicles
        if (!CarAction.IsOwned())
            return false;
        #endif

        #ifdef TraderPlus
        if (CarAction.m_CarLockPassword <= 0)
            return false;
        #endif

        CarReceipt receipt = CarReceipt.Cast(player.GetItemInHands());

        if (receipt.GetInventory().IsInventoryLocked())
            return false;


        if (!ConfigFaboMod.GetInstance().StoreVehicleWithCargo())
        {
            CargoBase cargo = CarAction.GetInventory().GetCargo();
            if (cargo && cargo.GetItemCount() > 0)
                return false;
        }

        if (CarAction.GetOwnerCar() != player.GetIdentity().GetId())
            return false;

        return true;
    }

    override void OnFinishProgressServer( ActionData action_data )
    {
        if (!action_data.m_MainItem || action_data.m_MainItem.GetHierarchyRootPlayer() != action_data.m_Player)
            return;

        PlayerIdentity identity = action_data.m_Player.GetIdentity();
        CarScript vehicle = CarScript.Cast(action_data.m_Target.GetObject());

        VirtualVehicle virtualVehicle = new VirtualVehicle(CreateUniqueID());

        virtualVehicle.SetType(vehicle.GetType());
        virtualVehicle.SetFuel(vehicle.GetFluidFraction(CarFluid.FUEL));
        virtualVehicle.SetOwner(identity.GetId());
        #ifdef TraderPlus
        virtualVehicle.SetPassword(vehicle.m_CarLockPassword);
        #endif
        virtualVehicle.SetAttachments(vehicle);

        virtualVehicle.Store();

        CarReceipt receipt = CarReceipt.Cast(action_data.m_Player.GetItemInHands());
        receipt.m_CarID = virtualVehicle.GetUniqueID();

        receipt.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
        receipt.SetSynchDirty();

        vehicle.Delete();
    }

    int CreateUniqueID()
    {
        return Math.RandomInt(1000000000, 9999999999);
    }
}