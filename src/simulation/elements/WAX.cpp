#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_WAX PT_WAX 33
Element_WAX::Element_WAX()
{
	Identifier = "DEFAULT_PT_WAX";
	Name = "WAX";
	Colour = PIXPACK(0xF0F0BB);
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
	Meltable = 0;
	Hardness = 10;

	Weight = 10;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 44;
	Description = "Wax. Melts at moderately high temperatures.";

	Properties = TYPE_SOLID | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 319.0f;
	HighTemperatureTransition = PT_MWAX;
	GasTemperaturetransition = ITH;
	GasTransition = NT;
	PlsmTemperaturetransition = -1;
	radabsorb = 15;
	defaultbreak = true;
	pressureresistance = 7.5f;
	Update = NULL;
}

Element_WAX::~Element_WAX() {}
