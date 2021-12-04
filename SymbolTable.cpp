#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
    fstream my_file;
    my_file.open(filename, ios::in);
    if (!my_file) cout<<"No such file.";

    regex regex_settings("(LINEAR|QUADRATIC|DOUBLE|INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)(\\s[0-9]{1,6})(\\s[0-9]{1,6})");
    string settings;
    getline(my_file, settings);
        if (!regex_match(settings, regex_settings)){
            throw InvalidInstruction(settings);
    }
    regex regex_instruction("(INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|[a-z][a-zA-Z_0-9]*\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\)))?(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|[a-z][a-zA-Z_0-9]*\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\)))?");
    string instruction;
    while(getline(my_file, instruction)){
        if (!regex_match(instruction, regex_instruction)){
            throw InvalidInstruction(instruction);
        }
        

    }
    cout<< "success";

}