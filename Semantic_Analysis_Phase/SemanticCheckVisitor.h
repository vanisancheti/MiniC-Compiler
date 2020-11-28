// #include "ast.h"
#include <bits/stdc++.h>
// #include "type.h"
using namespace std;

class SemanticCheckVisitor : public ASTvisitor
{
public:
    vector<unordered_map<string, Type> > variable_scope;
    unordered_map<string, ASTFuncParam*> functionparam_scope;
    unordered_map<string, ASTFuncNoparam*> functionnoparam_scope;
    Type functiontypes = Type::DEFAULT_TYPE;    

    virtual void visit(ASTProg &node)
    {
        cout << "ASTProg" << endl;
        unordered_map<string, Type> global_scope;
        variable_scope.push_back(global_scope);
        
        for (auto vardecl : node.variabledeclList)
            vardecl->accept(*this);

        for (auto funcdecl : node.funcdeclList)
            funcdecl->accept(*this);

        functiontypes = Type::VOID;
        node.block->accept(*this);
        if(functiontypes != Type::DEFAULT_TYPE)
            cerr << "Error:: Missing Return Statement" << endl;
    }

    virtual void visit(ASTBlock &node)
    {
        cout << "ASTBlock" << endl;
        unordered_map<string, Type> block_scope;
        variable_scope.push_back(block_scope);

        for (auto statement : node.statementList)
            statement->accept(*this);

        if(node.functionblock){
            if(node.expr){
                ASTExpr *expr = node.expr;
                expr->accept(*this);
                if(expr->type != functiontypes){
                    cerr << "Error:: Return type of the function not matched " << endl;
                }
            }
            else{
                if(functiontypes!=Type::VOID)
                {
                    cerr << "Error:: Return type of the function not matched " << endl;
                }
            }
            functiontypes = Type::DEFAULT_TYPE;
        }
        variable_scope.pop_back();
    }

    virtual void visit(ASTStatExpr &node)
    {
        cout << "ASTStatExpr" << endl;
        node.expr->accept(*this);
    }

    virtual void visit(ASTStatCond &node)
    {
        cout << "ASTStatCond" << endl;
        node.exprter->accept(*this);
    }

    virtual void visit(ASTStatBREAK &node)
    {
        cout << "ASTStatBreak" << endl;
    }

    virtual void visit(ASTStatCONTINUE &node)
    {
        cout << "ASTStatContinue" << endl;
    }

    virtual void visit(ASTIf &node)
    {
        cout << "ASTIf" << endl;
        ASTExpr *expr = node.getexpr();
        ASTBlock *ifblock = node.getifblock();
        ASTBlock *elseblock = node.getelseblock();

        expr->accept(*this);
        if(expr->type != Type::BOOL){
            cerr << "Error::Expression Type in the if block should be BOOL" << endl;
        }

        Type funcreturntype = functiontypes;
        ifblock->accept(*this);
        functiontypes = funcreturntype;
        if(elseblock)
            elseblock->accept(*this);
    }

    virtual void visit(ASTFor &node)
    {
        cout << "ASTFor" << endl;
        ASTExpr *expr1 = node.getexpr1();
        ASTExpr *expr2 = node.getexpr2();
        ASTExpr *expr3 = node.getexpr3();
        ASTBlock *block = node.getblock();

        expr1->accept(*this);
        expr2->accept(*this);
        if(expr2->type != Type::BOOL){
            cerr << "Error::Expression Type for the first expression in the For block should be BOOL" << endl;
        }
        expr3->accept(*this);

        Type funcreturntype = functiontypes;
        block->accept(*this);
        functiontypes = funcreturntype;
    }

    virtual void visit(ASTWhile &node)
    {
        cout << "ASTWhile" << endl;
        ASTExpr *expr = node.getexpr();
        ASTBlock *block = node.getblock();

        expr->accept(*this);
        if(expr->type != Type::BOOL){
            cerr << "Error::Expression Type in the while block should be BOOL" << endl;
        }
    
        Type funcreturntype = functiontypes;
        block->accept(*this);
        functiontypes = funcreturntype;
    }

