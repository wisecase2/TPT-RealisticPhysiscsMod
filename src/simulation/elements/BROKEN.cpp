#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_BROKEN PT_BROKEN 190
Element_BROKEN::Element_BROKEN()
{
	Identifier = "DEFAULT_PT_BROKEN";
	Name = "Broken";
	Colour = PIXPACK(0x705060);
	MenuVisible = 1;
	MenuSection = SC_POWDERS;
	Enabled = 0;

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
	Meltable = 2;
	Hardness = 2;

	Weight = 90;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 211;
	Description = "Broken solids.";

	Properties = TYPE_PART|PROP_CONDUCTS|PROP_HOT_GLOW | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = ST;
	GasTemperaturetransition = ITH;
	GasTransition = NT;
	PlsmTemperaturetransition = -1;

	Update = NULL;//&Element_BROKEN::update;
}

//#TPT-Directive ElementHeader Element_BROKEN static int update(UPDATE_FUNC_ARGS)



Element_BROKEN::~Element_BROKEN() {}
