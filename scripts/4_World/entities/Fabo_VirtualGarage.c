class Fabo_VirtualGarage: ItemBase
{
    override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

    override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}
    
    override void SetActions()
    {
        super.SetActions();

        AddAction(OpenMenuAction);
    }
}