#include "HelloWorldScene.h"
#include "OrderBtn.h"
#include "ContrlScene.h"
#include "ConfScene.h"
#include "Global.h"
#include "GetIpSprite.h"

bool HelloWorld::s_bWait = true;

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

    cleanLstCtrl();
    
    memset(&m_pidQuery, 0, sizeof(pthread_t));
    memset(&m_pidRsp, 0, sizeof(pthread_t));
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* bg = Sprite::create("home_bg.png");
    bg->setPosition(visibleSize/2);

    bg->setTag(HOMEBG);
    addChild(bg);
    
//    Label* lblSn = Label::create();
//    lblSn->setString("设备序列号");
//    lblSn->setTextColor(Color4B::WHITE);
//    lblSn->setSystemFontSize(28.00);
//    lblSn->setHorizontalAlignment(TextHAlignment::CENTER);
//    lblSn->setPosition(visibleSize.width*0.1550, visibleSize.height*0.9331);
//    addChild(lblSn);

    //editbox
    ui::Scale9Sprite* ss = ui::Scale9Sprite::create("edit_box.png");
//    ss->setPosition(visibleSize.width*0.5000, visibleSize.height*0.9331);
//    ss->setTag(EDITBOXPNG);
    Sprite* s = ss->getSprite();
    float sx = s->getTextureRect().getMaxX();
    float sy = s->getTextureRect().getMaxY();

    ui::EditBox* peb = ui::EditBox::create(Size(sx, sy), ss);
    peb->setPosition(Vec2(visibleSize.width*0.3000, visibleSize.height*0.9331));
    peb->setTag(EDITBOX);
    peb->setFontSize(25);
    peb->setPlaceholderFontSize(25);
    peb->setFontColor(Color3B::BLACK);
    peb->setPlaceholderFontColor (Color3B::GRAY);
    peb->setPlaceHolder("设备序列号");
    peb->setMaxLength(12);
    peb->setDelegate(this);
    addChild(peb);
    
    //下拉按钮
    Sprite* pLst = Sprite::create("list.png");
    pLst->setPosition(visibleSize.width*0.5859, visibleSize.height*0.9331);
    pLst->setTag(LST);
    addChild(pLst);
    
    //内网按钮
    Sprite* pNet = Sprite::create("innernet.png");
    pNet->setPosition(visibleSize.width*0.8297, visibleSize.height*0.9331);
    pNet->setTag(INNERNET);
    addChild(pNet);

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
//    string documentPath = FileUtils::sharedFileUtils()->getWritablePath();
//    string fileName = "kjSn";
//    m_strSnPath = documentPath + fileName;

//    log("filepath = %s", m_strSnPath.c_str());
  /*
    FILE* fp = fopen(Global::g_fileName.c_str(), "r");
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
*/
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

    getSnLstFromFile();
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
    }else if(getChildByTag(INNERNET)->getBoundingBox().containsPoint(touchLocation)){
        innerNet();
    }else if(getChildByTag(LST)->getBoundingBox().containsPoint(touchLocation)){
        initSnLstFromVct();
    }
    
    int nCount = m_mpLstCtrl.size();
    for(int i=0; i<nCount; i++){
        Label* lbl = (Label*)getChildByTag(FIRSTLBL+i);
        if(lbl->getBoundingBox().containsPoint(touchLocation)){
            ui::EditBox* peb = (ui::EditBox*)getChildByTag(EDITBOX);
            peb->setText(lbl->getString().c_str());

            Global::setSn(lbl->getString());
            setBtnOrd();
            
            cleanLstCtrl();
            break;
        }
    }
/*
    ui::EditBox* tf = (ui::EditBox*)getChildByTag(EDITBOX);
    if(tf->getBoundingBox().containsPoint(touchLocation)){
        tf->attachWithIME();
    }else{
        tf->detachWithIME();
    }
   */

    return false;
}

void HelloWorld::editBoxEditingDidBegin(ui::EditBox* editBox)
{}

void HelloWorld::editBoxEditingDidEnd(ui::EditBox* editBox)
{}

void HelloWorld::editBoxTextChanged(ui::EditBox* editBox, const std::string &text)
{
//    if(text.length() != 12) return;
//
//    setGlobalSn(text);
//    setBtnOrd();
}

