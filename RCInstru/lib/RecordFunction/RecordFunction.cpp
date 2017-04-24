#include "RecordFunction.h"

Function *func_count;
Function *func_check;
Function *func_pass;
Function *func_record;
Function *func_pair;
Function *func_enter;
Function *func_exit;

MyValue val_table[MAX_VALUE_NUM];
int val_num = 0;

MyPair mypair[MAX_PAIR_NUM];
int pair_num = 0;

MyFunc myfunc[MAX_FUNC_NUM];
int func_num = 0;

char g_entry_func[100];
char g_exit_func[100];

//Declare the extern function of MyValueChange	 	 	 void MyCountChange(void);
Function *CountChangeInfo(Module *mod)	 	 	 
{	 	 	 
    std::vector<Type*>FuncTy_args;	 	 	 
    FunctionType* FuncTy = FunctionType::get(Type::getVoidTy(mod->getContext()),	 	 	 
                           FuncTy_args, false);	 	 	 
	 	 	 
//Create a function declaration	 	 	 
    Function* func = mod->getFunction("MyCountChange");	 	 	 
	if (!func)	 	 	 
	{	 	 	 
		func = Function::Create(FuncTy, GlobalValue::ExternalLinkage,	 	 	 
                            "MyCountChange", mod);	 	 	 
    	func->setCallingConv(CallingConv::C);	 	 	 
	}	 	 	 
	AttrListPtr func_Add_PAL;	 	 	 
	func->setAttributes(func_Add_PAL);	 	 	 
 	 	 
	return func;	 	 	 
}	 	 	 
 	 	 	 
//Declare the extern function of MyResCheck	 	 	 void MyResCheck(void);
Function *ResCheckInfo(Module *mod)	 	 	 
{	 	 	 
	std::vector<Type*>FuncTy_args;	 	 	 
	FunctionType* FuncTy = FunctionType::get(Type::getVoidTy(mod->getContext()),	 	 	 
                        FuncTy_args, false);	 	 	 
 	 	 
	//Create a function declaration	 	 	 
	Function* func = mod->getFunction("MyResCheck");	 	 	 
	if (!func)	 	 	 
	{	 	 	 
	func = Function::Create(FuncTy, GlobalValue::ExternalLinkage,	 	 	 
                                "MyResCheck", mod);	 	 	 
	func->setCallingConv(CallingConv::C);	 	 	 
	}	 	 	 
	AttrListPtr func_Add_PAL;	 	 	 
	func->setAttributes(func_Add_PAL);	 	 	 
	 	 	 
return func;	 	 	 
}	 	 	 
 	 	 	 
//Declare the extern function of MyPassFunc	 	 	 int32 MyPassFunc(char* , char* );
Function* PassFuncInfo(Module *mod)	 	 	 
{	 	 	 
    //Initialize paramater's type	 	 	 
    PointerType *PointerChar = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);	 	 
    IntegerType *IntTy32 = IntegerType::get(mod->getContext(), 32);	 	 	 
    std::vector<Type*>FuncTy_args;	 	 	 
    FuncTy_args.push_back(PointerChar);	 	 	 
    FuncTy_args.push_back(IntTy32);	 
    FuncTy_args.push_back(IntTy32);	 	 	 
    FunctionType* FuncTy = FunctionType::get(IntTy32, FuncTy_args, false);	 	 	 
 	 	 	 
    //Create a function declaration	 	 	 
    Function* func = mod->getFunction("MyPassFunc");	 	 	 
    if (!func)	 	 	 
    {	 	 	 
        func = Function::Create(FuncTy, GlobalValue::ExternalLinkage,	 	 	 
                               "MyPassFunc", mod);	 	 	 
        func->setCallingConv(CallingConv::C);	 	 	 
    }	 	 	 
    AttrListPtr func_Add_PAL;	 	 	 
    func->setAttributes(func_Add_PAL);	 	 	 
 	 	 	 
    return func;	 	 	 
}

//Declare the extern function of MyRecFunc	 	    void MyRecFunc(char* , char* , char* );
Function* RecTargetInfo(Module *mod)	 	
{	 	
	//Initialize paramater's type	 	 	 
    PointerType *PointerChar = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);	 	 
    IntegerType *IntTy32 = IntegerType::get(mod->getContext(), 32);	 	 	 
    std::vector<Type*>FuncTy_args;	 	 	 
    FuncTy_args.push_back(PointerChar);	 	 	 
    FuncTy_args.push_back(IntTy32);	 
    FuncTy_args.push_back(IntTy32);	 
    FunctionType* FuncTy = FunctionType::get(Type::getVoidTy(mod->getContext()),
	                                            FuncTy_args, false);	 	
 	 	
    
    Function* func = mod->getFunction("MyRecFunc");
    if (!func)
    {	 
        func = Function::Create(FuncTy, GlobalValue::ExternalLinkage,	 	
                                "MyRecFunc", mod);
	    func->setCallingConv(CallingConv::C);		 
    }	 	 	 
    AttrListPtr func_Add_PAL;	 	 	 
    func->setAttributes(func_Add_PAL);	 	 	 
 	 	 	 
    return func;	 	 	 
}

