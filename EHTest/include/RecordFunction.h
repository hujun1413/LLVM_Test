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
#define MAX_PAIR_NUM	100

//Create words to paramater
Constant *CreateWords(Module *mod, string str);

//Declare the extern function of MyRecFunc
Function* RecFuncInfo(Module *mod);

//Insert record function
void CreateRecord(CallInst *func, Function *caller, Instruction *MyIn, Module *mod);

//Record function information
void LogFunction(Module* mod);

/*********************************************/
typedef struct MyValue
{
	Value *real;	//real value
	Value *temp;	//temporary value in llvm bytecode
	Value *struct_host;		//record the host for structure
	int struct_offset;		//record the offset for structure
	MyValue *store_from;	//which value store to this value
}MyValue;

//Analyze the code to find checks
//infile -- contain functions to be analyzed
//outfile -- contain functions whose return values are checked
void AnalyzeSource(Module *mod, char *infile, char *outfile);

//Locate the function 
bool FindFunction(Module *mod, char *name, char *caller);

//Find value in the value table
int FindValue(Value *swap, int opt);

//Find struct value in the value table
int FindStructValue(Value *struct_host, int struct_offset);

//Create a new value structure in data analysis
void CreateNewValue(Value *real, Value *temp, Value *struct_host,
                    int struct_offset, MyValue *store_from);

//Check whether the return value is checked after the function call
bool FindCheck(CallInst *mycall, BasicBlock *MyBB);

/*********************************************/
typedef struct MyPair
{
	char require_func[100];
	char release_func[100];
	int require_pos;
	int release_pos;
}MyPair;

//Read pair function from the file
bool ReadPairFile(char *file);

//Inject fault into functions' return values
void InjectFault(Module *mod, char *func, char *caller, char *pairfile);

//Declare the extern function of MyRecFunc
Function* PairInfo(Module *mod);

//Create MyPairRec function
void CreatePairRec(CallInst *mycall, char *name, Module *mod);

//Replace the value of each instruction in the function
void ReplaceReturnValue(CallInst *ret, Value *change, Function *caller);

//Insert change function
void InsertChange(CallInst *mycall, Function *caller);

