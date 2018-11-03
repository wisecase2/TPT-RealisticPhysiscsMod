#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_GRVT PT_GRVT 177
Element_GRVT::Element_GRVT()
{
	Identifier = "DEFAULT_PT_GRVT";
	Name = "GRVT";
	Colour = PIXPACK(0x00EE76);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 1.00f;
	Collision = -.99f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f * CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = -1;

	Temperature = R_TEMP+273.15f;
	HeatConduct = 61;
	Description = "Gravitons. Create Newtonian Gravity.";

	Properties = TYPE_ENERGY|PROP_LIFE_DEC|PROP_LIFE_KILL_DEC;

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

	Update = &Element_GRVT::update;
	Graphics = &Element_GRVT::graphics;
}

//#TPT-Directive ElementHeader Element_GRVT static int update(UPDATE_FUNC_ARGS)
int Element_GRVT::update(UPDATE_FUNC_ARGS)
{
	int r, typr, idr;
	float multiplier;
	//at higher tmps they just go completely insane
	if (parts[i].tmp >= 100)
		parts[i].tmp = 100;
	if (parts[i].tmp <= -100)
		parts[i].tmp = -100;

	r = pmap[y][x];
	idr = ID(r);
	typr = TYP(r);
	switch(typr){
		case PT_ACEL:
			if(parts[idr].life != 0){
				float change = parts[idr].life > 1000 ? 1000 : (parts[idr].life < 0 ? 0 : parts[idr].life);
				multiplier = 1.0f + (change / 100.0f);
			} else{
				multiplier = 1.1f;
			}
			parts[i].vx *= multiplier;
			parts[i].vy *= multiplier;
			parts[idr].tmp = 1;
			break;
		case PT_DCEL:
			multiplier = 1.0f / 1.1f;
			if(parts[idr].life != 0){
				multiplier = 1.0f - ((parts[idr].life > 100 ? 100 : (parts[idr].life < 0 ? 0 : parts[idr].life)) / 100.0f);
			} else{
				multiplier = 1.1f;
			}
			parts[i].vx *= multiplier;
			parts[i].vy *= multiplier;
			parts[idr].tmp = 1;
			break;
		case PT_CONV:
			if(parts[idr].tmp2 != 1){
				sim->create_part(i, x, y, TYP(parts[idr].ctype));
			} else{
				sim->part_change_type(i, x, y, TYP(parts[idr].ctype));
				parts[i].ctype = 0;
			}
			break;
		default:
			break;
	}

	sim->gravmap[(y/CELL)*(XRES/CELL)+(x/CELL)] = 0.2f*parts[i].tmp;
	return 0;
}

//#TPT-Directive ElementHeader Element_GRVT static int graphics(GRAPHICS_FUNC_ARGS)
int Element_GRVT::graphics(GRAPHICS_FUNC_ARGS)
{
	*firea = 5;
	*firer = 0;
	*fireg = 250;
	*fireb = 170;

	*pixel_mode |= FIRE_BLEND;
	return 1;
}

Element_GRVT::~Element_GRVT() {}
