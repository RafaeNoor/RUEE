#include "DCE.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"

#include <deque>


using namespace llvm;


DCE::DCE() : FunctionPass(ID) {}

bool DCE::runOnFunction(Function &F) {
  return false;
}

void DCE::getAnalysisUsage(AnalysisUsage &Info) const {
  Info.addRequired<DominatorTreeWrapperPass>();
  Info.setPreservesAll();
}



char DCE::ID = 0;
static RegisterPass<DCE>
X("loop-invar",         // pass option
    "Moves Loop Invariants outside the loop", // pass description
    false, // does not modify the CFG
    false  // and it's an analysis
 );
