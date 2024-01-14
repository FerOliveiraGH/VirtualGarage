modded class MissionServer
{
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

        ConfigVirtualGarage.GetInstance().SendRPC(identity);

        Fabo_StoreVehicleCore.GetInstance();
        Fabo_DeployVehicleCore.GetInstance();
        Fabo_ListGarageCore.GetInstance();
	}
}