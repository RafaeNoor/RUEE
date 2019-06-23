#include "llvm/Pass.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/Analysis/LoopInfo.h"



class LoopInvar : public llvm::LoopPass {
  public:

    static char ID;

    LoopInvar();


    void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;
    bool runOnLoop(llvm::Loop*, llvm::LPPassManager &) override;
    bool doInitialization(llvm::Loop*, llvm::LPPassManager &) override; 


};
