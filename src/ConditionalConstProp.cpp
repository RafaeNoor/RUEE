#include "ConditionalConstProp.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"

#include <map>
#include <queue>


using namespace llvm;



ConditionalConstProp::ConditionalConstProp() : FunctionPass(ID) {}

bool ConditionalConstProp::runOnFunction(Function &F) {
  bool Changed = false;

  // Update Terminator Insts
  for(Function::iterator FI = F.begin(); FI != F.end(); ){
    BasicBlock * BB = &*FI;
    FI++;

    TerminatorInst * TI = BB->getTerminator();

    if(BranchInst* BI = dyn_cast<BranchInst>(TI)){
      Changed |= handleBranchInst(BI);




    } else if(SwitchInst* SI = dyn_cast<SwitchInst>(TI)){
      Changed |= handleSwitchInst(SI);

    }

  }

  std::map<BasicBlock*, bool > visitedMap;


  std::queue<BasicBlock* > workList;
  workList.push(&F.getEntryBlock());

  while(!workList.empty()){
    BasicBlock* popped = workList.front();

    if(visitedMap.find(popped) == visitedMap.end()){
      visitedMap.insert({popped, true});

      TerminatorInst* TI = popped->getTerminator();

      if(BranchInst* BI = dyn_cast<BranchInst>(TI)){

        if(BI->isUnconditional()){
          workList.push(BI->getSuccessor(0));
        } else {

          for(unsigned i =0; i<BI->getNumSuccessors(); ++i){
            workList.push(BI->getSuccessor(i));
          }

        }
      }

    }

    workList.pop();

  }

  for(Function::iterator FI = F.begin(); FI != F.end(); ){
    BasicBlock * BB = &*FI;

    FI++;


    if(visitedMap.find(BB) == visitedMap.end()){ 
      //errs()<<*BB<<" has no preds and is not the entry block\n\n";
      //removeReferences(BB);
      //BB->removeFromParent();
    }



  }



  return Changed;
}

void ConditionalConstProp::getAnalysisUsage(AnalysisUsage &Info) const {}

void ConditionalConstProp::removeReferences(BasicBlock* BB){
  if(!BB){
    errs()<<"removeReferences invoked with nullptr\n";
    return;
  }
  TerminatorInst* TI = BB->getTerminator();

  BB->replaceSuccessorsPhiUsesWith(*pred_begin(BB));

  /*

     for(unsigned i = 0; i< TI->getNumSuccessors(); ++i){
     BasicBlock* succ = TI->getSuccessor(i);

     for(BasicBlock::iterator BI = succ->begin(); BI != succ-> end(); ){
     Instruction* I = &*BI;

     BI++;

     if(PHINode* PHI = dyn_cast<PHINode>(I)){
     errs()<<"PHI: "<<*PHI<<" with reff to "<<*BB<<"\n";
     PHI->removeIncomingValue(BB);
     errs()<<"PHI after: "<<*PHI<<"\n";
     } else {
     break;
     }
     }

     }*/
}

bool ConditionalConstProp::handleBranchInst(BranchInst* BI){
  if(!BI){
    errs()<<"Invoked handleBranchInst on nullptr...\n";
    return false;
  }


  if(BI->isConditional()){

    if(ConstantInt* C = dyn_cast<ConstantInt>(BI->getCondition())){
      errs()<<*BI<<" has conditional "<<*C<<" with type "<<*C->getType()<<", "<<C->getZExtValue()<<"\n";


      BranchInst* NewInst = NULL;
      if(C->getZExtValue() == 1){
        NewInst = BranchInst::Create(BI->getSuccessor(0), BI);
      } else {
        NewInst = BranchInst::Create(BI->getSuccessor(1), BI);
      }

      errs()<<"NewInst: "<<*NewInst<<"\n";
      BI->removeFromParent();

      return true;
    }
  }

  return false;
}

bool ConditionalConstProp::handleSwitchInst(SwitchInst* SI){

  if(!SI){
    errs()<<"Invoked handleBranchInst on nullptr...\n";
    return false;
  }

  errs()<<"Switch Inst: "<<*SI<<"\n";


  if(ConstantInt* C = dyn_cast<ConstantInt>(SI->getCondition())){

    for(unsigned i =0; i < SI->getNumSuccessors();++i){
      BasicBlock * succ = SI->getSuccessor(i);
      if(C == SI->findCaseDest(succ)){
        BranchInst* BI = BranchInst::Create(succ, SI);
        errs()<<"replaced with: "<<*BI<<"\n";
        SI->removeFromParent();
        break;
      }
    }
    return true;

  } else {
    errs()<<"Switch Condition not constant\n";
  }


  return false;
}





char ConditionalConstProp::ID = 0;
static RegisterPass<ConditionalConstProp>
X("cond-constprop",         // pass option
    "Replacing Conditional br and switch with unconditonal breaks. \nThen removing any unreachible BB.", // pass description
    false, // does not modify the CFG
    false  // and it's an analysis
 );
