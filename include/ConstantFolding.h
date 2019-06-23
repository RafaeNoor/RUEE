#include "llvm/Pass.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/ADT/SmallPtrSet.h"



class ConstantFolding : public llvm::FunctionPass {
  public:
    static char ID;

    ConstantFolding();


    void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;
    bool runOnFunction(llvm::Function &) override;
    void replaceAllUses(llvm::Instruction* , llvm::Constant *);


};
