#pragma once
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"


class LearnBakkes : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	Vector gravity;
	Vector gravity_ball;
public:
	virtual void onLoad();
	virtual void onUnload();
	void OnChangeActive(std::string oldValue, CVarWrapper cvar);
	void UpdateGravity(std::string oldValue, CVarWrapper cvar);
	void ApplyGravity(std::string eventName);
};
