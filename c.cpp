#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"
#include "llvm/Analysis/LoopInfo.h"
#include <vector>

using namespace llvm;

namespace {
  struct LoopCheck : public FunctionPass {
    static char ID;
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.addRequired<LoopInfoWrapperPass>();
    }
    LoopCheck() : FunctionPass(ID) {}
    bool runOnFunction(Function &F) override {
      LoopInfo *LI = &getAnalysis<LoopInfoWrapperPass>().getLoopInfo();\
      //getAnalysis<LoopInfoWrapperPass>().print(errs());
      errs() << "\n";
      errs() << "Function " << F.getName() +"\n";
      for (Loop *L :*LI)
        InLoop(L, 0);
      errs() << "\n";
      return false;
    }
    void InLoop(Loop *L, unsigned nest)
    {
      errs() << "Level(" << nest << ")\n";
      BasicBlock *header = L->getHeader();
      errs() << " Header Block: " << header->getName() << "\n";
      BasicBlock::iterator h_iter;
      for ( h_iter = header->begin(); h_iter != header->end(); ++h_iter)
	{
	  if (CmpInst *cmpInst = dyn_cast<CmpInst>(&*h_iter))
	    {
	      ///errs() << "opcodename: " << cmpInst->getOpcodeName() << "\n";
	      //errs() << "predicate: " << cmpInst->getPredicate() << "\n";
	      h_iter->print(errs());
	      errs() << "\n";
	      InBranch(cmpInst);
	      //PrintX(cmpInst);
	    }
	  //h_iter->print(errs());
	}
      
      std::vector<Loop *> subLoops = L->getSubLoops();
      Loop::iterator j, f;
      for (j = subLoops.begin(), f = subLoops.end(); j != f; ++j)
        InLoop(*j, nest + 1);
    }
    void InBranch(CmpInst *cmpInst)
    {
      /*
      switch(cmpInst->getPredicate())
	{
	  
	case CmpInst::ICMP_SGT: // Set on Greater Than
	case CmpInst::ICMP_SLT: // Branch on Lower Than
	  break;
	}*/
      /*errs() << cmpInst->getNumOperands()  << cmpInst->getOperand(0)->getName() <<
	" " << cmpInst->getOperand(1)->getName() << "\n";*/
      errs() << "  Value 1: ";
      PrintValue(cmpInst->getOperand(0));
      //errs() << " ";
      errs() << "  Value 2: ";
      PrintValue(cmpInst->getOperand(1));
      errs() << "  Predicate: " << cmpInst->getPredicate() << "\n";
      
      //errs() << "\n";
    }
    void PrintValue(Value *v)
    {
      /*if (ConstantInt *CI = dyn_cast<ConstantInt>(&*v))
	{
	  //errs() << CI->getSExtValue() << "\n";
	  }*/
      
      /*if (LoadInst *Inst = dyn_cast<LoadInst>(&*v))
	    {
	      errs() << "Load Inst !!" << "\n";
	      }*/
      //errs() << "  v-n: " << v->getValueName()->first() << "\n";
      v->dump();
    }
    void PrintX(CmpInst *ci)
    {
      errs() << "   opcode name: " << ci->getOpcodeName() << "\n";
      if (ci->hasMetadata())
	errs() << "  has metadata!!" << "\n";
    }
  };
}

char LoopCheck::ID = 0;
static RegisterPass<LoopCheck> X("loopcheck", "Hello World Pass");
