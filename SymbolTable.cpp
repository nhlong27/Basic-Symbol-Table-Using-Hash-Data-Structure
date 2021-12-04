#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
    //extract file
    fstream my_file;
    my_file.open(filename, ios::in);
    if (!my_file) cout<<"No such file.";

    //check first line (settings) and extract para 1 (hash table's size)
    regex regex_settings("(LINEAR|QUADRATIC|DOUBLE|INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)(\\s[0-9]{1,6})(\\s[0-9]{1,6})");
    string settings;
    getline(my_file, settings);
        if (!regex_match(settings, regex_settings)){
            throw InvalidInstruction(settings);
    }

    regex regex_settings_para("([0-9]{1,6})");
    smatch match_settings_para;
    regex_search(settings, match_settings_para, regex_settings_para);
    // //<sstream> disallowed
    // int y;
    // stringstream s(match_settings_para[0]);
    // s >> y;
    // unsigned int i = static_cast<unsigned int>(match_settings_para[0]);
    int hashtable_size = std::stoi(match_settings_para[0].str());
    HashTable* HT = new HashTable(hashtable_size);

    //check instructions and main body
    regex regex_instruction("(INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|[a-z][a-zA-Z_0-9]*\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\)))?(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|[a-z][a-zA-Z_0-9]*\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\)))?");

    regex regex_command("(INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)");
    regex regex_extra("([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|[a-z][a-zA-Z_0-9]*\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\))");

    string instruction;
    while(getline(my_file, instruction)){
        if (!regex_match(instruction, regex_instruction)){

            throw InvalidInstruction(instruction);
        }

        smatch match_command, match_extra;
        regex_search(instruction, match_command, regex_command);
        regex_search(instruction, match_extra, regex_extra);

        if (match_command[0] == "INSERT"){
            cout<<match_extra[0].str();
            cout<<HT->HashFunc(match_extra[0].str());
        }
    }
    
    cout<< "success";

}