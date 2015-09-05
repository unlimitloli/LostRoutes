#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"
#include "BaseLayer.h"
#include "Fighter.h"

class GameLayer : public cocos2d::Layer
{
private:
	Fighter *fighter;

	cocos2d::EventListenerTouchOneByOne *touchFighter;

public:
	static cocos2d::Scene *createScene();

	virtual bool init();

	void initBG();

	virtual void onEnter();
	virtual void onExit();

	CREATE_FUNC(GameLayer);
};

#endif