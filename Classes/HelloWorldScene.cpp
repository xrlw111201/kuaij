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
    
    OrderBtn::s_pHomeScene = this;

    bool rt = initBtn();
    return rt;
}

bool HelloWorld::initBtn(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* bg = Sprite::create("home_bg.png");
    bg->setPosition(visibleSize/2);

    bg->setTag(HOMEBG);
    addChild(bg);
    
    Label* lblSn = Label::create();
    lblSn->setString("设备序列号");
    lblSn->setTextColor(Color4B::WHITE);
    lblSn->setSystemFontSize(28.00);
    lblSn->setHorizontalAlignment(TextHAlignment::CENTER);
    lblSn->setPosition(visibleSize.width*0.1550, visibleSize.height*0.9331);
    addChild(lblSn);

    //editbox
    ui::Scale9Sprite* ss = ui::Scale9Sprite::create("edit_box.png");
    ss->setPosition(visibleSize.width*0.5000, visibleSize.height*0.9331);
    ss->setTag(EDITBOXPNG);
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
    string documentPath = FileUtils::sharedFileUtils()->getWritablePath();
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
    
    Label* lbl = Label::create();
    lbl->setString("");
    lbl->setTextColor(Color4B::WHITE);
    lbl->enableShadow();
    lbl->setSystemFontSize(25.00);
    lbl->setHorizontalAlignment(TextHAlignment::CENTER);
    lbl->setPosition(visibleSize.width*0.5000, visibleSize.height*0.1430);
    lbl->setTag(LBL);
    addChild(lbl);
    
    return true;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
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

void HelloWorld::editBoxEditingDidBegin(ui::EditBox* editBox)
{}

void HelloWorld::editBoxEditingDidEnd(ui::EditBox* editBox)
{}

void HelloWorld::editBoxTextChanged(ui::EditBox* editBox, const std::string &text)
{
    if(text.length() != 12) return;

    setGlobalSn(text);
    setBtnOrd();
}

void HelloWorld::editBoxReturn(ui::EditBox *editBox)
{}

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
