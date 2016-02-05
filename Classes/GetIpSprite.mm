//
//  GetIpSprite.cpp
//  kuaij
//
//  Created by liuhui on 16/1/27.
//
//
#include "GetIpSprite.h"
#include "GetIp.h"

bool GetIpSprite::init(){
    initWithFile("exit.png");
    return true;
}

std::string GetIpSprite::GetMobileIP(){
    std::string s = [[GetIp deviceIPAdress] UTF8String];
    return s;
}