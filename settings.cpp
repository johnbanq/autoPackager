#include "settings.h"
//implementation
unsigned short getCurrentWeek(QSettings &inSetting){

    //if not exist,create one
    if(!inSetting.contains(WEEK_CONFIG_PATH+"lastDateTime")||
       !inSetting.contains(WEEK_CONFIG_PATH+"lastWeek")){

        inSetting.setValue(WEEK_CONFIG_PATH+"lastDateTime",QDateTime::currentDateTime());
        inSetting.setValue(WEEK_CONFIG_PATH+"lastWeek",1);
        inSetting.sync();
    }

    QDateTime lastDate = inSetting.value(WEEK_CONFIG_PATH+"lastDateTime").value<QDateTime>();
    unsigned int lastWeek = inSetting.value(WEEK_CONFIG_PATH+"lastWeek").toUInt();

    //if its greater then a week,add it to lastWeek
    if(lastDate.daysTo(QDateTime::currentDateTime())>=7){
        lastWeek+= lastDate.daysTo(QDateTime::currentDateTime())/7;
    }

    //changes to current
    inSetting.setValue(WEEK_CONFIG_PATH+"lastDateTime",QDateTime::currentDateTime());
    inSetting.setValue(WEEK_CONFIG_PATH+"lastWeek",lastWeek);

    //if too large(means passed a holiday,just froze the counter to 1,till changed)
    if(lastWeek>20)return 1;
    return lastWeek;
}

void setWeek(QSettings &inSetting, const unsigned short inWeek){

    //changes to current
    inSetting.setValue(WEEK_CONFIG_PATH+"lastDateTime",QDateTime::currentDateTime());
    inSetting.setValue(WEEK_CONFIG_PATH+"lastWeek",inWeek);
    inSetting.sync();
}

userInfo getUserInfo(QSettings &inSetting){
    //if not exist,create one
    if(!inSetting.contains(USER_INFO_CONFIG_PATH+"name")||
       !inSetting.contains(USER_INFO_CONFIG_PATH+"studNumber")){

        inSetting.setValue(USER_INFO_CONFIG_PATH+"name","");
        inSetting.setValue(USER_INFO_CONFIG_PATH+"studNumber","");
        inSetting.sync();
    }
    userInfo readResult;
    readResult.name = inSetting.value(USER_INFO_CONFIG_PATH+"name").value<QString>();
    readResult.studNumber = inSetting.value(USER_INFO_CONFIG_PATH+"studNumber").value<QString>();
    return readResult;
}

void setUserInfo(QSettings &inSetting, const userInfo inUserInfo){
    //changes to current
    inSetting.setValue(USER_INFO_CONFIG_PATH+"name",inUserInfo.name);
    inSetting.setValue(USER_INFO_CONFIG_PATH+"studNumber",inUserInfo.studNumber);
    inSetting.sync();
}
