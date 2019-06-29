#include "ConstantFolding.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"


#include <map>


using namespace llvm;


ConstantFolding::ConstantFolding() : FunctionPass(ID) {}

bool ConstantFolding::runOnFunction(Function &F) {

  std::map<Instruction*, Constant*> simpleMemory;


  for(Function::iterator BI = F.begin(); BI != F.end(); ++BI){
    for(BasicBlock::iterator II = BI->begin(); II != BI->end(); II++){

      Instruction* I = &*II;

      if(BinaryOperator* BO = dyn_cast<BinaryOperator>(I))
        handleBinaryOperator(BO);
      else if(LoadInst * LI = dyn_cast<LoadInst>(I))
        handleLoadInst(LI, simpleMemory);
      else if(StoreInst * SI = dyn_cast<StoreInst>(I))
        handleStoreInst(SI, simpleMemory);
      else if(ICmpInst * IC = dyn_cast<ICmpInst>(I))
        handleICmpInst(IC);
    }
  }

  return true;
}

void ConstantFolding::getAnalysisUsage(AnalysisUsage &Info) const {}

void ConstantFolding::handleBinaryOperator(BinaryOperator* BO){
  ConstantInt* LO = dyn_cast<ConstantInt>(BO->getOperand(0));
  ConstantInt* RO = dyn_cast<ConstantInt>(BO->getOperand(1));

  if(LO && RO){

    auto OpCode = BO->getOpcode();


    int64_t LeftVal = LO->getSExtValue();
    int64_t RightVal = RO->getSExtValue();

    Constant* Result = NULL;


    //TODO: Add more OpCode support
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
    Instruction* I = &*BO;
    replaceAllUses(I, Result);
  }

}

void ConstantFolding::handleStoreInst(StoreInst* SI, std::map<Instruction*, Constant* > &mem){
  if(Constant* C = dyn_cast<Constant>(SI->getValueOperand())){
    Instruction *To = dyn_cast<Instruction>(SI->getPointerOperand());
    if(To)
      mem.insert({To, C});
  }
}

void ConstantFolding::handleLoadInst(LoadInst* LI, std::map<Instruction*, Constant*> & mem){
  Instruction* From = dyn_cast<Instruction>(LI->getPointerOperand());
  Instruction* I = &*LI;

  if(Constant* Result = dyn_cast<Constant>(LI->getPointerOperand())){
    replaceAllUses(I, Result);
  } else if(mem.find(From) != mem.end()){
    Constant* C = mem.find(From)->second;
    replaceAllUses(I,C);

  }

}

void ConstantFolding::handleICmpInst(ICmpInst* IC){
  if(!IC)
    return;

  bool isConst = true;
  Instruction* I = &*IC;

  for(unsigned i = 0; i < IC->getNumOperands(); i++){
    Value * op = IC->getOperand(i);

    if(!isa<ConstantInt>(op))
      isConst = false;
  }



  if(isConst){
    ConstantInt* Left = dyn_cast<ConstantInt>(IC->getOperand(0));
    ConstantInt* Right = dyn_cast<ConstantInt>(IC->getOperand(1));
    int64_t LS = Left->getSExtValue();
    int64_t LUS = Left->getZExtValue();

    int64_t RS = Right->getSExtValue();
    int64_t RUS = Right->getZExtValue();




    Constant* Result = NULL;

    //TODO: Add more predicates 
    switch(IC->getPredicate()){
      case ICmpInst::ICMP_EQ:
        Result = ConstantInt::get(IC->getType(), LS == RS);
        break;
      case ICmpInst::ICMP_NE:
        Result = ConstantInt::get(IC->getType(), LS != RS);
        break;
      default:
        break;
    }

    if(Result){
      replaceAllUses(I, Result);
    }
  }

}



void ConstantFolding::replaceAllUses(Instruction* I, Constant * C){
  if(!I || !C)
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
