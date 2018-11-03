#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_WATR PT_WATR 2
Element_WATR::Element_WATR()
{
	Identifier = "DEFAULT_PT_WATR";
	Name = "WATR";
	Colour = PIXPACK(0x2030D0);
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

	Weight = 30;

	Temperature = R_TEMP-2.0f	+273.15f;
	HeatConduct = 29;
	Description = "Water. Conducts electricity, freezes, and extinguishes fires.";

	Properties = TYPE_LIQUID|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_NEUTPASS;

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
	InvHeatcapacity = 0.238095f;
	radabsorb = 15;
	neutslowdown = 0.988f;

	Update = &Element_WATR::update;
}

//#TPT-Directive ElementHeader Element_WATR static int update(UPDATE_FUNC_ARGS)
int Element_WATR::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, oldtmp;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if (TYP(r)==PT_SALT && RNG::Ref().chance(1, 100))
				{
					sim->part_change_type(ID(r), x, y, PT_WATR);
					parts[i].tmp = parts[ID(r)].tmp = 0.5f*parts[i].tmp + 0.5f*2147483646;
				}
				else if ((TYP(r)==PT_RBDM||TYP(r)==PT_LRBD) && (sim->legacy_enable||parts[i].temp>(273.15f+12.0f)) && RNG::Ref().chance(1, 100))
				{
					sim->part_change_type(i,x,y,PT_FIRE);
					parts[i].life = 4;
					parts[i].ctype = PT_WATR;
				}
				else if (TYP(r)==PT_FIRE && parts[ID(r)].ctype!=PT_WATR)
				{
					sim->kill_part(ID(r));
					if (RNG::Ref().chance(1, 30))
					{
						sim->kill_part(i);
						return 1;
					}
				}
				else if (TYP(r)==PT_SLTW && RNG::Ref().chance(1, 100))
				{
					parts[i].tmp = parts[ID(r)].tmp = 0.5f*parts[i].tmp + 0.5f*parts[ID(r)].tmp;
				}
				else if(TYP(r) == PT_DSTW && RNG::Ref().chance(1, 100)){
					parts[i].tmp = parts[ID(r)].tmp = 0.5f*parts[i].tmp + 0.5f*parts[ID(r)].tmp;
				}
			}
	if(parts[i].tmp > 322122546){
		parts[i].type = PT_SLTW;
	} else if(parts[i].tmp <= 64424510){
		parts[i].type = PT_DSTW;
	}

	return 0;
}

Element_WATR::~Element_WATR() {}
