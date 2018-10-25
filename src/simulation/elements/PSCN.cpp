#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_PSCN PT_PSCN 35
Element_PSCN::Element_PSCN()
{
	Identifier = "DEFAULT_PT_PSCN";
	Name = "PSCN";
	Colour = PIXPACK(0x805050);
	MenuVisible = 1;
	MenuSection = SC_ELEC;
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
	Hardness = 1;
	PhotonReflectWavelengths = 0x00000000;

	Weight = 40;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 251;
	Description = "P-Type Silicon, Will transfer current to any conductor.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_LIFE_DEC | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1687.0f;
	HighTemperatureTransition = PT_LAVA;
	GasTemperaturetransition = 3000.f;
	GasTransition = PT_GASEOUS;
	PlsmTemperaturetransition = 9999.f;
	pressureresistance = 10.f;
	pressureblock = true;
	defaultbreak = true;
	radabsorb = 75;

	Update = NULL;
}

Element_PSCN::~Element_PSCN() {}
