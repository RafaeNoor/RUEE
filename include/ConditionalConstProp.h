#include "llvm/Pass.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/ADT/SmallPtrSet.h"



class ConditionalConstProp : public llvm::FunctionPass {
  public:
    static char ID;

    ConditionalConstProp();


    void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;
    bool runOnFunction(llvm::Function &) override;


};
