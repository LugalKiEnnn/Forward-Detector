#include <iostream>
#include <cmath>
#include "FairRootManager.h"
#include "TClonesArray.h"
#include <MyFunctions.h>
#define PI 3.1415926535
#define XPOS 0.5
#define YPOS -4.6
#define MINRAD 3.2
#define MAXRAD 8.6
using namespace std;


void initModsAngles(Double_t *modsAngles) {
    for(Int_t i = 0; i < 64; i++) {
        modsAngles[i] = i * 5.625;
    }
}

Int_t modOfPoint(Double_t x, Double_t y, Double_t *modsAngles){
    Double_t newX = x - XPOS;
    Double_t newY = y - YPOS;
    Double_t pAngle = pointAngle(newX,newY);
    for(Int_t i = 0; i < 64; i++) {
        if(pAngle >= modsAngles[i] && pAngle <= (modsAngles[i] + 5.6))
            return i;
    }
    return -1;   
}

/*Int_t modOfPoint(Double_t x, Double_t y, Double_t *modsAngles){
    Double_t pAngle = pointAngle(x,y);
    for(Int_t i = 0; i < 64; i++) {
        if(pAngle >= modsAngles[i] && pAngle <= (modsAngles[i] + 5.6))
            return i;
    }
    return -1;    
}*/



Double_t pointAngle(Double_t x, Double_t y){
    if ( y >= 0.0 && x <= 0.0 )
        return (atan2(y,x) * 180.0 / PI - 89.6);
    Double_t ang = atan2(y,x) * 180.0 / PI + 270.4;
    if ( ang >= 360.0) 
        return ang - 360.0;
    else
        return ang;

}
