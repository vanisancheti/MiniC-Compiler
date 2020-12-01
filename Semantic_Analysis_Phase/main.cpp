#include <iostream>
#include <vector>
#include <string>

#include "antlr4-runtime.h"
#include "ExprLexer.cpp"
#include "ExprParser.cpp"

#include "ExprBuildASTVisitor.h"
#include "PostfixVisitor.h"
#include "SemanticCheckVisitor.h"
#include "llvmIRVisitor.h"
// #include "ast.h"

using namespace std;
using namespace antlr4;



int main(int argc, const char* argv[]) {
    std::ifstream stream;

    cout << "Processing input file " << argv[1] << endl;
    stream.open(argv[1]);
    

    ANTLRInputStream input(stream);

    ExprLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    ExprParser parser(&tokens);    

    ExprParser::ProgContext *ctx = parser.prog();

    cout << "AST Construction------------------------------------" << endl;    
    ExprVisitor *visitor = new ExprBuildASTVisitor();
    ASTProg *program_root = visitor->visitProg(ctx);
    cout << "------------------------------------AST Construction Completed" << endl;

    cout << "PostFix ------------------------------------" << endl;    
    PostFixVisitor *pv = new PostFixVisitor();
    pv->visit(*program_root);
    cout << "------------------------------------PostFix Completed" << endl;
    
    cout << "Semantic Checking ------------------------------------" << endl;
    SemanticCheckVisitor *sc = new SemanticCheckVisitor();
    sc->visit(*program_root);
    cout << "------------------------------------Semantic Completed" << endl;

    cout << "LLVMIRGeneration -------------------------------------" << endl;
    llvmIRVisitor * llvmvisitor = new llvmIRVisitor();
    llvmvisitor->visit(*program_root); 
    cout << "------------------------------------LLVMIRGeneration Completed" << endl;

    return 0;
}