//Declare the extern function of MyFuncEnter            void MyFuncEnter(char* );
Function *FuncEnterInfo(Module *mod) 
{
	//Initialize paramater's type
	PointerType *PointerChar = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);
	std::vector<Type*>FuncTy_args;
	FuncTy_args.push_back(PointerChar);
	FunctionType* FuncTy = FunctionType::get(Type::getVoidTy(mod->getContext()),
  											 FuncTy_args, false);

	//Create a function declaration
	Function* func = mod->getFunction("MyFuncEnter");
	if (!func) 
	{
 		func = Function::Create(FuncTy, GlobalValue::ExternalLinkage,
								"MyFuncEnter", mod);
		func->setCallingConv(CallingConv::C);
	}
	AttrListPtr func_Add_PAL;
	func->setAttributes(func_Add_PAL);

	return func;
}

//Declare the extern function of MyFuncExit     void MyFuncExit(char* );
Function *FuncExitInfo(Module *mod) 
{
	//Initialize paramater's type
	PointerType *PointerChar = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);
	std::vector<Type*>FuncTy_args;
	FuncTy_args.push_back(PointerChar);
	FunctionType* FuncTy = FunctionType::get(Type::getVoidTy(mod->getContext()),
  											 FuncTy_args, false);

	//Create a function declaration
	Function* func = mod->getFunction("MyFuncExit");
	if (!func) 
	{
 		func = Function::Create(FuncTy, GlobalValue::ExternalLinkage,
								"MyFuncExit", mod);
		func->setCallingConv(CallingConv::C);
	}
	AttrListPtr func_Add_PAL;
	func->setAttributes(func_Add_PAL);

	return func;
}

//Create words to paramater
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
		return const_ptr;
	}

	//if not find it, then create a new word in global value table
	ArrayType *ArrayTy = ArrayType::get(IntegerType::get(mod->getContext(), 8), str.length()+1);
	GlobalVariable *global_name = new GlobalVariable(*mod, ArrayTy, true, 
									GlobalValue::PrivateLinkage, 0, mystr);
	global_name->setAlignment(1);

	//Constant Definitions 
	Constant* const_array = ConstantDataArray::getString(mod->getContext(), str, true);
	ConstantInt* const_int = ConstantInt::get(mod->getContext(), APInt(32, 0));
	std::vector<Constant*> const_ptr_indices;
	const_ptr_indices.push_back(const_int);
	const_ptr_indices.push_back(const_int);
	Constant* const_ptr = ConstantExpr::getGetElementPtr(global_name, const_ptr_indices);

	//Global Variable Definitions
	global_name->setInitializer(const_array);
	return const_ptr;
}

//Insert count value change function			 	 在函数的第一条语句前，调用void MyCountChange(void);
void CreateCountChange(Function *MyFn, Module *mod)	 	 	 
{	 	 	 
    Instruction *first_inst = (MyFn->getEntryBlock()).getFirstNonPHI();	 	 	 
 	 	 	 
    //Prepare parameters	 	 	 
    std::vector<Value*> para;	 	 	 
 	 	 	 
    //Call MyCountChange function before the first instruction of the caller	 	 	 
    CallInst *mycall;	 	 	 
    mycall = CallInst::Create(func_count, para, "", first_inst);	 	 	 
    mycall->setCallingConv(CallingConv::C);	 	 	 
    mycall->setTailCall(false);	 	 	 
}	 	 	 
 	 	 	 
//Insert resource check function	 	 	 
/*	 	 	 
void CreateResCheck(Function *MyFn, Module *mod)	 	 	 
{	 	 	 
    for (Function::iterator it_Fn = MyFn->begin(); it_Fn != MyFn->end(); it_Fn++)	 	 	 
    {	 	 	 
        BasicBlock *MyBB = &(*it_Fn);	 	 	 
        for (BasicBlock::iterator it_BB = MyBB->begin(); it_BB != MyBB->end(); it_BB++)	 	 	 
        {	 	 	 
            Instruction *MyIn = &(*it_BB);	 	 	 
            unsigned OpCode = MyIn->getOpcode();	 	 	 
            if (OpCode == Instruction::Ret)	 	 	 
            {	 	 	 
                //Prepare parameters	 	 	 
                std::vector<Value*> para;	 	 	 
 	 	 	 
                //Call MyResCheck function before the return instruction	 	 	 
                CallInst *mycall;	 	 	 
                mycall = CallInst::Create(func_check, para, "", MyIn);	 	 	 
                mycall->setCallingConv(CallingConv::C);	 	 	 
                mycall->setTailCall(false);	 	 	 
            }	 	 	 
        }  	 	 	 
    }	 	 	 
}	 	 	 
*/	 	 	 
 	 	 	 
