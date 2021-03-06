#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_IRON PT_IRON 76
Element_IRON::Element_IRON()
{
	Identifier = "DEFAULT_PT_IRON";
	Name = "IRON";
	Colour = PIXPACK(0x707070);
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
	Meltable = 1;
	Hardness = 50;

	Weight = 45;

	Temperature = R_TEMP+0.0f +273.15f;
	HeatConduct = 251;
	Description = "Rusts with salt, can be used for electrolysis of WATR.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_HOT_GLOW | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1687.0f;
	HighTemperatureTransition = PT_LAVA;
	GasTemperaturetransition = 3100.f;
	GasTransition = PT_GASEOUS;
	PlsmTemperaturetransition = 9999.f;
	SolidLiquidlatent = 28.9f;
	LiquidGaslatent = 634.3f;
	GasPlsmlatent = 5000.f;
	pressureresistance = 15.f;
	pressureblock = true;
	defaultbreak = true;
	radabsorb = 100;

	Update = &Element_IRON::update;
}

//#TPT-Directive ElementHeader Element_IRON static int update(UPDATE_FUNC_ARGS)
int Element_IRON::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry;
	if (parts[i].life)
		return 0;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				switch TYP(r)
				{
				case PT_SALT:
					if (RNG::Ref().chance(1, 47))
						goto succ;
					break;
				case PT_SLTW:
					if (RNG::Ref().chance(1, 67))
						goto succ;
					break;
				case PT_WATR:
					if (RNG::Ref().chance(1, 1200))
						goto succ;
					break;
				case PT_O2:
					if (RNG::Ref().chance(1, 250))
						goto succ;
					break;
				case PT_LO2:
					goto succ;
				default:
					break;
				}
			}
	return 0;
succ:
	sim->part_change_type(i,x,y,PT_BMTL);
	parts[i].tmp = RNG::Ref().between(20, 29);
	return 0;
}


Element_IRON::~Element_IRON() {}