void HelloWorld::editBoxReturn(ui::EditBox *editBox)
{}

//void HelloWorld::setGlobalSn(string strSn){
//    FILE* fp = fopen(Global::g_fileName.c_str(), "w+");
//
//    fputs(strSn.c_str(), fp);
//    fclose(fp);
//    fp = NULL;
//    Global::setSn(strSn);
//}

void HelloWorld::setBtnOrd(){
    for(int i=0; i<5; i++){
        OrderBtn* ob = (OrderBtn*)getChildByTag(200+i);
        ob->setOrd();
    }
}

void HelloWorld::innerNet(){
    
    Global::g_bInnerNet = true;
    Global::g_vSnIp.clear();
    
    std::string strIp = getMobileIpString();
    querySn(strIp);
    while(s_bWait){}
    s_bWait = true;
}

std::string HelloWorld::getMobileIpString(){
    
    std::string strIp = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Size visibleSize = Director::getInstance()->getVisibleSize();
    GetIpSprite* sp = GetIpSprite::create();
    sp->setPosition(visibleSize/2);
    sp->setVisible(false);
    this->addChild(sp);
    strIp = sp->GetMobileIP();

#else
    int i=0;
    int sockfd;
    struct ifconf ifconf;
    struct ifreq* ifreq = NULL;
    memset(&ifconf, 0, sizeof(struct ifconf));
    char buf[512] = {0};
    
    //初始化ifconf
    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
    {
        perror("socket");
    }
    ioctl(sockfd, SIOCGIFCONF, &ifconf);    //获取所有接口信息
    
    //一个个获取IP地址
    ifreq = (struct ifreq*)buf;
    for(i=(ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--)
    {
        if(0 == strcmp(ifreq->ifr_name, "wlan0")){
            strIp = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
            break;
        }
        ifreq++;
    }
    if(sockfd >=0){
        close(sockfd);
    }
#endif
    
    return strIp;
}

void HelloWorld::cleanLstCtrl(){
    std::map<Sprite*, Label*>::iterator it;
    for(it=m_mpLstCtrl.begin(); it!=m_mpLstCtrl.end(); it++){
        if(NULL != it->first){
            it->first->removeFromParentAndCleanup(true);
        }
        if(NULL != it->second){
            it->second->removeFromParentAndCleanup(true);
        }
    }
    m_mpLstCtrl.clear();
}

void HelloWorld::querySn(std::string strIp){

    std::string* pStr = new std::string(strIp);

    pthread_create(&m_pidRsp, NULL, recvRsp, this);
    pthread_create(&m_pidQuery, NULL, sendQuery, pStr);
}

void* HelloWorld::sendQuery(void* args){

    std::string* pStrIp = (std::string*)args;
    int sock = Global::getSocket();
    int nLastDot = pStrIp->rfind('.');
    std::string _strIp = pStrIp->substr(0, nLastDot+1);    //得到 "192.168.1."这样的字符串
    
    unsigned char ord[18] = {0};
    ord[0] = 0x86; ord[1] = 0xac; ord[2] = 0xcf; ord[3] = 0x23; ord[4] = 0x29; ord[5] = 0x69; ord[6] = 0xe6;
    ord[7] = 0x05; ord[8] = 0x00; ord[9] = 0x00; ord[10] = 0x00; ord[11] = 0x00; ord[12] = 0x00; ord[13] = 0x00;
    ord[14] = 0x00; ord[15] = 0x00; ord[16] = 0xd8; ord[17] = 0xe2;

    sockaddr_in tarAddr;
    memset(&tarAddr, 0, sizeof(sockaddr_in));
    tarAddr.sin_family = AF_INET;
    tarAddr.sin_port = htons(1985);
   
    char szTarAddr[16] = {0};
    for(int i=1; i<255; i++){
        sprintf(szTarAddr, "%s%d", _strIp.c_str(), i);
        if(0 == strcmp(szTarAddr, pStrIp->c_str())){
            continue;
        }
        log("==%s==", szTarAddr);
        tarAddr.sin_addr.s_addr = inet_addr(szTarAddr);
        sendto(sock, ord, 18, 0, (sockaddr*)&tarAddr, sizeof(sockaddr));
    }

    delete pStrIp;
    return NULL;
}

void* HelloWorld::recvRsp(void* args){

    log("应该与请稍候");
    HelloWorld* pHw = (HelloWorld*)args;
    Label* lbl = (Label*)pHw->getChildByTag(LBL);
    lbl->setString("正在配对，请稍候...");
    
    int sock = Global::getSocket();
    
    fd_set fds;
    bool b = true;
    int maxfdp = sock + 1;
    struct timeval tm_out = {10, 0};

    sockaddr_in sa;
    memset(&sa, 0, sizeof(sockaddr));
    
    FD_ZERO(&fds);
    FD_SET(sock, &fds);
    
    timeval ttt;
    gettimeofday(&ttt, NULL);
    log("start at %ld", ttt.tv_sec);

    while(b){
        int r = select(maxfdp, &fds, NULL, NULL, &tm_out);
        log("timeout: %ld", tm_out.tv_sec);
        switch(r){
            case -1: {
                log("select err.");
                b = false;
                break;
            }
            case 0: {
                log("select time out");
                b = false;
                break;
            }
            default:{
                char buf[18] = {0};
                socklen_t fromlen = sizeof(sockaddr);
                recvfrom(sock, buf, 18, 0, (sockaddr*)&sa, &fromlen);
                inet_ntoa(sa.sin_addr);
                log("recvd: %s: %s", inet_ntoa(sa.sin_addr), buf);
                
                //todo: 去重
                std::string strIp = inet_ntoa(sa.sin_addr);
                std::string strTmp = buf;
                std::string strSn = strTmp.substr(1, 6);
                if(!Global::isRefrain(strSn)){
                    log("insert...");
                    Global::g_vSnIp.push_back(strSn+':'+strIp);
                }

                break;
            }
        }
    }
    timeval tt;
    gettimeofday(&tt, NULL);
    log("end at %ld", tt.tv_sec);
//    log("%map size = %ld", Global::g_mp.size());
    
    if(Global::g_vSnIp.size() > 0){
        FILE* fp = fopen(Global::g_fileName.c_str(), "w");
        std::vector<std::string>::iterator it;
        for(it=Global::g_vSnIp.begin(); it!=Global::g_vSnIp.end(); it++){
            fputs(((*it)+'\n').c_str(), fp);
        }
        fclose(fp);
    }
    s_bWait = false;
    return NULL;
}

void HelloWorld::initSnLstFromVct(){
    
    cleanLstCtrl();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    int i = 0;
    std::vector<std::string>::iterator it;
    
    for(it=Global::g_vSnIp.begin(); it!=Global::g_vSnIp.end(); it++){
        Sprite* s = NULL;
        if(0 == i%2){
            s = Sprite::create("edit_box_gray.png");
        }else{
            s = Sprite::create("edit_box.png");
        }
        float sy = s->getTextureRect().getMaxY();
        s->setPosition(Vec2(visibleSize.width*0.3000, visibleSize.height*0.9331 - sy*(i+1)));
        addChild(s);

        Label* lbl = Label::create();
        lbl->setString(*it);
        lbl->setTextColor(Color4B::GRAY);
        lbl->setSystemFontSize(25.00);
        lbl->setHorizontalAlignment(TextHAlignment::CENTER);
        lbl->setPosition(visibleSize.width*0.3000, visibleSize.height*0.9331-sy*(i+1));
        lbl->setTag(FIRSTLBL+i);
        addChild(lbl);

        m_mpLstCtrl.insert(pair<Sprite*, Label*>(s, lbl));

        i++;
    }

}

void HelloWorld::getSnLstFromFile(){

    Global::g_vSnIp.clear();

    FILE* fp = fopen(Global::g_fileName.c_str(), "r");
    
    if(NULL == fp)
        return;
    
    char buf[512] = {0};
    
    while(NULL != fgets(buf, 512, fp))
    {
        std::string str = "";
        buf[strlen(buf)-1] = 0;
        str = buf;
        memset(buf, 0, 512);
        Global::g_vSnIp.push_back(str);
    }
    fclose(fp);
    fp= NULL;
    
    //把第一个设备显示在editbox上
    if(Global::g_vSnIp.size()>0){
        ui::EditBox* peb = (ui::EditBox*)getChildByTag(EDITBOX);
        peb->setText(Global::g_vSnIp.at(0).c_str());
        Global::setSn(Global::g_vSnIp.at(0));
        setBtnOrd();
    }
}