//Insert function enter and exit log                 在函数的第一条语句前，调用void MyFuncEnter(char* 函数名);
void CreateFuncEnterExit(Function *MyFn, Module *mod)     //在函数的return语句前，调用void MyFuncExit(char* 函数名);
{
    int flag = 0;	 	 	 
    Constant *func_name = CreateWords(mod, MyFn->getName().str());	 	 	 
//  if (strstr(MyFn->getName().str().c_str(), "llvm."))	 	 	 
//      return;	 	 	 
 	 	 	 
    //Prepare parameters	
	std::vector<Value*> para;
	para.push_back(func_name);

	for (Function::iterator it_Fn = MyFn->begin(); it_Fn != MyFn->end(); it_Fn++)
	{
		BasicBlock *MyBB = &(*it_Fn);
		for (BasicBlock::iterator it_BB = MyBB->begin(); it_BB != MyBB->end(); it_BB++)
		{
			Instruction *MyIn = &(*it_BB);
			//outs() << *it_BB << "\n";
			//outs() << "OpCode = " << MyIn->getOpcode() << "\n";
			unsigned OpCode = MyIn->getOpcode();
 			//Call MyFuncEnter function berfore the first instruction of the caller
			if (flag == 0) {
				CallInst *mycall;
				mycall = CallInst::Create(func_enter, para, "", MyIn);
				mycall->setCallingConv(CallingConv::C);
				mycall->setTailCall(false);	
				flag = 1;
				//break;
			}

			if (OpCode == Instruction::Ret) 
			{
				//Call MyFuncExit function before the return instruction
				CallInst *mycall;
				mycall = CallInst::Create(func_exit, para, "", MyIn);
				mycall->setCallingConv(CallingConv::C);
				mycall->setTailCall(false);	
				//break;
			}
		}
	}
}

//Insert pass function	 	 	 在赋值之前，调用int32 MyPassFunc(char* 被调用者, char* 调用者);
CallInst *CreatePass(Instruction *valInst, char *func, int line, int fault, Module *mod)	 	 	 
{	 	 	 
	string str(func);
    Constant *func_name = CreateWords(mod, str);	
    ConstantInt* const_line = ConstantInt::get(mod->getContext(), APInt(32, line)); 	
    ConstantInt* const_fault = ConstantInt::get(mod->getContext(), APInt(32, fault)); 	 	 
 	 	 	 
    //Prepare parameters for MyPassFunc	 	 	 
    std::vector<Value*> para;	 	 	 
    para.push_back(func_name);	 	 	 
    para.push_back(const_line);	
    para.push_back(const_fault);	 	 	 
 	 	 	 
    //Call MyPassFunc function	 	 	 
    CallInst *mycall;	 	 	 
    mycall = CallInst::Create(func_pass, para, "", valInst);	 	 	 
    mycall->setCallingConv(CallingConv::C);	 	 	 
    mycall->setTailCall(false);	 	 	 
    return mycall;	 	 	 
}

void CreateRecTarget(ICmpInst *mycmp, char *func, int line, int fault, Module *mod)
{
	string str(func);
    Constant *func_name = CreateWords(mod, str);	
    ConstantInt* const_line = ConstantInt::get(mod->getContext(), APInt(32, line)); 	
    ConstantInt* const_fault = ConstantInt::get(mod->getContext(), APInt(32, fault)); 	 	 
 	 	 	 
    //Prepare parameters for MyPassFunc	 	 	 
    std::vector<Value*> para;	 	 	 
    para.push_back(func_name);	 	 	 
    para.push_back(const_line);	
    para.push_back(const_fault);	 	 	 
 	 	 	 
    //Call MyPassFunc function	 	 	 
    CallInst *mycall;	 	 	 
    mycall = CallInst::Create(func_record, para, "", mycmp);	 	 	 
    mycall->setCallingConv(CallingConv::C);	 	 	 
    mycall->setTailCall(false);	 	 	 
    //return mycall;	 	 	 
}

//Make function information log

void GetTarget(Module *mod, char *infile)
{
	func_record = RecTargetInfo(mod);
	char func[100];
	int line, fault;
	FILE *fp_in;

	fp_in = fopen(infile, "r");

	if (!fp_in)
	{
		printf("RCTest: Fail to open file!\n");
		return;
	}

	while (fscanf(fp_in, "%s%d%d", func, &line, &fault) != EOF)
	{
		InsertRecTarget(func, line, fault, mod);
	}
	fclose(fp_in);
}

void InsertRecTarget(char* func, int line, int fault, Module *mod)
{
	for (Module::iterator it_MM = mod->begin(); it_MM != mod->end(); it_MM++)
	{
		Function *MyFn = &(*it_MM);
		char caller_name[100];
		strcpy(caller_name, MyFn->getName().str().c_str());

		//Leave alone inline callers 
		AttrListPtr attrList = MyFn->getAttributes();
		int attr_num = attrList.getNumAttrs();
		bool attr_flag = false;
		for (int i=0; i<attr_num; i++)
		{
			Attributes attr = attrList.getAttributesAtIndex(i);
			if (attr.hasAttribute(Attributes::InlineHint) == true)
			{
				attr_flag = true;
				break;
			}
		}
		if (attr_flag == true)
			continue;

		if (strcmp(caller_name, func))	 
	        continue;

		for (Function::iterator it_Fn = MyFn->begin(); it_Fn != MyFn->end(); it_Fn++)
		{
			BasicBlock *MyBB = &(*it_Fn);
			for (BasicBlock::iterator it_BB = MyBB->begin(); it_BB != MyBB->end(); it_BB++)
			{
				Instruction *MyIn = &(*it_BB);
				//outs() << *MyIn << "\n";

				unsigned OpCode = MyIn->getOpcode();
				if(OpCode == Instruction::ICmp)    //是if判断
				{
					if (MDNode *N = MyIn->getMetadata("dbg")) 
					{
						DILocation Loc(N);//DILocation is in DebugInfo.h
						int Line = Loc.getLineNumber();
						if(Line == line)    //在某行
						{
							ICmpInst *mycmp = cast <ICmpInst> (MyIn);
							CreateRecTarget(mycmp, func, line, fault, mod); 	
							printf("Insert Rectarget done!\n");
							return; 
						}
					}
				}
			}
		}
	}
}

