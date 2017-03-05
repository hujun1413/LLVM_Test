#include "Hujunlib.h"

//Create words to paramater
//创建一个指向字符串的指针，若已存在则直接返回指针
Constant *CreateWords(Module *mod, string str)
{
	//look up the word in global value table
	string mystr = "%%" + str;
	GlobalValue *my_name = mod->getNamedValue(mystr);
	if (my_name)
	{
		GlobalVariable *my_global = cast <GlobalVariable> (my_name);
		ConstantInt* const_int = ConstantInt::get(mod->getContext(), APInt(32, 0));
		std::vector<Constant*> const_ptr_indices;
		const_ptr_indices.push_back(const_int);
		const_ptr_indices.push_back(const_int);
		Constant* const_ptr = ConstantExpr::getGetElementPtr(my_global, const_ptr_indices);   
		//ConstantExpr - a constant value that is initialized with an expression using other constant values.
		return const_ptr;
	}

	//if not find it, then create a new word in global value table
	ArrayType *ArrayTy = ArrayType::get(IntegerType::get(mod->getContext(), 8), str.length()+1);
	GlobalVariable *global_name = new GlobalVariable(*mod, ArrayTy, true, 
									GlobalValue::PrivateLinkage, 0, mystr);
	global_name->setAlignment(1);

	//Constant Definitions 
	Constant* const_array = ConstantDataArray::getString(mod->getContext(), str, true);  //This method constructs a CDS and initializes it with a text string.
	ConstantInt* const_int = ConstantInt::get(mod->getContext(), APInt(32, 0));
	std::vector<Constant*> const_ptr_indices;
	const_ptr_indices.push_back(const_int);
	const_ptr_indices.push_back(const_int);
	Constant* const_ptr = ConstantExpr::getGetElementPtr(global_name, const_ptr_indices);

	//Global Variable Definitions
	global_name->setInitializer(const_array);
	return const_ptr;
}

//Declare the extern function of PrintWrite
//extern void PrintWrite(i32)
Function* PrintInfo(Module *mod)
{
	//Initialize paramater's type
	IntegerType *IntTy32 = IntegerType::get(mod->getContext(), 32);
	std::vector<Type*>FuncTy_args;
	FuncTy_args.push_back(IntTy32);
	FunctionType* FuncTy = FunctionType::get(Type::getVoidTy(mod->getContext()),
  											 FuncTy_args, false);

	//Create a function declaration
	Function* func = mod->getFunction("PrintWrite");
	if (!func) 
	{
 		func = Function::Create(FuncTy, GlobalValue::ExternalLinkage,
								"PrintWrite", mod);
		func->setCallingConv(CallingConv::C);
	}
	AttrListPtr func_Add_PAL;
	func->setAttributes(func_Add_PAL);

	return func;
}



int main(int argc, char** argv)  
{  
	InitializeNativeTarget();     //The main program should call this function to initialize the native target corresponding to the host.
								  //This is useful for JIT applications to ensure that the target gets linked in correctly.
	LLVMContext &Context = getGlobalContext();
  
	// Load the bitcode...	
	SMDiagnostic Err;
	Module *mod = ParseIRFile(argv[1], Err, Context);
	if(!mod) 
	{
		errs() << "Unknown error loading Module!\n";
		return 1;
	}
	
	Function* PrintWrite = PrintInfo(mod);
  
    for (Module::iterator FunIt = mod->begin() ; FunIt != mod->end() ; ++FunIt) 
    {  
        for (Function::iterator b = FunIt->begin(), e = FunIt->end(); b != e; ++b)  //获取每个函数中的basic block
        {  
        	cout<< "Basic block name=" << b->getName().str() << endl;  
        	for (BasicBlock::iterator i = b->begin(), e = b->end(); i != e; ++i)  //获取每个basic block中的instruction
        	{
        		outs() << *i << "\n";
        		Instruction* inst = &(*i);
        		
        		if(inst->getOpcode() == Instruction::Call)  //找调用
	  			{
	  				outs() << "我找到调用了" << "\n";
	  				//outs() << "操作数1：" << *inst->getOperand(0) << "\n操作数2：" << *inst->getOperand(1) << "\n返回值的名字："  << inst->getName() << "\n返回值："  << *inst << "\n\n";
	  				CallInst *mycall = cast <CallInst> (inst);
					Function *myfunc = mycall->getCalledFunction();   //myfunc为被调用函数
					
					//outs() << "被调用函数的名字：" << myfunc->getName().str() << "\n";
					
					if(myfunc->getName().str() == "printf")
					{
						outs() << "被调用函数的名字：" << myfunc->getName().str() << "\n";
						
						Function *print = mod->getFunction("printf");
						if(print)
						{
							CallInst *newcall;
							
							++i;
							
							std::vector<Value*> para1;
							Constant *content = CreateWords(mod, "sum outputted by Hujun:\n");
							para1.push_back(content);
							newcall = CallInst::Create(print, para1, "", i);  //CallInst represents a function call，在i之前插入一条PrintWrite
							newcall->setCallingConv(CallingConv::C);
							newcall->setTailCall(false);	
							
							std::vector<Value*> para;
							para.push_back(inst->getOperand(1));
							newcall = CallInst::Create(PrintWrite, para, "", i);  //CallInst represents a function call，在i之前插入一条PrintWrite
							newcall->setCallingConv(CallingConv::C);
							newcall->setTailCall(false);	

							--i;
							
							//free(szBuffer);                                    //释放动态分配的内存
						}
					}
					
					
					else if(myfunc->getName().str() == "rand")
					{
						outs() << "被调用函数的名字是：" << myfunc->getName().str() << "\n";
						
						if (ConstantInt* CI = dyn_cast<ConstantInt>(inst)) 
						{
							outs() << "转换成功\n"; 
  							if (CI->getBitWidth() <= 64) 
  							{
    							int constIntValue = CI->getSExtValue();
    							outs() << "整型值为：" << constIntValue << "\n";
  							}
						}
						else
						{
							outs() << "转换失败！\n";
						}
						
						CallInst *newcall;
						std::vector<Value*> para;
						para.push_back(mycall);
						
						++i;
						newcall = CallInst::Create(PrintWrite, para, "", i);  //CallInst represents a function call，在i之前插入一条printInt
						newcall->setCallingConv(CallingConv::C);
						newcall->setTailCall(false);	
						--i;
					}
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

