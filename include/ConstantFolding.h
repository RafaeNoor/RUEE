#include "llvm/Pass.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/IR/Instructions.h"

#include <map>

using namespace llvm;


class ConstantFolding : public FunctionPass {
  public:
    static char ID;

    ConstantFolding();


    void getAnalysisUsage(AnalysisUsage &Info) const override;
    bool runOnFunction(Function &) override;
    void replaceAllUses(Instruction* , Constant *);

    void handleBinaryOperator(BinaryOperator*);
    void handleLoadInst(LoadInst*, std::map<Instruction* , Constant* > &);
    void handleStoreInst(StoreInst * , std::map<Instruction* , Constant* > &);
    void handleICmpInst(ICmpInst *);



};
