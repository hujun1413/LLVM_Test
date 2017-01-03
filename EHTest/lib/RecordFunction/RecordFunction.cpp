#include "RecordFunction.h"

Function *func_record;
Function *func_pair;

MyValue val_table[MAX_VALUE_NUM];
int val_num = 0;

MyPair mypair[MAX_PAIR_NUM];
int pair_num = 0;

//Declare the extern function of MyRecFunc
Function* RecFuncInfo(Module *mod)
{
	//Initialize paramater's type
	PointerType *PointerChar = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);
	std::vector<Type*>FuncTy_args;
	FuncTy_args.push_back(PointerChar);
	FuncTy_args.push_back(PointerChar);
	FuncTy_args.push_back(PointerChar);
	FunctionType* FuncTy = FunctionType::get(Type::getVoidTy(mod->getContext()),
  											 FuncTy_args, false);

	//Create a function declaration
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


//Record functions returning pointers or integers
void CreateRecFunc(CallInst *func, Function *caller, Instruction *MyIn, Module *mod)
{
	Type *retType = func->getType();
	if (retType->isIntegerTy(32) == true || retType->isPointerTy() == true)
	{
		//Get type name
		string str("");
		raw_string_ostream stream(str);
		stream << *retType;
		Constant *ret_type = CreateWords(mod, stream.str());

		//Get function name
		Function *myfunc = func->getCalledFunction();
		if (myfunc->getName().find("llvm") != string::npos)
			return;
		Constant *func_name = CreateWords(mod, myfunc->getName().str());

		//Get caller-function name
		Constant *caller_name = CreateWords(mod, caller->getName().str());

		//Prepare parameters for MyRecFunc
		std::vector<Value*> para;
		para.push_back(func_name);
		para.push_back(caller_name);
		para.push_back(ret_type);

		//Call MyRecFunc function
		CallInst *mycall;
		mycall = CallInst::Create(func_record, para, "", MyIn);
		mycall->setCallingConv(CallingConv::C);
		mycall->setTailCall(false);	
	}
}

//Make function information log
void LogFunction(Module *mod)
{
	func_record = RecFuncInfo(mod);

	for (Module::iterator it_MM = mod->begin(); it_MM != mod->end(); it_MM++)
	{
		Function *MyFn = &(*it_MM);

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
					
					//Avoid function pointer and asm call
					if (myfunc == NULL)	
						continue;

					//Handle each function call
					it_BB ++;
					Instruction *MyTemp = &(*it_BB);	
					CreateRecFunc(mycall, MyFn, MyTemp, mod);
					it_BB --;
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

	//Scan the instructions after the function call
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
			int index = FindValue(val1, FIND_VALUE_TEMP);
			if (index < 0)
			{
				//outs() << "Error: An unknown value in ICmpInst!\n";
				MyIn = MyIn->getNextNode();
				continue;
			}

			//Compare with constant int or null pointer
			Value *val2 = mycmp->getOperand(1);
			if (val2->getValueID() != Value::ConstantIntVal &&
				val2->getValueID() != Value::ConstantPointerNullVal)
			{
				//outs() << "Error: No constant value in ICmpInst!\n";
				MyIn = MyIn->getNextNode();
				continue;
			}

			//Constant int must be zero
			if (val2->getValueID() == Value::ConstantIntVal)
			{
				ConstantInt *myconst = cast <ConstantInt> (val2);
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

//Inject fault into functions' return values
void InjectFault(Module *mod, char *name, char *caller, char *pairfile)
{
	if (ReadPairFile(pairfile) == false)
		return;

	func_pair = PairInfo(mod);

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
					if (strcmp(caller_name, caller) == 0)
					{
						if (strcmp(mystr, name) == 0)
						{
							it_BB ++;
							InsertChange(mycall, MyFn);
							continue;
						}
					}
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
/*		IntegerType *IntTy = IntegerType::get(mod->getContext(), 32);
		AllocaInst* alloc_data = new AllocaInst(IntTy, "", MyIn);
		CastInst *cast_data = new BitCastInst(myvalue, IntTy, "", MyIn);
		StoreInst* store_data = new StoreInst(cast_data, alloc_data, false, MyIn);
		store_data->setAlignment(8);
		LoadInst* load_data = new LoadInst(alloc_data, "", false, MyIn);
		load_data->setAlignment(8);
*/
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

//Replace the value of each instruction in the function
void ReplaceReturnValue(CallInst *ret, Value *change, Function *caller)
{
	for(Function::iterator it_Fn = caller->begin(); it_Fn != caller->end(); it_Fn++)
	{
		BasicBlock *MyBB = &(*it_Fn);
		for(BasicBlock::iterator it_BB = MyBB->begin(); it_BB != MyBB->end(); it_BB++)
		{
			Instruction *MyIn = &(*it_BB);
			MyIn->replaceUsesOfWith(ret, change);
		}
	}
}

//Insert change function
void InsertChange(CallInst *mycall, Function *caller)
{	
	//return value type is int32 
	if (mycall->getType()->isIntegerTy(32) == true)
	{
		Value *fault_int = ConstantInt::get(mycall->getContext(), APInt(32, FAULT_INT));
		ReplaceReturnValue(mycall, fault_int, caller);
		mycall->eraseFromParent();
	}
	//return value type is pointer
	else if (mycall->getType()->isPointerTy() == true)
	{
		//CreateChange(mycall, caller, MyIn, mod);
		PointerType *Pty = cast <PointerType> (mycall->getType());
		Value *fault_pointer = ConstantPointerNull::get(Pty);
		ReplaceReturnValue(mycall, fault_pointer, caller);
		mycall->eraseFromParent();
	}
}