//Analyze the code to find checks
void AnalyzeSource(Module *mod, char *infile, char *outfile)
{
	char name[100], caller[100];
	FILE *fp_in, *fp_out;

	fp_in = fopen(infile, "r");
	fp_out = fopen(outfile, "a");
	if (!fp_in || !fp_out)
	{
		printf("EHTest: Fail to open file!\n");
		return;
	}

	while (fscanf(fp_in, "%s%s", name, caller) != EOF)
	{
		if (FindFunction(mod, name, caller) == true)
			fprintf(fp_out, "%s   %s\n", name, caller);
	}

	fclose(fp_in);
	fclose(fp_out);
}

//Locate the function
bool FindFunction(Module *mod, char *name, char *caller)
{
	for (Module::iterator it_MM = mod->begin(); it_MM != mod->end(); it_MM++)
	{
		Function *MyFn = &(*it_MM);
		char str[100];
		strcpy(str, MyFn->getName().str().c_str());

		if (strcmp(str, caller) == 0)
		{
			for (Function::iterator it_Fn = MyFn->begin(); it_Fn != MyFn->end(); it_Fn++)
			{
				BasicBlock *MyBB = &(*it_Fn);
				for (BasicBlock::iterator it_BB = MyBB->begin(); it_BB != MyBB->end(); it_BB++)
				{
					Instruction *MyIn = &(*it_BB);
					unsigned OpCode = MyIn->getOpcode();
					if (OpCode == Instruction::Call)
					{
						//Get called function's handle
						CallInst *mycall = cast <CallInst> (MyIn);
						Function *myfunc = mycall->getCalledFunction();

						if (!myfunc)
							continue;

						//If the function and its caller are matched
						char mystr[100];
						strcpy(mystr, myfunc->getName().str().c_str());
						if (strcmp(mystr, name) == 0)
						{
							if (FindCheck(mycall, MyBB) == true)
								return true;
						}
					}
				}	
			}
		}
	}
	return false;
}

//Find value in the value table
int FindValue(Value *swap, int opt)
{
	int i;

	for (i=0; i<val_num; i++)
	{
		if (opt == FIND_VALUE_REAL)
		{
			if (val_table[i].real == swap)
				return i;
		}
		else if (opt == FIND_VALUE_TEMP)
		{
			if (val_table[i].temp == swap)
				return i;
		}
	}
	return -1;
}

//Find struct value in the value table
int FindStructValue(Value *struct_host, int struct_offset)
{
	int i;

	for (i=0; i<val_num; i++)
	{
		if (val_table[i].struct_host == struct_host &&
			val_table[i].struct_offset == struct_offset)
			return i;
	}
	return -1;
}

//Create a new value structure in data analysis
void CreateNewValue(Value *real, Value *temp, Value *struct_host,
					int struct_offset, MyValue *store_from)
{
	val_table[val_num].real = real;
	val_table[val_num].temp = temp;
	val_table[val_num].struct_host = struct_host;
	val_table[val_num].struct_offset = struct_offset;
	val_table[val_num].store_from = store_from;
	val_num ++;
}

