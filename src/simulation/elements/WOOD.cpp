#include "common/tpt-minmax.h"
#include "simulation/Elements.h"

//#TPT-Directive ElementClass Element_WOOD PT_WOOD 17
Element_WOOD::Element_WOOD()
{
	Identifier = "DEFAULT_PT_WOOD";
	Name = "WOOD";
	Colour = PIXPACK(0xC0A040);
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

	Flammable = 20;
	Explosive = 0;
	Meltable = 0;
	Hardness = 15;

	Weight = 4;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 164;
	Description = "Wood, flammable.";

	Properties = TYPE_SOLID | PROP_NEUTPENETRATE | PROP_GAMMAPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 873.0f;
	HighTemperatureTransition = PT_FIRE;
	GasTemperaturetransition = ITH;
	GasTransition = NT;
	PlsmTemperaturetransition = -1;
	radabsorb = 20;
	defaultbreak = true;
	pressureresistance = 7.5f;
	neutslowdown = 0.99f;

	Update = &Element_WOOD::update;
	Graphics = &Element_WOOD::graphics;
}

//#TPT-Directive ElementHeader Element_WOOD static int update(UPDATE_FUNC_ARGS)
int Element_WOOD::update(UPDATE_FUNC_ARGS)
{
	if (parts[i].temp > 450 && parts[i].temp > parts[i].tmp)
		parts[i].tmp = (int)parts[i].temp;
	return 0;
}
//#TPT-Directive ElementHeader Element_WOOD static int graphics(GRAPHICS_FUNC_ARGS)
int Element_WOOD::graphics(GRAPHICS_FUNC_ARGS)
{
	float maxtemp = std::max((float)cpart->tmp, cpart->temp);
	if (maxtemp > 400)
	{
		*colr -= (int)restrict_flt((maxtemp-400)/3,0,172);
		*colg -= (int)restrict_flt((maxtemp-400)/4,0,140);
		*colb -= (int)restrict_flt((maxtemp-400)/20,0,44);
	}
	if (maxtemp < 273)
	{
		*colr -= (int)restrict_flt((273-maxtemp)/5,0,40);
		*colg += (int)restrict_flt((273-maxtemp)/4,0,40);
		*colb += (int)restrict_flt((273-maxtemp)/1.5,0,150);
	}
	return 0;
}

Element_WOOD::~Element_WOOD() {}
