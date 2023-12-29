class Fabo_VirtualGarage_Static: Fabo_VirtualGarage
{
    void Fabo_VirtualGarage_Static()
    {
        Deployed = 1;
        StaticClass = 1;
    }

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
        AddAction(OpenMenuAction);
    }
}