//Check whether the return value is checked after the function call
bool FindCheck(CallInst *mycall, BasicBlock *MyBB)
{
	Instruction *MyIn, *LastIn;

	//Create a value structure for return value
	val_num = 0;
	CreateNewValue(mycall, mycall, NULL, -1, NULL);

	//Scan the instructions after the function call in the same basicblock
    //([MAYBE] function is better)
	MyIn = mycall->getNextNode();
	LastIn = MyBB->getTerminator();
	while (MyIn != LastIn)
	{
		unsigned OpCode = MyIn->getOpcode();

		if (OpCode == Instruction::BitCast)
		{
			BitCastInst *mycast = cast <BitCastInst> (MyIn);

			Value *val = mycast->getOperand(0);
			int index = FindValue(val, FIND_VALUE_REAL);
			if (index >= 0)
				val_table[index].temp = mycast;
			else
				CreateNewValue(val, mycast, NULL, -1, NULL);
		}

		if (OpCode == Instruction::Load)
		{
			LoadInst *myload = cast <LoadInst> (MyIn);

			Value *val = myload->getOperand(0);	
			int index = FindValue(val, FIND_VALUE_REAL);
			if (index >= 0)
				val_table[index].temp = myload;
			else
				CreateNewValue(val, myload, NULL, -1, NULL);
		}

		if (OpCode == Instruction::Store)
		{
			StoreInst *mystore = cast <StoreInst> (MyIn);

			Value *val1 = mystore->getOperand(0);
			int index1 = FindValue(val1, FIND_VALUE_TEMP);
			if (index1 < 0)
			{
				//outs() << "Error: An unknown temp value in StoreInst!\n";
				MyIn = MyIn->getNextNode();
				continue;	
			}

			Value *val2 = mystore->getOperand(1);
			int index2 = FindValue(val2, FIND_VALUE_REAL);
			if (index2 < 0)
			{
				CreateNewValue(val2, NULL, NULL, -1, NULL);
				index2 = val_num - 1;
			}
			val_table[index2].store_from = &(val_table[index1]);
		}

		if (OpCode == Instruction::GetElementPtr)
		{
			GetElementPtrInst *myptr = cast <GetElementPtrInst> (MyIn);
			if (myptr->getNumOperands() != 3)
			{
				MyIn = MyIn->getNextNode();
				continue;
			}

			//Get structure host value
			Value *val = myptr->getOperand(0);
			int index = FindValue(val, FIND_VALUE_TEMP);
			if (index < 0)
			{
				int index = FindValue(val, FIND_VALUE_REAL);
				if (index < 0)
				{
					//outs() << "Error: An unknown value in ElementPtrInst!\n";
					MyIn = MyIn->getNextNode();
					continue;
				}
			}
			Value *host = val_table[index].real;

			//Get structure offset
			Value *tmp = myptr->getOperand(2);
			if (tmp->getValueID() != Value::ConstantIntVal)
			{
				//outs() << "Error: No constant offset in GetElementPtrInst!\n";
				MyIn = MyIn->getNextNode();
				continue;
			}
			ConstantInt *myconst = cast <ConstantInt> (tmp);
			int offset = myconst->getSExtValue();

			//Update or handle the structure data
			int struct_index = FindStructValue(host, offset);
			if (struct_index < 0)
				CreateNewValue(myptr, NULL, host, offset, NULL);
			else
				val_table[struct_index].real = myptr;
		}

		if (OpCode == Instruction::ICmp)
		{
			ICmpInst *mycmp = cast <ICmpInst> (MyIn);
			Value *val1 = mycmp->getOperand(0);
			Value *val2 = mycmp->getOperand(1);	
            Value *ret_val, *const_val;	 	 	 
            int flag = 0;	 	 	 
 	 	
            //Compare with constant int or null pointer	 	 	 
            if (val1->getValueID() != Value::ConstantIntVal &&	 	 	 
                val1->getValueID() != Value::ConstantPointerNullVal)	 	 	 
            {	
                flag++;	
                ret_val = val1;	 	 	 
                const_val = val2;	 	 	 
            }	 	 	 
            if (val2->getValueID() != Value::ConstantIntVal &&	 	 	 
                val2->getValueID() != Value::ConstantPointerNullVal)	 	 	 
            {	 	 	 
	               flag++;	 	 	 
                ret_val = val2;	 	 	 
                const_val = val1;	 	 	 
            }	 	 	 
            if (flag != 1) {	 	 	 
                MyIn = MyIn->getNextNode();
                continue;
            }	 	
 	 	
            int index = FindValue(ret_val, FIND_VALUE_TEMP);	
            if (index < 0)	 	
            {	
                //outs() << "Error: An unknown value in ICmpInst!\n";	
                MyIn = MyIn->getNextNode();	
                continue;	 	
            }	
 	 	
            //Constant int must be zero	 	
            if (const_val->getValueID() == Value::ConstantIntVal)	
            {	
                ConstantInt *myconst = cast <ConstantInt> (const_val);
				int tmp = myconst->getSExtValue();
				if (tmp)
				{
					MyIn = MyIn->getNextNode();
					continue;
				}
			}

			if (val_table[index].real == mycall)
				return true;
			
			MyValue *store_from = val_table[index].store_from;
			if (store_from)
			{
				if (store_from->real == mycall)
					return true;
			}
		}

		MyIn = MyIn->getNextNode();
	}

	return false;
}

//Read pair function from the file
bool ReadPairFile(char *file)
{
	FILE *fp;
	char require_func[100];
	char release_func[100];
	int require_pos;
	int release_pos;

	fp = fopen(file, "r");
	if (!fp)
	{
		printf("EHTest: Fail to open pair file!\n");
		return false;
	}

	pair_num = 0;
	while (fscanf(fp, "%s%s%d%d", require_func, release_func,
								&require_pos, &release_pos) != EOF)
	{
		strcpy(mypair[pair_num].require_func, require_func);
		strcpy(mypair[pair_num].release_func, release_func);
		mypair[pair_num].require_pos = require_pos;
		mypair[pair_num].release_pos = release_pos;
		pair_num ++;
	}

	fclose(fp);
	return true;
}

//Read target function from the file	
bool ReadFuncFile(char *file)	 	
{	
    FILE *fp;	
    char func[100];	 	 	 
    int line, fault;	 	 	 
 	 	 	 
    fp = fopen(file, "r");	 	 	 
    if (!fp)	 	 	 
    {	 	 	 
        printf("EHTest: Fail to open target function file!\n");	 	 	 
        return false;	 	
    }	 	 	 
 	
    func_num = 0;	
    while (fscanf(fp, "%s%d%d", func, &line, &fault) != EOF)	 	 	 
    {	 	 	 
        strcpy(myfunc[func_num].func, func);	 	 	 
        myfunc[func_num].line = line;
        myfunc[func_num].fault = fault;	 	 	 
        func_num ++;	 	 	 
    }	 	 	 
    fclose(fp);	 	 	 
    return true;	 	 	 
}

//Read entry and exit function from the file	 
/*bool ReadEntryExitFile(char *file)	 	 	 
{	 	 	 
    FILE *fp;	 	 	 
    fp = fopen(file, "r");	 	 	 
    if (!fp)	 	 	 
    {	 	 	 
        printf("EHTest: Fail to open target function file!\n");	 	 	 
        return false;	 	 	 
    }	 	 	 
    fscanf(fp, "%s", g_entry_func);	 	 	 
    fclose(fp);	 	 	 
    return true;	 	 	 
}*/	 	 	 
 	 	 	 
