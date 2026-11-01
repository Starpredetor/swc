#include <iostream>
#include <clang-c/Index.h>
#include "cx_string.hpp"


void printDiagnostics(CXTranslationUnit unit) {
    unsigned count = clang_getNumDiagnostics(unit);

    for (unsigned i = 0; i < count; ++i) {
        CXDiagnostic diagnostic = clang_getDiagnostic(unit, i);

        CXString message = clang_formatDiagnostic(
            diagnostic,
            clang_defaultDiagnosticDisplayOptions()
        );

        const char* text = clang_getCString(message);

        if (text != nullptr) {
            std::cerr << text << '\n';
        }

        clang_disposeString(message);
        clang_disposeDiagnostic(diagnostic);
    }
}

int main (int argc, char** argv){
    
    if(argc < 2) {
        std::cerr << "usage: swc filepath" << "\n";
        return 1;
    };

    swc::CXStringWrapper cxs(clang_getClangVersion());

    std::cout << cxs.str() << "\n";

    CXIndex cx_index = clang_createIndex(0, 0);

    CXTranslationUnit unit = clang_parseTranslationUnit(
        cx_index,
        argv[1], nullptr, 0,
        nullptr, 0,
        CXTranslationUnit_None
    );

    if (unit == nullptr){
        std::cerr << "Unable to parse translation unit. Quitting.\n";
        return 0;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    
    clang_defaultDiagnosticDisplayOptions();
    unsigned count = clang_getNumDiagnostics(unit);
    std::cerr << "Total diagnostics: " << count << '\n';

    printDiagnostics(unit);
    clang_visitChildren(
        cursor, 
        [](CXCursor current_cursor, CXCursor parent, CXClientData client_data){
        CXString current_display_name = clang_getCursorDisplayName(current_cursor);
        CXType cursor_type = clang_getCursorType(current_cursor);
        CXString type_kind_spelling = clang_getTypeKindSpelling(cursor_type.kind);
        std::cout << "Type kind:" << clang_getCString(type_kind_spelling);
        clang_disposeString(type_kind_spelling);
        clang_disposeString(current_display_name);
        
        if (cursor_type.kind == CXType_Pointer ||
            cursor_type.kind == CXType_LValueReference ||
            cursor_type.kind == CXType_RValueReference){
                CXType pointed_to_type = clang_getPointeeType(cursor_type);

                CXString pointed_to_type_spelling = clang_getTypeSpelling(pointed_to_type);
                std::cout << "pointing to type: " << clang_getCString(pointed_to_type_spelling);
                clang_disposeString(pointed_to_type_spelling);

            }
        else if (cursor_type.kind == CXType_Record){
            CXString type_spelling = clang_getTypeSpelling(cursor_type);
            std::cout << ", namely: " << clang_getCString(type_spelling);
        }
        std::cout << "\n";

        return CXChildVisit_Recurse;
            }, 
        nullptr 
    );
    clang_visitChildren(
        cursor,
        [](CXCursor current_cursor, CXCursor parent, CXClientData client_data){

        CXType cursor_type = clang_getCursorType(current_cursor);
        CXString cursor_spelling = clang_getCursorSpelling(current_cursor);
        CXSourceRange cursor_range = clang_getCursorExtent(current_cursor);
        std::cout << "Cursor " << clang_getCString(cursor_spelling);

        CXFile file;
        unsigned start_line, start_column, start_offset;
        unsigned end_line, end_column, end_offset;

        clang_getExpansionLocation(clang_getRangeStart(cursor_range), &file, &start_line, &start_column, &start_offset);
        clang_getExpansionLocation(clang_getRangeEnd  (cursor_range), &file, &end_line  , &end_column  , &end_offset);
        std::cout << " spanning lines " << start_line << " to " << end_line;
        clang_disposeString(cursor_spelling);

        std::cout << "\n";
        return CXChildVisit_Recurse;
        },
        nullptr
    );


    return 0;
}