    virtual void visit(ASTPrint &node)
    {
        cout << "ASTPrint" << endl;
        ASTExpr *expr = node.getexpr();
        expr->accept(*this);
    }

    virtual void visit(ASTRead &node)
    {
        cout << " ASTRead" << endl;
    }

    virtual void visit(ASTReadInput &node)
    {
        cout << " ASTReadInput" << endl;
        ASTVarID *var = node.getvar();
        var->accept(*this);

        string id = var->getID();
        bool var_found = false;
        Type type;

        for (auto it = variable_scope.rbegin(); it != variable_scope.rend(); it++) {
            if (it->find(id) != it->end()) {
                var_found = true;
                type = it->find(id)->second;
                break;
            }
        }

        if (!var_found)
            cerr << "ERROR:: " << id << " not found" << endl;
        else
            var->type = type;
    }

    virtual void visit(ASTReadFile &node)
    {
        ASTVarID *var = node.getvar();
        ASTVarID *ID = node.getvarId();
        ASTVarSTRING *STRING = node.getvarSTRING();

        var->accept(*this);

        string id = var->getID();
        bool var_found = false;
        Type type;

        for (auto it = variable_scope.rbegin(); it != variable_scope.rend(); it++) {
            if (it->find(id) != it->end()) {
                var_found = true;
                type = it->find(id)->second;
                break;
            }
        }

        if (!var_found)
            cerr << "ERROR:: " << id << " not found" << endl;
        else
            var->type = type;

        if(ID)
            ID->accept(*this);
        if(STRING){
            STRING->accept(*this);
        }
    }

    virtual void visit(ASTFunc &node)
    {
        cout << "ASTFunc" << endl;
    }

    virtual void visit(ASTFuncParam &node)
    {
        cout << "ASTFucnParam" << endl;
        functionparam_scope.insert({node.id, &node});
        unordered_map<string, Type> func_scope;
        variable_scope.push_back(func_scope);

        for (auto vardecl : node.variabledeclList)
            vardecl->accept(*this);
        node.vardecl->accept(*this);

        if(node.type == "int")
            node.rettype = Type::INT;
        if(node.type == "bool")
            node.rettype = Type::BOOL;
        if(node.type == "char")
            node.rettype = Type::CHAR;
        if(node.type == "void")
            node.rettype = Type::VOID;

        functiontypes = node.rettype;
        node.block->accept(*this);
        if(functiontypes != Type::DEFAULT_TYPE)
            cerr << "Error:: Missing Return Statement" << endl;
    }

    virtual void visit(ASTFuncNoparam &node)
    {
        cout << "ASTFucnNoParam" << endl;
        functionnoparam_scope.insert({node.id, &node});

        if(node.type == "int")
            node.rettype = Type::INT;
        if(node.type == "bool")
            node.rettype = Type::BOOL;
        if(node.type == "char")
            node.rettype = Type::CHAR;
        if(node.type == "void")
            node.rettype = Type::VOID;

        functiontypes = node.rettype;
        node.block->accept(*this);
        if(functiontypes != Type::DEFAULT_TYPE)
            cerr << "Error:: Missing Return Statement" << endl;
    }

    virtual void visit(ASTFunccall &node)
    {
        cout << "ASTFunccall" << endl;
    }