//Inject fault into a single target function
void InjectSingleFault(Module *mod, char *func, int line, int fault) {
	for (Module::iterator it_MM = mod->begin(); it_MM != mod->end(); it_MM++)
	{
		Function *MyFn = &(*it_MM);
		char caller_name[100];
		strcpy(caller_name, MyFn->getName().str().c_str());

		//Leave alone inline callers 
		AttrListPtr attrList = MyFn->getAttributes();
		int attr_num = attrList.getNumAttrs();
		bool attr_flag = false;
		for (int i=0; i<attr_num; i++)
		{
			Attributes attr = attrList.getAttributesAtIndex(i);
			if (attr.hasAttribute(Attributes::InlineHint) == true)
			{
				attr_flag = true;
				break;
			}
		}
		if (attr_flag == true)
			continue;

		if (strcmp(caller_name, func))	 
            continue;

		for (Function::iterator it_Fn = MyFn->begin(); it_Fn != MyFn->end(); it_Fn++)
		{
			BasicBlock *MyBB = &(*it_Fn);
			for (BasicBlock::iterator it_BB = MyBB->begin(); it_BB != MyBB->end(); it_BB++)
			{
				Instruction *MyIn = &(*it_BB);
				//outs() << *MyIn << "\n";

				unsigned OpCode = MyIn->getOpcode();
				if(OpCode == Instruction::ICmp || OpCode == Instruction::Switch)    //是if判断
				{
					if (MDNode *N = MyIn->getMetadata("dbg")) 
					{
						DILocation Loc(N);//DILocation is in DebugInfo.h
						int Line = Loc.getLineNumber();
						if(Line == line)    //在某行
						{
							//Compare with constant int or null pointer	 
							int const_para = 0;	 
							if(OpCode == Instruction::ICmp)
							{
								if (MyIn->getOperand(1)->getValueID() == Value::ConstantIntVal)	 	 	 
								{	
									const_para = 0;
								}
								else if(MyIn->getOperand(0)->getValueID() == Value::ConstantIntVal)
								{
									const_para = 1;
								}
								else
								{
									continue;
								}
							}
							else if(OpCode == Instruction::Switch)
							{
								const_para = 0;
							}
							
							if (Instruction* inst = dyn_cast<Instruction>(MyIn->getOperand(const_para))) 
							{
								unsigned OpCode = inst->getOpcode();
								if(OpCode == Instruction::And)
								{
									int inst_para = 0;
									if(inst->getOperand(1)->getValueID() == Value::ConstantIntVal)
									{
										inst_para = 0;
									}
									else if(inst->getOperand(0)->getValueID() == Value::ConstantIntVal)
									{
										inst_para = 1;
									}
									else
									{
										continue;
									}
									if (LoadInst* valInst = dyn_cast<LoadInst>(inst->getOperand(inst_para))) 
									{
										outs() << "& struct simple, find LoadInst:" << *valInst<< "    " << *valInst->getType() << "\n";
										InsertChange(valInst, MyFn, func, line, fault, mod);
										outs() << "instrunment done\n";
										return;
									}
									else if(ZExtInst* zeInst = dyn_cast<ZExtInst>(inst->getOperand(inst_para)))
									{
										if (LoadInst* valInst = dyn_cast<LoadInst>(zeInst->getOperand(0))) 
										{
											outs() << "& array, ind LoadInst:" << *valInst<< "    " << *valInst->getType() << "\n";
											InsertChange(valInst, MyFn, func, line, fault, mod);
											outs() << "instrunment done\n";
											return;
										}
									}
								}
								else if(OpCode == Instruction::Load)
								{
									outs() << "struct simple, find LoadInst:" << *inst<< "    " << *inst->getType() << "\n";
									InsertChange(inst, MyFn, func, line, fault, mod);
									outs() << "instrunment done\n";
									return;
								}
								else if(OpCode == Instruction::ZExt)
								{
									if (LoadInst* valInst = dyn_cast<LoadInst>(inst->getOperand(0))) 
									{
										outs() << "array, find LoadInst:" << *valInst<< "    " << *valInst->getType() << "\n";
										InsertChange(valInst, MyFn, func, line, fault, mod);
										outs() << "instrunment done\n";
										return;
									}
								}
							}
						}
					}
				}
			}	
		}
		return;
	}
}

