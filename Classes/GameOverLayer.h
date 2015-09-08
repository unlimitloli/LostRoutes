#ifndef __GAME_OVER_LAYER_H__
#define __GAME_OVER_LAYER_H__

#include "cocos2d.h"

class GameOverLayer : public cocos2d::Layer
{
private:
	int score;

public:
	GameOverLayer(int s);

	virtual bool init();
	virtual void onExit();

	static GameOverLayer *createWinScore(int s);
};

#endif