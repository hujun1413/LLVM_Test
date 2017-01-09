#include "RecordInst.h"

int main(int argc, char** argv)  
{  
InitializeNativeTarget();
	LLVMContext &Context = getGlobalContext();
  
	// Load the bitcode...	
	SMDiagnostic Err;
	Module *mod = ParseIRFile(argv[1], Err, Context);
	if(!mod) 
	{
		errs() << "Unknown error loading Module!\n";
		return 1;
	}
  
    for (Module::iterator FunIt = mod->begin() ; FunIt != mod->end() ; ++FunIt) {  
        
        /*for (Function::iterator i = FunIt->begin(), e = FunIt->end(); i != e; ++i) {  
            cout<< "Basic block name=" << i->getName().str() << std::endl;  
	     	outs() << *i << "\n";   //打印basic blocks的名字和内容
        }  */
        
        
        Function* F = &(*FunIt);  //获取模块中读每一个函数
	    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
	    {
	    	Instruction* inst = &(*I);
  			//outs() << inst->getName() << "    " << inst->getOpcode() << "\n";
  			if(inst->getOpcode() == Instruction::Add)  //找加法
  			{
  				outs() << "我找到加法了" << "\n";
  				
  				outs() << *inst->getOperand(0) << "    " << *inst->getOperand(1) << "    "  << inst->getOpcodeName() << "\n";

			}
  			
        }
        
    }  
        
}  

