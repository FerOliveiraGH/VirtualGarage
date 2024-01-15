modded class PlayerBase extends ManBase
{
	ref Fabo_VirtualGarageMenu m_Fabo_VirtualGarageMenu;

	override void Init()
	{
		super.Init();
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(Fabo_OpenMenuAction);
	}
};
