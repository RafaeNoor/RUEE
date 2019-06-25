#include "ConstantFolding.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"


#include <map>
#include <iostream>


using namespace llvm;


ConstantFolding::ConstantFolding() : FunctionPass(ID) {}

bool ConstantFolding::runOnFunction(Function &F) {

  std::map<Instruction*, Constant*> simpleMemory;




  for(Function::iterator BI = F.begin(); BI != F.end(); ++BI){
    for(BasicBlock::iterator II = BI->begin(); II != BI->end(); II++){
      
      Instruction* I = &*II;
      errs()<<*I<<"\n";

      if(BinaryOperator* BO = dyn_cast<BinaryOperator>(I)){

        ConstantInt* LO = dyn_cast<ConstantInt>(BO->getOperand(0));
        ConstantInt* RO = dyn_cast<ConstantInt>(BO->getOperand(1));

        if(LO && RO){
          IRBuilder<> Builder(BO);

          auto OpCode = BO->getOpcode();
          //errs()<<"BinaryInst: "<<*BO<<", Opcode: "<<OpCode<<"\n";


          int64_t LeftVal = LO->getSExtValue();
          int64_t RightVal = RO->getSExtValue();

          Constant* Result = NULL;

          switch(OpCode){
            case Instruction::Add :
              Result = ConstantInt::get(BO->getType(), LeftVal+RightVal);
              break;
            case Instruction::Sub :
              Result = ConstantInt::get(BO->getType(), LeftVal-RightVal);
              break;
            case Instruction::Mul :
              Result = ConstantInt::get(BO->getType(), LeftVal*RightVal);
              break;
            default:
              Result = NULL;
              break;
          }
          replaceAllUses(I, Result);
        }
      } else if(LoadInst * LI = dyn_cast<LoadInst>(I)){
        Instruction* From = dyn_cast<Instruction>(LI->getPointerOperand());

        if(Constant* Result = dyn_cast<Constant>(LI->getPointerOperand())){
          //errs()<<"Load result is constant\n";
          replaceAllUses(I, Result);
        } else if(simpleMemory.find(From) != simpleMemory.end()){
          //errs()<<"Load result is found in memory!\n";
          Constant* C = simpleMemory.find(From)->second;
          //errs()<<"Replacing load with "<<*C<<"\n";
          replaceAllUses(I,C);
          
        }
      } else if(StoreInst * SI = dyn_cast<StoreInst>(I)){

        if(Constant* C = dyn_cast<Constant>(SI->getValueOperand())){
          //errs()<<"Inserting Constant into memory\n";
          Instruction *To = dyn_cast<Instruction>(SI->getPointerOperand());
          if(To)
            simpleMemory.insert({To, C});
        }
      }
    }
  }

  return true;
}

void ConstantFolding::getAnalysisUsage(AnalysisUsage &Info) const {

}

void ConstantFolding::replaceAllUses(Instruction* I, Constant * C){
  if(!I)
    return;
  I->replaceAllUsesWith(C); //Neat function

}



char ConstantFolding::ID = 0;
static RegisterPass<ConstantFolding>
X("const-fold",         // pass option
    "Resolve any expressions into constants", // pass description
    false, // does not modify the CFG
    false  // and it's an analysis
 );
