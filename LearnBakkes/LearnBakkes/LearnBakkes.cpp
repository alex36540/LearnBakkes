#include "LearnBakkes.h"
#define _USE_MATH_DEFINES
#include <math.h>

BAKKESMOD_PLUGIN(LearnBakkes, "Learning Bakkes", "1.0", PLUGINTYPE_FREEPLAY)

void LearnBakkes::onLoad()
{
	cvarManager->registerCvar("gravity_on", "0", "Determines if custom gravity is on.", true, true, 0, true, 1, false).addOnValueChanged(std::bind(&LearnBakkes::OnChangeActive, this, std::placeholders::_1, std::placeholders::_2));;
	cvarManager->registerCvar("gravity_strength", "650", "Strength of custom gravity.", true, true, 0.0f, true, 5000.0f, true).addOnValueChanged(std::bind(&LearnBakkes::UpdateGravity, this, std::placeholders::_1, std::placeholders::_2));;
	cvarManager->registerCvar("gravity_azimuth", "0", "Gravity direction azimuth angle", true, true, 0.0f, true, 360.0f, true).addOnValueChanged(std::bind(&LearnBakkes::UpdateGravity, this, std::placeholders::_1, std::placeholders::_2));;
	cvarManager->registerCvar("gravity_polar", "180", "Gravity direction polar angle", true, true, 0.0f, true, 180.0f, true).addOnValueChanged(std::bind(&LearnBakkes::UpdateGravity, this, std::placeholders::_1, std::placeholders::_2));;
	cvarManager->getCvar("gravity_strenth").notify();
}

void LearnBakkes::onUnload()
{
}

void LearnBakkes::OnChangeActive(std::string oldValue, CVarWrapper cvar)
{
	if (oldValue.compare("0") == 0 && cvar.getBoolValue())
	{
		gameWrapper->HookEvent("Function TAGame.Car_TA.SetVehicleInput", bind(&LearnBakkes::ApplyGravity, this, std::placeholders::_1));
		cvarManager->getCvar("sv_soccar_gravity").setValue(0.000001f);
	}
	else if (oldValue.compare("1") == 0 && !cvar.getBoolValue())
	{
		gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");
		cvarManager->getCvar("sv_soccar_gravity").setValue(-650.0f);
	}
}

void LearnBakkes::UpdateGravity(std::string oldValue, CVarWrapper cvar)
{
	float tickrate = 120.0f;
	float r = cvarManager->getCvar("gravity_strength").getFloatValue() / tickrate;
	float theta = cvarManager->getCvar("gravity_polar").getFloatValue() * M_PI / 180.0f;
	float phi = cvarManager->getCvar("gravity_azimuth").getFloatValue() * M_PI / 180.0f;
	float x = r * sin(theta) * cos(phi);
	float y = r * sin(theta) * sin(phi);
	float z = r * cos(theta);
	gravity = { x, y, z };
}

void LearnBakkes::ApplyGravity(std::string eventName)
{
	if (!gameWrapper->IsInGame())
		return;
	auto server = gameWrapper->GetGameEventAsServer();

	auto cars = server.GetCars();
	auto car = cars.Get(0);

	auto velocity = car.GetVelocity();
	car.AddVelocity(gravity);
	
}

