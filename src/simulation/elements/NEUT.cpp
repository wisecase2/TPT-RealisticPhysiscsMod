#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_NEUT PT_NEUT 18
Element_NEUT::Element_NEUT()
{
	Identifier = "DEFAULT_PT_NEUT";
	Name = "NEUT";
	Colour = PIXPACK(0x20E0FF);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.005f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 1.00f;
	Collision = -0.99f;
	Gravity = 0.0f;
	Diffusion = 0.01f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = -1;

	Temperature = R_TEMP+4.0f	+273.15f;
	HeatConduct = 60;
	Description = "Neutrons. Interact with matter in odd ways.";

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

	Update = &Element_NEUT::update;
	Graphics = &Element_NEUT::graphics;
}

//#TPT-Directive ElementHeader Element_NEUT static int update(UPDATE_FUNC_ARGS)
int Element_NEUT::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, identity, tempadd, ident, neut2, cont2, tmp2n, tmpp, idr, typr;
	unsigned int pressureFactor = 3 + (int)sim->pv[y/CELL][x/CELL];
	r = pmap[y][x];

	////// joins the neutrons if you have more than 4 in the same position.
	
	cont2 = 0;
	ident = sim->pmap2[y][x][1];
	for (int cont = 0; cont < 6 && cont <= sim->pmap2[y][x][3]; cont++) {
		if(parts[ident].type == PT_NEUT && ident != i){
			cont2++;
			if(cont2 == 1) {
				neut2 = ident;
			}else if(cont2 == 3) {
				parts[i].tmp2 += parts[neut2].tmp2;
				parts[i].temp = 0.5f*(parts[i].temp + parts[neut2].temp);
				sim->kill_part(neut2);
				neut2 = ident;
				cont2 = 1;
			}
		}
		if(sim->idpointer[ident][0] == (sim->currentTick)){
			ident = sim->idpointer[ident][1];
		}else{
			break;
		}
	}
	
	idr = ID(r);
	typr = TYP(r);
	
	//// nuclear fission uranium and plutonium:
	if (typr == PT_PLUT || typr == PT_GASEOUS && parts[idr].ctype == PT_PLUT || typr == PT_PLSM && parts[idr].ctype == PT_PLUT || typr == PT_LAVA && parts[idr].ctype == PT_PLUT){
		if (RNG::Ref().chance(1, 3) && (parts[idr].tmp > 0)){
			tmpp = parts[idr].tmp;
			identity = sim->create_part(-3, x, y, PT_NEUT);
			tempadd = parts[identity].tmp2 = parts[i].tmp2;
			parts[idr].temp += tempadd;
			parts[identity].temp = parts[i].temp;
			parts[identity].temp += tempadd;
			parts[i].temp += tempadd;
			tmpp -= parts[i].tmp2;
				
			parts[idr].tmp = tmpp;
		
			//sim->pv[y/CELL][x/CELL] += 5.0f * CFDS * parts[i].tmp2 * 0.0000152587f; //Used to be 2, some people said nukes weren't powerful enough
			Element_FIRE::update(UPDATE_FUNC_SUBCALL_ARGS);
		}
	}else if(typr == PT_URAN || typr == PT_GASEOUS && parts[idr].ctype == PT_URAN || typr == PT_PLSM && parts[idr].ctype == PT_URAN || typr == PT_LAVA && parts[idr].ctype == PT_URAN) {
		if (RNG::Ref().chance(1, 5) && (parts[idr].tmp > 0)) {
			tmpp = parts[idr].tmp;
			identity = sim->create_part(-3, x, y, PT_NEUT);
			tempadd = parts[identity].tmp2 = parts[i].tmp2;
			parts[idr].temp += tempadd;
			parts[identity].temp = parts[i].temp;
			parts[identity].temp += tempadd;
			parts[i].temp += tempadd;
			tmpp -= parts[i].tmp2;

			parts[idr].tmp = tmpp;

			//sim->pv[y / CELL][x / CELL] += 5.0f * CFDS * parts[i].tmp2 * 0.0000152587f; //Used to be 2, some people said nukes weren't powerful enough
			Element_FIRE::update(UPDATE_FUNC_SUBCALL_ARGS);
		}
	}
	
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK)
			{
				r = pmap[y+ry][x+rx];
				idr = ID(r);
				typr = TYP(r);
				switch (typr)
				{
				case PT_WATR:
					if (RNG::Ref().chance(3, 20))
						sim->part_change_type(idr,x+rx,y+ry,PT_DSTW);
				case PT_ICEI:
				case PT_SNOW:
					parts[i].vx *= 0.995;
					parts[i].vy *= 0.995;
					break;
#ifdef SDEUT
				case PT_DEUT:
					if (RNG::Ref().chance(1+(parts[idr].life/100), 1000))
					{
						DeutExplosion(sim, parts[idr].life, x+rx, y+ry, restrict_flt(parts[idr].temp + parts[idr].life*500.0f, MIN_TEMP, MAX_TEMP), PT_NEUT);
						sim->kill_part(idr);
					}
					break;
#else
				case PT_DEUT:
					if (RNG::Ref().chance(pressureFactor+1, 1000))
					{
						create_part(idr, x+rx, y+ry, PT_NEUT);
						parts[idr].vx = 0.25f*parts[idr].vx + parts[i].vx;
						parts[idr].vy = 0.25f*parts[idr].vy + parts[i].vy;
						parts[idr].life --;
						parts[idr].temp = restrict_flt(parts[idr].temp + parts[idr].life*17.0f, MIN_TEMP, MAX_TEMP);
						pv[y/CELL][x/CELL] += 6.0f * CFDS;

					}
					break;
#endif
				case PT_GUNP:
					if (RNG::Ref().chance(3, 200))
						sim->part_change_type(idr,x+rx,y+ry,PT_DUST);
					break;
				case PT_DYST:
					if (RNG::Ref().chance(3, 200))
						sim->part_change_type(idr,x+rx,y+ry,PT_YEST);
					break;
				case PT_YEST:
					sim->part_change_type(idr,x+rx,y+ry,PT_DYST);
					break;
				case PT_PLEX:
					if (RNG::Ref().chance(3, 200))
						sim->part_change_type(idr,x+rx,y+ry,PT_GOO);
					break;
				case PT_NITR:
					if (RNG::Ref().chance(3, 200))
						sim->part_change_type(idr,x+rx,y+ry,PT_DESL);
					break;
				case PT_PLNT:
					if (RNG::Ref().chance(1, 20))
						sim->create_part(idr, x+rx, y+ry, PT_WOOD);
					break;
				case PT_DESL:
				case PT_OIL:
					if (RNG::Ref().chance(3, 200))
						sim->part_change_type(idr,x+rx,y+ry,PT_GAS);
					break;
				case PT_COAL:
					if (RNG::Ref().chance(1, 20))
						sim->create_part(idr, x+rx, y+ry, PT_WOOD);
					break;
				case PT_BCOL:
					if (RNG::Ref().chance(1, 20))
						sim->create_part(idr, x+rx, y+ry, PT_SAWD);
					break;
				case PT_DUST:
					if (RNG::Ref().chance(1, 20))
						sim->part_change_type(idr, x+rx, y+ry, PT_FWRK);
					break;
				case PT_FWRK:
					if (RNG::Ref().chance(1, 20))
						parts[idr].ctype = PT_DUST;
					break;
				case PT_ACID:
					if (RNG::Ref().chance(1, 20))
						sim->create_part(idr, x+rx, y+ry, PT_ISOZ);
					break;
				case PT_TTAN:
					if (RNG::Ref().chance(1, 20))
					{
						sim->kill_part(i);
						return 1;
					}
					break;
				case PT_EXOT:
					if (RNG::Ref().chance(1, 20))
						parts[idr].life = 1500;
					break;
				case PT_RFRG:
					if (RNG::Ref().chance(1, 2))
						sim->create_part(idr, x+rx, y+ry, PT_GAS);
					else
						sim->create_part(idr, x+rx, y+ry, PT_CAUS);
					break;
				default:
					break;
				}
			}


	return 0;
}



//#TPT-Directive ElementHeader Element_NEUT static int graphics(GRAPHICS_FUNC_ARGS)
int Element_NEUT::graphics(GRAPHICS_FUNC_ARGS){
	float alpha;
	alpha = sqrt(cpart->tmp2);
	*firea = 80 + int(9.0625f*alpha);
	*firer = 10;
	*fireg = 80;
	*fireb = 120;

	*pixel_mode |= FIRE_ADD;
	return 1;
}

//#TPT-Directive ElementHeader Element_NEUT static int DeutExplosion(Simulation * sim, int n, int x, int y, float temp, int t)
int Element_NEUT::DeutExplosion(Simulation * sim, int n, int x, int y, float temp, int t)//testing a new deut create part
{
	int i;
	n = (n/50);
	if (n < 1)
		n = 1;
	else if (n > 340)
		n = 340;

	for (int c = 0; c < n; c++)
	{
		i = sim->create_part(-3, x, y, t);
		if (i >= 0)
			sim->parts[i].temp = temp;
		else if (sim->pfree < 0)
			break;
	}
	sim->pv[y/CELL][x/CELL] += (6.0f * CFDS)*n;
	return 0;
}

Element_NEUT::~Element_NEUT() {}