    virtual void visit(ASTFunccallParam &node)
    {
        cout << "ASTFunccallParam" << endl;
        ASTFuncParam *func_node = nullptr;

        if(functionparam_scope.find(node.id) == functionparam_scope.end()){
            cerr << "Error:: Function " << node.id << " not defined" << endl;
        }
        else{
            func_node = functionparam_scope.find(node.id)->second;
            if(func_node->variabledeclList.size() != node.variableList.size()){
                cerr << "Error:: Function " << node.id << " has " << func_node->variabledeclList.size() << " arguments but "<< node.variableList.size() << " arguments are provided" << endl;
            }
            else{
                node.type = func_node->rettype;
                vector<ASTVardecl*>::iterator var2 = func_node->variabledeclList.begin();

                for (auto var1 :  node.variableList){
                    var1->accept(*this);
                    if(var1->type != (*var2)->type1){
                        cerr << "Error:: Invalid argument type" << endl;
                    }
                    var2++;
                }

                node.var->accept(*this);
                if(node.var->type != func_node->vardecl->type1){
                    cerr << "Error:: Invalid argument type" << endl;
                }

                
            }
        }
    }

    virtual void visit(ASTFunccallNoparam &node)
    {
        cout << "ASTFunccallNoparam" << endl;
        if(functionnoparam_scope.find(node.id) == functionnoparam_scope.end()){
            if(functionparam_scope.find(node.id) != functionparam_scope.end()){
                cerr << "Error:: Function " << node.id << " have " << functionparam_scope.find(node.id)->second->variabledeclList.size()+1 << " arguments" << endl;
            }
            else{
                cerr << "Error:: Function " << node.id << " not defined" << endl;
            }
        }
        else{
            node.type = (functionnoparam_scope.find(node.id)->second)->rettype;
        }
    }

    virtual void visit(ASTVardecl &node)
    {
        cout << "ASTVardecl" << endl;
    }

    virtual void visit(ASTVarSingle &node)
    {
        cout << "ASTVarSingle" << endl;
        auto currscope = variable_scope.rbegin();
        //check with current scope
        string type = node.getTYPE();
        string id = node.getID();

        if(currscope->find(id) != currscope->end())
            cerr << "Error:: Variable already defined" << endl;   
        
        if(type == "int"){
            currscope->insert({id, Type::INT});
            node.type1 = Type::INT;
        }

        if(type == "bool"){
            currscope->insert({id, Type::BOOL});
            node.type1 = Type::BOOL;
        }

        if(type == "char"){
            currscope->insert({id, Type::CHAR});
            node.type1 = Type::CHAR;
        }
    }

    virtual void visit(ASTVar1darray &node)
    {
        cout << "ASTVar1darray" << endl;
        auto currscope = variable_scope.rbegin();
        //check with current scope
        string type = node.getTYPE();
        string id = node.getID();

        if(currscope->find(id) != currscope->end())
            cerr << "Error:: Variable already defined" << endl;

        if(type == "int"){
            currscope->insert({id, Type::INT});
            node.type1 = Type::INT;
        }

        if(type == "bool"){
            currscope->insert({id, Type::BOOL});
            node.type1 = Type::BOOL;
        }

        if(type == "char"){
            currscope->insert({id, Type::CHAR});
            node.type1 = Type::CHAR;
        }

        ASTExpr *expr = node.getexpr();

        if(expr!=NULL)
        {
            expr->accept(*this);
            if(expr->type != Type::INT){
                cerr << "ERROR:: Expression in the 1D array is not of INT type" << endl;
            }
        }

    }

    virtual void visit(ASTVar2darray &node)
    {
        cout << "ASTVar2darray" << endl;
        auto currscope = variable_scope.rbegin();
        string type = node.getTYPE();
        string id = node.getID();

        if(currscope->find(id) != currscope->end())
            cerr << "Error:: Variable already defined" << endl;

        if(type == "int"){
            currscope->insert({id, Type::INT});
            node.type1 = Type::INT;
        }

        if(type == "bool"){
            currscope->insert({id, Type::BOOL});
            node.type1 = Type::BOOL;
        }

        if(type == "char"){
            currscope->insert({id, Type::CHAR});
            node.type1 = Type::CHAR;
        }
        
        
        ASTExpr *rexpr = node.getexpr1();
        ASTExpr *cexpr = node.getexpr2();

        if(rexpr!=NULL)
        {
            rexpr->accept(*this);
            if(rexpr->type != Type::INT){
                cerr << "ERROR:: Expression in the 2D array is not of INT type" << endl;
            }
        }
        if(cexpr!=NULL)
        {
            cexpr->accept(*this);
            if(cexpr->type != Type::INT){
                cerr << "ERROR:: Expression in the 2D array is not of INT type" << endl;
            }
        }
    }

