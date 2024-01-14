class Fabo_VirtualGarage: ItemBase
{
    int Deployed = 0;
    int StaticClass = 0;

    void Fabo_VirtualGarage()
    {
        RegisterNetSyncVariableInt("Deployed");
    }

    override void OnStoreSave( ParamsWriteContext ctx )
    {
        super.OnStoreSave(ctx);

        ctx.Write(Deployed);
    }

    override bool OnStoreLoad( ParamsReadContext ctx, int version )
    {
        if ( !super.OnStoreLoad( ctx, version ) )
            return false;

        ctx.Read(Deployed);

        return true;
    }

    override bool CanPutInCargo( EntityAI parent )
	{
        if (Deployed == 1) {
		    return false;
        }

        return true;
	}

    override bool CanPutIntoHands( EntityAI parent )
	{
        if (Deployed == 1) {
		    return false;
        }

		return true;
	}

    override bool IsContainer()
	{
		return false;
	}

    override void SetActions()
    {
        AddAction(ActionTogglePlaceObject);
        AddAction(ActionPlaceObject);
        AddAction(ActionTakeItem);
        AddAction(ActionTakeItemToHands);
        AddAction(Fabo_OpenMenuAction);
    }

    override void OnEndPlacement()
	{
        super.OnEndPlacement();

        Deployed = 1;

        SetSynchDirty();
	}
}