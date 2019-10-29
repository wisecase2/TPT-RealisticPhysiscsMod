#include <cmath>
#include <algorithm>
#include "Config.h"
#include "Air.h"
#include "Simulation.h"
//#include <powder.h>
//#include <defines.h>
#include "Gravity.h"
#include "common/tpt-rand.h"

/*float kernel[9];

float vx[YRESC][XRESC], ovx[YRESC][XRESC];
float vy[YRESC][XRESC], ovy[YRESC][XRESC];
float pv[YRESC][XRESC], opv[YRESC][XRESC];
unsigned char bmap_blockair[YRESC][XRESC];

float cb_vx[YRESC][XRESC];
float cb_vy[YRESC][XRESC];
float cb_pv[YRESC][XRESC];
float cb_hv[YRESC][XRESC];

float fvx[YRESC][XRESC], fvy[YRESC][XRESC];

float hv[YRESC][XRESC], ohv[YRESC][XRESC]; // For Ambient Heat */

void Air::make_kernel(void) //used for velocity
{
	int i, j;
	float s = 0.0f;
	for (j = -1; j < 2; j++) {
		for (i = -1; i < 2; i++)
		{
			kernel[(i + 1) + 3 * (j + 1)] = expf(-2.0f*(i*i + j * j));
			s += kernel[(i + 1) + 3 * (j + 1)];
		}
	}
	s = 1.0f / s;
	for (j = -1; j < 2; j++) {
		for (i = -1; i < 2; i++) {
			kernel[(i + 1) + 3 * (j + 1)] *= s;
		}
	}

	boundary_coeffs[0] = 0.6f;
	boundary_coeffs[1] = 0.8f;
	boundary_coeffs[2] = 0.9f;
	boundary_coeffs[3] = 0.95f;
	boundary_coeffs[4] = 0.975f;
}

void Air::Clear()
{
	std::fill(&pv[0][0], &pv[0][0]+((XRESC)*(YRESC)), 0.0f);
	std::fill(&vy[0][0], &vy[0][0]+((XRESC)*(YRESC)), 0.0f);
	std::fill(&vx[0][0], &vx[0][0]+((XRESC)*(YRESC)), 0.0f);
}

void Air::ClearAirH()
{
	std::fill(&hv[0][0], &hv[0][0]+((XRESC)*(YRESC)), ambientAirTemp);
	std::fill(&sim.dthv[0][0], &sim.dthv[0][0] + ((XRESC)*(YRESC)), ambientAirTemp);
}

