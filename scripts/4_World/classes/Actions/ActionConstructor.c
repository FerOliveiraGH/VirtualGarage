modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        actions.Insert(Fabo_OpenMenuAction);
//        actions.Insert(Fabo_CreateVehicle);
//        actions.Insert(Fabo_DeployVehicleAction);
//        actions.Insert(Fabo_StoreVehicleAction);
        actions.Insert(AttachDetachWithToolAction);
    }
}
