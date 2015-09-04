#include "HelpLayer.h"
#include "SystemHeader.h"

USING_NS_CC;

Scene *HelpLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = HelpLayer::create();
	scene->addChild(layer);
	return scene;
}

bool HelpLayer::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto top = Sprite::createWithSpriteFrameName("help-top.png");
	top->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - top->getContentSize().height / 2));
	addChild(top);

	return true;
}
