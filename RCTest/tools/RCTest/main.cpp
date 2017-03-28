#include "RecordFunction.h"

static ExecutionEngine *EE = 0;

int main(int argc, char **argv) 
{ 	
	// If we have a native target, initialize it to ensure it is linked in and
	// usable by the JIT.
	InitializeNativeTarget();
	LLVMContext &Context = getGlobalContext();

	int opt;
	if (strcmp(argv[1], "-r") == 0)  
		opt = LLVM_RECORD;
	else if (strcmp(argv[1], "-a") == 0)
		opt = LLVM_ANALYZE;
	else if (strcmp(argv[1], "-f") == 0)
		opt = LLVM_INJECT;
	else
	{
		outs() << "Invalid option!\n";
		exit(-1);
	}

	// Load the bitcode...	
	SMDiagnostic Err;
	Module *mod = ParseIRFile(argv[2], Err, Context);
	if(!mod) 
	{
		errs() << "Unknown error loading Module!\n";
		exit(-1);
	}

	//Create the engine
	EngineBuilder builder(mod);
	EE = builder.create();
	if (!EE) 
	{
		errs() << "Unknown error creating EE!\n";
		exit(-1);
	}
	
	outs() << "\nEHTest: Start handling " << argv[2] << "\n";

	if (opt == LLVM_RECORD)
		LogFunction(mod); //当mycall返回指针或者整数时，在MyIn（调用语句）之后插入call void func_record(called_name, caller_name, ret_type)
	else if (opt == LLVM_ANALYZE)
		AnalyzeSource(mod, argv[3], argv[4]); //找到argv[3]中返回值被检查的函数，写入argv[4]
	else if (opt == LLVM_INJECT)
		InjectFaults(mod, argv[3], argv[4]); //argv[3]func.txt，argv[4]pair_file，不错误注入的call指令之后调用MyPairRec

	outs() << "\nEHTest: Finish handling " << argv[2] << "\n\n";

	verifyModule(*mod);

	//Create the new .bc file according to Module
	if (opt == LLVM_RECORD || opt == LLVM_INJECT)
	{
		std::string ErrorInfo;
  		OwningPtr<tool_output_file> Out
  		(new tool_output_file(argv[2], ErrorInfo, raw_fd_ostream::F_Binary));
		WriteBitcodeToFile(mod, Out->os());
		Out->keep();
	}

    return 0;
}
