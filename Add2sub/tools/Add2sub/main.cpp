#include "Add2sub.h"

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
        
        /*for (Function::iterator i = FunIt->begin(), e = FunIt->end(); i != e; ++i) {  //获取每个函数中的basic block
            cout<< "Basic block name=" << i->getName().str() << std::endl;  
	     	outs() << *i << "\n";   //打印basic blocks的名字和内容
        }  */
        
        
        /*Function* F = &(*FunIt);  //获取模块中读每一个函数
	    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)  //直接获取每个函数中的instruction
	    {
	    	Instruction* inst = &(*I);
  			outs() << inst->getName() << "    " << inst->getOpcode() << "\n";
        }*/
        
        for (Function::iterator b = FunIt->begin(), e = FunIt->end(); b != e; ++b)  //获取每个函数中的basic block
        {  
        	for (BasicBlock::iterator i = b->begin(), e = b->end(); i != e; ++i)  //获取每个basic block中的instruction
        	{
        		//outs() << *i << "\n";
        		Instruction* inst = &(*i);
        		
        		if(inst->getOpcode() == Instruction::Add)  //找加法
	  			{
	  				outs() << "我找到加法了" << "\n";
	  				outs() << "操作数1：" << *inst->getOperand(0) << "\n操作数2：" << *inst->getOperand(1) << "\n返回值："  << inst->getName() << "\n\n";
				
					Instruction* toInsert = BinaryOperator::Create(Instruction::Sub, inst->getOperand(0), inst->getOperand(1), "sub"); //构造减法指令
					BasicBlock::iterator ii(inst);  //找到要替换指令的位置
					ReplaceInstWithInst(inst->getParent()->getInstList(), ii, toInsert);   //减法替换加法
				
					i = ii;  //*****************特别注意，替换后，被替换的BasicBlock::iterator的值发生变化，用替换后的指令的iterator进行赋值*************
					
					//outs() << *b << "\n";  //打印basic block，查看是否替换成功
				}
        	}
        }
    }  
    
    outs() << *mod << "\n";  //输出mod
    if(verifyModule(*mod))   //检查字节码是否出错
    {
    	errs() << "构建的LLVM字节码出错!\n";
		return 1;
    }
    
    std::string ErrorInfo;  //将字节码输出到.bc文件
	OwningPtr<tool_output_file> Out
	(new tool_output_file("update.bc", ErrorInfo, raw_fd_ostream::F_Binary));
	WriteBitcodeToFile(mod, Out->os());
	Out->keep();
	
	cout << "输出到.bc文件成功！" << endl;
}  

