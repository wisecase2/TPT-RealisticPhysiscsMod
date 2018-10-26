#include "simulation/Elements.h"
#include "simulation/Air.h"
//#TPT-Directive ElementClass Element_TTAN PT_TTAN 144
Element_TTAN::Element_TTAN()
{
	Identifier = "DEFAULT_PT_TTAN";
	Name = "TTAN";
	Colour = PIXPACK(0x909090);
	MenuVisible = 1;
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.3f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 1;
	Hardness = 50;

	Weight = 25;

	Temperature = R_TEMP+0.0f +273.15f;
	HeatConduct = 251;
	Description = "Titanium. Higher melting temperature than most other metals, blocks all air pressure.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_HOT_GLOW|PROP_LIFE_DEC | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1941.0f;
	HighTemperatureTransition = PT_LAVA;
	GasTemperaturetransition = 3560.15f;
	GasTransition = PT_GASEOUS;
	PlsmTemperaturetransition = 9999.f;
	SolidLiquidlatent = 15.f;
	LiquidGaslatent = 421.f;
	GasPlsmlatent = 5000.f;
	pressureresistance = 30.f;
	pressureblock = true;
	defaultbreak = true;
	radabsorb = 50;

	//Update = NULL;// &Element_TTAN::update;
}

//88#TPT-Directive ElementHeader Element_TTAN static int update(UPDATE_FUNC_ARGS)
/*
int Element_TTAN::update(UPDATE_FUNC_ARGS)
{
	int ttan = 0;
	if (nt <= 2)
		ttan = 2;
	else if (parts[i].tmp)
		ttan = 2;
	else if (nt <= 6)
	{
		for (int rx = -1; rx <= 1; rx++)
			for (int ry = -1; ry <= 1; ry++)
				if ((!rx != !ry) && BOUNDS_CHECK)
				{
					if (TYP(pmap[y+ry][x+rx]) == PT_TTAN)
						ttan++;
				}
	}

	if (ttan >= 2)
	{
		sim->air->bmap_blockair[y/CELL][x/CELL] = 1;
		sim->air->bmap_blockairh[y/CELL][x/CELL] = 0x8;
	}
	return 0;
}
*/

Element_TTAN::~Element_TTAN() {}
