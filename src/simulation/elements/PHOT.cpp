#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_PHOT PT_PHOT 31
Element_PHOT::Element_PHOT()
{
	Identifier = "DEFAULT_PT_PHOT";
	Name = "PHOT";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 1.00f;
	Collision = -0.99f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = -1;

	Temperature = R_TEMP+900.0f+273.15f;
	HeatConduct = 251;
	Description = "Photons. Refracts through glass, scattered by quartz, and color-changed by different elements. Ignites flammable materials.";

	Properties = TYPE_ENERGY|PROP_LIFE_DEC|PROP_LIFE_KILL_DEC;

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

	Update = &Element_PHOT::update;
	Graphics = &Element_PHOT::graphics;
}

//#TPT-Directive ElementHeader Element_PHOT static int update(UPDATE_FUNC_ARGS)
int Element_PHOT::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry,idr,typr;
	float rr, rrr, multiplier;
	if (!(parts[i].ctype&0x3FFFFFFF)) {
		sim->kill_part(i);
		return 1;
	}
	if (parts[i].temp > 506)
		if (RNG::Ref().chance(1, 10))
			Element_FIRE::update(UPDATE_FUNC_SUBCALL_ARGS);
	for(rx = -1; rx < 2; rx++){
		for(ry = -1; ry < 2; ry++){
			if(BOUNDS_CHECK){
				r = pmap[y + ry][x + rx];
				if(!r)
					continue;
				if(TYP(r) == PT_ISOZ || TYP(r) == PT_ISZS){
					if(RNG::Ref().chance(1, 400)){
						parts[i].vx *= 0.90;
						parts[i].vy *= 0.90;
						sim->create_part(ID(r), x + rx, y + ry, PT_PHOT);
						rrr = RNG::Ref().between(0, 359) * 3.14159f / 180.0f;
						if(TYP(r) == PT_ISOZ)
							rr = RNG::Ref().between(128, 255) / 127.0f;
						else
							rr = RNG::Ref().between(128, 355) / 127.0f;
						parts[ID(r)].vx = rr * cosf(rrr);
						parts[ID(r)].vy = rr * sinf(rrr);
						sim->pv[y / CELL][x / CELL] -= 15.0f * CFDS;
					}
				} else if((TYP(r) == PT_QRTZ || TYP(r) == PT_PQRT) && !ry && !rx)//if on QRTZ
				{
					float a = RNG::Ref().between(0, 359) * 3.14159f / 180.0f;
					parts[i].vx = 3.0f*cosf(a);
					parts[i].vy = 3.0f*sinf(a);
					if(parts[i].ctype == 0x3FFFFFFF)
						parts[i].ctype = 0x1F << RNG::Ref().between(0, 25);
					if(parts[i].life)
						parts[i].life++; //Delay death
				} else if(TYP(r) == PT_BGLA && !ry && !rx)//if on BGLA
				{
					float a = RNG::Ref().between(-50, 50) * 0.001f;
					float rx = cosf(a), ry = sinf(a), vx, vy;
					vx = rx * parts[i].vx + ry * parts[i].vy;
					vy = rx * parts[i].vy - ry * parts[i].vx;
					parts[i].vx = vx;
					parts[i].vy = vy;
				} else if(TYP(r) == PT_FILT && parts[ID(r)].tmp == 9){
					parts[i].vx += ((float)RNG::Ref().between(-500, 500)) / 1000.0f;
					parts[i].vy += ((float)RNG::Ref().between(-500, 500)) / 1000.0f;
				}
			}
		}
	}
	r = pmap[y][x];
	idr = ID(r);
	typr = TYP(r);
	switch(typr){
		case PT_ACEL:
			if(parts[idr].life != 0){
				float change = parts[idr].life > 1000 ? 1000 : (parts[idr].life < 0 ? 0 : parts[idr].life);
				multiplier = 1.0f + (change / 100.0f);
			} else{
				multiplier = 1.1f;
			}
			parts[i].vx *= multiplier;
			parts[i].vy *= multiplier;
			parts[idr].tmp = 1;
			break;
		case PT_DCEL:
			multiplier = 1.0f / 1.1f;
			if(parts[idr].life != 0){
				multiplier = 1.0f - ((parts[idr].life > 100 ? 100 : (parts[idr].life < 0 ? 0 : parts[idr].life)) / 100.0f);
			} else{
				multiplier = 1.1f;
			}
			parts[i].vx *= multiplier;
			parts[i].vy *= multiplier;
			parts[idr].tmp = 1;
			break;
		case PT_CONV:
			if(parts[idr].tmp2 != 1){
				sim->create_part(i, x, y, TYP(parts[idr].ctype));
			} else{
				sim->part_change_type(i, x, y, TYP(parts[idr].ctype));
				parts[i].ctype = 0;
			}
			break;
		default:
			break;
	}

	return 0;
}



//#TPT-Directive ElementHeader Element_PHOT static int graphics(GRAPHICS_FUNC_ARGS)
int Element_PHOT::graphics(GRAPHICS_FUNC_ARGS)

{
	int x = 0;
	*colr = *colg = *colb = 0;
	for (x=0; x<12; x++) {
		*colr += (cpart->ctype >> (x+18)) & 1;
		*colb += (cpart->ctype >>  x)     & 1;
	}
	for (x=0; x<12; x++)
		*colg += (cpart->ctype >> (x+9))  & 1;
	x = 624/(*colr+*colg+*colb+1);
	*colr *= x;
	*colg *= x;
	*colb *= x;

	*firea = 100;
	*firer = *colr;
	*fireg = *colg;
	*fireb = *colb;

	*pixel_mode &= ~PMODE_FLAT;
	*pixel_mode |= FIRE_ADD | PMODE_ADD | NO_DECO;
	if (cpart->flags & FLAG_PHOTDECO)
	{
		*pixel_mode &= ~NO_DECO;
	}
	return 0;
}


Element_PHOT::~Element_PHOT() {}
