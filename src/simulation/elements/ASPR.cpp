#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_ASPR PT_ASPR 192
Element_ASPR::Element_ASPR()
{
	Identifier = "DEFAULT_PT_ASPR";
	Name = "ASPR";
	Colour = PIXPACK(0x908080);
	MenuVisible = 1;
	MenuSection = SC_ELEC;
	Enabled = 1;

	Advection = 0.f;
	AirDrag = 0.0f * CFDS;
	AirLoss = 1.0f;
	Loss = 1.0f;
	Collision = -0.1f;
	Gravity = 0.f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	PhotonReflectWavelengths = 0x00000000;

	Weight = 40;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 0;
	Description = "Subframe spark.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_LIFE_DEC | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	GasTemperaturetransition = ITH;
	GasTransition = NT;
	PlsmTemperaturetransition = -1;
	pressureblock = true;
	radabsorb = 1000;


	Update = &Element_ASPR::update;;
}
//#TPT-Directive ElementHeader Element_ASPR static int update(UPDATE_FUNC_ARGS)

int Element_ASPR::update(UPDATE_FUNC_ARGS){
	int r, rx, ry;
	if(parts[i].tmp2 == 0){
		parts[i].type = PT_SPRK;
		parts[i].ctype = PT_ASPR;
		parts[i].life = 3;
	}

	return 0;
}

Element_ASPR::~Element_ASPR() {}
