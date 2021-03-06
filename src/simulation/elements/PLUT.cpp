#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_PLUT PT_PLUT 19
Element_PLUT::Element_PLUT()
{
	Identifier = "DEFAULT_PT_PLUT";
	Name = "PLUT";
	Colour = PIXPACK(0x407020);
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
	PhotonReflectWavelengths = 0x001FCE00;

	Weight = 90;

	Temperature = R_TEMP+4.0f	+273.15f;
	HeatConduct = 251;
	Description = "Plutonium. Heavy, fissile particles.";

	Properties = TYPE_SOLID|PROP_NEUTPASS|PROP_RADIOACTIVE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 912.55f;
	HighTemperatureTransition = PT_LAVA;
	GasTemperaturetransition = 3500.f;
	GasTransition = PT_GASEOUS;
	PlsmTemperaturetransition = 9999.f;
	SolidLiquidlatent = 2.8f;
	LiquidGaslatent = 344.f;
	GasPlsmlatent = 5000.f;
	pressureresistance = 10.f;
	pressureblock = true;
	defaultbreak = true;
	radabsorb = 220;
	specialupdate = true;

	//Update = &Element_PLUT::update;
	Update = &Element_PLUT::update;
}


//#TPT-Directive ElementHeader Element_PLUT static int update(UPDATE_FUNC_ARGS)
int Element_PLUT::update(UPDATE_FUNC_ARGS){
	int tempadd, ident;
	if (parts[i].tmp <= 0) {
		tempadd = parts[i].temp;
		if(parts[i].type == PT_PLUT){
			sim->create_part(i, x, y, PT_RWASTE);
		} else{
			parts[i].ctype = PT_RWASTE;
		}
		//radioactivity
		//parts[i].ctype = PT_PLUT;
		parts[i].tmp = 1048576;
		parts[i].temp = tempadd;
	}
	//radioactivity
	if(RNG::Ref().chance(20000+(1048576 - parts[i].tmp), 20000000) && (parts[i].tmp > 0)){
		ident = sim->create_part(-3, x, y, PT_GAMMA);
		parts[ident].temp = 5.f*RNG::Ref().uniform01() + 5.f;
		parts[i].tmp -= 1;
	}
	return 0;
}


Element_PLUT::~Element_PLUT() {}
