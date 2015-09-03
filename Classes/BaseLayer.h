#ifndef __BASELAYER_H__
#define __BASELAYER_H__

#include "cocos2d.h"

class BaseLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

	void menuBackCallback(cocos2d::Ref *pSender);

	CREATE_FUNC(BaseLayer);
};

#endif