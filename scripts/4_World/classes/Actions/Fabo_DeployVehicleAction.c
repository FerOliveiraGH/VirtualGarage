class Fabo_DeployVehicleActionCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( UATimeSpent.DEFAULT );
	}
}

class Fabo_DeployVehicleAction: ActionContinuousBase
{
    void Fabo_DeployVehicleAction()
	{
	    m_CallbackClass = Fabo_DeployVehicleActionCB;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;

		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_FullBody = true;
	}

	override void OnActionInfoUpdate( PlayerBase player, ActionTarget target, ItemBase item )
    {
        m_Text = "#STR_Action_DeployVehicle0";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTNone;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target.GetObject())
            return false;

        if (item.GetQuantity() > 0)
            return false;

        CarReceipt receipt = CarReceipt.Cast(player.GetItemInHands());
        if (receipt.GetInventory().IsInventoryUnlocked())
            return false;

        vector position = getObjectPosition(player);
        vector orientation = getObjectOrientation(player);

        ref array<ref AreaSpawnVehicleBlocked> areasSpawnVehicleBlocked = ConfigFaboMod.GetInstance().getAreasSpawnVehicleBlocked();

        foreach(ref AreaSpawnVehicleBlocked areaSpawnVehicleBlocked: areasSpawnVehicleBlocked)
        {
            vector vectorBlocked = areaSpawnVehicleBlocked.getLocation();

            float distance = vector.Distance(vectorBlocked, position);

            if (distance < areaSpawnVehicleBlocked.getRadius())
                return false;
        }

        if (IsColliding(position, orientation))
            return false;

        if (IsSurfaceWater(position))
            return false;

        return true;
    }

    override bool ActionConditionContinue(ActionData action_data)
    {
         return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (!action_data.m_MainItem || action_data.m_MainItem.GetHierarchyRootPlayer() != action_data.m_Player)
            return;

        PlayerBase player = PlayerBase.Cast(action_data.m_Player);
        CarReceipt receipt = CarReceipt.Cast(player.GetItemInHands());

        CreateVehicle(getObjectPosition(player), getObjectOrientation(player), receipt);
    }

    bool IsColliding(vector position, vector orientation)
    {
        return GetGame().IsBoxCollidingGeometry(position, orientation, Vector(2, 4, 2), ObjIntersect.View, ObjIntersect.Geom, new array<Object>);
    }

    bool IsSurfaceWater(vector position)
    {
        return GetGame().SurfaceIsSea(position[0], position[2]) || GetGame().SurfaceIsPond(position[0], position[2]);
    }

    vector getObjectPosition(PlayerBase player)
    {
        float meters = 5;

        vector objectPosition = player.GetPosition() + player.GetDirection() * meters;
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
}