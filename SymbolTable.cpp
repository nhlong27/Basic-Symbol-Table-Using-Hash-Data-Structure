#include "SymbolTable.h"
using namespace std;

void SymbolTable::run(string filename)
{
    //extract file
    fstream my_file;
    my_file.open(filename, ios::in);
    if (!my_file) cout<<"No such file.";

    //check first line (settings) and extract para 1 (hash table's size)
    regex regex_settings("(LINEAR|QUADRATIC|DOUBLE|INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)(\\s[0-9]{1,6})(\\s[0-9]{1,6})(\\s[0-9]{1,6})?");
    string settings;
    getline(my_file, settings);
        if (!regex_match(settings, regex_settings)){
            throw InvalidInstruction(settings);
    }

    regex regex_settings_para("([0-9]{1,6})");
    int array[3] = {0,0,0};
    int i = 0;
    for(std::sregex_iterator iter = std::sregex_iterator(settings.begin(), settings.end(), regex_settings_para); iter != std::sregex_iterator(); ++iter )
        {
            std::smatch m = *iter;
            array[i] = std::stoi(m.str());
            i++;
        }
    HashTable* HT = new HashTable(array[0],array[1],array[2]);

    //check instructions and main body
    regex regex_instruction("(INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|(\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\))))?(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|(\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\))))?");

    regex regex_command("(INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)");
    regex regex_extra("([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|(\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\)))");
    // char regex_extra2[] = "[a-z][a-zA-Z_0-9]*\'[0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,+\'";
    // [a-z][a-zA-Z_0-9]*\'[0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,\'
    string instruction;
    while(getline(my_file, instruction)){
        if (!regex_match(instruction, regex_instruction)){

            throw InvalidInstruction(instruction);
        }
        smatch match_command;
        regex_search(instruction, match_command, regex_command);

        string a[10] = {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0"};
        int i = 0;

        for(std::sregex_iterator iter = std::sregex_iterator(instruction.begin(), instruction.end(), regex_extra); iter != std::sregex_iterator(); ++iter )
        {
            std::smatch m = *iter;
            a[i] = m.str();
            i++;
        }

        if (match_command[0] == "INSERT"){
            int key = HT->HashFunc(HT->encode(a[0]));
            HT->Insert(key, a[0]);
        }
        else if (match_command[0] == "PRINT"){
            HT->print();
        }
        cout<<a[0]<<" "<<a[1]<<" "<<a[2]<<" "<<a[3]<<" "<<a[4]<<endl;
    }
    
    cout<< "success";

}