//Inject faults into the code	 	 	 
void InjectFaults(Module *mod, char *funcfile, char *pairfile)	 	 	 
{	 	
    if (ReadPairFile(pairfile) == false)	 	 	 
        return;	 	 	 
    if (ReadFuncFile(funcfile) == false)	 	 	 
        return;	 	 	 
    func_pair = PairInfo(mod);	 	 	 
    func_pass = PassFuncInfo(mod);	 	 	 
    func_count = CountChangeInfo(mod);	 	 	 
    func_check = ResCheckInfo(mod);	 	 	 
    func_enter = FuncEnterInfo(mod);	 	 	 
    func_exit = FuncExitInfo(mod);	 	 	 
 	 	
    //Inject fault into each target function	 	 	 
    for (int i = 0; i < func_num; i++)	//************************************************change here, not inject now! 	 	 
        InjectSingleFault(mod, myfunc[i].func, myfunc[i].line, myfunc[i].fault);	 	 	 
 	 	 	 
    //Instrument pairchecker	 	 	 
    for (Module::iterator it_MM = mod->begin(); it_MM != mod->end(); it_MM++)	 	 	 
    {	 	 	 
        Function *MyFn = &(*it_MM);	 	 	 
 	 	 	 
        //Find the entry function and insert count change function	 	 	 
        char func_name[100];	 	 	 
        strcpy(func_name, MyFn->getName().str().c_str());	 	 	 
//      if (strcmp(func_name, g_entry_func) == 0)	 	 	 
//          CreateCountChange(MyFn, mod);	 	
 	 	
        //Find the exit function and resource check function	 	 	 
//      if (strcmp(func_name, g_exit_func) == 0)	 	 	 
//          CreateResCheck(MyFn, mod);	 	 	 
 	 	 	 
        //Leave alone inline callers	 	 	 
        AttrListPtr attrList = MyFn->getAttributes();	 	 	 
        int attr_num = attrList.getNumAttrs();	 	 	 
        bool attr_flag = false;	 	 	 
        for (int i=0; i<attr_num; i++)	 	 	 
        {	 	 	 
            Attributes attr = attrList.getAttributesAtIndex(i);	 	 	 
            if (attr.hasAttribute(Attributes::InlineHint) == true)	 	 	 
            {	 	 	 
                attr_flag = true;	 	 	 
                break;	 	 	 
            }	 	
        }	 	
        if (attr_flag == true)	 	 	 
            continue;	 	 	 
 	 	 	 
        CreateFuncEnterExit(MyFn, mod);	 	 	 
        for (Function::iterator it_Fn = MyFn->begin(); it_Fn != MyFn->end(); it_Fn++)	 	 	 
        {	 	 	 
            BasicBlock *MyBB = &(*it_Fn);	 	 	 
            for (BasicBlock::iterator it_BB = MyBB->begin(); it_BB != MyBB->end(); it_BB++)	 	 	 
            {	 	 	 
                Instruction *MyIn = &(*it_BB);	 	 	 
//              outs() << *MyIn << "\n";	 	 	 
 	 	 	 	unsigned OpCode = MyIn->getOpcode();	
 	 	 	 	
                if (OpCode == Instruction::Call)	 	 	 
                {	 	 	 
                    //Get called function's handle	 	 	 
                    CallInst *mycall = cast <CallInst> (MyIn);	 	 	 
                    Function *myfunc = mycall->getCalledFunction();	 	 	 
                    if (!myfunc)	 	 	 
                        continue;	 	 	 
 	 	 	 
                    //If the function and its caller are matched	 	 	 
                    char mystr[100];	 	 	 
                    strcpy(mystr, myfunc->getName().str().c_str());	 	 	 
                    CreatePairRec(mycall, mystr, mod);	
                }	 	
            }  	 	
        }	 	
    }	 	
}

//Declare the extern function of MyRecFunc
Function* PairInfo(Module *mod)
{
	//Initialize paramater's type
	PointerType *PointerChar = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);
	PointerType *PointerPtr = PointerType::get(IntegerType::get(mod->getContext(), 64), 0);
	IntegerType *IntTy32 = IntegerType::get(mod->getContext(), 32);
	std::vector<Type*>FuncTy_args;
	FuncTy_args.push_back(PointerChar);
	FuncTy_args.push_back(IntTy32);
	FuncTy_args.push_back(PointerPtr);
	FuncTy_args.push_back(IntTy32);
	FunctionType* FuncTy = FunctionType::get(Type::getVoidTy(mod->getContext()),
  											 FuncTy_args, false);

	//Create a function declaration
	Function* func = mod->getFunction("MyPairRec");
	if (!func) 
	{
 		func = Function::Create(FuncTy, GlobalValue::ExternalLinkage,
								"MyPairRec", mod);
		func->setCallingConv(CallingConv::C);
	}
	AttrListPtr func_Add_PAL;
	func->setAttributes(func_Add_PAL);

	return func;
}

//Create MyPairRec function
void CreatePairRec(CallInst *mycall, char *name, Module *mod)
{
	int pair_flag = 0;	
	int pair_pos = 0;

	for (int i=0; i<pair_num; i++)
	{
		if (strcmp(name, mypair[i].require_func) == 0)
		{
			pair_flag = PAIR_REQUIRE;
			pair_pos = mypair[i].require_pos;
			break;
		}
		else if (strcmp(name, mypair[i].release_func) == 0)
		{
			pair_flag = PAIR_RELEASE;
			pair_pos = mypair[i].release_pos;
			break;
		}
	}
	if (pair_flag == 0)
		return;

	Constant *func_name = CreateWords(mod, name);
	Value *func_flag = ConstantInt::get(mycall->getContext(), APInt(32, pair_flag)); 

	//zero pointer or null pointer
	PointerType* ZeroPty = PointerType::get(IntegerType::get(mod->getContext(), 64), 0);
	Value *zero_ptr = ConstantPointerNull::get(ZeroPty);
	Value *zero_int = ConstantInt::get(mod->getContext(), APInt(32, 0, 10));
	Value *myvalue, *pair_ptr, *pair_int;

	if (pair_pos == -1)
		myvalue = mycall;
	else
		myvalue = mycall->getOperand(pair_pos);

	Instruction *MyIn = mycall->getNextNode();
	if (myvalue->getType()->isPointerTy() == true)
	{
		PointerType *PtrTy = PointerType::get(IntegerType::get(mod->getContext(), 64), 0);
		AllocaInst* alloc_data = new AllocaInst(PtrTy, "", MyIn);
		CastInst *cast_data = new BitCastInst(myvalue, PtrTy, "", MyIn);
		StoreInst* store_data = new StoreInst(cast_data, alloc_data, false, MyIn);
		store_data->setAlignment(8);
		LoadInst* load_data = new LoadInst(alloc_data, "", false, MyIn);
		load_data->setAlignment(8);

		pair_ptr = load_data;
		pair_int = zero_int;
	}
	else if (myvalue->getType()->isIntegerTy(32) == true)
	{
		pair_ptr = zero_ptr;
		pair_int = myvalue;
	}
	else
	{
		outs() << name << "\n";
		outs() << "EHTest: The type of paired data is unrecognized!\n";
		outs() << "EHTest: Only int32 and pointer types are supported!\n";
		return;
	}
	
	std::vector<Value *> para;
	para.push_back(func_name);
	para.push_back(func_flag);
	para.push_back(pair_ptr);
	para.push_back(pair_int);

	CallInst *pair_call;
	pair_call = CallInst::Create(func_pair, para, "", MyIn);
	pair_call->setCallingConv(CallingConv::C);
	pair_call->setTailCall(false);
}

