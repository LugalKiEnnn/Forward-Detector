#ifndef BMNTRIGDIGITIZER_H
#define BMNTRIGDIGITIZER_H 1


#include "FairTask.h"

#include <map>

class TClonesArray;

class BmnTrigDigitizer : public FairTask
{

 public:

  /** Default constructor **/  
  BmnTrigDigitizer();

  /** Destructor **/
  ~BmnTrigDigitizer();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

 private:

  /** Input array of BmnFdPoints **/
  TClonesArray* fPointArray;

  /** Output array of BmnTrigDigits **/
  TClonesArray* fDigitArray;  

  //map for storage energies at modules
  std::map<Int_t,Double_t> fAmpMap; 
  
  BmnTrigDigitizer(const BmnTrigDigitizer&);
  BmnTrigDigitizer& operator=(const BmnTrigDigitizer&);

  ClassDef(BmnTrigDigitizer,0);

};

#endif