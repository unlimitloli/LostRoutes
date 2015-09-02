#include "BaseLayer.h"
#include "MyUtility.h"

USING_NS_CC;

Scene* BaseLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = BaseLayer::create();

	scene->addChild(layer);
	return scene;
}

bool BaseLayer::init()
{
	if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	string str = MyUtility::getUTF8Char("lblMusic");

	auto label = Label::createWithTTF(str, "fonts/STLITI.ttf", 24);
	label->setPosition(100, 100);

	addChild(label);
}