void Air::update_airh(void)
{
	int x, y, i, j, kx, ky;
	float odh, dh, dx, dy, f, tx, ty;
	
	for (i=0; i<YRESC; i++) //reduces pressure/velocity on the edges every frame
	{
		for (int k = 0; k < 4; k++) {
			if (i >= k && i < YRESC - k) {
				hv[i][k] = (hv[i][k] - ambientAirTemp) * boundary_coeffs[k+1] + ambientAirTemp;
				hv[i][XRESC - k - 1] = (hv[i][XRESC - k - 1] - ambientAirTemp) * boundary_coeffs[k+1] + ambientAirTemp;
			}
		}
		/*
		hv[i][0] = (hv[i][0] - ambientAirTemp) * 0.8f + ambientAirTemp;
		hv[i][XRESC - 1] = (hv[i][XRESC - 1] - ambientAirTemp) * 0.8f + ambientAirTemp;

		if (i >= 1 && i < YRESC - 1) {
			hv[i][1] = (hv[i][1] - ambientAirTemp) * 0.9f + ambientAirTemp;
			hv[i][XRESC - 2] = (hv[i][XRESC - 2] - ambientAirTemp) * 0.9f + ambientAirTemp;
		}

		if (i >= 2 && i < YRESC - 2) {
			hv[i][2] = (hv[i][2] - ambientAirTemp) * 0.95f + ambientAirTemp;
			hv[i][XRESC - 3] = (hv[i][XRESC - 3] - ambientAirTemp) * 0.95f + ambientAirTemp;
		}

		if (i >= 3 && i < YRESC - 3) {
			hv[i][3] = (hv[i][3] - ambientAirTemp) * 0.975f + ambientAirTemp;
			hv[i][XRESC - 4] = (hv[i][XRESC - 4] - ambientAirTemp) * 0.975f + ambientAirTemp;
		}
		*/
	}
	for (i=0; i<XRESC; i++) //reduces pressure/velocity on the edges every frame
	{
		for (int k = 0; k < 4; k++) {
			if (i >= k && i < XRESC - k) {
				hv[k][i] = (hv[k][i] - ambientAirTemp) * boundary_coeffs[k + 1] + ambientAirTemp;
				hv[YRESC - k - 1][i] = (hv[YRESC - k - 1][i] - ambientAirTemp) * boundary_coeffs[k + 1] + ambientAirTemp;
			}
		}
		/*
		hv[0][i] = (hv[0][i] - ambientAirTemp) * 0.8f + ambientAirTemp;
		hv[YRESC - 1][i] = (hv[YRESC - 1][i] - ambientAirTemp) * 0.8f + ambientAirTemp;

		if (i >= 1 && i < XRESC - 1) {
			hv[1][i] = (hv[1][i] - ambientAirTemp) * 0.9f + ambientAirTemp;
			hv[YRESC - 2][i] = (hv[YRESC - 2][i] - ambientAirTemp) * 0.9f + ambientAirTemp;
		}

		if (i >= 2 && i < XRESC - 2) {
			hv[2][i] = (hv[2][i] - ambientAirTemp) * 0.95f + ambientAirTemp;
			hv[YRESC - 3][i] = (hv[YRESC - 3][i] - ambientAirTemp) * 0.95f + ambientAirTemp;
		}

		if (i >= 3 && i < XRESC - 3) {
			hv[3][i] = (hv[3][i] - ambientAirTemp) * 0.975f + ambientAirTemp;
			hv[YRESC - 4][i] = (hv[YRESC - 4][i] - ambientAirTemp) * 0.975f + ambientAirTemp;
		}
		*/
	}
	
	for (y=0; y<YRESC; y++) //update velocity and pressure
	{
		for (x=0; x<XRESC; x++)
		{
			dh = 0.0f;
			dx = 0.0f;
			dy = 0.0f;
			for (j=-1; j<2; j++)
			{
				for (i=-1; i<2; i++)
				{
					f = kernel[i + 1 + (j + 1) * 3];
					if (y+j>0 && y+j<YRESC-2 &&
					        x+i>0 && x+i<XRESC-2)
						{
						if (!(bmap_blockairh[y + j][x + i] & 0x8)) {
							dh += hv[y + j][x + i] * f;
							dx += vx[y + j][x + i] * f;
							dy += vy[y + j][x + i] * f;
						}
						else {
							dh += hv[y][x] * f;
							dx += vx[y][x] * f;
							dy += vy[y][x] * f;
						}
					}
					else {
						dh += ambientAirTemp*f;
					}
					/*else
					{
						f = kernel[i+1+(j+1)*3];
						dh += hv[y][x]*f;
						dx += vx[y][x]*f;
						dy += vy[y][x]*f;
					}*/
				}
			}
			tx = x - dx*0.2f;
			ty = y - dy*0.2f;
			i = (int)tx;
			j = (int)ty;
			//i = round(tx);
			//j = round(ty);

			tx -= i;
			ty -= j;
			if (i>=2 && i<XRESC-3 && j>=2 && j<YRESC-3)
			{
				odh = dh;
				dh *= 1.0f - AIR_VADV;
				dh += AIR_VADV*(1.0f-tx)*(1.0f-ty)*((bmap_blockairh[j][i]&0x8) ? odh : hv[j][i]);
				
				//kx = (tx >= 0) ? 1 : -1;
				//ky = (ty >= 0) ? 1 : -1;
				//tx = abs(tx);
				//ty = abs(ty);
				/*
				dh += AIR_VADV * tx*(1.0f - ty)*((bmap_blockairh[j][i + kx] & 0x8) ? odh : hv[j][i + kx]);
				dh += AIR_VADV * (1.0f - tx)*ty*((bmap_blockairh[j + ky][i] & 0x8) ? odh : hv[j + ky][i]);
				dh += AIR_VADV * tx*ty*((bmap_blockairh[j + ky][i + kx] & 0x8) ? odh : hv[j + ky][i + kx]);
				*/
				
				dh += AIR_VADV*tx*(1.0f-ty)*((bmap_blockairh[j][i+1]&0x8) ? odh : hv[j][i+1]);
				dh += AIR_VADV*(1.0f-tx)*ty*((bmap_blockairh[j+1][i]&0x8) ? odh : hv[j+1][i]);
				dh += AIR_VADV*tx*ty*((bmap_blockairh[j+1][i+1]&0x8) ? odh : hv[j+1][i+1]);
				
			}
			if(!sim.gravityMode)
			{ //Vertical gravity only for the time being
				float airdiff = hv[y-1][x]-hv[y][x];
				if(airdiff>0 && !(bmap_blockairh[y-1][x]&0x8))
					vy[y][x] -= airdiff/50000.0f;
			}
			ohv[y][x] = dh;
		}
	}
	memcpy(hv, ohv, sizeof(hv));
}

