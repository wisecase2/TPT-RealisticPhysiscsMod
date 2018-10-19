#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_FILT PT_FILT 125
Element_FILT::Element_FILT()
{
	Identifier = "DEFAULT_PT_FILT";
	Name = "FILT";
	Colour = PIXPACK(0x000056);
	MenuVisible = 1;
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 251;
	Description = "Filter for photons, changes the color.";

	Properties = TYPE_SOLID | PROP_NOAMBHEAT | PROP_LIFE_DEC;

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

	Update = NULL;
	Graphics = &Element_FILT::graphics;
}

//#TPT-Directive ElementHeader Element_FILT static int graphics(GRAPHICS_FUNC_ARGS)
int Element_FILT::graphics(GRAPHICS_FUNC_ARGS)
{
	int x, wl = Element_FILT::getWavelengths(cpart);
	*colg = 0;
	*colb = 0;
	*colr = 0;
	for (x=0; x<12; x++) {
		*colr += (wl >> (x+18)) & 1;
		*colb += (wl >>  x)     & 1;
	}
	for (x=0; x<12; x++)
		*colg += (wl >> (x+9))  & 1;
	x = 624/(*colr+*colg+*colb+1);
	if (cpart->life>0 && cpart->life<=4)
		*cola = 127+cpart->life*30;
	else
		*cola = 127;
	*colr *= x;
	*colg *= x;
	*colb *= x;
	*pixel_mode &= ~PMODE;
	*pixel_mode |= PMODE_BLEND;
	return 0;
}

//#TPT-Directive ElementHeader Element_FILT static int interactWavelengths(Particle* cpart, int origWl)
// Returns the wavelengths in a particle after FILT interacts with it (e.g. a photon)
// cpart is the FILT particle, origWl the original wavelengths in the interacting particle
int Element_FILT::interactWavelengths(Particle* cpart, int origWl)
{
	const int mask = 0x3FFFFFFF;
	int filtWl = getWavelengths(cpart);
	switch (cpart->tmp)
	{
		case 0:
			return filtWl; //Assign Colour
		case 1:
			return origWl & filtWl; //Filter Colour
		case 2:
			return origWl | filtWl; //Add Colour
		case 3:
			return origWl & (~filtWl); //Subtract colour of filt from colour of photon
		case 4:
		{
			int shift = int((cpart->temp-273.0f)*0.025f);
			if (shift<=0) shift = 1;
			return (origWl << shift) & mask; // red shift
		}
		case 5:
		{
			int shift = int((cpart->temp - 273.0f)*0.025f);
			if(shift <= 0) shift = 1;
			return (origWl >> shift) & mask; // blue shift
		}
		case 6:
			return origWl; // No change
		case 7:
			return origWl ^ filtWl; // XOR colours
		case 8:
			return (~origWl) & mask; // Invert colours
		case 9:
		{
			int t1 = (origWl & 0x0000FF) + RNG::Ref().between(-2, 2);
			int t2 = ((origWl & 0x00FF00)>>8) + RNG::Ref().between(-2, 2);
			int t3 = ((origWl & 0xFF0000)>>16) + RNG::Ref().between(-2, 2);
			return (origWl & 0xFF000000) | (t3<<16) | (t2<<8) | t1;
		}
		case 10:
		{
			long long int lsb = filtWl & (-filtWl);
			return (origWl * lsb) & 0x3FFFFFFF; //red shift
		}
		case 11:
		{
			long long int lsb = filtWl & (-filtWl);
			return (origWl / lsb) & 0x3FFFFFFF; // blue shift
		}
		case 12:
		{
			return origWl + filtWl; // adder number color
		}
		case 13:
		{
			return origWl - filtWl; // subtract number color
		}
		case 14: //integer multiplication
		{
			return origWl * filtWl;
		}
		case 15: //integer division
		{
			return origWl / filtWl;
		}
		case 16: //integer exponential
		{
			return pow(filtWl, origWl);
		}
		case 17: //integer logarithm
		{
			return log(origWl)/log(filtWl);
		}
		case 18: // float point ADDER
		{
			float a, b, c;
			a = origWl;
			b = filtWl;
			c = a + b;
			return *(int *)&c;  
		}
		case 19: // float point SUB
		{
			float a, b, c;
			a = origWl;
			b = filtWl;
			c = a - b;
			return *(int *)&c;  
		}
		case 20: // float point multiplication
		{
			float a,b,c;
			a = origWl;
			b = filtWl;
			c = a * b;                      
			return *(int *)&c;  
		}
		case 21: // float point DIV
		{
			float a, b, c;
			a = origWl;
			b = filtWl;
			c = a / b;
			return *(int *)&c;
		}
		case 22: // float point EXP;
		{
			float a, c;
			a = origWl;
			c = exp(a);
			return *(int *)&c;
		}
		case 23: // float point LN
		{
			float a, b, c;
			a = origWl;
			b = filtWl;
			c = log(a)/log(b);
			return *(int *)&c;
		}
		case 24: // float point POW
		{
			float a, b, c;
			a = origWl;
			b = filtWl;
			c = pow(b, a);
			return *(int *)&c;
		}
		case 25: // float point COS
		{
			float a, c;
			a = origWl;
			c = cos(a);
			return *(int *)&c;
		}
		case 26: // float point SIN
		{
			float a, c;
			a = origWl;
			c = sin(a);
			return *(int *)&c;
		}
		case 27: //integer SQRT
		{
			return sqrt(origWl);
		}
		case 28: // float point SQRT
		{
			float a, c;
			a = origWl;
			c = sqrt(a);
			return *(int *)&c;
		}
		default:
			return filtWl;
	}
}

//#TPT-Directive ElementHeader Element_FILT static int getWavelengths(Particle* cpart)
int Element_FILT::getWavelengths(Particle* cpart)
{
	if (cpart->ctype&0x3FFFFFFF)
	{
		return cpart->ctype;
	}
	else
	{
		int temp_bin = (int)((cpart->temp-273.0f)*0.025f);
		if (temp_bin < 0) temp_bin = 0;
		if (temp_bin > 25) temp_bin = 25;
		return (0x1F << temp_bin);
	}
}

Element_FILT::~Element_FILT() {}
