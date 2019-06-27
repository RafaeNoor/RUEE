#include "llvm/Pass.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/IR/Instructions.h"


using namespace llvm;

class ConditionalConstProp : public FunctionPass {
  public:
    static char ID;

    ConditionalConstProp();


    void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;
    bool runOnFunction(llvm::Function &) override;

    bool handleBranchInst(BranchInst*);

    bool handleSwitchInst(SwitchInst*);

    void removeReferences(BasicBlock *);


};
