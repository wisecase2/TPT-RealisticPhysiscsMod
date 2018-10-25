#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_RWASTE PT_RWASTE 188
Element_RWASTE::Element_RWASTE()
{
	Identifier = "DEFAULT_PT_RWASTE";
	Name = "RWASTE";
	Colour = PIXPACK(0x707020);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.f;
	AirDrag = 0.0f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.f;
	Collision = 0.0f;
	Gravity = 0.f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	PhotonReflectWavelengths = 0x003FC000;

	Weight = 90;

	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Radioactive waste.";

	Properties = TYPE_PART | PROP_RADIOACTIVE | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1500.f;
	HighTemperatureTransition = PT_LAVA;
	GasTemperaturetransition = 4404.15f;
	GasTransition = PT_GASEOUS;
	PlsmTemperaturetransition = 9999.f;
	radabsorb = 200;

	Update = NULL; // &Element_RWASTE::update;
}
/*
//#TPT-Directive ElementHeader Element_RWASTE static int update(UPDATE_FUNC_ARGS)
int Element_RWASTE::update(UPDATE_FUNC_ARGS)
{
	if (!sim->legacy_enable && sim->pv[y/CELL][x/CELL]>0.0f)
	{
		if (parts[i].temp == MIN_TEMP)
		{
			parts[i].temp += .01f;
		}
		else
		{
			parts[i].temp = restrict_flt((parts[i].temp*(1 + (sim->pv[y / CELL][x / CELL] / 2000))) + MIN_TEMP, MIN_TEMP, MAX_TEMP);
		}
	}
	return 0;
}
*/

Element_RWASTE::~Element_RWASTE() {}
