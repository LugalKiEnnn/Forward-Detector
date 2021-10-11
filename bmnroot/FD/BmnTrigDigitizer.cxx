// -------------------------------------------------------------------------
// -----                   BmnFdHitProducer source file                -----
// -----                  Created 23/12/20  by I. Kozlov            -----
// -------------------------------------------------------------------------

#include "BmnTrigDigitizer.h"
#include "BmnFDPoint.h"
#include <MyFunctions.h>
#include "BmnTrigDigit.h"

#include "FairRootManager.h"

#include "TClonesArray.h"

#include <iostream>

using std::cout;
using std::endl;

// -----   Default constructor   -------------------------------------------
BmnTrigDigitizer::BmnTrigDigitizer() 
  : FairTask("Bmn Trigger Digitizer"),
    fPointArray(NULL),
    fDigitArray()
{ 
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
BmnTrigDigitizer::~BmnTrigDigitizer() { }
// -------------------------------------------------------------------------

// -----   Public method Init   --------------------------------------------
InitStatus BmnTrigDigitizer::Init() {

  // Get RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) {
    cout << "-E- BmnTrigDigitizer::Init: "
	 << "RootManager not instantiated!" << endl;
    return kFATAL;
  }

  // Get input array
  fPointArray = (TClonesArray*) ioman->GetObject("FDPoint");
  if ( ! fPointArray ) {
    cout << "-W- BmnTrigDigitizer::Init: "
	 << "No FDPoint array!" << endl;
    return kERROR;
  }

  // Create and register output array
  fDigitArray = new TClonesArray("BmnTrigDigit");
  //ioman->Register("FDDigit", "FD", fDigitArray, kTRUE);
  ioman->Register("FDDigit", "FD", fDigitArray, kTRUE);

  cout << "-I- BmnTrigDigitizer: Intialization successfull" << endl;
  return kSUCCESS;

}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// -----   Public method Exec   --------------------------------------------
void BmnTrigDigitizer::Exec(Option_t* opt) {

  // Reset output array
  if ( ! fDigitArray ) Fatal("Exec", "No TrigDigit array");

  fDigitArray->Delete();
  fHitMap.clear();
  fAmpMap.clear();

  // Declare some variables
  BmnFDPoint* point = NULL;
  Double_t x, y, z;         // Position

  //Инициализируем массив углов 
  //Int_t modsAngles[64] = initModsAngles(modsAngles);
  Double_t modsAngles[64];
  initModsAngles(modsAngles);

  // Loop over FdPoints
  Int_t nPoints = fPointArray->GetEntriesFast();
  for (Int_t iPoint=0; iPoint<nPoints; iPoint++) {
    point = (BmnFDPoint*) fPointArray->UncheckedAt(iPoint);
    if ( ! point) continue;

    // Hit position 
    x  = point->GetXOut();
    y  = point->GetYOut();
    z  = point->GetZOut();

    //Находим модуль для точки
    Int_t mod = modOfPoint(x,y,modsAngles);
    
    fHitMap[iPoint] = mod;
    /*
    // Ищем что за модули для точек
    bool flag1 = false;
    for(map<Int_t,Double_t>::iterator it = fAmpMap.begin(); it != fAmpMap.end(); ++it) {
      if(it->first == mod) {
        fAmpMap[mod] = fAmpMap[mod] + point->GetEnergyLoss();
        flag1 = true;
        break;
      }
    }
    if(flag1 == false)
      fAmpMap[mod] = point->GetEnergyLoss();
*/
  }   // Loop over MCPoints
  
  //создаём диджиты
  BmnTrigDigit digi = BmnTrigDigit();
  bool flag; 
/*
  for(map<Int_t,Double_t>::iterator mit = fAmpMap.begin(); mit != fAmpMap.end(); ++mit){
    digi.SetMod((Short_t) mit->first);

    digi.SetAmp(mit->second);

    new ((*fDigitArray)[fDigitArray->GetEntriesFast()]) BmnTrigDigit(digi);
    cout << "digi = " << digi.GetMod() << " amp = " << digi.GetAmp() << endl;

  }
  */
  //for(Int_t i = -1; i < 64; i++){
    for(Int_t i = -1; i < 64; i++){

    flag = false;

    for(map<Int_t,Int_t>::iterator mit = fHitMap.begin(); mit != fHitMap.end(); ++mit){

      point = (BmnFDPoint*) fPointArray->UncheckedAt(mit->first);
      
      if(mit->second == i){

        if(flag == false){
          
          digi.SetMod((Short_t) i);
          digi.SetAmp(point->GetEnergyLoss());

          flag = true;
          
          new ((*fDigitArray)[fDigitArray->GetEntriesFast()]) BmnTrigDigit(digi);
        }
        else{          
          
          BmnTrigDigit* dig = (BmnTrigDigit*) fDigitArray->At(fDigitArray->GetLast()); 

          dig->IncreaseAmp(point->GetEnergyLoss());
        }
      }
    }    
  }
  
  
  // Event summary
  cout << "-I- BmnTrigDigitizer: " << nPoints << " FDPoints, "
       << fDigitArray->GetEntriesFast() << " Digits created." << endl;

}
// -------------------------------------------------------------------------

ClassImp(BmnTrigDigitizer)