modded class Screwdriver extends ToolBase
{
	override void SetActions()
	{
		super.SetActions();

		AddAction(AttachDetachWithToolAction);
	}
}