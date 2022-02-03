#include <stdio.h>
#include <assert.h>
#include <iostream>

#include <sstream>

#include "Game.h"
#include <System.h>
#include <GraphicsSystem.h>
#include <GraphicsBuffer.h>
#include <Font.h>
#include <FontManager.h>
#include <GraphicsBufferManager.h>
#include <InputSystem.h>
#include "GameMessageManager.h"
#include <Sprite.h>
#include "SpriteManager.h"
#include <Timer.h>
#include <DataRepository.h>
#include "PlayerMoveToMessage.h"
#include "CreateUnitMessage.h"
#include "DeleteUnitMessage.h"
#include "ExitGameMessage.h"
#include "ComponentManager.h"
#include "UnitManager.h"
#include "DataLoader.h"
#include "ExtraColors.h"

Game* gpGame = NULL;

Game::Game()
	:Trackable("game class")
	,mpSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpFontManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mpFont(NULL)
	,mShouldExit(false)
	,mBackgroundBufferID("")
	,mpMessageManager(NULL)
	,mpComponentManager(NULL)
	,mpUnitManager(NULL)
	,mpRepository(NULL)
	,mTimeLastFrame(0.0f)
{
	//For improved input responses
	mAButtonDown = false;
	mMButtonDown = false;
	mXButtonDown = false;
	mCollisionDetected = false;
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	mpRepository = new DataRepository;
	DataLoader loader("data.txt", mpRepository);

	//create and init GraphicsSystem
	mpSystem = new System();
	bool goodGraphics = mpSystem->init( mpRepository->getEntry(DataKeyEnum::SCREEN_WIDTH).getUIntVal(), mpRepository->getEntry(DataKeyEnum::SCREEN_HEIGHT).getUIntVal());
	if(!goodGraphics) 
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager(mpSystem->getGraphicsSystem());
	mpSpriteManager = new SpriteManager;
	mpFontManager = new FontManager;

	mpMessageManager = new GameMessageManager();

	UINT maxUnits = mpRepository->getEntry(DataKeyEnum::MAX_UNITS).getUIntVal();
	mpComponentManager = new ComponentManager(maxUnits);
	mpUnitManager = new UnitManager(maxUnits);

	//load buffers
	GraphicsBuffer* pBuff;
	//Load buffers through the data repository to get the correct file paths.
	pBuff = mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, mpRepository->getEntry(DataKeyEnum::WALLPAPER).getStringVal());
	pBuff = mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID, mpRepository->getEntry(DataKeyEnum::ARROW).getStringVal());
	pBuff = mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID, mpRepository->getEntry(DataKeyEnum::ENEMY_ARROW).getStringVal());
	pBuff = mpGraphicsBufferManager->loadBuffer(mTargetBufferID, mpRepository->getEntry(DataKeyEnum::TARGET).getStringVal());

	//load Font
	mpFont = mpFontManager->createAndManageFont(COUR_24_FONT_ID, "cour.ttf", 24);
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{

		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	Sprite* pArrowSprite = NULL;
	if( pPlayerBuffer != NULL )
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite( PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight() );
	}
	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}

	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}

	
	//setup units
	Unit* pUnit = NULL;

	if (pArrowSprite)
	{
		Vector2D center((int)getGraphicsSystem()->getDisplayWidth() / 2, (int)getGraphicsSystem()->getDisplayHeight() / 2);
		pUnit = mpUnitManager->createPlayerUnit(*pArrowSprite, false, PositionData(center, 0.0f));
		pUnit->setShowTarget(false);
		pUnit->setSteering(Steering::FACE, Vector2D((int)getGraphicsSystem()->getDisplayWidth() / 2, 700));
	}
	/*
	//create 2 enemies
	if (pEnemyArrow)
	{
		pUnit = mpUnitManager->createUnit(*pEnemyArrow, true, PositionData(Vector2D((float)GraphicsSystem::getDisplayWidth() - 1, 0.0f), 0.0f));
		pUnit->setShowTarget(true);
		pUnit->setSteering(Steering::SEEK, ZERO_VECTOR2D, PLAYER_UNIT_ID);

		pUnit = mpUnitManager->createUnit(*pEnemyArrow, true, PositionData(Vector2D(0.0f, (float)GraphicsSystem::getDisplayHeight() - 1 - 1), 0.0f));
		pUnit->setShowTarget(false);
		pUnit->setSteering(Steering::FLEE, ZERO_VECTOR2D, PLAYER_UNIT_ID);
	}
	*/

	mTargetFPS = mpRepository->getEntry(DataKeyEnum::TARGET_FPS).getUIntVal();
	mTargetElapsedTime = 1000.0f / mTargetFPS;

	return true;
}

