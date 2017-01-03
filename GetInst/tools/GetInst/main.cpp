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
	Module *mod = ParseIRFile(argv[2], Err, Context);
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
	
	//Handle function
	if (strcmp(argv[1], "-r") == 0)
	{
		outs() << "Make revision: " << argv[2] << "\n";
		LogFunction(mod);
		outs() << "Finish in revision: " << argv[2] << "\n\n";
		verifyModule(*mod);
	}
	//Print function
	else if (strcmp(argv[1], "-b") == 0)
		PrintInfo(0, mod);
	else if (strcmp(argv[1], "-i") == 0)
		PrintInfo(1, mod);

	//Create the new .bc file according to Module
	std::string ErrorInfo;
  	OwningPtr<tool_output_file> Out
  	(new tool_output_file(argv[2], ErrorInfo,
                        raw_fd_ostream::F_Binary));
	WriteBitcodeToFile(mod, Out->os());
	Out->keep();

    return 0;
}
