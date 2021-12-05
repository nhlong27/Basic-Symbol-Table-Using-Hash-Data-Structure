#include "SymbolTable.h"
using namespace std;

void mainBody(HashTable *HT, fstream& my_file, int scope){
    HT->scope = scope;
    regex regex_instruction("(INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|[a-z][a-zA-Z_0-9]*(\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\))))?(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|[a-z][a-zA-Z_0-9]*(\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\))))?");

    regex regex_command("(INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)");
    regex regex_extra("([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|[a-z][a-zA-Z_0-9]*(\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\)))");
    regex regex_checknum("([0-9]+)");
    regex regex_checkstring("(\'[a-z0-9\\s]+\')");
    regex regex_checkid("([a-z][a-zA-Z_0-9]*)");
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
            temp->para = std::stoi(a[1]);
        }
        else if (match_command[0] == "ASSIGN"){
            int key = HT->encode(a[0]);
            HashTableEntry *temp = new HashTableEntry(key, a[0]);
            int h = HT->SearchKey(temp);
            int i = HT->t[h]->para;
            if (regex_match(a[i+1].begin(),a[i+1].end(),regex_checknum)){
                if (HT->t[h]->type == "none" || HT->t[h]->type == "number")
                    HT->t[h]->type = "number";
                else throw TypeMismatch(instruction);
            }
            if (regex_match(a[i+1].begin(),a[i+1].end(),regex_checkstring)){
                if (HT->t[h]->type == "none" || HT->t[h]->type == "string")
                    HT->t[h]->type = "string";
                else throw TypeMismatch(instruction);
            }
            if (regex_match(a[i+1].begin(),a[i+1].end(),regex_checkid)){
                int key = HT->encode(a[i+1]);
                HashTableEntry *temp = new HashTableEntry(key, a[i+1]);
                int h2 = HT->SearchKey(temp);
                if (HT->t[h]->type == "none" && HT->t[h2]->type != "none") HT->t[h]->type = HT->t[h2]->type; 
                else if (HT->t[h]->type != "none" && HT->t[h2]->type == "none") HT->t[h2]->type = HT->t[h]->type;
                else if (HT->t[h]->type == "none" && HT->t[h2]->type == "none") throw TypeCannotBeInferred(instruction);
                else if (HT->t[h]->type != HT->t[h2]->type) throw TypeMismatch(instruction);
            }
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
            int result = 0;
            for (int j = HT->scope; j >= 0; j--){
                string id = a[0];
                unsigned long a;
                unsigned long b = 0;
                for (int i = 0; id[i]; i++){
                    a = b;
                    b = id[i] - 48;
                    int times = 1;
                    while (times <= b){
                        times *= 10;
                    }
                    a = a*times + b;
                }
                int times2 = 1;
                while (times2 <= a){
                    times2 *= 10;
                }
                a = j*times2 + a;

                HashTableEntry *temp = new HashTableEntry(a, "a");
                result = HT->SearchKey(temp);
                if (result != -1) break;
            }
            cout<<result<<endl;
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