void Air::update_air(void)
{
	int x = 0, y = 0, i = 0, j = 0, k = 0, kx, ky;
	float dp = 0.0f, dx = 0.0f, dy = 0.0f, f = 0.0f, tx = 0.0f, ty = 0.0f, vv = 0.0f;
	const float advDistanceMult = 0.6f;
	float stepX, stepY, norm;
	int stepLimit, step;

	if (airMode != 4) { //airMode 4 is no air/pressure update
		for (i=0; i<YRESC; i++) //reduces pressure/velocity on the edges every frame
		{
			for (k = 0; k < 4; k++) {
				if (i >= k && i < YRESC - k) {
					pv[i][k] *= boundary_coeffs[k];
					pv[i][XRESC - k - 1] *= boundary_coeffs[k];

					vx[i][k] *= boundary_coeffs[k+1];
					vx[i][XRESC - k - 1] *= boundary_coeffs[k+1];
					vy[i][k] *= boundary_coeffs[k+1];
					vy[i][XRESC - k - 1] *= boundary_coeffs[k+1];
				}
			}
			/*
			//part 1 
			pv[i][0] *= 0.6f;
			pv[i][XRESC-1] *= 0.6f;

			vx[i][0] *= 0.8f;
			vx[i][XRESC - 1] *= 0.8f;
			vy[i][0] *= 0.8f;
			vy[i][XRESC - 1] *= 0.8f;
			
			//part 2
			if (i >= 1 && i < YRESC - 1) {
				pv[i][1] *= 0.8f;
				pv[i][XRESC- 2] *= 0.8f;

				vx[i][1] *= 0.9f;
				vx[i][XRESC - 2] *= 0.9f;
				vy[i][1] *= 0.9f;
				vy[i][XRESC - 2] *= 0.9f;
			}

			//part 3
			if (i >= 2 && i < YRESC - 2) {
				pv[i][2] *= 0.9f;
				pv[i][XRESC - 3] *= 0.9f;

				vx[i][2] *= 0.95f;
				vx[i][XRESC - 3] *= 0.95f;
				vy[i][2] *= 0.95f;
				vy[i][XRESC - 3] *= 0.95f;
			}

			//part 3
			if (i >= 3 && i < YRESC - 3) {
				pv[i][3] *= 0.95f;
				pv[i][XRESC - 4] *= 0.95f;

				vx[i][3] *= 0.975f;
				vx[i][XRESC - 4] *= 0.975f;
				vy[i][3] *= 0.975f;
				vy[i][XRESC - 4] *= 0.975f;
			}
			*/
		}
		for (i=0; i<XRESC; i++) //reduces pressure/velocity on the edges every frame
		{
			for (k = 0; k < 4; k++) {
				if (i >= k && i < XRESC - k) {
					pv[k][i] *= boundary_coeffs[k];
					pv[YRESC - k - 1][i] *= boundary_coeffs[k];

					vx[k][i] *= boundary_coeffs[k + 1];
					vx[YRESC - k - 1][i] *= boundary_coeffs[k + 1];
					vy[k][i] *= boundary_coeffs[k + 1];
					vy[YRESC - k - 1][i] *= boundary_coeffs[k + 1];
				}
			}
			/*
			//part 1
			pv[0][i] *= 0.6f;
			pv[YRESC - 1][i] *= 0.6f;

			vx[0][i] *= 0.8f;
			vx[YRESC - 1][i] *= 0.8f;
			vy[0][i] *= 0.8f;
			vy[YRESC - 1][i] *= 0.8f;
			
			//part 2
			if (i >= 1 && i < XRESC - 1) {
				pv[1][i] *= 0.8f;
				pv[YRESC - 2][i] *= 0.8f;

				vx[1][i] *= 0.9f;
				vx[YRESC - 2][i] *= 0.9f;
				vy[1][i] *= 0.9f;
				vy[YRESC - 2][i] *= 0.9f;
			}

			//part 3
			if (i >= 2 && i < XRESC - 2) {
				pv[2][i] *= 0.9f;
				pv[YRESC - 3][i] *= 0.9f;

				vx[2][i] *= 0.95f;
				vx[YRESC - 3][i] *= 0.95f;
				vy[2][i] *= 0.95f;
				vy[YRESC - 3][i] *= 0.95f;
			}

			//part 4
			if (i >= 3 && i < XRESC - 3) {
				pv[3][i] *= 0.95f;
				pv[YRESC - 4][i] *= 0.95f;

				vx[3][i] *= 0.975f;
				vx[YRESC - 4][i] *= 0.975f;
				vy[3][i] *= 0.975f;
				vy[YRESC - 4][i] *= 0.975f;
			}
			*/
		}
		
		for (j=0; j<YRESC; j++) //clear some velocities near walls
		{
			for(i = 0; i < XRESC; i++){
				if(bmap_blockair[j][i]){
					//set 0 vx left and right
					if(i > 0){
						vx[j][i - 1] = 0.0f;
					}
					vx[j][i] = 0.0f;
					if(i < (XRESC - 1)){
						vx[j][i + 1] = 0.0f;
					}
					//set 0 vy up and bottom
					if(j > 0){
						vy[j - 1][i] = 0.0f;
					}
					vy[j][i] = 0.0f;
					if(j < (XRESC - 1)){
						vy[j + 1][i] = 0.0f;
					}
					//pv[j][i] = 0;
				}
			}
		} 
		for(y = 1; y < YRESC-1; y++){ //pressure adjustments from velocity
			for(x = 1; x < XRESC-1; x++){
				/*dp = 0.0f;
				if(x > 0){
					dp += vx[y][x - 1] - vx[y][x];
				}
				if((x + 1) < (XRESC)){
					dp += vx[y][x] - vx[y][x + 1];
				}
				if(y > 0){
					dp += vy[y - 1][x] - vy[y][x];
				}
				if((y + 1) < (YRESC)){
					dp += vy[y][x] - vy[y + 1][x];
				}*/

				dp = vx[y][x - 1] - vx[y][x + 1];
				dp += vy[y - 1][x] - vy[y + 1][x];

				//opv[y][x] = pv[y][x];
				//pv[y][x] *= AIR_PLOSS;
				pv[y][x] += dp * AIR_TSTEPP;
				sim.dthv[y][x] = hv[y][x];

				if(bmap_blockair[y][x]){
					pv[y][x] = 0;
				}
			}
		} 
		for (y=1; y<YRESC-1; y++) //velocity adjustments from pressure
			for (x=1; x<XRESC-1; x++)
			{
				//dx = dy = 0.0f;
				/*if(x + 1 != (XRESC)){
					dx += pv[y][x] - pv[y][x+1];
				}
				if(x - 1 != -1){
					dx += pv[y][x-1] - pv[y][x];
				}
				if(y + 1 != (YRESC)){
					dy += pv[y][x] - pv[y+1][x];
				}
				if(y - 1 != -1){
					dy += pv[y-1][x] - pv[y][x];
				}*/

				dx = pv[y][x - 1] - pv[y][x + 1];
				dy = pv[y - 1][x] - pv[y + 1][x];

				vx[y][x] *= AIR_VLOSS;
				vy[y][x] *= AIR_VLOSS;
				vx[y][x] += dx*AIR_TSTEPV;
				vy[y][x] += dy*AIR_TSTEPV;
				
				if (bmap_blockair[y][x] || (bmap_blockair[y][x+1] && x != (YRESC-1)) || (bmap_blockair[y][x-1] && x != 0))
					vx[y][x] = 0;
				if (bmap_blockair[y][x] || (bmap_blockair[y+1][x] && y != (YRESC-1)) || (bmap_blockair[y-1][x] && x != 0))
					vy[y][x] = 0;

			}
		for (y=0; y<YRESC; y++) //update velocity and pressure
			for (x=0; x<XRESC; x++)
			{
				dx = 0.0f;
				dy = 0.0f;
				dp = 0.0f;
				for (j = -1; j < 2; j++) {
					for (i = -1; i < 2; i++) {
						f = kernel[i + 1 + (j + 1) * 3];
						if (y + j > 0 && y + j < YRESC - 1 &&
							x + i>0 && x + i < XRESC - 1)
						{
							if (!bmap_blockair[y + j][x + i]) {
								dx += vx[y + j][x + i] * f;
								dy += vy[y + j][x + i] * f;
								dp += pv[y + j][x + i] * f;
							}
							else {
								dx += vx[y][x] * f;
								dy += vy[y][x] * f;
								dp += pv[y][x] * f;
							}
						}
						/*else
						{
							f = kernel[i+1+(j+1)*3];
							dx += vx[y][x]*f;
							dy += vy[y][x]*f;
							dp += pv[y][x]*f;
						}*/
					}
				}

				tx = x - dx*advDistanceMult;
				ty = y - dy*advDistanceMult;
				if ((dx*advDistanceMult>1.0f || dy*advDistanceMult>1.0f) && (tx>=2 && tx<XRESC-2 && ty>=2 && ty<YRESC-2))
				{
					// Trying to take velocity from far away, check whether there is an intervening wall. Step from current position to desired source location, looking for walls, with either the x or y step size being 1 cell
					if (std::abs(dx)>std::abs(dy))
					{
						stepX = (dx<0.0f) ? 1 : -1;
						stepY = -dy/fabsf(dx);
						stepLimit = (int)(fabsf(dx*advDistanceMult));
					}
					else
					{
						stepY = (dy<0.0f) ? 1 : -1;
						stepX = -dx/fabsf(dy);
						stepLimit = (int)(fabsf(dy*advDistanceMult));
					}
					tx = x;
					ty = y;
					for (step=0; step<stepLimit; ++step)
					{
						tx += stepX;
						ty += stepY;
						if (bmap_blockair[(int)(ty+0.5f)][(int)(tx+0.5f)])
						{
							tx -= stepX;
							ty -= stepY;
							break;
						}
					}
					if (step==stepLimit)
					{
						// No wall found
						tx = x - dx*advDistanceMult;
						ty = y - dy*advDistanceMult;
					}
				}
				//i = (int)tx;
				//j = (int)ty;
				i = round(tx);
				j = round(ty);

				tx -= i;
				ty -= j;
				if (!bmap_blockair[y][x] && i>=2 && i<=XRESC-3 &&
				        j>=2 && j<=YRESC-3)
				{
					dx *= 1.0f - AIR_VADV;
					dy *= 1.0f - AIR_VADV;

					kx = (tx >= 0) ? 1 : -1;
					ky = (ty >= 0) ? 1 : -1;
					tx = abs(tx);
					ty = abs(ty);

					dx += AIR_VADV * (1.0f - tx)*(1.0f - ty)*vx[j][i];
					dy += AIR_VADV * (1.0f - tx)*(1.0f - ty)*vy[j][i];

					dx += AIR_VADV * tx*(1.0f - ty)*vx[j][i + kx];
					dy += AIR_VADV * tx*(1.0f - ty)*vy[j][i + kx];

					dx += AIR_VADV * (1.0f - tx)*ty*vx[j + ky][i];
					dy += AIR_VADV * (1.0f - tx)*ty*vy[j + ky][i];

					dx += AIR_VADV * tx*ty*vx[j + ky][i + kx];
					dy += AIR_VADV * tx*ty*vy[j + ky][i + kx];

					/*
					dx += AIR_VADV*tx*(1.0f-ty)*vx[j][i+1];
					dy += AIR_VADV*tx*(1.0f-ty)*vy[j][i+1];

					dx += AIR_VADV*(1.0f-tx)*ty*vx[j+1][i];
					dy += AIR_VADV*(1.0f-tx)*ty*vy[j+1][i];

					dx += AIR_VADV*tx*ty*vx[j+1][i+1];
					dy += AIR_VADV*tx*ty*vy[j+1][i+1];
					*/
				}
				

				if (bmap[y][x] == WL_FAN)
				{
					dx += fvx[y][x];
					dy += fvy[y][x];
				}
				// pressure/velocity caps
				if (dp > 256.0f) dp = 256.0f;
				if (dp < -256.0f) dp = -256.0f;
				/*
				if (dx > 256.0f) dx = 256.0f;
				if (dx < -256.0f) dx = -256.0f;
				if (dy > 256.0f) dy = 256.0f;
				if (dy < -256.0f) dy = -256.0f;
				*/
				//limit velocity radius 256;
				norm = sqrt(dx*dx + dy * dy);
				norm = 256.f / norm;
				if (norm < 1) {
					dx *= norm;
					dy *= norm;
				}

				switch (airMode)
				{
				default:
				case 0:  //Default
					break;
				case 1:  //0 Pressure
					dp = 0.0f;
					break;
				case 2:  //0 Velocity
					dx = 0.0f;
					dy = 0.0f;
					break;
				case 3: //0 Air
					dx = 0.0f;
					dy = 0.0f;
					dp = 0.0f;
					break;
				case 4: //No Update
					break;
				}

				//if(bmap_blockair[y][x]){
				//	dp = 0;
				//}

				ovx[y][x] = dx;
				ovy[y][x] = dy;
				opv[y][x] = dp;
			}
		memcpy(vx, ovx, sizeof(vx));
		memcpy(vy, ovy, sizeof(vy));
		memcpy(pv, opv, sizeof(pv));
	}
}

