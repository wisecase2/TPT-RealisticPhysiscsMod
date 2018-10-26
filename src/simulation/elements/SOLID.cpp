#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_SOLID PT_SOLID 192
Element_SOLID::Element_SOLID()
{
	Identifier = "DEFAULT_PT_SOLID";
	Name = "SOLID";
	Colour = PIXPACK(0xA0C0FF);
	MenuVisible = 0;
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.6f;
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
	Hardness = 20;

	Weight = 0.5;

	Temperature = R_TEMP-50.0f+273.15f;
	HeatConduct = 10;
	Description = "Solid state.";

	Properties = TYPE_SOLID|PROP_NEUTPASS;

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
	pressureresistance = 2.f;
	defaultbreak = true;

	Update = &Element_SOLID::update;
}

//#TPT-Directive ElementHeader Element_SOLID static int update(UPDATE_FUNC_ARGS)
int Element_SOLID::update(UPDATE_FUNC_ARGS){
	if(sim->IsValidElement(parts[i].ctype)){
		if(parts[i].temp > sim->elements[parts[i].ctype].solidtransition){
			parts[i].type = PT_LIQUID;
			parts[i].temp -= sim->elements[parts[i].ctype].SolidLiquidlatent;
		}
	}
	return 0;
}

Element_SOLID::~Element_SOLID() {}
