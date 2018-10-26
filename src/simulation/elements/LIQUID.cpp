#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_LIQUID PT_LIQUID 189
Element_LIQUID::Element_LIQUID()
{
	Identifier = "DEFAULT_PT_LIQUID";
	Name = "LIQUID";
	Colour = PIXPACK(0x80A0CF);
	MenuVisible = 0;
	MenuSection = SC_LIQUID;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 30;

	Temperature = R_TEMP-2.0f	+273.15f;
	HeatConduct = 23;
	Description = "liquid state.";

	Properties = TYPE_LIQUID|PROP_NEUTPASS;

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
	radabsorb = 15;

	Update = &Element_LIQUID::update;
	//Graphics = NULL;// &Element_LIQUID::graphics;
}

//#TPT-Directive ElementHeader Element_LIQUID static int update(UPDATE_FUNC_ARGS)
int Element_LIQUID::update(UPDATE_FUNC_ARGS)
{
	if(sim->IsValidElement(parts[i].ctype) && sim->elements[parts[i].ctype].solidtransition > 0){
		if(parts[i].temp <= sim->elements[parts[i].ctype].solidtransition){
			parts[i].type = PT_SOLID;
			parts[i].temp += sim->elements[parts[i].ctype].SolidLiquidlatent;
		}
	}
	return 0;
}

Element_LIQUID::~Element_LIQUID() {}