void Air::Invert()
{
	int nx, ny;
	for (nx = 0; nx<XRESC; nx++)
		for (ny = 0; ny<YRESC; ny++)
		{
			pv[ny][nx] = -pv[ny][nx];
			vx[ny][nx] = -vx[ny][nx];
			vy[ny][nx] = -vy[ny][nx];
		}
}

// called when loading saves / stamps to ensure nothing "leaks" the first frame
void Air::RecalculateBlockAirMaps()
{
	for (int i = 0; i <= sim.parts_lastActiveIndex; i++)
	{
		int type = sim.parts[i].type;
		if (!type)
			continue;
		// Real TTAN would only block if there was enough TTAN
		// but it would be more expensive and complicated to actually check that
		// so just block for a frame, if it wasn't supposed to block it will continue allowing air next frame
		if (type == PT_TTAN)
		{
			int x = ((int)(sim.parts[i].x+0.5f))/CELL, y = ((int)(sim.parts[i].y+0.5f))/CELL;
			if (sim.InBounds(x, y))
			{
				bmap_blockair[y][x] = 1;
				bmap_blockairh[y][x] = 0x8;
			}
		}
		// mostly accurate insulator blocking, besides checking GEL
		else if ((type == PT_HSWC && sim.parts[i].life != 10) || sim.elements[type].HeatConduct <= (random_gen()%250))
		{
			int x = ((int)(sim.parts[i].x+0.5f))/CELL, y = ((int)(sim.parts[i].y+0.5f))/CELL;
			if (sim.InBounds(x, y) && !(bmap_blockairh[y][x]&0x8))
				bmap_blockairh[y][x]++;
		}
	}
}

