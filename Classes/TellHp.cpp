//
//  TellHp.cpp
//  kuaij
//
//  Created by liuhui on 16/2/5.
//
//

#include "TellHp.h"
#include "Global.h"
#include "HelloWorldScene.h"

Scene* TellHp::createScene()
{
    auto scene = Scene::create();
    auto layer = TellHp::create();
    scene->addChild(layer);
    return scene;
}

bool TellHp::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true); // not need
    listener->onTouchBegan = CC_CALLBACK_2(TellHp::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(TellHp::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    Sprite* bg = Sprite::create("conf_back.png");
    bg->setPosition(visibleSize/2);
    
    bg->setTag(HOMEBG);
    addChild(bg);
    
    //editbox_name
    ui::Scale9Sprite* name = ui::Scale9Sprite::create("eb.png");
//    name->setPosition(visibleSize.width*0.3109, visibleSize.height*0.6901);
    Sprite* sName = name->getSprite();
    float sx = sName->getTextureRect().getMaxX();
    float sy = sName->getTextureRect().getMaxY();
    
    ui::EditBox* pName = ui::EditBox::create(Size(sx, sy), name);
    pName->setPosition(Vec2(visibleSize.width*0.3109, visibleSize.height*0.6901));
    pName->setTag(EBNAME);
    pName->setFontSize(23);
    pName->setPlaceholderFontSize(23);
    pName->setFontColor(Color3B::BLACK);
    pName->setPlaceholderFontColor (Color3B::GRAY);
    pName->setPlaceHolder("输入wifi名");
    pName->setDelegate(this);
    addChild(pName);
    
    //editbox_password
    ui::Scale9Sprite* pw = ui::Scale9Sprite::create("eb.png");
//    pw->setPosition(visibleSize.width*0.7781, visibleSize.height*0.6901);
    Sprite* sPw = pw->getSprite();
    sx = sPw->getTextureRect().getMaxX();
    sy = sPw->getTextureRect().getMaxY();
    
    ui::EditBox* pPw = ui::EditBox::create(Size(sx, sy), pw);
    pPw->setPosition(Vec2(visibleSize.width*0.7781, visibleSize.height*0.6901));
    pPw->setTag(EBPW);
    pPw->setFontSize(23);
    pPw->setPlaceholderFontSize(23);
    pPw->setFontColor(Color3B::BLACK);
    pPw->setPlaceholderFontColor (Color3B::GRAY);
    pPw->setPlaceHolder("输入wifi密码");
    pPw->setDelegate(this);
    addChild(pPw);
    
    Sprite* snd_b = Sprite::create("send_b.png");
    snd_b->setPosition(visibleSize.width*0.5000, visibleSize.height*0.5035);
    snd_b->setTag(SEND_B);
    addChild(snd_b);
    
    Sprite* snd = Sprite::create("send.png");
    snd->setPosition(visibleSize.width*0.5000, visibleSize.height*0.5035);
    snd->setTag(SEND);
    snd->setVisible(false);
    addChild(snd);
    
    Sprite* ext_b = Sprite::create("exits_b.png");
    ext_b->setPosition(visibleSize.width*0.5000, visibleSize.height*0.3979);
    ext_b->setTag(EXITS_B);
    addChild(ext_b);
    
    
    Sprite* ext = Sprite::create("exits.png");
    ext->setPosition(visibleSize.width*0.5000, visibleSize.height*0.3979);
    ext->setTag(EXITS);
    ext->setVisible(false);
    addChild(ext);
    
    

    initAddr();
    
    return true;
}

void TellHp::initAddr(){
    
    memset(&m_sa, 0, sizeof(sockaddr_in));
    m_sa.sin_family = AF_INET;
    m_sa.sin_port = htons(49000);
    m_sa.sin_addr.s_addr = inet_addr("10.10.100.254");
}

bool TellHp::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    Vec2 touchLocation = convertTouchToNodeSpace(touch);
    
    ui::EditBox* name = (ui::EditBox*)getChildByTag(EBNAME);
    ui::EditBox* password = (ui::EditBox*)getChildByTag(EBPW);
    if(!(name->getBoundingBox().containsPoint(touchLocation)) && !(name->getBoundingBox().containsPoint(touchLocation))){
        name->detachWithIME();
        password->detachWithIME();
    }else{
        if(name->getBoundingBox().containsPoint(touchLocation))
            name->attachWithIME();
        if(password->getBoundingBox().containsPoint(touchLocation))
            password->attachWithIME();
    }
    
    if(getChildByTag(SEND)->getBoundingBox().containsPoint(touchLocation)){

        Sprite* snd = (Sprite*)getChildByTag(SEND);
        snd->setVisible(true);
    }
    
    if(getChildByTag(EXITS)->getBoundingBox().containsPoint(touchLocation)){
        Sprite* exts = (Sprite*)getChildByTag(EXITS);
        exts->setVisible(true);
    }
    return true;
}

void TellHp::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    Vec2 touchLocation = convertTouchToNodeSpace(touch);
    
    ((Sprite*)getChildByTag(SEND))->setVisible(false);
    ((Sprite*)getChildByTag(EXITS))->setVisible(false);
    
    if(getChildByTag(SEND)->getBoundingBox().containsPoint(touchLocation)){
        sendWifi2device();
    }
    if(getChildByTag(EXITS)->getBoundingBox().containsPoint(touchLocation)){
        Director::getInstance()->stopAnimation();
        Global::closeSock();
        Director::getInstance()->end();
        exit(0);
    }
}

void TellHp::sendWifi2device(){
    ui::EditBox* pName = (ui::EditBox*)getChildByTag(EBNAME);
    ui::EditBox* pPw = (ui::EditBox*)getChildByTag(EBPW);
    std::string strName = pName->getText();
    std::string strPw = pPw->getText();
    
    int lenName = strName.length();
    int lenPw = strPw.length();
    int len = lenName + lenPw + 8;
    
    log("%s, %ld", strName.c_str(), strName.length());
    log("%s, %ld", strPw.c_str(), strPw.length());
    
    char* buf = new char[len+1];
    memset(buf, 0, len);
    
    *buf = 0xff;
    
    if((lenName+lenPw) <= 255){
        *(buf+1) = 0;
        *(buf+2) = 4 + lenName + lenPw;
    }else{
        *(buf+1) = 4 + lenName + lenPw;
    }
    *(buf+3) = 0x02;
    *(buf+4) = 0x00;
    for(int i=0; i<lenName; i++){
        *(buf+5+i) = strName.at(i);
    }
    
    *(buf+lenName+5) = 0x0d;
    *(buf+lenName+6) = 0x0a;
    for(int j=0; j<lenPw; j++){
        *(buf+lenName+7+j) = strPw.at(j);
    }
    
    char count = 0;
    for(int k=0; k<len-2; k++){
        count += buf[k+1];
    }
    
    *(buf+len-1) = count;
    log("--%u---", count);
    for(int j=0; j<len; j++){
        log("0x%x", buf[j]);
    }
    log("len = %d", len);
    sendto(Global::getSocket(), buf, len, 0, (sockaddr*)&m_sa, sizeof(sockaddr));
    
    delete []buf;
    buf = NULL;
}

void TellHp::editBoxEditingDidBegin(ui::EditBox* editBox)
{}

void TellHp::editBoxEditingDidEnd(ui::EditBox* editBox)
{}

void TellHp::editBoxTextChanged(ui::EditBox* editBox, const std::string &text)
{}

void TellHp::editBoxReturn(ui::EditBox *editBox)
{}