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

      if(StoreInst* SI = dyn_cast<StoreInst>(I)){

        Instruction* SI_Point = dyn_cast<Instruction>(SI->getPointerOperand());

        if(SI_Point && L->contains(SI_Point)){
          isLoopInvar = false;
        }

        Instruction* ValI = dyn_cast<Instruction>(SI->getValueOperand());

        if(ValI && !isa<ConstantInt>(ValI) && insToMove.find(ValI) == insToMove.end()){
          isLoopInvar = false;
        }

      } else if(LoadInst* LI = dyn_cast<LoadInst>(I)){

        Instruction* LI_Point = dyn_cast<Instruction>(LI->getPointerOperand());

        if(LI_Point && L->contains(LI_Point) && insToMove.find(LI_Point) == insToMove.end()){
          isLoopInvar = false;
        }

      } else {

        for(unsigned OP_I = 0; OP_I < I->getNumOperands(); ++OP_I){
          Value* Val_I = I->getOperand(OP_I);

          if(isa<ConstantInt>(Val_I)){
            continue;
          } else if(Instruction* Oper = dyn_cast<Instruction>(Val_I)){
            if(insToMove.find(Oper) == insToMove.end())
              isLoopInvar = false;
          } else {
            isLoopInvar = false;
          }
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
  return;
}



char LoopInvar::ID = 0;
static RegisterPass<LoopInvar>
X("loop-invar",         // pass option
    "Moves Loop Invariants outside the loop", // pass description
    false, // does not modify the CFG
    false  // and it's an analysis
 );
