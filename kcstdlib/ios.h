/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: ios.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\ios.h
Created by Nathaniel on 3/7/2015 at 08:30

Description: Input Output - Base Class
**********************************************************/
#ifndef IOS_H
#define IOS_H
#include "stdafx.h"
KCSTDLIB_API_CLASS(ios) {
public:
	void setbase(unsigned b){iosbase = b;}
	unsigned getbase(){return iosbase;}
	enum Base{BS_ERR0,BS_ERR1,/**/BS_BINARY/**/,BS_3,BS_4,BS_5,BS_6,BS_7,/**/BS_OCTAL/**/,BS_9,/**/BS_DECIMAL/**/,BS_11,BS_12,BS_13,BS_14,BS_15,/**/BS_HEX/**/};
protected:
	unsigned iosbase;
};
#endif
