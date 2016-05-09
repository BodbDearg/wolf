#include "WCLexer.hpp"
#include "WCToken.hpp"
#include <cstdlib>

WCLexer::WCLexer(size_t initialTokenCapacity) :
    mTokenList(nullptr),
    mTokenCapacity(initialTokenCapacity),
    mTokenCount(0)
{
    increaseTokenListCapacity(initialTokenCapacity);
}

void WCLexer::increaseTokenListCapacity(size_t newCapacity) {
    if (newCapacity < mTokenCapacity) {
        return;
    }
    
    mTokenCapacity = newCapacity;
    mTokenList = reinterpret_cast<WCToken*>(std::realloc(mTokenList, mTokenCapacity * sizeof(WCToken)));
}

WCToken * WCLexer::allocToken() {
    // If not enough room, grow capacity according to the Golden ratio (approx 1.6) and add 1 to ensure we alloc at least one token.
    if (mTokenCount + 1 > mTokenCapacity) {
        increaseTokenListCapacity(((mTokenCapacity * 16) / 10) + 1);
    }
    
    // Give the token back
    WCToken * token = mTokenList + mTokenCount;
    ++mTokenCount;
    return token;
}
