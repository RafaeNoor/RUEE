#include "llvm/Pass.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/ADT/SmallPtrSet.h"



class DCE : public llvm::FunctionPass {
  public:
    static char ID;

    DCE();


    void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;
    bool runOnFunction(llvm::Function &) override;


};
