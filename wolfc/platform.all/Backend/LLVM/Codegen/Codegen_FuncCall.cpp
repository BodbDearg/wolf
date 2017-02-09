#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/FuncCall.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::FuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Generate the basic argument values firstly
    const auto & args = astNode.getArgs();
    std::vector<Value> argVals;
    argVals.reserve(args.size());
    
    for (const AST::AssignExpr * assignExpr : args) {
        WC_ASSERT(assignExpr);
        assignExpr->accept(*this);
        argVals.push_back(mCtx.popValue());
    }
    
    // Now push them back in exact order for the calling code, even if there are
    // invalid values we popped due to the stack being empty. This way we'll preserve
    // the relative ordering of the arguments for the calling code generation code:
    for (const Value & argVal : argVals) {
        mCtx.pushValue(argVal);
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
