#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_BMTL PT_BMTL 29
Element_BMTL::Element_BMTL()
{
	Identifier = "DEFAULT_PT_BMTL";
	Name = "BMTL";
	Colour = PIXPACK(0x505070);
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
	Hardness = 1;

	Weight = 42;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 251;
	Description = "Breakable metal. Common conductive building material, can melt and break under pressure.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_HOT_GLOW | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = 1.0f;
	HighPressureTransition = ST;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1273.0f;
	HighTemperatureTransition = PT_LAVA;
	GasTemperaturetransition = 2743.15;
	GasTransition = PT_GASEOUS;
	PlsmTemperaturetransition = 9999.f;
	SolidLiquidlatent = 28.9f;
	LiquidGaslatent = 634.3f;
	GasPlsmlatent = 5000.f;
	pressureresistance = 20.f;
	pressureblock = true;
	defaultbreak = false;
	radabsorb = 100;

	Update = &Element_BMTL::update;
}

//#TPT-Directive ElementHeader Element_BMTL static int update(UPDATE_FUNC_ARGS)
int Element_BMTL::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry;
	if (parts[i].tmp>1)
	{
		parts[i].tmp--;
		for (rx=-1; rx<2; rx++)
			for (ry=-1; ry<2; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y+ry][x+rx];
					if (!r)
						continue;
					if ((TYP(r)==PT_METL || TYP(r)==PT_IRON) && RNG::Ref().chance(1, 100))
					{
						sim->part_change_type(ID(r),x+rx,y+ry,PT_BMTL);
						parts[ID(r)].tmp = (parts[i].tmp<=7) ? parts[i].tmp=1 : parts[i].tmp - RNG::Ref().between(0, 4);
					}
				}
	}
	else if (parts[i].tmp==1 && RNG::Ref().chance(1, 1000))
	{
		parts[i].tmp = 0;
		sim->part_change_type(i,x,y,PT_BRMT);
	}
	return 0;
}


Element_BMTL::~Element_BMTL() {}
