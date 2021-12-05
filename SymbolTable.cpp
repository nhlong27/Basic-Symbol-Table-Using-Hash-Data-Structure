#include "SymbolTable.h"
using namespace std;

void mainBody(HashTable *HT, fstream& my_file, int scope){
    HT->scope = scope;
    regex regex_instruction("(INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|(\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\))))?(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|(\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\))))?");

    regex regex_command("(INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)");
    regex regex_extra("([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|(\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\)))");
    // char regex_extra2[] = "[a-z][a-zA-Z_0-9]*\'[0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,+\'";
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
        // cout<<B;
        if (match_command[0] == "INSERT"){
            int key = HT->encode(a[0]);
            HashTableEntry *temp = new HashTableEntry(key, a[0]);
            // cout<<C;
            HT->Insert(temp);
        }
        else if (match_command[0] == "BEGIN"){
            HashTable *newHT = new HashTable(HT->size,HT->c1,HT->c2);
            newHT->probeType = HT->probeType;
            for (int i = 0; i<HT->size; i++){
                newHT->t[i] = HT->t[i];
            }
            mainBody(newHT, my_file, scope+1);
        }
        else if (match_command[0] == "END"){
            // for (int i = 0; i < HT->size; i++) {
            //     if (HT->t[i] != nullptr)
            //         delete HT->t[i];
            //         delete[] HT->t;
            //     }
            break;
        }
        else if (match_command[0] == "LOOKUP"){
            int key = HT->encode(a[0]);
            HashTableEntry *temp = new HashTableEntry(key, a[0]);
            HT->SearchKey(temp);
        }
        else if (match_command[0] == "PRINT"){
            HT->print();
        }
    }
    
    cout<< "success";
}
void SymbolTable::run(string filename)
{
    //extract file
    fstream my_file;
    my_file.open(filename, ios::in);
    if (!my_file) cout<<"No such file.";
    //
    //
    //check first line (settings) and extract para 1 (hash table's size)
    //
    //
    regex regex_settings("(LINEAR|QUADRATIC|DOUBLE|INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)(\\s[0-9]{1,6})(\\s[0-9]{1,6})(\\s[0-9]{1,6})?");
    regex regex_type("(LINEAR|QUADRATIC|DOUBLE)");
    string settings;
    getline(my_file, settings);
        if (!regex_match(settings, regex_settings)){
            throw InvalidInstruction(settings);
    }
    smatch match_settings;
    regex_search(settings, match_settings, regex_type);

    // cout<<A;
    regex regex_settings_para("([0-9]{1,6})");
    int array[3] = {0,0,0};
    int i = 0;
    for(std::sregex_iterator iter = std::sregex_iterator(settings.begin(), settings.end(), regex_settings_para); iter != std::sregex_iterator(); ++iter )
        {
            std::smatch m = *iter;
            array[i] = std::stoi(m.str());
            i++;
        }
    HashTable* HT = new HashTable(array[0],array[1],array[2]);;
    if (match_settings[0] == "LINEAR")
        HT->probeType = "LINEAR";
    else if (match_settings[0] == "QUADRATIC")
        HT->probeType = "QUADRATIC";
    else if (match_settings[0] == "DOUBLE")
        HT->probeType = "DOUBLE";
    //
    //
    //check instructions and main body
    //
    //
    mainBody(HT, my_file, 0);
    

}
