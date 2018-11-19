#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_DMND PT_DMND 193
Element_DMND::Element_DMND()
{
	Identifier = "28_is_not_DMND"; // fix replace type 28 "blck" to 193 "DMND"
	Name = "DMND";
	Colour = PIXPACK(0xCCFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
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
	Hardness = 0;

	Weight = 95;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 255;
	Description = "Diamond. Crystallized carbon.";

	Properties = TYPE_SOLID | PROP_NEUTPASS;

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
	defaultbreak = true;
	pressureresistance = 60.f;
	radabsorb = 35;
	neutslowdown = 0.98f;
	
	Update = &Element_DMND::update;
	Graphics = &Element_DMND::graphics;
}

//#TPT-Directive ElementHeader Element_DMND static int update(UPDATE_FUNC_ARGS)
int Element_DMND::update(UPDATE_FUNC_ARGS){
	int id;

	if(parts[i].life <= 0){
		id = sim->create_part(i, x, y, PT_FIRE);
		parts[id].temp = parts[i].temp + restrict_flt(0.0056401f*(1973 - parts[i].temp)*RNG::Ref().between(50, 100), 50, 2000);
		return 1;
	} else if(parts[i].life < 100 && parts[i].temp > 750.f){
		id = sim->create_part(-1, x + RNG::Ref().between(-1, 1), y + RNG::Ref().between(-1, 1), PT_FIRE);
		if(id > -1){
			parts[id].temp = parts[i].temp + restrict_flt(0.0056401f*(1973 - parts[i].temp)*RNG::Ref().between(50, 100), 50, 2000);
			parts[i].life -= RNG::Ref().between(1, 4);
		}
	}
	if(parts[i].temp > 1200.f && parts[i].life > 99){
		id = sim->create_part(-1, x + RNG::Ref().between(-1, 1), y + RNG::Ref().between(-1, 1), PT_FIRE); // auto-ignition
		if(id > -1){
			parts[i].life = 99;
		}
	}
	if(parts[i].temp > 5000.f){
		parts[i].type = PT_COAL;
		return 1;
	}

	return 0;
}

//#TPT-Directive ElementHeader Element_DMND static int graphics(GRAPHICS_FUNC_ARGS)
int Element_DMND::graphics(GRAPHICS_FUNC_ARGS){
	double startTemp = (2300.f - 1500.0);
	double tempOver = (((cpart->temp - startTemp) / 1500.0)*M_PI) - (M_PI / 2.0);
	if(tempOver > -(M_PI / 2.0)){
		if(tempOver > (M_PI / 2.0))
			tempOver = (M_PI / 2.0);
		double gradv = sin(tempOver) + 1.0;
		*firer = (int)(gradv * 258.0);
		*fireg = (int)(gradv * 156.0);
		*fireb = (int)(gradv * 112.0);
		*firea = 30;

		*colr += *firer;
		*colg += *fireg;
		*colb += *fireb;
		*pixel_mode |= FIRE_ADD;
	}
	return 0;
}

Element_DMND::~Element_DMND() {}
