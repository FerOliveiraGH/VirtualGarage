class AttachDetachWithToolActionCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( UATimeSpent.DEFAULT );
	}
}

class AttachDetachWithToolAction: ActionContinuousBase
{
    void AttachDetachWithToolAction()
	{
	    m_CallbackClass = AttachDetachWithToolActionCB;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;

		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_FullBody = true;
		m_Text = "#STR_Action_fabo_virtual_garage_enable";
	}

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.SMALL);
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!target)
            return false;

        Fabo_VirtualGarage virtualGarage = Fabo_VirtualGarage.Cast(target.GetObject());

        if (!virtualGarage)
            return false;

        if (virtualGarage.StaticClass == 1)
            return false;

		if (virtualGarage && virtualGarage.Deployed == 0)
		    m_Text = "#STR_Action_fabo_virtual_garage_enable";
        else
            m_Text = "#STR_Action_fabo_virtual_garage_disable";

        if (!ConfigVirtualGarage.GetInstance().AttachDetachWithTool())
            return false;

        return true;
    }

    override bool ActionConditionContinue(ActionData action_data)
    {
         return true;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (!action_data.m_Target)
            return;

        Fabo_VirtualGarage virtualGarage = Fabo_VirtualGarage.Cast(action_data.m_Target.GetObject());
        if (!virtualGarage)
            return;

        if(virtualGarage.Deployed == 1)
		    virtualGarage.Deployed = 0;
        else
            virtualGarage.Deployed = 1;

        virtualGarage.SetSynchDirty();
    }
}
