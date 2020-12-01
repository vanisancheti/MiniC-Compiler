//#include "ast.h"
#include <iostream>
#include"llvm/IR/BasicBlock.h"
#include"llvm/IR/Constants.h"
#include"llvm/IR/DerivedTypes.h"
#include"llvm/IR/Function.h"
#include"llvm/IR/IRBuilder.h"
#include"llvm/IR/LLVMContext.h"
#include"llvm/IR/Module.h"
#include"llvm/IR/Type.h"
#include"llvm/IR/Verifier.h"
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LegacyPassManagers.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/IRPrintingPasses.h>

using namespace std;

static llvm::LLVMContext &TheContext = llvm::getGlobalContext();
static llvm::IRBuilder<> Builder(TheContext);
static llvm::Module *TheModule;
static unordered_map<string, llvm::Value*> NamedValues;

static vector<pair<unordered_map<string, llvm::Value *>, llvm::BasicBlock* > > blockscope;
// static unordered_map<string, llvm::Value *> functionscope;

class llvmIRVisitor : public ASTvisitor
{
public:
    llvm::Value* ret;
    virtual void visit(ASTProg &node)
    {
        cout << "llvmIRVisit invoked" << endl;
        TheModule = new llvm::Module("llvmIRmodule", TheContext);

        llvm::FunctionType *ftype = llvm::FunctionType::get(llvm::Type::getVoidTy(TheContext), false);    // Initialising the return type of the main function i.e. main
        llvm::Function* mainFunction = llvm::Function::Create(ftype, llvm::GlobalValue::ExternalLinkage, "main", TheModule);

        llvm::BasicBlock *block = llvm::BasicBlock::Create(TheContext, "entry", mainFunction, 0);   // 0 to insert at the end of the function
        unordered_map<string, llvm::Value*> variables;
        blockscope.push_back(make_pair(variables, block));

        for (auto vardecl : node.variabledeclList)
            vardecl->accept(*this);

        for (auto funcdecl : node.funcdeclList)
            funcdecl->accept(*this);

        node.block->accept(*this);
        llvm::CallInst::Create(mainFunction, "", blockscope.back().second);

        Builder.CreateRetVoid();
        llvm::verifyFunction(*mainFunction);

        llvm::legacy::PassManager PM;       // Manages what all passes do we want to run on our module
        PM.add(llvm::createPrintModulePass(llvm::outs()));  // To generate the IR from the ast we gave
        PM.run(*TheModule);
    }

