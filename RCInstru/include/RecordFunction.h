#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Type.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/CodeGen/LinkAllCodegenComponents.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/JITEventListener.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/IRReader.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/PluginLoader.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Process.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IRBuilder.h"
#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/ValueSymbolTable.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/DebugInfo.h"
#include <stdio.h>
#include <stdlib.h>

using namespace llvm;
using namespace std;

#define LLVM_RECORD		1
#define LLVM_ANALYZE	2
#define LLVM_INJECT		3

#define MAX_VALUE_NUM	100

#define FIND_VALUE_REAL		0
#define FIND_VALUE_TEMP		1

#define FAULT_INT	-1

#define PAIR_REQUIRE	1
#define PAIR_RELEASE	2
#define MAX_PAIR_NUM	10000
#define MAX_FUNC_NUM    50000

//Create words to paramater
Constant *CreateWords(Module *mod, string str);


Function *PassFuncInfo(Module *mod);


CallInst *CreatePass(Instruction *valInst, char *func, int line, int fault, Module *mod);

//Declare the extern function of MyRecFunc
Function* RecFuncInfo(Module *mod);


/*********************************************/
typedef struct MyPair
{
	char require_func[100];
	char release_func[100];
	int require_pos;
	int release_pos;
}MyPair;

typedef struct MyFunc 	 
{	 	 	 
   	char func[100];	 
   	int line;
   	int fault; 
}MyFunc;

//Read pair function from the file
bool ReadPairFile(char *file);

//Read target function from the file		 	 
bool ReadFuncFile(char *file);	 	 	 
 	 
 	 	 	 
//Inject a fault into a single target function	 
void InjectSingleFault(Module *mod, char *name, char *caller);	 	 	 
 	 	 	 
//Inject faults into the code	 	 	 
void InjectFaults(Module *mod, char *funcfile, char *pairfile);

//Declare the extern function of MyRecFunc
Function* PairInfo(Module *mod);

//Create MyPairRec function
void CreatePairRec(CallInst *mycall, char *name, Module *mod);

//Replace the value in the basicblock
void ReplaceReturnValue(Instruction *ret, Value *change, BasicBlock *MyBB);

//Insert change function
void InsertChange(Instruction *valInst, Function *caller, char *func, int line, int fault, Module *mod);	

//Declare the extern function of MyFuncEnter 
Function *FuncEnterInfo(Module *module);	 	 	 
	 	 	 
//Declare the extern function of MyFuncExit	 	 	 
Function *FuncExitInfo(Module *module);	 	 	 
 	 	 	 
//Insert function enter and exit log	 	 	 
void CreateFuncEnterExit(Function *MyFn, Module *mod);

void LogFunction(Module *mod);

void CreateRecFunc(Function *caller, Instruction *MyIn, Module *mod);

void AnalyzeSource(Module *mod, char *infile, char *outfile);

