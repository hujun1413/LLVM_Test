#include "RecordInst.h"

static ExecutionEngine *EE = 0;

int main(int argc, char **argv) 
{ 	
	// If we have a native target, initialize it to ensure it is linked in and
	// usable by the JIT.
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

	//Create the engine
	EngineBuilder builder(mod);
	EE = builder.create();
	if (!EE) 
	{
		errs() << "Unknown error creating EE!\n";
		exit(1);
	}
	
	//run your pass
	outs() << "\nGetCmp: Start handling " << argv[1] << "\n";
	FindCmp(mod);
	outs() << "\nGetCmp: Finish handling " << argv[1] << "\n";
	
	if(verifyModule(*mod))   //检查字节码是否出错
    {
    	errs() << "Wrong llvm bitcode!\n";
		return 1;
    }

	//Create the new .bc file according to Module
	std::string ErrorInfo;

	//std::string outfile = argv[1];
	//outfile = "update_" + outfile;
	
  	OwningPtr<tool_output_file> Out
  	(new tool_output_file(argv[1], ErrorInfo,
                        raw_fd_ostream::F_Binary));
	WriteBitcodeToFile(mod, Out->os());
	Out->keep();

    return 0;
}
