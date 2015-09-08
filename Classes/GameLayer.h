#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"
#include "BaseLayer.h"
#include "Fighter.h"
#include "Enemy.h"

typedef enum
{
	EnemyStone_Score = 5,
	Enemy1_Score = 10,
	Enemy2_Score = 15,
	EnemyPlanet_Score = 20
} EnemyScore;

class GameLayer : public cocos2d::Layer
{
private:
	Fighter *fighter;
	cocos2d::Menu *menu;
	int score;
	int scorePlaceholder;

	cocos2d::EventListenerTouchOneByOne *touchFighter;

public:
	static cocos2d::Scene *createScene();

	virtual bool init();

	void initBG();

	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

	void menuPauseCallBack(cocos2d::Ref *pSender);
	void menuBackCallBack(cocos2d::Ref *pSender);
	void menuResumeCallBack(cocos2d::Ref *pSender);

	void shootButtle(float dt);

	void bulleHitEnemy(Enemy *enemy);
	void fighterHitEnemy(Enemy *enemy);
	void updateStatusBarFighter();
	void updateStatusBarScore();

	CREATE_FUNC(GameLayer);
};

#endif