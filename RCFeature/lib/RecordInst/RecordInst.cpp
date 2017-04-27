#include "RecordInst.h"

void FindCmp(Module* mod)
{
	FILE *fp_write;
	fp_write = fopen(LOGFILE, "a");
	if (!fp_write)
	{
		outs() << "Fail to open " << LOGFILE << "!\n";
		return;
	}
	
	for (Module::iterator it_MM = mod->begin(); it_MM != mod->end(); it_MM++)
	{
		Function *MyFn = &(*it_MM);

		//Leave alone inline callers 
		/*AttrListPtr attrList = MyFn->getAttributes();
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
			continue;*/
		
		//outs() << MyFn->getName().str() << "\n";

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
					//Compare with constant int or null pointer	 
					int const_para = 0;	 
					int fault = 0;
					if(OpCode == Instruction::ICmp)
					{
						if(ConstantInt* CI = dyn_cast<ConstantInt>(MyIn->getOperand(1)))	 	 	 
						{	
							const_para = 0;
							fault = CI->getSExtValue();
						}
						else if(ConstantInt* CI = dyn_cast<ConstantInt>(MyIn->getOperand(0)))	 	
						{
							const_para = 1;
							fault = CI->getSExtValue();
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
							if(ConstantInt* CI = dyn_cast<ConstantInt>(inst->getOperand(1)))
							{
								inst_para = 0;
								fault = CI->getSExtValue();
							}
							else if(ConstantInt* CI = dyn_cast<ConstantInt>(inst->getOperand(0)))
							{
								inst_para = 1;
								fault = CI->getSExtValue();
							}
							else
							{
								continue;
							}
							if (LoadInst* valInst = dyn_cast<LoadInst>(inst->getOperand(inst_para))) 
							{
								if (MDNode *N = MyIn->getMetadata("dbg")) 
								{
									DILocation Loc(N);//DILocation is in DebugInfo.h
									int Line = Loc.getLineNumber();
									string File = Loc.getFilename().str();
			  						string Dir = Loc.getDirectory().str();
			  						//outs() << "& struct simple, find LoadInst:" << *valInst<< "    " << *valInst->getType() << "\n";
									fprintf(fp_write, "%s %s %d %d\n", File.c_str(), MyFn->getName().str().c_str(), Line, fault);
			  					}
							}
							else if(ZExtInst* zeInst = dyn_cast<ZExtInst>(inst->getOperand(inst_para)))
							{
								if (LoadInst* valInst = dyn_cast<LoadInst>(zeInst->getOperand(0))) 
								{
									if (MDNode *N = MyIn->getMetadata("dbg")) 
									{
										DILocation Loc(N);//DILocation is in DebugInfo.h
										int Line = Loc.getLineNumber();
										string File = Loc.getFilename().str();
				  						string Dir = Loc.getDirectory().str();
				  						//outs() << "& struct simple, find LoadInst:" << *valInst<< "    " << *valInst->getType() << "\n";
										fprintf(fp_write, "%s %s %d %d\n", File.c_str(), MyFn->getName().str().c_str(), Line, fault);
				  					}
								}
							}
						}
						else if(OpCode == Instruction::Load)
						{
							if (MDNode *N = MyIn->getMetadata("dbg")) 
							{
								DILocation Loc(N);//DILocation is in DebugInfo.h
								int Line = Loc.getLineNumber();
								string File = Loc.getFilename().str();
		  						string Dir = Loc.getDirectory().str();
		  						//outs() << "& struct simple, find LoadInst:" << *valInst<< "    " << *valInst->getType() << "\n";
								fprintf(fp_write, "%s %s %d %d\n", File.c_str(), MyFn->getName().str().c_str(), Line, fault);
		  					}
						}
						else if(OpCode == Instruction::ZExt)
						{
							if (LoadInst* valInst = dyn_cast<LoadInst>(inst->getOperand(0))) 
							{
								if (MDNode *N = MyIn->getMetadata("dbg")) 
								{
									DILocation Loc(N);//DILocation is in DebugInfo.h
									int Line = Loc.getLineNumber();
									string File = Loc.getFilename().str();
			  						string Dir = Loc.getDirectory().str();
			  						//outs() << "& struct simple, find LoadInst:" << *valInst<< "    " << *valInst->getType() << "\n";
									fprintf(fp_write, "%s %s %d %d\n", File.c_str(), MyFn->getName().str().c_str(), Line, fault);
			  					}
							}
						}
					}
				}
			}	
		}
	}
}


