#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_DSTW PT_DSTW 25
Element_DSTW::Element_DSTW()
{
	Identifier = "DEFAULT_PT_DSTW";
	Name = "DSTW";
	Colour = PIXPACK(0x1020C0);
	MenuVisible = 1;
	MenuSection = SC_LIQUID;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.5f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 5;

	Temperature = R_TEMP-2.0f	+273.15f;
	HeatConduct = 10;
	Description = "Distilled water, does not conduct electricity.";

	Properties = TYPE_LIQUID|PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 273.15f;
	LowTemperatureTransition = PT_ICEI;
	HighTemperature = 373.0f;
	HighTemperatureTransition = ST;
	GasTemperaturetransition = ITH;
	GasTransition = NT;
	PlsmTemperaturetransition = -1;
	SolidLiquidlatent = 52.f;
	LiquidGaslatent = 350.f;
	GasPlsmlatent = 5000.f;
	Heatcapacity = 4.2f;
	InvHeatcapacity = 1.0f;
	radabsorb = 15;

	neutslowdown = 0.985f;

	Update = &Element_DSTW::update;
}

//#TPT-Directive ElementHeader Element_DSTW static int update(UPDATE_FUNC_ARGS)
int Element_DSTW::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, averagetmp, oldtmp;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				switch (TYP(r))
				{
				case PT_SALT:
					if(RNG::Ref().chance(1, 50)){
						sim->part_change_type(ID(r), x, y, PT_WATR);
						parts[i].tmp = parts[ID(r)].tmp = 0.5f*parts[i].tmp + 0.5f * 2147483646;
					}
					break;
				case PT_SLTW:
					if (RNG::Ref().chance(1, 50))
					{
						parts[i].tmp = parts[ID(r)].tmp = 0.5f*parts[i].tmp + 0.5f*parts[ID(r)].tmp;
					}
					break;
				case PT_WATR:
					if (RNG::Ref().chance(1, 100))
					{
						parts[i].tmp = parts[ID(r)].tmp = 0.5f*parts[i].tmp + 0.5f*parts[ID(r)].tmp;
					}
					break;
				case PT_RBDM:
				case PT_LRBD:
					if ((sim->legacy_enable||parts[i].temp>12.0f) && RNG::Ref().chance(1, 100))
					{
						sim->part_change_type(i,x,y,PT_FIRE);
						parts[i].life = 4;
					}
					break;
				case PT_FIRE:
					sim->kill_part(ID(r));
					if (RNG::Ref().chance(1, 30))
					{
						sim->kill_part(i);
						return 1;
					}
					break;
				default:
					continue;
				}
			}
	if(parts[i].tmp > 64424510){
		parts[i].type = PT_WATR;
	}
	return 0;
}


Element_DSTW::~Element_DSTW() {}
