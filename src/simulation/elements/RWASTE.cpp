#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_RWASTE PT_RWASTE 188
Element_RWASTE::Element_RWASTE()
{
	Identifier = "DEFAULT_PT_RWASTE";
	Name = "RWASTE";
	Colour = PIXPACK(0x606010);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.2f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.5f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	PhotonReflectWavelengths = 0x003FC000;

	Weight = 80;

	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Radioactive waste.";

	Properties = TYPE_SOLID | PROP_RADIOACTIVE | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1500.f;
	HighTemperatureTransition = PT_LAVA;
	GasTemperaturetransition = 4404.15f;
	GasTransition = PT_GASEOUS;
	PlsmTemperaturetransition = 9999.f;
	SolidLiquidlatent = 2.8f;
	LiquidGaslatent = 344.f;
	GasPlsmlatent = 5000.f;
	radabsorb = 200;
	pressureresistance = 10.f;
	pressureblock = true;
	defaultbreak = true;
	specialupdate = true;

	Update = &Element_RWASTE::update;
}

//#TPT-Directive ElementHeader Element_RWASTE static int update(UPDATE_FUNC_ARGS)
int Element_RWASTE::update(UPDATE_FUNC_ARGS)
{
	int ident;
	//radioactivity
	if(RNG::Ref().chance(parts[i].tmp, 20000000) && (parts[i].tmp > 0)){
		ident = sim->create_part(-3, x, y, PT_GAMMA);
		parts[ident].temp = 10.f;
		parts[i].tmp -= 1;
	}
	return 0;
}


Element_RWASTE::~Element_RWASTE() {}
