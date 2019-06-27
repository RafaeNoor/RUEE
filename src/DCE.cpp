#include "DCE.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"

#include <deque>


using namespace llvm;


DCE::DCE() : FunctionPass(ID) {}

bool DCE::runOnFunction(Function &F) {
  bool Removed = false;
  for(Function::iterator BI = F.begin(); BI != F.end(); BI++){
    for(BasicBlock::iterator II = BI->begin(); II != BI->end(); ){
      Instruction* I = &*II;

      II++;

      if(I->use_empty()){
        I->removeFromParent();
        Removed = true;
      }
    }
  }
  return Removed;
}

void DCE::getAnalysisUsage(AnalysisUsage &Info) const {
  //Info.setPreservesAll();
}



char DCE::ID = 0;
static RegisterPass<DCE>
X("deadcode",         // pass option
    "Remove any dead code", // pass description
    false, // does not modify the CFG
    false  // and it's an analysis
 );
