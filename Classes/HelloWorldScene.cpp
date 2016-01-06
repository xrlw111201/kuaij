#include "HelloWorldScene.h"
#include "OrderBtn.h"
#include "ContrlScene.h"
#include "ConfScene.h"
#include "Global.h"

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    //auto rootNode = CSLoader::createNode("MainScene.csb");
    //addChild(rootNode);
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true); // not need
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    m_nCurBtnTag = 0;
    bool rt = initBtn();
    return rt;
}

bool HelloWorld::initBtn(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* bg = Sprite::create("home_bg.png");
    bg->setPosition(visibleSize/2);

    bg->setTag(90);
    addChild(bg);
    
    OrderBtn::s_pHomeScene = this;

    //editbox
    Scale9Sprite* ss = Scale9Sprite::create("edit_box.png");
    ss->setPosition(visibleSize.width*0.5000, visibleSize.height*0.9331);
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
    lbl->setPosition(visibleSize.width*0.5000, visibleSize.height*0.0565);
    lbl->setTag(9002);
    addChild(lbl);

    for(int i=0; i<5; i++){
        OrderBtn* pob = OrderBtn::create();
        pob->setTag(i+100);
        pob->setProperties(i+100);
        addChild(pob, 100);
    }
    for(int j=0; j<5; j++){
        OrderBtn* pob = OrderBtn::create();
        pob->setTag(j+200);
        pob->setProperties(j+200);
        addChild(pob, 100);
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

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    Vec2 touchLocation = convertTouchToNodeSpace(touch);

    if(getChildByTag(1000)->getBoundingBox().containsPoint(touchLocation)){
        auto trans = TransitionMoveInR::create(0.5, ContrlScene::createScene());
        Director::getInstance()->replaceScene(trans);
    }
    if(getChildByTag(2000)->getBoundingBox().containsPoint(touchLocation)){
        auto trans = TransitionMoveInR::create(0.5, ConfScene::createScene());
        Director::getInstance()->replaceScene(trans);
    }
    //TextFieldTTF* tf = (TextFieldTTF*)getChildByTag(9001);
    EditBox* tf = (EditBox*)getChildByTag(9001);
    if(tf->getBoundingBox().containsPoint(touchLocation)){
        tf->attachWithIME();
    }else{
        tf->detachWithIME();
    }
    
    return false;
}

void HelloWorld::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox)
{
}

void HelloWorld::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox)
{
//    if(strlen(editBox->getText()) != 12) return;
//    
//    string strTmp;
//    strTmp.assign(editBox->getText(), 12);
//    setGlobalSn(strTmp);
//    setBtnOrd();
}

void HelloWorld::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string &text)
{
    if(text.length() != 12) return;
    
    setGlobalSn(text);
    setBtnOrd();
}

void HelloWorld::editBoxReturn(cocos2d::extension::EditBox *editBox)
{
}

void HelloWorld::setGlobalSn(string strSn){
    FILE* fp = fopen(m_strSnPath.c_str(), "w+");

    fputs(strSn.c_str(), fp);
    fclose(fp);
    fp = NULL;
    
    Global::setSn(strSn);
}

void HelloWorld::setBtnOrd(){
    for(int i=0; i<5; i++){
        OrderBtn* ob = (OrderBtn*)getChildByTag(200+i);
        ob->setOrd();
    }
}
