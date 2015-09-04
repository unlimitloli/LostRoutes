#ifndef __HOME_MENU_H__
#define __HOME_MENU_H__

#include "cocos2d.h"

typedef enum
{
	MenuItemStart,
	MenuItemSetting,
	MenuItemHelp
} ActionType;

class HomeMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene *createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();

	void menuItemCallBack(cocos2d::Ref *pSender);
	CREATE_FUNC(HomeMenu);
};



#endif
