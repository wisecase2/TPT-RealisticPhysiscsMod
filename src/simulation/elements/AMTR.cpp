#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_AMTR PT_AMTR 72
Element_AMTR::Element_AMTR()
{
	Identifier = "DEFAULT_PT_AMTR";
	Name = "AMTR";
	Colour = PIXPACK(0x808080);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.7f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.96f;
	Loss = 0.80f;
	Collision = 0.00f;
	Gravity = 0.50f;
	Diffusion = 1.00f;
	HotAir = 0.0000f * CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = 100;

	Temperature = R_TEMP+0.0f +273.15f;
	HeatConduct = 70;
	Description = "Anti-Matter, destroys a majority of particles.";

	Properties = TYPE_GAS;

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
	radabsorb = 50;

	Update = &Element_AMTR::update;
	Graphics = &Element_AMTR::graphics;
}

//#TPT-Directive ElementHeader Element_AMTR static int update(UPDATE_FUNC_ARGS)
int Element_AMTR::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, rt, identy;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				rt = TYP(r);
				if (rt!=PT_AMTR && rt!=PT_DMND && rt!=PT_CLNE && rt!=PT_PCLN && rt!=PT_VOID && rt!=PT_BHOL && rt!=PT_NBHL && rt!=PT_PRTI && rt!=PT_PRTO && rt!= PT_GAMMA && rt!=PT_PHOT)
				{
					for(int i = 0; i < 64; i++){
						identy = sim->create_part(-3, x + rx, y + ry, PT_GAMMA);
						parts[identy].temp = MAX_TEMP;
					}
					sim->kill_part(ID(r));
					sim->kill_part(i);
				}
			}
	return 0;
}


//#TPT-Directive ElementHeader Element_AMTR static int graphics(GRAPHICS_FUNC_ARGS)
int Element_AMTR::graphics(GRAPHICS_FUNC_ARGS)
{
	// don't render AMTR as a gas
	// this function just overrides the default graphics
	return 1;
}

Element_AMTR::~Element_AMTR() {}