Air::Air(Simulation & simulation):
	sim(simulation),
	airMode(0),
	ambientAirTemp(295.15f)
{
	//Simulation should do this.
	make_kernel();
	std::fill(&bmap_blockair[0][0], &bmap_blockair[0][0]+((XRESC)*(YRESC)), 0);
	std::fill(&bmap_blockairh[0][0], &bmap_blockairh[0][0]+((XRESC)*(YRESC)), 0);
	std::fill(&vx[0][0], &vx[0][0]+((XRESC)*(YRESC)), 0.0f);
	std::fill(&ovx[0][0], &ovx[0][0]+((XRESC)*(YRESC)), 0.0f);
	std::fill(&vy[0][0], &vy[0][0]+((XRESC)*(YRESC)), 0.0f);
	std::fill(&ovy[0][0], &ovy[0][0]+((XRESC)*(YRESC)), 0.0f);
	std::fill(&hv[0][0], &hv[0][0]+((XRESC)*(YRESC)), 0.0f);
	std::fill(&ohv[0][0], &ohv[0][0]+((XRESC)*(YRESC)), 0.0f);
	std::fill(&pv[0][0], &pv[0][0]+((XRESC)*(YRESC)), 0.0f);
	std::fill(&opv[0][0], &opv[0][0]+((XRESC)*(YRESC)), 0.0f);
}