void Game::cleanup()
{
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	delete mpFontManager;
	mpFontManager = NULL;

	delete mpRepository;
	mpRepository = NULL;

	mpFont = NULL;

	//delete the graphics system
	delete mpSystem;
	mpSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpMessageManager;
	mpMessageManager = NULL;
	delete mpUnitManager;
	mpUnitManager = NULL;
	delete mpComponentManager;
	mpComponentManager = NULL;
}

void Game::doLoop()
{
	//game loop
	while (!mShouldExit)
	{
		gpPerformanceTracker->clearTracker("loop");
		gpPerformanceTracker->startTracking("loop");

		gpGame->beginLoop();

		gpPerformanceTracker->clearTracker("process");
		gpPerformanceTracker->startTracking("process");

		gpGame->processLoop();

		gpPerformanceTracker->stopTracking("process");

		gpGame->endLoop();

		gpPerformanceTracker->stopTracking("loop");
		//std::cout << "loop took:" << gpPerformanceTracker->getElapsedTime("loop") << "ms    ";
		//std::cout << "processing took:" << gpPerformanceTracker->getElapsedTime("process") << "ms\n";
		mTimeLastFrame = (float)gpPerformanceTracker->getElapsedTime("loop");
	}
}

void Game::beginLoop()
{
	mpLoopTimer->start();
}

