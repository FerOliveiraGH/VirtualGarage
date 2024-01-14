class Fabo_VirtualCartridge
{
    int IndexMuzzle = 0;
    float Damage = 0;
    string TypeName = "";
    int Internal = 0;
    int AmmoCount = 0;

    void Fabo_VirtualCartridge(int indexMuzzle, float damage, string typeName, int internal = 0, int ammoCount = 0)
    {
        IndexMuzzle = indexMuzzle;
        Damage = damage;
        TypeName = typeName;
        Internal = internal;
        AmmoCount = ammoCount;
    }
}