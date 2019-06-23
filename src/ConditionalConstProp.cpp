#include "ConditionalConstProp.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"

#include <deque>


using namespace llvm;


ConditionalConstProp::ConditionalConstProp() : FunctionPass(ID) {}

bool ConditionalConstProp::runOnFunction(Function &F) {
  return false;
}

void ConditionalConstProp::getAnalysisUsage(AnalysisUsage &Info) const {
  Info.addRequired<DominatorTreeWrapperPass>();
  Info.setPreservesAll();
}



char ConditionalConstProp::ID = 0;
static RegisterPass<ConditionalConstProp>
X("cond-constprop",         // pass option
    "Replacing Conditional br and switch with unconditonal breaks. \nThen removing any unreachible BB.", // pass description
    false, // does not modify the CFG
    false  // and it's an analysis
 );
