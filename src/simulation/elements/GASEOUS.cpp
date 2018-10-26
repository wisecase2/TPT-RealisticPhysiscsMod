#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_GASEOUS PT_GASEOUS 187
Element_GASEOUS::Element_GASEOUS()
{
	Identifier = "DEFAULT_PT_GASEOUS";
	Name = "GAS";
	Colour = PIXPACK(0x5070FF);
	MenuVisible = 0;
	MenuSection = SC_GAS;
	Enabled = 1;

	Advection = 2.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.30f;
	Collision = -0.10f;
	Gravity = 0.00f;
	Diffusion = 1.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 2;

	Temperature = R_TEMP+0.0f +273.15f;
	HeatConduct = 251;
	Description = "GASEOUS";

	Properties = TYPE_GAS | PROP_NEUTPASS;

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
	radabsorb = 5;

	//Update = NULL;// &Element_GASEOUS::update;
	Graphics = &Element_GASEOUS::graphics;
}

//88#TPT-Directive ElementHeader Element_GASEOUS static int update(UPDATE_FUNC_ARGS)
/*
int Element_GASEOUS::update(UPDATE_FUNC_ARGS)
{
	return 0;
}
*/
//#TPT-Directive ElementHeader Element_GASEOUS static int graphics(GRAPHICS_FUNC_ARGS)
int Element_GASEOUS::graphics(GRAPHICS_FUNC_ARGS)

{
	*firea = 255;
	*firer = 255;
	*fireg = 200;
	*fireb = 220;

	*pixel_mode = PMODE_GLOW | PMODE_ADD; //Clear default, don't draw pixel
	*pixel_mode |= FIRE_ADD;
	//Returning 0 means dynamic, do not cache
	return 0;
}


Element_GASEOUS::~Element_GASEOUS() {}
