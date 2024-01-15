modded class MissionGameplay
{
	void MissionGameplay()
	{
	    ConfigVirtualGarage.GetInstance().AddRPC();
	}

	override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

		if ( key == KeyCode.KC_ESCAPE )
		{
			if (player && player.m_Fabo_VirtualGarageMenu)
			    player.m_Fabo_VirtualGarageMenu.OnHide();
		}
	}
}