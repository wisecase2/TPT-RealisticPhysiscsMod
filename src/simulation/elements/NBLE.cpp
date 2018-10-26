#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_NBLE PT_NBLE 52
Element_NBLE::Element_NBLE()
{
	Identifier = "DEFAULT_PT_NBLE";
	Name = "NBLE";
	Colour = PIXPACK(0xEB4917);
	MenuVisible = 1;
	MenuSection = SC_GAS;
	Enabled = 1;

	Advection = 1.0f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.30f;
	Collision = -0.1f;
	Gravity = 0.0f;
	Diffusion = 0.75f;
	HotAir = 0.001f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;
	PhotonReflectWavelengths = 0x3FFF8000;

	Weight = 1;

	Temperature = R_TEMP+2.0f	+273.15f;
	HeatConduct = 106;
	Description = "Noble Gas. Diffuses and conductive. Ionizes into plasma when introduced to electricity.";

	Properties = TYPE_GAS | PROP_CONDUCTS | PROP_LIFE_DEC;

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
	PlsmTemperaturetransition = 20000.f;
	Liquidtransition = 4.222f;
	SolidLiquidlatent = 0.5f;
	LiquidGaslatent = 2.1f;
	GasPlsmlatent = 5000.f;
	solidtransition = 0.95f;
	radabsorb = 5;

	//Update = NULL;// &Element_NBLE::update;
	//Graphics = NULL;// &Element_NBLE::graphics;
}
/*
//88#TPT-Directive ElementHeader Element_NBLE static int update(UPDATE_FUNC_ARGS)
int Element_NBLE::update(UPDATE_FUNC_ARGS)
{
	if (parts[i].temp > 52730.15 && sim->pv[y/CELL][x/CELL] > 100.0f)
	{
		parts[i].tmp |= 0x1;
		if (RNG::Ref().chance(1, 5))
		{
			int j;
			float temp = parts[i].temp;
			sim->create_part(i,x,y,PT_CO2);

			j = sim->create_part(-3,x,y,PT_NEUT);
			if (j != -1)
				parts[j].temp = temp + 17500 + RNG::Ref().between(0, 4999);
			if (RNG::Ref().chance(1, 25))
			{
				j = sim->create_part(-3,x,y,PT_ELEC);
				if (j != -1)
					parts[j].temp = temp + 17500 + RNG::Ref().between(0, 4999);
			}
			j = sim->create_part(-3,x,y,PT_PHOT);
			if (j != -1)
			{
				parts[j].ctype = 0xF800000;
				parts[j].temp = temp + 17500 + RNG::Ref().between(0, 4999);
				parts[j].tmp = 0x1;
			}
			int rx = x + RNG::Ref().between(-1, 1), ry = y + RNG::Ref().between(-1, 1), rt = TYP(pmap[ry][rx]);
			if (sim->can_move[PT_PLSM][rt] || rt == PT_NBLE)
			{
				j = sim->create_part(-3,rx,ry,PT_PLSM);
				if (j != -1)
				{
					parts[j].temp = temp + 17500 + RNG::Ref().between(0, 4999);
					parts[j].tmp |= 4;
				}
			}
			parts[i].temp = temp + 17500 + RNG::Ref().between(0, 4999);
			sim->pv[y/CELL][x/CELL] += 50;
		}
	}
	return 0;
}
*/
/*
int Element_NBLE::graphics(GRAPHICS_FUNC_ARGS){
	int caddress = restrict_flt(restrict_flt((float)cpart->life, 0.0f, 200.0f) * 3, 0.0f, (200.0f * 3) - 3);
	*colr = (unsigned char)ren->flm_data[caddress];
	*colg = (unsigned char)ren->flm_data[caddress + 1];
	*colb = (unsigned char)ren->flm_data[caddress + 2];

	*firea = 255;
	*firer = *colr;
	*fireg = *colg;
	*fireb = *colb;

	*pixel_mode = PMODE_NONE; //Clear default, don't draw pixel
	*pixel_mode |= FIRE_ADD;
	//Returning 0 means dynamic, do not cache
	return 0;
}
*/

Element_NBLE::~Element_NBLE() {}
