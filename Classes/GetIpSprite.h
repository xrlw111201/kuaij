//
//  GetIpSprite.h
//  kuaij
//
//  Created by liuhui on 16/1/27.
//
//

#ifndef __kuaij__GetIpSprite__
#define __kuaij__GetIpSprite__

#include "cocos2d.h"
#include <string>
using namespace cocos2d;

class GetIpSprite: public Sprite {
public:
    virtual bool init();
    CREATE_FUNC(GetIpSprite);
    std::string GetMobileIP();
};

#endif /* defined(__kuaij__GetIpSprite__) */
