#include "RecordInst.h"

Function* create_add_function(Module* module, LLVMContext& context) {
  // 函数基本信息
  Constant* c = module->getOrInsertFunction("add", // 函数名
    // 返回值
    IntegerType::get(context, 32),                                             
    // 函数参数
    IntegerType::get(context, 32), IntegerType::get(context, 32), 
    NULL);
  Function* add = cast<Function> (c);
  // 调用约定
  add->setCallingConv(CallingConv::C);                            

  // 参数名
  Function::arg_iterator args = add->arg_begin();
  Value* a = args++;
  a->setName("a");
  Value* b = args++;
  b->setName("b");

  // 指令块
  BasicBlock* block = BasicBlock::Create(context, "entry", add);
  IRBuilder<> builder(block);
  // tmp = a + b
  Value* tmp = builder.CreateBinOp(Instruction::Add, a, b, "tmp");
  // return tmp
  builder.CreateRet(tmp);

  return add;
}

int main(int argc, char** argv) {
  // 创建Context
  LLVMContext& context = getGlobalContext();
  // 创建Module
  Module* module = new Module("test", context);
  // 基于Module和Context创建函数
  Function* add = create_add_function(module, context);

  // 校验Module
  verifyModule(*module, PrintMessageAction);

  // 输出Module的内容
  outs() << *module << "\n";

  // 准备运行函数
  InitializeNativeTarget();
  ExecutionEngine* engine = EngineBuilder(module).create();

  // 准备实参
  vector<GenericValue> args(2);
  args[0].IntVal = APInt(32, 1);
  args[1].IntVal = APInt(32, 2);

  // 调用函数
  GenericValue result = engine->runFunction(add, args);
  // 显示返回值
  outs() << "Result: " << result.IntVal << "\n";

  delete module;

  return 0;
}
