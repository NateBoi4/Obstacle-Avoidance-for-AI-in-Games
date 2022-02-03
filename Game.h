#pragma once

#include "Trackable.h"
#include "PerformanceTracker.h"
#include "Defines.h"
#include <string>

class System;
class GraphicsSystem;
class GraphicsBuffer;
class Font;
class FontManager;
class GraphicsBufferManager;
class SpriteManager;
class GameMessageManager;
class Timer;
class ComponentManager;
class UnitManager;
class DataRepository;

const IDType BACKGROUND_SPRITE_ID = 0;
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;
const IDType TARGET_SPRITE_ID = 3;

const IDType COUR_24_FONT_ID = 0;


class Game:public Trackable
{
public:
	Game();
	~Game();

	bool init();//returns true if no errors, false otherwise
	void cleanup();

	//game loop
	void doLoop();

	inline System* getSystem() const { return mpSystem; };
	inline GraphicsSystem* getGraphicsSystem() const;
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline FontManager* getFontManager() const { return mpFontManager; };
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline ComponentManager* getComponentManager() { return mpComponentManager; };
	inline UnitManager* getUnitManager() { return mpUnitManager; };
	inline DataRepository* getDataRepository() { return mpRepository; };
	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline void setRunning(bool exit) { mShouldExit = exit; };
	inline void addCollision() { mCollisions++; };

private:
	System* mpSystem;
	GraphicsBufferManager* mpGraphicsBufferManager;
	SpriteManager* mpSpriteManager;
	FontManager* mpFontManager;
	GameMessageManager* mpMessageManager;
	ComponentManager* mpComponentManager;
	UnitManager* mpUnitManager;
	DataRepository* mpRepository;
	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	Font* mpFont;
	float mTimeLastFrame;
	float mTimeMult = 1.0f;
	bool mDrawDebugData = false;
	bool mShouldExit;
	UINT mTargetFPS=0;
	float mTargetElapsedTime = 0.0f;
	int mCollisions = 0;

	//For improved input system
	bool mAButtonDown;
	bool mMButtonDown;
	bool mXButtonDown;
	bool mCollisionDetected;

	//should be somewhere else
	GraphicsBufferID mBackgroundBufferID = "woods";
	GraphicsBufferID mPlayerIconBufferID = "player";
	GraphicsBufferID mEnemyIconBufferID = "enemy";
	GraphicsBufferID mTargetBufferID = "target";

	void beginLoop();
	void processLoop();
	void endLoop();

	void drawDebugData();

};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge

extern Game* gpGame;
extern PerformanceTracker* gpPerformanceTracker;