//Replace the value in the basicblock	<>	644	//Replace the value of each instruction in the function
void ReplaceReturnValue(Instruction *ret, Value *change, BasicBlock *MyBB)	 	
{	
    Instruction *first_inst = MyBB->getFirstNonPHI();	
    LoadInst *my_load = new LoadInst(change, "_load_temp", first_inst);	 	 	 
    BasicBlock *temp_BB = MyBB;	 	 	 
 	 	 	 
    while (temp_BB && temp_BB->getParent() == MyBB->getParent())	 	 	 
    {	
        for (BasicBlock::iterator it_BB = temp_BB->begin(); it_BB != temp_BB->end(); it_BB++)	 	
        {	
            Instruction *MyIn = &(*it_BB);	 	
            MyIn->replaceUsesOfWith(ret, my_load);	
        }	
        temp_BB = temp_BB->getNextNode();		 
    }	
}	 	
	 	 	
//Insert change function	 	
void InsertChange(Instruction *valInst, Function *caller, char *func, int line, int fault, Module *mod)	
{  		 
    //insert a new temp data and pass function	 	 
    //IntegerType *IntTy32 = IntegerType::get(mod->getContext(), 32);	 
    //AllocaInst* temp_data = new AllocaInst(IntTy32, "_temp_data", valInst);	 	

    AllocaInst* temp_data = new AllocaInst(valInst->getType(), "_temp_data", valInst);	 	 	 
    CallInst *mypass = CreatePass(valInst, func, line, fault, mod);	  	 
 	 	 	 
    //insert a compare of pass function ret value	 	
    Value *zero_int = ConstantInt::get(valInst->getContext(), APInt(32, 1, 10));	 	 	 
    ICmpInst *mycmp = new ICmpInst(valInst, CmpInst::ICMP_EQ, mypass, zero_int, "_pass");	 	 	 
 	 	 	 
    //split and generat new basicblocks	 	 	 
    BasicBlock *my_bb = valInst->getParent();	 	 	 
    BasicBlock *org_bb = my_bb->splitBasicBlock(valInst, "OrgBB");	 	 	 
    BasicBlock *tar_bb = org_bb->splitBasicBlock(valInst->getNextNode(), "TarBB");	 	 	 
    BasicBlock *new_bb = BasicBlock::Create(valInst->getContext(), "NewBB", caller, org_bb);	 	 	 
    IRBuilder <>builder(new_bb);	 	 	 
 	 	 	 
    //handle the branch in the basicblock of MyPassFunc	 	 	 
    Instruction *last_inst = my_bb->getTerminator();	 	 	 
    last_inst->eraseFromParent();	 	 	 
    BranchInst::Create(new_bb, org_bb, mycmp, my_bb);	 	 	 
 	 	 	 
    //build the new basicblock	 	 	 	 	
	Value *fault_int = ConstantInt::get(valInst->getContext(), APInt(64, fault));	 
	if(valInst->getType()->isIntegerTy(32) == true)	
    {	 	
        fault_int = ConstantInt::get(valInst->getContext(), APInt(32, fault));	 	 
    }	
    else if(valInst->getType()->isIntegerTy(8) == true)
    {
    	fault_int = ConstantInt::get(valInst->getContext(), APInt(8, fault));	 
    }
    else if(valInst->getType()->isIntegerTy(16) == true)
    {
    	fault_int = ConstantInt::get(valInst->getContext(), APInt(16, fault));	 
    }
    
    builder.CreateStore(fault_int, valInst->getOperand(0));	 
    LoadInst *temp_load = builder.CreateLoad(valInst->getOperand(0), "_temp_load");	 
    builder.CreateStore(temp_load, temp_data);	 
    
    BranchInst::Create(tar_bb, new_bb);	 	 
 	 	 	 
    //handle original function value in the OrgBB  	 	 	 
    Instruction *next_inst = valInst->getNextNode();	 	  
    new StoreInst(valInst, temp_data, next_inst);	 
    	 	 
    //replace the return value with temp_data	 
    ReplaceReturnValue(valInst, temp_data, tar_bb);	 
    
    /*outs() << *new_bb << "\n";
    outs() << *org_bb << "\n";
    outs() << *tar_bb << "\n";*/
}

