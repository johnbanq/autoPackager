#ifndef PACKAGEINFO_H
#define PACKAGEINFO_H

#include<QSettings>
#include<QDateTime>
struct userInfo{
    QString name;
    QString studNumber;
};


//STATIC CONSTANTS
const QString WEEK_CONFIG_PATH("autoPackager/weeks/");
const QString USER_INFO_CONFIG_PATH("autoPackager/userInfo/");
//week manipulator
unsigned short getCurrentWeek(QSettings &inSetting);
void setWeek(QSettings &inSetting,const unsigned short inWeek);

//userInfo manipulator
userInfo getUserInfo(QSettings &inSetting);
void setUserInfo(QSettings &inSetting,const userInfo inUserInfo);

//windowSize manipulator
userInfo getWindowSize(QSettings &inSetting,QString windowName);
void setUserInfo(QSettings &inSetting,const userInfo inUserInfo);
#endif // PACKAGEINFO_H
