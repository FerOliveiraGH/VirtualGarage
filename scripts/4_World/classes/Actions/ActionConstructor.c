modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        actions.Insert(Fabo_OpenMenuAction);
        actions.Insert(AttachDetachWithToolAction);
        actions.Insert(Fabo_InsuranceActiveAction);
        actions.Insert(Fabo_InsuranceCheckAction);
    }
}
