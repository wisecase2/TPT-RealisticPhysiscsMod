#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_URAN PT_URAN 32
Element_URAN::Element_URAN()
{
	Identifier = "DEFAULT_PT_URAN";
	Name = "URAN";
	Colour = PIXPACK(0x707020);
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

	Weight = 85;

	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Uranium. Heavy, fissile particles.";

	Properties = TYPE_SOLID | PROP_RADIOACTIVE | PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1405.15f;
	HighTemperatureTransition = PT_LAVA;
	GasTemperaturetransition = 4404.15f;
	GasTransition = PT_GASEOUS;
	PlsmTemperaturetransition = 9999.f;
	SolidLiquidlatent = 8.5f;
	LiquidGaslatent = 477.f;
	GasPlsmlatent = 5000.f;
	pressureresistance = 10.f;
	pressureblock = true;
	defaultbreak = true;
	radabsorb = 200;
	specialupdate = true;

	Update = Element_URAN::update; // &Element_URAN::update;
}

//#TPT-Directive ElementHeader Element_URAN static int update(UPDATE_FUNC_ARGS)
int Element_URAN::update(UPDATE_FUNC_ARGS){
	int tempadd, ident;
	if (parts[i].tmp <= 0) {
		tempadd = parts[i].temp;
		if(parts[i].type == PT_URAN){
			sim->create_part(i, x, y, PT_RWASTE);
		} else{
			parts[i].ctype = PT_RWASTE;
		}
		//radioactivity
		//parts[i].ctype = PT_URAN;
		parts[i].tmp = 524288;
		parts[i].temp = tempadd;
	}
	//radioactivity
	if(RNG::Ref().chance(5000+(524288-parts[i].tmp), 20000000) && (parts[i].tmp > 0)){
		ident = sim->create_part(-3, x, y, PT_GAMMA);
		parts[ident].temp = 5.f;
		parts[i].tmp -= 1;
	}

	return 0;
}


Element_URAN::~Element_URAN() {}
