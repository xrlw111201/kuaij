//
//  ConfScene.cpp
//  kj
//
//  Created by liuhui on 15/12/11.
//
//
#include "ConfScene.h"
#include "ConfBtn.h"
#include "ContrlScene.h"
#include "Global.h"

Scene* ConfScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ConfScene::create();
    scene->addChild(layer);
    return scene;
}

bool ConfScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true); // not need
    listener->onTouchBegan = CC_CALLBACK_2(ConfScene::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    ConfBtn::s_pConfScene = this;
    
    bool rt = initBtn();
    return rt;
}

bool ConfScene::initBtn(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* bg = Sprite::create("conf_bg.png");
    bg->setPosition(visibleSize/2);
    
    bg->setTag(90);
    addChild(bg);
    
    //editbox
    Scale9Sprite* ss = Scale9Sprite::create("edit_box.png");
    ss->setPosition(visibleSize.width*0.5000, visibleSize.height*9331);
    ss->setTag(9000);
    //addChild(ss);
    Sprite* s = ss->getSprite();
    float sx = s->getTextureRect().getMaxX();
    float sy = s->getTextureRect().getMaxY();
    
    EditBox* peb = EditBox::create(Size(sx, sy), ss);
    peb->setPosition(Vec2(visibleSize.width*0.5000, visibleSize.height*0.9331));
    peb->setTag(9001);
    peb->setFont("Arial", 35);
    peb->setPlaceholderFont("Arial", 25);
    peb->setFontColor(Color3B::BLACK);
    peb->setPlaceholderFontColor (Color3B::GRAY);
    peb->setPlaceHolder("输入设备序列号");
    peb->setMaxLength(12);
    peb->setDelegate(this);
    addChild(peb);
    
    Label* lbl = Label::create();
    lbl->setString("");
    lbl->setTextColor(Color4B::WHITE);
    lbl->enableShadow();
    lbl->setSystemFontSize(25.00);
    lbl->setHorizontalAlignment(TextHAlignment::CENTER);
    lbl->setPosition(visibleSize.width*0.5000, visibleSize.height*0.2200);
    lbl->setTag(9002);
    addChild(lbl);
    
    for(int i=0; i<5; i++){
        ConfBtn* pb = ConfBtn::create();
        pb->setTag(i+500);
        pb->setProperties(i+500);
        addChild(pb, 100);
    }
    
    for(int i=0; i<5; i++){
        ConfBtn* pb = ConfBtn::create();
        pb->setTag(i+600);
        pb->setProperties(i+600);
        addChild(pb, 100);
    }
    //read sn
    string documentPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    string fileName = "kjSn";
    m_strSnPath = documentPath + fileName;
    
    FILE* fp = fopen(m_strSnPath.c_str(), "r");
    if(NULL != fp){
        fseek(fp, 0, SEEK_END);
        long nLen = ftell(fp);
        if(12 == nLen){
            char* pBuf = new char[nLen+1];
            memset(pBuf, 0, nLen+1);
            rewind(fp);
            nLen = fread(pBuf, sizeof(char), nLen, fp);
            //log("====%s=======%ld======", pBuf, nLen);
            string strTmp;
            strTmp.assign(pBuf, 12);
            peb->setText(pBuf);
            delete []pBuf;
            pBuf = NULL;
            Global::setSn(strTmp);
            setBtnOrd();
        }
        fclose(fp);
    }
    fp = NULL;
    
    Sprite* contrl = Sprite::create("control.png");
    contrl->setPosition(visibleSize.width*0.1688, visibleSize.height*0.0539);
    contrl->setTag(1000);
    addChild(contrl);
    
    Sprite* conf = Sprite::create("conf.png");
    conf->setPosition(visibleSize.width*0.8406, visibleSize.height*0.0574);
    conf->setTag(2000);
    addChild(conf);

    return true;
}

bool ConfScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    Vec2 touchLocation = convertTouchToNodeSpace(touch);
    
    if(getChildByTag(1000)->getBoundingBox().containsPoint(touchLocation)){
        auto trans = TransitionMoveInR::create(0.5, ContrlScene::createScene());
        Director::getInstance()->replaceScene(trans);
    }
    EditBox* tf = (EditBox*)getChildByTag(9001);
    if(tf->getBoundingBox().containsPoint(touchLocation)){
        tf->attachWithIME();
    }else{
        tf->detachWithIME();
    }
    return false;
}

void ConfScene::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox)
{
}

void ConfScene::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox)
{
//    if(strlen(editBox->getText()) != 12) return;
//    
//    string strTmp;
//    strTmp.assign(editBox->getText(), 12);
//    setGlobalSn(strTmp);
//    setBtnOrd();
}

void ConfScene::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string &text)
{
    if(text.length() != 12) return;
    setGlobalSn(text);
    setBtnOrd();
}

void ConfScene::editBoxReturn(cocos2d::extension::EditBox *editBox)
{
}

void ConfScene::setGlobalSn(string strSn){
    FILE* fp = fopen(m_strSnPath.c_str(), "w+");
    fputs(strSn.c_str(), fp);
    fclose(fp);
    fp = NULL;
    
    Global::setSn(strSn);
}

void ConfScene::setBtnOrd(){
    for(int i=0; i<4; i++){
        ConfBtn* ob = (ConfBtn*)getChildByTag(600+i);
        ob->setOrd();
    }
}