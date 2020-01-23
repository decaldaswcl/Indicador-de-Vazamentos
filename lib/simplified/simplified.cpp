#include <simplified.h>

simplified::simplified(/* args */)
{
}

simplified::~simplified()
{
}
char* simplified::strToChar(String st){
    
    char *nStr = new char[st.length()+1];
    memcpy(nStr, st.c_str(), st.length() + 1);
    return nStr;
}