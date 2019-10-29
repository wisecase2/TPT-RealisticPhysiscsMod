#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_GAMMA PT_GAMMA 190
Element_GAMMA::Element_GAMMA()
{
	Identifier = "DEFAULT_PT_GAMMA";
	Name = "GAMA";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 1.00f;
	Collision = -0.99f;
	Gravity = 0.f;
	Diffusion = 0.0f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = -1;

	Temperature = 273.15f;
	HeatConduct = 0;
	Description = "Gamma rays, high penetration power.";

	Properties = TYPE_ENERGY | PROP_LIFE_DEC | PROP_LIFE_KILL_DEC;

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

	bool notransitionstate = true;


	Update = &Element_GAMMA::update;
	Graphics = &Element_GAMMA::graphics;
}

//#TPT-Directive ElementHeader Element_GAMMA static int update(UPDATE_FUNC_ARGS)

int Element_GAMMA::update(UPDATE_FUNC_ARGS)
{
	int r, idr, typr, absorb, ctype1;
	float multiplier, prob;
	bool valid1 = false;
	r = pmap[y][x];
	idr = ID(r);
	typr = TYP(r);
	ctype1 = parts[idr].ctype;
	valid1 = (ctype1 >= 0 && ctype1 < PT_NUM && sim->elements[ctype1].Enabled);

	if(valid1 && (typr == PT_LAVA || typr == PT_LIQUID || typr == PT_GASEOUS || typr == PT_PLSM || typr == PT_BRMT || typr == PT_SOLID)){
		absorb = sim->elements[ctype1].radabsorb;
		if(absorb > 0){
			if(RNG::Ref().chance(absorb, 350)){
				if(RNG::Ref().chance(MAX_TEMP - parts[idr].temp, MAX_TEMP)){
					parts[idr].temp += parts[i].temp;
					sim->kill_part(i);
					return 0;
				}
			}
		}
	} else{
		absorb = sim->elements[typr].radabsorb;
		if(absorb > 0){
			if(RNG::Ref().chance(absorb, 350)){
				prob = parts[idr].temp*1.0000100001e-5f; // temp/MAX_TEMP
				prob = MAX_TEMP - MAX_TEMP * (2.f*prob - prob*prob);
				if(RNG::Ref().chance(prob, MAX_TEMP)){
					parts[idr].temp += parts[i].temp;
					sim->kill_part(i);
					return 0;
				}
			}
		}
	}
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
	return 0;
}

//#TPT-Directive ElementHeader Element_GAMMA static int graphics(GRAPHICS_FUNC_ARGS)
int Element_GAMMA::graphics(GRAPHICS_FUNC_ARGS){
	*pixel_mode = PMODE_ADD;
	*cola = 128;
	*colr = 255;
	*colg = 255;
	*colb = 255;
	return 1;
}


Element_GAMMA::~Element_GAMMA() {}
