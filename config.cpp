class CfgPatches
{
    class VirtualGarage
    {
        units[]={};
        weapons[]={};
        shoulder[]={};
        melee[]={};
        requiredVersion=0.1;
        requiredAddons[]=
        {
            "DZ_Data",
            "DZ_Characters_Backpacks",
            "DZ_Scripts",
            "DZ_Gear_Consumables"
        };
    };
};
class CfgMods
{
    class VirtualGarage
	{
        dir="VirtualGarage";
        name="VirtualGarage";
        credits="";
        author="Fernando Oliveira";
        authorID=0;
        version="1.0";
        extra=0;
        type="mod";
        dependencies[]=
        {
            "Game",
            "Mission",
            "World"
        };
        class defs
        {
            class gameScriptModule
            {
                value="";
                files[]=
                {
                    "VirtualGarage/scripts/3_Game"
                };
            };
            class worldScriptModule
            {
                value="";
                files[]=
                {
                    "VirtualGarage/scripts/4_World"
                };
            };
            class missionScriptModule
			{
				value="";
				files[]=
				{
					"VirtualGarage/scripts/5_Mission"
				};
			};
        };
	};
};
class CfgVehicles
{
    class PowerGenerator;
    class Fabo_VirtualGarage: PowerGenerator
    {
        name="Fabo_VirtualGarage";
        displayName="$STR_CfgVehicles_fabo_virtual_garage_title";
        descriptionShort="$STR_CfgVehicles_fabo_virtual_garage_description";
        scope=2;
        itemSize[]={5,10};
        itemsCargoSize[]={0,0};
        model="VirtualGarage\data\items\virtualgarage";
        hiddenSelectionsTextures[]=
        {
            "VirtualGarage\data\images\parquimetro_co.paa",
            "VirtualGarage\data\images\parquimetro_co.paa",
            "VirtualGarage\data\images\parquimetro_co.paa"
        };
        hiddenSelectionsMaterials[]=
		{
			"VirtualGarage\data\images\parquimetro_co.rvmat"
		};
        class Cargo
		{
			itemsCargoSize[]={0,0};
			openable=0;
			allowOwnedCargoManipulation=0;
		};
        attachments[]={};
        hiddenSelections[]={};
        hologramMaterial="";
        hologramMaterialPath="";
        class DamageSystem {};
        class EnergyManager {};
        class AnimationSources {};
        class AnimEvents {};
    };
    class Fabo_VirtualGarage_Static: Fabo_VirtualGarage
    {
    };
    class Paper;
    class Fabo_InsurancePaper: Paper
	{
		scope=2;
		displayName="$STR_CfgVehicles_fabo_paper_insurance";
		descriptionShort="$STR_CfgVehicles_fabo_paper_insurance_desc";
		isMeleeWeapon=1;
		weight=2;
		itemSize[]={2,2};
		rotationFlags=16;
		hiddenSelectionsTextures[]=
		{
			"VirtualGarage\data\images\insurance_paper.paa",
			"VirtualGarage\data\images\insurance_paper.paa",
			"VirtualGarage\data\images\insurance_paper.paa"
		};
        hiddenSelectionsMaterials[]=
		{
			"VirtualGarage\data\images\insurance_paper.rvmat"
		};
		attachments[]={};
        hiddenSelections[]=
		{
			"zbytek"
		};
        hologramMaterial="";
        hologramMaterialPath="";
		class DamageSystem {};
        class EnergyManager {};
        class AnimationSources {};
        class AnimEvents {};
	};
};