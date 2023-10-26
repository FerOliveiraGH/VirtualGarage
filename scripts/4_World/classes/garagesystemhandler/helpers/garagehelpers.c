modded class GarageHelpers
{
	override static CarScript CheckForCarAtPos(vector position)
	{
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition(position, 6, objects, proxyCargos);
		CarScript car;
		if (objects)
		{
			for (int i = 0; i < objects.Count(); i++ )
			{
				if (Class.CastTo( car, objects.Get(i) ) )
						return car;
			}
		}
		return NULL;
	}
}
