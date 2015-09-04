#ifndef __SETTING_LAYER_H__
#define __SETTING_LAYER_H__

#include "cocos2d.h"
#include "BaseLayer.h"

class SettingLayer : public BaseLayer
{
public:
	static cocos2d::Scene *createScene();
	virtual bool init();

	void menuMusicCallBack(cocos2d::Ref *pSender);
	void menuSoudnCallBack(cocos2d::Ref *pSender);

	CREATE_FUNC(SettingLayer);
};

#endif