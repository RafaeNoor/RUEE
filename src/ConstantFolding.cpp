#include "ConstantFolding.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"

#include <deque>


using namespace llvm;


ConstantFolding::ConstantFolding() : FunctionPass(ID) {}

bool ConstantFolding::runOnFunction(Function &F) {
  return false;
}

void ConstantFolding::getAnalysisUsage(AnalysisUsage &Info) const {
  Info.addRequired<DominatorTreeWrapperPass>();
  Info.setPreservesAll();
}



char ConstantFolding::ID = 0;
static RegisterPass<ConstantFolding>
X("loop-invar",         // pass option
    "Moves Loop Invariants outside the loop", // pass description
    false, // does not modify the CFG
    false  // and it's an analysis
 );