void Game::processLoop()
{
	InputSystem* pInputSystem = mpSystem->getInputSystem();

	float dt = (mTargetElapsedTime * mTimeMult) / 1000.0f;
	mpUnitManager->updateAll(dt);
	mpComponentManager->update(dt);
	pInputSystem->update(dt);
	
	//draw background
	//GraphicsSystem::renderFilledRect(*GraphicsSystem::getBackBuffer(), ZERO_VECTOR2D, GraphicsSystem::getDisplayWidth(), GraphicsSystem::getDisplayHeight(), BACKGROUND_BLUE_COLOR);

	Sprite* pBackgroundSprite = mpSpriteManager->getSprite(BACKGROUND_SPRITE_ID);
	GraphicsSystem::draw(Vector2D(0, 0), *pBackgroundSprite);

	//draw units
	mpUnitManager->drawAll();

	if (mDrawDebugData)
	{
		drawDebugData();
	}

	std::stringstream textStream;
	textStream << mCollisions;

	GraphicsSystem::writeText(Vector2D(200, 20), *mpFont, BLACK_COLOR, "Collisions: " + textStream.str(), Font::RIGHT);

	textStream.str("");
	textStream.clear();

	GraphicsSystem::writeText(Vector2D(270, 40), *mpFont, BLACK_COLOR, "Max Collisions: 15", Font::RIGHT);

	textStream.str("");
	textStream.clear();

	//std::cout << mCollisions << std::endl;

	//DRAW LINE BEFORE FLIP

	mpSystem->getGraphicsSystem()->flip();

	Vector2D pos = pInputSystem->getCurrentMousePos();

	mpMessageManager->processMessagesForThisframe();

	bool newCollision = mpUnitManager->countCollisions();
	if (newCollision && !mCollisionDetected) 
	{
		addCollision();
	}
	mCollisionDetected = newCollision;

	if (mCollisions >= 15) 
	{
		GameMessage* pMessage = new ExitGameMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	if(pInputSystem->isMouseButtonPressed(InputSystem::LEFT))
	{
		//Use ArriveAndFaceSteering to move player unit to clicked location
		GameMessage* pMessage = new PlayerMoveToMessage( pos );
		MESSAGE_MANAGER->addMessage( pMessage, 0 );
	}

	if(pInputSystem->isKeyPressed(InputSystem::ESCAPE_KEY))
	{
		//Exit the game
		//ADD TO MESSANGER SYSTEM???
		//mShouldExit = true;
		GameMessage* pMessage = new ExitGameMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	bool newMButtonDown = pInputSystem->isKeyPressed(InputSystem::M_KEY);
	if (newMButtonDown && !mMButtonDown)
	{
		//Add 5 units at random locations
		//Run WanderAndChaseSteering
		//Made number created be data driven (DataRepository) - DO THIS
		for (int i = 0; i < mpRepository->getEntry(DataKeyEnum::SPAWN_COUNT).getFloatVal(); i++)
		{
			GameMessage* pMessage = new CreateUnitMessage;
			MESSAGE_MANAGER->addMessage(pMessage, i);
		}
	}
	mMButtonDown = newMButtonDown;
	/*
	if (pInputSystem->isKeyPressed(InputSystem::UP_KEY))
	{
		GameMessage* pMessage = new CreateUnitMessage;
		MESSAGE_MANAGER->addMessage(pMessage);
	}
	*/

	bool newXButtonDown = pInputSystem->isKeyPressed(InputSystem::X_KEY);
	if (newXButtonDown && !mXButtonDown)
	{
		//Delete random unit that is not the player unit
		//ADD TO MESSANGER SYSTEM
		//mpUnitManager->deleteRandomUnit();
		GameMessage* pMessage = new DeleteUnitMessage;
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	mXButtonDown = newXButtonDown;

	if (pInputSystem->isKeyPressed(InputSystem::LEFT_KEY))
	{
		mTimeMult -= 0.1f;
		if (mTimeMult < 0.0f)
			mTimeMult = 0.0f;
	}
	else if (pInputSystem->isKeyPressed(InputSystem::RIGHT_KEY))
	{
		mTimeMult += 0.1f;
	}
	else if (pInputSystem->isKeyPressed(InputSystem::SPACE_KEY))
	{
		mTimeMult = 1.0f;
	}


	if (pInputSystem->isKeyPressed(InputSystem::D_KEY))
	{
		mDrawDebugData = true;
	}
	else
	{
		mDrawDebugData = false;
	}

	/*
	//Check F key input to determine if units should seek or flee from the player unit.
	if (pInputSystem->isKeyPressed(InputSystem::F_KEY)) 
	{
		mpUnitManager->switchSteering(true);
	}
	else
	{
		mpUnitManager->switchSteering(false);
	}
	*/

	bool newAButtonDown = pInputSystem->isKeyPressed(InputSystem::A_KEY);
	if (newAButtonDown && !mAButtonDown) 
	{
		//Add a unit at a random location on screen
		//Run the WanderAndChaseSteering
		GameMessage* pMessage = new CreateUnitMessage;
		MESSAGE_MANAGER->addMessage(pMessage);
	}
	mAButtonDown = newAButtonDown;
}

void Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( mTargetElapsedTime );
}

void Game::drawDebugData()
{
	InputSystem* pInputSystem = mpSystem->getInputSystem();

	Vector2D pos = pInputSystem->getCurrentMousePos();

	//create mouse text
	std::stringstream textStream;
	textStream << pos;

	//write text at mouse position
	GraphicsSystem::writeText(pos, *mpFont, BLACK_COLOR, textStream.str(), Font::RIGHT);

	textStream.str("");
	textStream.clear();

	//write out current number of Units
	Uint32 numUnits = mpUnitManager->getNumUnits();
	textStream << "Units:" << numUnits;
	GraphicsSystem::writeText(Vector2D(GraphicsSystem::getDisplayWidth(), 0), *mpFont, BLACK_COLOR, textStream.str(), Font::RIGHT);

	textStream.str("");
	textStream.clear();

	//write out current fps
	int fps = (int)((1000.0f / mTimeLastFrame) + .5f);//+.5f does rounding
	textStream << "FPS:" << fps;
	GraphicsSystem::writeText(Vector2D(GraphicsSystem::getDisplayWidth() / 2, 0), *mpFont, BLACK_COLOR, textStream.str(), Font::CENTER);

	textStream.str("");
	textStream.clear();

	//write out time multiple
	textStream << "time mult:" << mTimeMult;
	GraphicsSystem::writeText(Vector2D(GraphicsSystem::getDisplayWidth() / 4, 0), *mpFont, BLACK_COLOR, textStream.str(), Font::CENTER);

}

GraphicsSystem* Game::getGraphicsSystem() const
{
	return mpSystem->getGraphicsSystem();
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}