    virtual void visit(ASTExpr &node)
    {
        cout << "ASTExpr" << endl;
    }

    virtual void visit(ASTExprBinary &node)
    {
        cout << "ASTExprBinary" << endl;
        ASTExpr *left = node.getLeft();
        ASTExpr *right = node.getRight();

        left->accept(*this);
        right->accept(*this);

        if(left->type != right->type){
            cerr << "Error:: Type Mismatch in the binary operation " << node.getBin_operator() << endl;
        }
        else{
            list<string> bool_op = {"<", ">", ">=", "<=", "==" , "!=", "AND", "OR", "NOT"};
            if(find(bool_op.begin(), bool_op.end(), node.getBin_operator()) != bool_op.end()){
                node.type = Type::BOOL;
            }
            else{
                node.type = left->type;
            }
        }
    }

    virtual void visit(ASTExprTernary &node)
    {
        cout << "ASTExprTernary" << endl;
        ASTExpr *first = node.getFirst();
        ASTExpr *second = node.getSecond();
        ASTExpr *third = node.getThird();

        first->accept(*this);
        if(first->type != Type::BOOL){
            cerr << "Error:: Type Mismatch in the first Ternary operation" << endl;
        }
        second->accept(*this);
        third->accept(*this);
    }

    virtual void visit(ASTVar &node)
    {
        cout << "ASTVar" << endl;
    }

    virtual void visit(ASTVarID &node)
    {
        cout << "ASTVarID" << endl;
        // Find form the scope and set the type
        string id = node.getID();
        bool var_found = false;
        Type type;

        for (auto it = variable_scope.rbegin(); it != variable_scope.rend(); it++) {
            if (it->find(id) != it->end()) {
                var_found = true;
                type = it->find(id)->second;
                break;
            }
        }

        if (!var_found)
            cerr << "ERROR:: " << id << " not found" << endl;
        else
            node.type = type;
    }

    virtual void visit(ASTVar1darr &node)
    {
        cout << "ASTVar1darr" << endl;
        string id = node.getID();
        bool var_found = false;
        Type type;

        for (auto it = variable_scope.rbegin(); it != variable_scope.rend(); it++) {
            if (it->find(id) != it->end()) {
                var_found = true;
                type = it->find(id)->second;
                break;
            }
        }

        if (!var_found)
            cerr << "ERROR:: " << id << " not found" << endl;
        else
            node.type = type;
    }

    virtual void visit(ASTVar2darr &node)
    {
        cout << "ASTVar2darr" << endl;
        string id = node.getID();
        bool var_found = false;
        Type type;

        for (auto it = variable_scope.rbegin(); it != variable_scope.rend(); it++) {
            if (it->find(id) != it->end()) {
                var_found = true;
                type = it->find(id)->second;
                break;
            }
        }

        if (!var_found)
            cerr << "ERROR:: " << id << " not found" << endl;
        else
            node.type = type;
    }

    virtual void visit(ASTVarINT &node)
    {
        cout << "ASTVarINT" << endl;
        node.type = Type::INT;
    }

    virtual void visit(ASTVarBOOL &node)
    {
        cout << "ASTVarBOOL" << endl;
        node.type = Type::BOOL;
    }

    virtual void visit(ASTVarCHAR &node)
    {
        cout << "ASTVarCHAR" << endl;
        node.type = Type::CHAR;
    }

    virtual void visit(ASTVarSTRING &node)
    {
        cout << "ASTVarSTRING" << endl;
        node.type = Type::STRING;
    }
};