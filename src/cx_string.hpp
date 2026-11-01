#pragma once
#include <clang-c/Index.h>
#include <string>

namespace swc {

    class CXStringWrapper {

        CXString ver;

        public:

            explicit CXStringWrapper(CXString s) : ver(s) {}
        
            ~CXStringWrapper () {
                clang_disposeString(ver);
            }
        
            CXStringWrapper(const CXStringWrapper&) = delete;
            CXStringWrapper& operator = (const CXStringWrapper&) = delete;
        
            std::string str() const {
                const char * c_str = clang_getCString(ver);
                if (c_str == NULL) {
                    return "";
                }
                return c_str;
            }

     
    };
}