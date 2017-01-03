#include "RecordInst.h"

//External function pointer
static Function *func_rec;

//Declare the extern function of MyRecord
Function* RecordInfo(Module *mod)
{
	//Initialize paramater's type
	IntegerType *IntTy32 = IntegerType::get(mod->getContext(), 32);
	std::vector<Type*>FuncTy_args;
	FuncTy_args.push_back(IntTy32);
	FuncTy_args.push_back(IntTy32);
	FunctionType* FuncTy = FunctionType::get(/*Result=*/Type::getVoidTy(mod->getContext()),
  											 /*Params=*/FuncTy_args,
											 /*isVarArg=*/false);

	//Create a function declaration
	Function* func = mod->getFunction("MyRecord");
	if (func == NULL) 
	{
 		func = Function::Create(/*Type=*/FuncTy,
									/*Linkage=*/GlobalValue::ExternalLinkage,
									/*Name=*/"MyRecord", mod);
		func->setCallingConv(CallingConv::C);
	}
	AttrListPtr func_Add_PAL;
	func->setAttributes(func_Add_PAL);

	return func;
}

//Insert record function
void CreateRecord(BasicBlock *MyBB, int FnNum, int BBNum, Module *mod)
{

	Value *FnNum_val = ConstantInt::get(mod->getContext(), APInt(32, FnNum));
	Value *InstNum_val = ConstantInt::get(mod->getContext(), APInt(32, BBNum));

	//get the first instruction in basic block
	BasicBlock::iterator it_BB = MyBB->begin();	
	Instruction *MyIn = &(*it_BB);
	if (MyIn->getOpcode() == Instruction::PHI)
		MyIn = &(*(++ it_BB));

	//prepare all value
	std::vector<Value*> para;
	para.push_back(FnNum_val);
	para.push_back(InstNum_val);

	//put MyRecord at the first place of the basic block
	//If the first instruction is PHI, it will be placed at the second
	CallInst *mycall;
	mycall = CallInst::Create(func_rec, para, "", MyIn);
	mycall->setCallingConv(CallingConv::C);
	mycall->setTailCall(false);
}

//Handle function information
void LogFunction(Module *mod)
{
	int FnNum, BBNum;

	//Declare external functions
	func_rec = RecordInfo(mod);

	//Deal with each basic block
	FnNum = 0;
	for (Module::iterator it_MM = mod->begin(); it_MM != mod->end(); it_MM++)
	{
		Function *MyFn = &(*it_MM);
		BBNum = 0;
		for (Function::iterator it_Fn = MyFn->begin(); it_Fn != MyFn->end(); it_Fn++)
		{
			BasicBlock *MyBB = &(*it_Fn);
			CreateRecord(MyBB, FnNum, BBNum, mod);
			BBNum ++;
		}
		FnNum ++;
	}
}

//Print basicblock(0) or instruction(1) names
void PrintInfo(int option, Module* mod)
{
	FILE *fp_read, *fp_write;
	int i, FnNum, BBNum, tmp = -1;

	fp_read = fopen(MY_FILE, "r");
	fp_write = fopen(LOG_FILE, "w");
	if (!fp_read || !fp_write)
	{
		outs() << "Fail to open file!\n";
	}

	if (!(option == 0 || option == 1))
	{	
		outs() << "Invalid option!\n";
		return;
	}

	while (fscanf(fp_read, "%d%d", &FnNum, &BBNum) != EOF)
	{
		Module::iterator it_MM = mod->begin();
		i = 0;
		while (i != FnNum)
		{
			it_MM ++;
			i ++;
		}
		Function *MyFn = &(*it_MM);
		
		Function::iterator it_Fn = MyFn->begin();
		i = 0;
		while (i != BBNum)
		{
			it_Fn ++;
			i ++;
		}
		BasicBlock *MyBB = &(*it_Fn);
		
		if (tmp != FnNum)
		{
			tmp = FnNum;
			fprintf(fp_write, "%s\n", (MyFn->getName()).str().c_str()); 
		}

		fprintf(fp_write, "\t%s\n", (MyBB->getName()).str().c_str());

		if (option == 1)
		{
			BasicBlock::iterator it_BB;
			for (it_BB = MyBB->begin(); it_BB != MyBB->end(); it_BB++)
			{
				string str;
				raw_string_ostream stream(str);
				Instruction *MyIn = &(*it_BB);
				stream << *MyIn;
				if (stream.str().find("MyRecord") != string::npos)
					continue;
				fprintf(fp_write, "\t\t%s\n", stream.str().c_str());
			}
		}
	}

	fclose(fp_read);
	fclose(fp_write);
}
