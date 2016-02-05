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
    
    bg->setTag(CFGBG);
    addChild(bg);
    
    Label* lblSn = Label::create();
    lblSn->setString("设备序列号");
    lblSn->setTextColor(Color4B::WHITE);
    lblSn->setSystemFontSize(28.00);
    lblSn->setHorizontalAlignment(TextHAlignment::CENTER);
    lblSn->setPosition(visibleSize.width*0.1500, visibleSize.height*0.9331);
    addChild(lblSn);
    
    //editbox
    ui::Scale9Sprite* ss = ui::Scale9Sprite::create("edit_box.png");
//    ss->setPosition(visibleSize.width*0.5000, visibleSize.height*0.9331);
//    ss->setTag(EDITBOXPNG);
    //addChild(ss);
    Sprite* s = ss->getSprite();
    float sx = s->getTextureRect().getMaxX();
    float sy = s->getTextureRect().getMaxY();
    
    ui::EditBox* peb = ui::EditBox::create(Size(sx, sy), ss);
    peb->setPosition(Vec2(visibleSize.width*0.5000, visibleSize.height*0.9331));
    peb->setTag(EDITBOX);
    peb->setFontSize(35);
    peb->setPlaceholderFontSize(25);
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
    lbl->setPosition(visibleSize.width*0.5000, visibleSize.height*0.1430);
    lbl->setTag(LBL);
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
    std::string documentPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fileName = "kjSn";
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
    contrl->setPosition(visibleSize.width*0.5000, visibleSize.height*0.0579);
    contrl->setTag(CTRL);
    addChild(contrl);
    
    Sprite* conf = Sprite::create("conf.png");
    conf->setPosition(visibleSize.width*0.8406, visibleSize.height*0.0579);
    conf->setTag(CFG);
    addChild(conf);
    
    Sprite* exit = Sprite::create("exit.png");
    exit->setPosition(visibleSize.width*0.1672, visibleSize.height*0.0552);
    exit->setTag(EXIT);
    addChild(exit);

    return true;
}

bool ConfScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    Vec2 touchLocation = convertTouchToNodeSpace(touch);
    
    if(getChildByTag(CTRL)->getBoundingBox().containsPoint(touchLocation)){
        auto trans = TransitionMoveInR::create(0.5, ContrlScene::createScene());
        Director::getInstance()->replaceScene(trans);
    }else if(getChildByTag(CFG)->getBoundingBox().containsPoint(touchLocation)){
        auto trans = TransitionMoveInR::create(0.5, ConfScene::createScene());
        Director::getInstance()->replaceScene(trans);
    }else if(getChildByTag(EXIT)->getBoundingBox().containsPoint(touchLocation)){
        Director::getInstance()->stopAnimation();
        Global::closeSock();
        Director::getInstance()->end();
        exit(0);
    }
    ui::EditBox* tf = (ui::EditBox*)getChildByTag(EDITBOX);
    if(tf->getBoundingBox().containsPoint(touchLocation)){
        tf->attachWithIME();
    }else{
        tf->detachWithIME();
    }
    return false;
}

void ConfScene::editBoxEditingDidBegin(ui::EditBox* editBox)
{}

void ConfScene::editBoxEditingDidEnd(ui::EditBox* editBox)
{}

void ConfScene::editBoxTextChanged(ui::EditBox* editBox, const std::string &text)
{
//    if(text.length() != 12) return;
//    setGlobalSn(text);
//    setBtnOrd();
}

void ConfScene::editBoxReturn(ui::EditBox *editBox)
{}

void ConfScene::setGlobalSn(std::string strSn){
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