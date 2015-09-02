#ifndef __BASELAYER_H__
#define __BASELAYER_H__

#include "cocos2d.h"

class BaseLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(BaseLayer);
};

#endif