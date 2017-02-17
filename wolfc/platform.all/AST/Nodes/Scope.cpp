#include "Scope.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"
#include "Stmnt.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

Scope * Scope::parse(ParseCtx & parseCtx) {
    // Skip any newlines and commas (statement separators) that follow:
    parseCtx.skipNewlinesAndCommas();
    
    // Save start token:
    const Token * startToken = parseCtx.tok();
    
    // Parse all statements we can:
    std::vector<Stmnt*> stmnts;
    bool requireNewlineBetweenStmnts = false;
    
    while (Stmnt::peek(parseCtx.tok())) {
        // Parse the statement, if that fails then try again:
        Stmnt * stmnt = Stmnt::parse(parseCtx);
        
        if (!stmnt) {
            continue;
        }
        
        // Each subsequent statement must be put on a new line, if we find
        // that not to be the case then issue errors:
        //
        // TODO: Allow commas to enable multiple statements per line
        if (requireNewlineBetweenStmnts) {
            Stmnt * lastStmnt = stmnts.back();
            const Token & thisStmntStart = stmnt->getStartToken();
            const Token & prevStmntEnd = lastStmnt->getEndToken();
            
            if (thisStmntStart.startLine <= prevStmntEnd.startLine) {
                parseCtx.error("Statements must be separated onto different lines, or by ','! "
                               "The statement starting at line %zu, col %zu is on the same line as "
                               "the statement ending at line %zu, col %zu!",
                               thisStmntStart.startLine + 1,
                               thisStmntStart.startCol + 1,
                               prevStmntEnd.startLine + 1,
                               prevStmntEnd.startCol + 1);
            }
        }
            
        // Save the statement and skip any newlines and commas that follow
        stmnts.push_back(stmnt);

        // Skip any newlines and commas that folllow. If any newlines or commas are found, these reset the
        // requirement for the next statement to be on a new line.
        if (parseCtx.skipNewlinesAndCommas() > 0) {
            requireNewlineBetweenStmnts = false;
        }
        else {
            requireNewlineBetweenStmnts = true;
        }
    }
    
    // Return the parsed scope
    return WC_NEW_AST_NODE(parseCtx, Scope, *startToken, std::move(stmnts));
}

Scope::Scope(const Token & startToken, std::vector<Stmnt*> && stmnts)
:
    mStartToken(startToken),
    mStmnts(stmnts)
{
    for (Stmnt * stmnt : mStmnts) {
        stmnt->mParent = this;
    }
}

void Scope::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & Scope::getStartToken() const {
    if (!mStmnts.empty()) {
        return mStmnts.front()->getStartToken();
    }
    
    return mStartToken;
}

const Token & Scope::getEndToken() const {
    if (!mStmnts.empty()) {
        return mStmnts.back()->getEndToken();
    }
    
    return mStartToken;
}

bool Scope::allCodepathsHaveUncondRet() const {
    for (Stmnt * stmnt : mStmnts) {
        if (stmnt->allCodepathsHaveUncondRet()) {
            return true;
        }
    }
    
    return false;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
