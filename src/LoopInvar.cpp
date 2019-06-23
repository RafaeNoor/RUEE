#include "LoopInvar.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"

#include <vector>
#include <set>


using namespace llvm;


LoopInvar::LoopInvar() : LoopPass(ID) {}

bool LoopInvar::doInitialization(Loop* L, LPPassManager &LPM){
  return false;
}

bool LoopInvar::runOnLoop(Loop* L,LPPassManager &LPM) {
  errs()<<*L<<"\n";

  std::set<Instruction *> insToMove;

  for(Loop::block_iterator LB = L->block_begin(); LB != L->block_end(); ++LB){
    BasicBlock* BB= *LB;
    for(auto bb = BB->begin(); bb != BB->end(); ++bb){
      Instruction* I = &*bb;

      bool isLoopInvar = true;


      for(unsigned OP_I = 0; OP_I < I->getNumOperands(); ++OP_I){
        Value* Val_I = I->getOperand(OP_I);
        if(ConstantInt * CI = dyn_cast<ConstantInt>(Val_I)){
        } else if(Instruction* Oper = dyn_cast<Instruction>(Val_I)){
          if(insToMove.find(Oper) == insToMove.end())
            isLoopInvar = false;

        } else {
          isLoopInvar = false;
        }
      }
      
      
      if(isLoopInvar){
        errs()<<"Loop Invariant: "<<*I<<"\n";
        BasicBlock* LoopPreheader = L -> getLoopPreheader();
        if(!LoopPreheader)
          errs()<<"Loop doesn't have a preheader\n";

        insToMove.insert(I);
      } else {
        errs()<<"Regular Instruction: "<<*I<<"\n";
      }
    } 
  }


  for(auto v = insToMove.begin(); v != insToMove.end(); v++){
    Instruction* I = *v;
    BasicBlock* LoopPreheader = L->getLoopPreheader();
    I -> moveBefore(LoopPreheader->getTerminator());
  }
  return true;
}

void LoopInvar::getAnalysisUsage(AnalysisUsage &Info) const {
  Info.addRequired<DominatorTreeWrapperPass>();
  Info.setPreservesAll();
}



char LoopInvar::ID = 0;
static RegisterPass<LoopInvar>
X("loop-invar",         // pass option
    "Moves Loop Invariants outside the loop", // pass description
    false, // does not modify the CFG
    false  // and it's an analysis
 );
