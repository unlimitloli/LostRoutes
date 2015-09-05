#ifndef __FIGHTER_H__
#define __FIGHTER_H__

#include "cocos2d.h"

class Fighter : public cocos2d::Sprite
{
	CC_SYNTHESIZE(int, hitPoints, HitPoints);

public:
	void setPosition(const cocos2d::Vec2 &newPosistion);
	static Fighter *createWithSpriteFrameName(const char *spriteFrmaeName);
};

#endif