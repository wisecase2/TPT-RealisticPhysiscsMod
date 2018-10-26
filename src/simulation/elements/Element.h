#ifndef ELEMENTCLASS_H
#define ELEMENTCLASS_H

#include "graphics/Pixel.h"
#include "simulation/Simulation.h"
#include "simulation/Elements.h"
#include "simulation/StructProperty.h"

class Simulation;
class Renderer;
class VideoBuffer;
struct Particle;
class Element
{
public:
	ByteString Identifier;
	ByteString Name;
	pixel Colour;
	int MenuVisible;
	int MenuSection;
	int Enabled;

	float Advection;
	float AirDrag;
	float AirLoss;
	float Loss;
	float Collision;
	float Gravity;
	float Diffusion;
	float HotAir;
	int Falldown;
	int Flammable;
	int Explosive;
	int Meltable;
	int Hardness;
	// Photon wavelengths are ANDed with this value when a photon hits an element, meaning that only wavelengths present in both this value and the original photon will remain in the reflected photon
	unsigned int PhotonReflectWavelengths;
	int Weight;
	float Temperature;
	unsigned char HeatConduct;
	String Description;
	unsigned int Properties;

	float LowPressure;
	int LowPressureTransition;
	float HighPressure;
	int HighPressureTransition;
	float LowTemperature;
	int LowTemperatureTransition;
	float HighTemperature;
	int HighTemperatureTransition;
	
	// new transitions
	float GasTemperaturetransition = MAX_TEMP+1.f;
	int GasTransition = -1;
	float PlsmTemperaturetransition = -1.f;
	float solidtransition = 0;
	
	// heat latent transition
	float Liquidtransition = -1.f;
	float SolidLiquidlatent = 0.f;
	float LiquidGaslatent = 0.f;
	float GasPlsmlatent = 5000.f; // default
	
    //heat capacity not used yet
	float Heatcapacity = 1.f;
	float InvHeatcapacity = 1.f;
	
	// broken state
	float pressureresistance = 0.f;
	bool pressureblock = false;
	bool defaultbreak = false;

	//radioactive
	float radabsorb = 0;

	bool specialupdate = false;


	int (*Update) (UPDATE_FUNC_ARGS);
	int (*Graphics) (GRAPHICS_FUNC_ARGS);
	VideoBuffer * (*IconGenerator)(int, int, int);

	Element();
	virtual ~Element() {}
	static int defaultGraphics(GRAPHICS_FUNC_ARGS);
	static int legacyUpdate(UPDATE_FUNC_ARGS);

	/** Returns a list of properties, their type and offset within the structure that can be changed
	 by higher-level processes referring to them by name such as Lua or the property tool **/
	static std::vector<StructProperty> GetProperties();
};

#endif
