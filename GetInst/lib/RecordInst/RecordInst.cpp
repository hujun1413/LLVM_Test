#include "RecordInst.h"

//External function pointer
static Function *func_rec;

//Declare the extern function of MyRecord,创造记录函数
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
		//创造函数Function::Create
 		func = Function::Create(/*Type=*/FuncTy,
									/*Linkage=*/GlobalValue::ExternalLinkage,
									/*Name=*/"MyRecord", mod);
		func->setCallingConv(CallingConv::C);
	}
	AttrListPtr func_Add_PAL;
	func->setAttributes(func_Add_PAL);

	return func;
}

//Insert record function，在基本块的第一条指令处插入一条RecordInfo的function call
void CreateRecord(BasicBlock *MyBB, int FnNum, int BBNum, Module *mod)
{

	Value *FnNum_val = ConstantInt::get(mod->getContext(), APInt(32, FnNum));   //Return a ConstantInt with the specified value and an implied Type.
	Value *InstNum_val = ConstantInt::get(mod->getContext(), APInt(32, BBNum));
	/*
	llvm::APInt::APInt	(	unsigned 	numBits,
					uint64_t 	val,
					bool 	isSigned = false 
				)		Create a new APInt of numBits width, initialized as val.*/

	//get the first instruction in basic block
	BasicBlock::iterator it_BB = MyBB->begin();	 //Instruction iterator methods.获取基本块中的第一条指令
	Instruction *MyIn = &(*it_BB);

	if (MyIn->getOpcode() == Instruction::PHI)       //Returns a member of one of the enums like Instruction::Add.
		MyIn = &(*(++ it_BB));

	//prepare all value
	std::vector<Value*> para;
	para.push_back(FnNum_val);
	para.push_back(InstNum_val);

	//put MyRecord at the first place of the basic block
	//If the first instruction is PHI, it will be placed at the second
	CallInst *mycall;
	mycall = CallInst::Create(func_rec, para, "", MyIn); //CallInst represents a function call，在MyIn之前插入一条名字为“”，参数为para的函数func_rec
	mycall->setCallingConv(CallingConv::C);
	mycall->setTailCall(false);   //不设置尾递归
	//outs() << *mycall->getType() << "\n";   //被调用函数的返回值是void
}

//Handle function information
void LogFunction(Module *mod)
{
	int FnNum, BBNum;

	//Declare external functions
	func_rec = RecordInfo(mod);  //Declare the extern function of MyRecord，创造记录函数

	//Deal with each basic block
	FnNum = 0;
	for (Module::iterator it_MM = mod->begin(); it_MM != mod->end(); it_MM++)
	{
		Function *MyFn = &(*it_MM);  //获取模块中读每一个函数
		BBNum = 0;
		for (Function::iterator it_Fn = MyFn->begin(); it_Fn != MyFn->end(); it_Fn++)  //MyFn->begin() = basicBlocks->begin() 获取函数里的第一个基本块
		{
			BasicBlock *MyBB = &(*it_Fn);   //获取函数里的所有基本块
			CreateRecord(MyBB, FnNum, BBNum, mod);  //在基本块的第一条指令处插入一条function call
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
		return;
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
		while (i != FnNum)         //根据记录的函数条数，找到函数的位置
		{
			it_MM ++;
			i ++;
		}
		Function *MyFn = &(*it_MM);  //得到函数
		
		Function::iterator it_Fn = MyFn->begin();   //typedef BasicBlockListType::iterator(Iterator for intrusive lists based on ilist_node.) llvm::Function::iterator

		i = 0;
		while (i != BBNum)
		{
			it_Fn ++;       //由function的iterator获取basicBlock的iterator
			i ++;
		}
		BasicBlock *MyBB = &(*it_Fn);   //由function/basicBlock的itetator获取basicBlock
		
		if (tmp != FnNum)    //FnNum发生变化时（即不记录相同的函数），记录此时函数的名字
		{
			tmp = FnNum;
			fprintf(fp_write, "%s\n", (MyFn->getName()).str().c_str()); 
		}

		fprintf(fp_write, "\t%s\n", (MyBB->getName()).str().c_str());  //记录basicBlock的名字

		if (option == 1)
		{
			BasicBlock::iterator it_BB;    //typedef InstListType::iterator llvm::BasicBlock::iterator

			for (it_BB = MyBB->begin(); it_BB != MyBB->end(); it_BB++)   //打印原始的basicBlock
			{
				string str;
				raw_string_ostream stream(str);
				Instruction *MyIn = &(*it_BB);    //由function的iterator获取Instruction
				stream << *MyIn;
				if (stream.str().find("MyRecord") != string::npos)  //找到MyRecord这条指令,跳过不输出
					continue;
				fprintf(fp_write, "\t\t%s\n", stream.str().c_str());  
			}
		}
	}

	fclose(fp_read);
	fclose(fp_write);
}