    virtual void visit(ASTBlock &node)
    {
        for (auto statement : node.statementList)
            statement->accept(*this);

        ASTExpr *expr = node.expr;
        if(node.functionblock){
            if(node.expr){
                expr->accept(*this);
                llvm::ReturnInst::Create(TheContext, llvm::ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), 0, true), blockscope.back().second);
            }
            else{
                llvm::ReturnInst::Create(TheContext, blockscope.back().second);
            }
        }
    }

    virtual void visit(ASTStatExpr &node)
    {
        node.expr->accept(*this);
    }

    virtual void visit(ASTStatCond &node)
    {
        node.exprter->accept(*this);
    }

    virtual void visit(ASTRead &node)
    {}

    virtual void visit(ASTReadInput &node)
    {
        ASTVar *var = node.getvar();
        var->accept(*this);
    }

    virtual void visit(ASTReadFile &node)
    {
        ASTVarID *var = node.getvar();
        ASTVarID *ID = node.getvarId();
        ASTVarSTRING *STRING = node.getvarSTRING();

        var->accept(*this);
        if(ID)
            ID->accept(*this);
        if(STRING)
            STRING->accept(*this);
    }

    virtual void visit(ASTStatBREAK &node)
    {}

    virtual void visit(ASTStatCONTINUE &node)
    {}

    virtual void visit(ASTIf &node)
    {
        ASTExpr *expr = node.getexpr();
        ASTBlock *ifblock = node.getifblock();
        ASTBlock *elseblock = node.getelseblock();

        llvm::BasicBlock *entryblock = blockscope.back().second;
        llvm::Function *functionenclosing = entryblock->getParent();
        llvm::BasicBlock *IFblock = llvm::BasicBlock::Create(TheContext, "ifblock", functionenclosing);
        llvm::BasicBlock *mergeblock = llvm::BasicBlock::Create(TheContext, "mergeblock", functionenclosing);
        
        expr->accept(*this);
        llvm::Value* cond = ret;
        llvm::Constant *constval = llvm::ConstantInt::get(llvm::Type::getInt64Ty(TheContext), 0, true);
        llvm::ICmpInst *comparison = new llvm::ICmpInst(*entryblock, llvm::ICmpInst::ICMP_NE, cond, constval, "tmp");

        unordered_map<string, llvm::Value*> variables;
        blockscope.push_back(make_pair(variables, IFblock));
        ifblock->accept(*this);
        llvm::BasicBlock *retblock = blockscope.back().second;
        blockscope.pop_back();

        if(retblock->getTerminator() == NULL)
            llvm::BranchInst::Create(mergeblock, retblock);
        
        unordered_map<string, llvm::Value*> currvariables;
        if(elseblock){
            llvm::BasicBlock *ELSEblock = llvm::BasicBlock::Create(TheContext, "elseBlock", functionenclosing);
            unordered_map<string, llvm::Value*> tmpvariables;
            blockscope.push_back(make_pair(tmpvariables, ELSEblock));
            elseblock->accept(*this);
            retblock = blockscope.back().second;
            blockscope.pop_back();
            if(retblock->getTerminator() == NULL)
                llvm::BranchInst::Create(mergeblock, retblock);

            llvm::BranchInst::Create(IFblock, ELSEblock, comparison, entryblock);
            currvariables = blockscope.back().first;
        }
        else{
            llvm::BranchInst::Create(IFblock, mergeblock, comparison, entryblock);
            currvariables = blockscope.back().first;
        }
        blockscope.pop_back();
        blockscope.push_back(make_pair(currvariables, mergeblock));
        return;
    }

    virtual void visit(ASTFor &node)
    {
        llvm::BasicBlock *entryblock = blockscope.back().second;
        llvm::Function  *enclosingfunction = entryblock->getParent(); 
        llvm::BasicBlock *headerblock = llvm::BasicBlock::Create(TheContext, "headerblock", enclosingfunction, 0); 
        llvm::BasicBlock *bodyblock = llvm::BasicBlock::Create(TheContext, "bodyblock", enclosingfunction, 0);
        llvm::BasicBlock *afterblock = llvm::BasicBlock::Create(TheContext, "afterblock", enclosingfunction, 0); 

        ASTExpr *expr1 = node.getexpr1();
        expr1->accept(*this);

        ASTExpr *expr2 = node.getexpr2();
        expr2->accept(*this);
        llvm::Value* cond = ret;
        llvm::Constant *constval = llvm::ConstantInt::get(llvm::Type::getInt64Ty(TheContext), 0, true);
        llvm::ICmpInst *comparison = new llvm::ICmpInst(*entryblock, llvm::ICmpInst::ICMP_NE, cond, constval, "tmp");
        llvm::BranchInst::Create(bodyblock, afterblock, comparison, headerblock);
        llvm::BranchInst::Create(headerblock, entryblock);

        ASTExpr *expr3 = node.getexpr3();
        expr3->accept(*this);        
        
        ASTBlock *block = node.getblock();
        unordered_map<string, llvm::Value*> variables;
        blockscope.push_back(make_pair(variables, bodyblock));
        block->accept(*this);
        bodyblock = blockscope.back().second;
        blockscope.pop_back();

        if(bodyblock->getTerminator() == NULL)
            llvm::BranchInst::Create(headerblock, bodyblock);
        
        unordered_map<string, llvm::Value*> tmpvariables;
        tmpvariables = blockscope.back().first;
        blockscope.pop_back();
        blockscope.push_back(make_pair(tmpvariables,afterblock));
        blockscope.back().first = tmpvariables;
    }

    virtual void visit(ASTWhile &node)
    {
        llvm::BasicBlock *entryblock = blockscope.back().second;
        llvm::Function  *enclosingfunction = entryblock->getParent(); 
        llvm::BasicBlock *headerblock = llvm::BasicBlock::Create(TheContext, "headerblock", enclosingfunction, 0); 
        llvm::BasicBlock *bodyblock = llvm::BasicBlock::Create(TheContext, "bodyblock", enclosingfunction, 0);
        llvm::BasicBlock *afterblock = llvm::BasicBlock::Create(TheContext, "afterblock", enclosingfunction, 0); 

        ASTExpr *expr = node.getexpr();
        expr->accept(*this);

        llvm::Value* cond = ret;
        llvm::Constant *constval = llvm::ConstantInt::get(llvm::Type::getInt64Ty(TheContext), 0, true);
        llvm::ICmpInst *comparison = new llvm::ICmpInst(*entryblock, llvm::ICmpInst::ICMP_NE, cond, constval, "tmp");
        llvm::BranchInst::Create(bodyblock, afterblock, comparison, headerblock);
        llvm::BranchInst::Create(headerblock, entryblock);

        ASTBlock *block = node.getblock();
        unordered_map<string, llvm::Value*> variables;
        blockscope.push_back(make_pair(variables, bodyblock));
        block->accept(*this);
        bodyblock = blockscope.back().second;
        blockscope.pop_back();

        if(bodyblock->getTerminator() == NULL)
            llvm::BranchInst::Create(headerblock, bodyblock);
        
        unordered_map<string, llvm::Value*> tmpvariables;
        tmpvariables = blockscope.back().first;
        blockscope.pop_back();
        blockscope.push_back(make_pair(tmpvariables,afterblock));
        blockscope.back().first = tmpvariables;
    }

    virtual void visit(ASTPrint &node)
    {
        ASTExpr *expr = node.getexpr();
        expr->accept(*this);
    }

    virtual void visit(ASTFunc &node)
    {}

    virtual void visit(ASTFuncParam &node)
    {
        llvm::Type* x;
        vector<llvm::Type*> argTypes;
        if(node.rettype == Type::INT)
            x = llvm::Type::getInt64Ty(TheContext);
        else if(node.rettype == Type::VOID)
            x = llvm::Type::getVoidTy(TheContext);
        
        for (auto vardecl : node.variabledeclList){
            if(vardecl->type1 != Type::DEFAULT_TYPE && vardecl->type1 != Type::VOID)
                x = llvm::Type::getInt64Ty(TheContext);
            else if(vardecl->type1 == Type::VOID)
                x = llvm::Type::getVoidTy(TheContext);
            argTypes.push_back(x);
        }

        if(node.vardecl->type1 != Type::DEFAULT_TYPE && node.vardecl->type1 != Type::VOID)
            x = llvm::Type::getInt64Ty(TheContext);
        else if(node.vardecl->type1 == Type::VOID)
            x = llvm::Type::getVoidTy(TheContext);
        argTypes.push_back(x);


        llvm::FunctionType *ftype = llvm::FunctionType::get(x, llvm::makeArrayRef(argTypes), false);
        llvm::Function* Function = llvm::Function::Create(ftype, llvm::GlobalValue::ExternalLinkage, node.id , TheModule);
        llvm::BasicBlock *block = llvm::BasicBlock::Create(TheContext, "entry", Function, 0);
        
        llvm::Function::arg_iterator it = Function->arg_begin();
        for (auto vardecl : node.variabledeclList){
            llvm::Value *arg = &(*it);
            arg->setName(vardecl->getID());
            vardecl->accept(*this);
            it++;
        }
        llvm::Value *arg = &(*it);
        arg->setName(node.vardecl->getID());
        node.vardecl->accept(*this);

        unordered_map<string, llvm::Value*> variables;
        blockscope.push_back(make_pair(variables, block));    
        node.block->accept(*this);
        blockscope.pop_back();
    }

    virtual void visit(ASTFuncNoparam &node)
    {
        llvm::Type* x;
        vector<llvm::Type*> argTypes;
        if(node.rettype == Type::INT)
            x = llvm::Type::getInt64Ty(TheContext);
        else if(node.rettype == Type::VOID)
            x = llvm::Type::getVoidTy(TheContext);
        
        llvm::FunctionType *ftype = llvm::FunctionType::get(x, llvm::makeArrayRef(argTypes), false);
        llvm::Function* Function = llvm::Function::Create(ftype, llvm::GlobalValue::ExternalLinkage, node.id , TheModule);
        llvm::BasicBlock *block = llvm::BasicBlock::Create(TheContext, "entry", Function, 0);
        
        unordered_map<string, llvm::Value*> variables;
        blockscope.push_back(make_pair(variables, block));    
        node.block->accept(*this);
        blockscope.pop_back();
    }

    virtual void visit(ASTFunccall &node)
    {}

    virtual void visit(ASTFunccallParam &node)
    {
        llvm::Function *function = TheModule->getFunction(node.id);
        vector<llvm::Value*> args;

        for (auto var : node.variableList){
            var->accept(*this);
            args.push_back(ret);
        }
        node.var->accept(*this);
        args.push_back(ret);
        ret = llvm::CallInst::Create(function, llvm::makeArrayRef(args), node.id, blockscope.back().second);
        return; 
    }

    virtual void visit(ASTFunccallNoparam &node)
    {
        llvm::Function *function = TheModule->getFunction(node.id);
        ret = llvm::CallInst::Create(function, node.id, blockscope.back().second);
        return;
    }

    virtual void visit(ASTVardecl &node)
    {}

    virtual void visit(ASTVarSingle &node)
    {
        cout << "llvmIRVarSingle" << endl;
        llvm::AllocaInst *Alloca = new llvm::AllocaInst(llvm::Type::getInt64Ty(TheContext), node.getID()/*name*/, blockscope.back().second/*for which basic block*/);
        new llvm::StoreInst(llvm::ConstantInt::get(llvm::Type::getInt64Ty(TheContext), 0, true), Alloca, false, blockscope.back().second);        
        blockscope.back().first.insert({node.getID(), Alloca});
        return;
    }

    virtual void visit(ASTVar1darray &node)
    {   
        ASTExpr *expr = node.getexpr();
        llvm::AllocaInst *Alloca = nullptr;
        if(expr){
            expr->accept(*this);
            // llvm::ConstantInt* CI = dynamic_cast<llvm::ConstantInt*>(ret);
            // cout << CI->getSExtValue() << endl;
            // int num = CI->getSExtValue();
            int num = 10;
            Alloca = new llvm::AllocaInst(llvm::ArrayType::get(llvm::Type::getInt64Ty(TheContext), num), "1darray", blockscope.back().second);
            new llvm::StoreInst(llvm::ConstantInt::get(llvm::Type::getInt64Ty(TheContext), 0, true), Alloca, false, blockscope.back().second);
        }
        blockscope.back().first.insert({node.getID(), Alloca});
    }

    virtual void visit(ASTVar2darray &node)
    {
        ASTExpr *rexpr = node.getexpr1();
        ASTExpr *cexpr = node.getexpr2();

        if(rexpr)
            rexpr->accept(*this);

        if(cexpr)
            cexpr->accept(*this);

    }

    virtual void visit(ASTExpr &node)
    {}

    virtual void visit(ASTExprBinary &node)
    {
        cout << "llvmExprBianry" << endl;
        ASTExpr *left = node.getLeft();
        ASTExpr *right = node.getRight();

        left->accept(*this);
        llvm::Value* leftval = ret;

        right->accept(*this);
        llvm::Value* rightval = ret;

        string op = node.getBin_operator();
        if(op == "*")
            ret = llvm::BinaryOperator::Create(llvm::Instruction::Mul, leftval, rightval, "tmp", blockscope.back().second);    
        if(op == "/")
            ret = llvm::BinaryOperator::Create(llvm::Instruction::SDiv, leftval, rightval, "tmp", blockscope.back().second);
        if(op == "%")
            ret = llvm::BinaryOperator::Create(llvm::Instruction::SRem, leftval, rightval, "tmp", blockscope.back().second);
        if(op == "+")
            ret = llvm::BinaryOperator::Create(llvm::Instruction::Add, leftval, rightval, "tmp", blockscope.back().second);
        if(op == "-")
            ret = llvm::BinaryOperator::Create(llvm::Instruction::Sub, leftval, rightval, "tmp", blockscope.back().second);
        if(op == "<")
            ret = new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SLT, leftval, rightval,"tmp", blockscope.back().second), llvm::Type::getInt64Ty(TheContext), "zext", blockscope.back().second);
        if(op == ">")
            ret = new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SGT, leftval, rightval,"tmp", blockscope.back().second), llvm::Type::getInt64Ty(TheContext), "zext", blockscope.back().second);
        if(op == "<=")
            ret = new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SLE, leftval, rightval,"tmp", blockscope.back().second), llvm::Type::getInt64Ty(TheContext), "zext", blockscope.back().second);
        if(op == ">=")
            ret = new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_SGE, leftval, rightval,"tmp", blockscope.back().second), llvm::Type::getInt64Ty(TheContext), "zext", blockscope.back().second);
        if(op == "==")
            ret = new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_EQ, leftval, rightval,"tmp", blockscope.back().second), llvm::Type::getInt64Ty(TheContext), "zext", blockscope.back().second);
        if(op == "!=")
            ret = new llvm::ZExtInst(llvm::CmpInst::Create(llvm::Instruction::ICmp, llvm::ICmpInst::ICMP_NE, leftval, rightval,"tmp", blockscope.back().second), llvm::Type::getInt64Ty(TheContext), "zext", blockscope.back().second);
        if(op == "=")
            new llvm::StoreInst(rightval, leftval, false, blockscope.back().second);
        if(op == "AND")
            ret = llvm::BinaryOperator::Create(llvm::Instruction::And, leftval, rightval, "tmp", blockscope.back().second);
        if(op == "OR")
            ret = llvm::BinaryOperator::Create(llvm::Instruction::Or, leftval, rightval, "tmp", blockscope.back().second);    

        return;
    }

    virtual void visit(ASTExprTernary &node)
    {
        ASTExpr *first = node.getFirst();
        ASTExpr *second = node.getSecond();
        ASTExpr *third = node.getThird();

        first->accept(*this);
        second->accept(*this);
        third->accept(*this);
    }

    virtual void visit(ASTVar &node)
    {}

    virtual void visit(ASTVarID &node)
    {
        cout << "llvmVarId" << endl;
        llvm::Value *val;
        for(auto i = blockscope.rbegin(); i!=blockscope.rend(); i++){
            if(i->first.find(node.getID()) != i->first.end()){
                val = i->first[node.getID()];
                break;
            }
        }

        if(val != nullptr)
            ret = new llvm::LoadInst(val, "tmp", blockscope.back().second);
        return;
    }

    virtual void visit(ASTVar1darr &node)
    {
        if(node.expr)
            node.expr->accept(*this);

    }

    virtual void visit(ASTVar2darr &node)
    {
        ASTExpr *rexpr = node.rexpr;
        ASTExpr *cexpr = node.cexpr;

        if(rexpr)
            rexpr->accept(*this);

        if(cexpr)
            cexpr->accept(*this);

    }

    virtual void visit(ASTVarINT &node)
    {
        llvm::IntegerType *it = llvm::Type::getInt64Ty(TheContext);
        ret = llvm::ConstantInt::get(it, node.getIntLit(), true);
        return;
    }

    virtual void visit(ASTVarBOOL &node)
    {
        llvm::IntegerType *it = llvm::Type::getInt64Ty(llvm::getGlobalContext());
        ret = llvm::ConstantInt::get(it, node.getBoolLit(), true);
        return;
    }

    virtual void visit(ASTVarCHAR &node)
    {
        llvm::IntegerType *it = llvm::Type::getInt64Ty(llvm::getGlobalContext());
        ret = llvm::ConstantInt::get(it, node.getCharLit(), true);
        return;
    }

    virtual void visit(ASTVarSTRING &node)
    {
        llvm::IntegerType *it = llvm::Type::getInt64Ty(llvm::getGlobalContext());
        ret = llvm::ConstantInt::get(it, node.getStringLit(), true);
        return;
    }
};