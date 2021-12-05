#include "SymbolTable.h"
using namespace std;

void mainBody(HashTable *HT, fstream& my_file, int scope){
    HT->scope = scope;
    regex regex_instruction("(INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT|CALL)(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|[a-z][a-zA-Z_0-9]*(\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\))))?(\\s([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|[a-z][a-zA-Z_0-9]*(\\(([0-9]+|\'[a-z0-9\\s]+\'|[a-z][a-zA-Z_0-9]*|\\,)+\\))))?");

    regex regex_command("(INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT|CALL)");
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
            // cout<<key<<" ";
            int i = HT->Insert(temp);
            if (i == -1) throw Redeclared(temp->v);
            if (i > HT->size) throw Overflow(instruction);
            temp->para = std::stoi(a[1]);
            if (temp->para != 0 && HT->scope>0) throw InvalidDeclaration(temp->v);
            cout<<i<<endl;
            
        }
        else if (match_command[0] == "ASSIGN"){
            int key = HT->encode(a[0]);
            int rebound1 = 0;
            int rebound2 = 0;
            int rebound_para1 = 0;
            int rebound_para2 = 0;
            HashTableEntry *temp = new HashTableEntry(key, a[0]);
            int h = HT->SearchKey(temp);
            if (h == -1) throw Undeclared(temp->v);
            rebound1 = temp->rebound-1;
            int i = HT->t[h]->para;
            
            if (i>0){
                for (int j=1;j<=i;j++){
                    if (regex_match(a[j].begin(),a[j].end(),regex_checkid)){
                        int key3 = HT->encode(a[j]);
                        HashTableEntry *temp3 = new HashTableEntry(key3, a[j]);
                        int h3 = HT->SearchKey(temp3);
                        if (h3 == -1) throw Undeclared(temp3->v);
                        rebound_para1 += temp3->rebound-1;
                    }
                }
            }


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
                int key2 = HT->encode(a[i+1]);
                HashTableEntry *temp2 = new HashTableEntry(key2, a[i+1]);
                int h2 = HT->SearchKey(temp2);
                if (h2 == -1) throw Undeclared(temp2->v);
                rebound2 = temp2->rebound-1;
                if (HT->t[h]->type == "none" && HT->t[h2]->type != "none") HT->t[h]->type = HT->t[h2]->type; 
                else if (HT->t[h]->type != "none" && HT->t[h2]->type == "none") HT->t[h2]->type = HT->t[h]->type;
                else if (HT->t[h]->type == "none" && HT->t[h2]->type == "none") throw TypeCannotBeInferred(instruction);
                else if (HT->t[h]->type != HT->t[h2]->type) throw TypeMismatch(instruction);
                if (temp2->para>0){
                for (int j=1;j<=temp2->para;j++){
                    if (regex_match(a[j].begin(),a[j].end(),regex_checkid)){
                        int key4 = HT->encode(a[j]);
                        HashTableEntry *temp4 = new HashTableEntry(key4, a[j]);
                        int h4 = HT->SearchKey(temp4);
                        if (h4 == -1) throw Undeclared(temp4->v);
                        rebound_para2 += temp4->rebound-1;
                    }
                }
            }
            }
            cout<<rebound1 + rebound2 + rebound_para1 + rebound_para2<<endl;
        }
        else if (match_command[0] == "CALL"){
            int key = HT->encode(a[0]);
            HashTableEntry *temp = new HashTableEntry(key, a[0]);
            int h = HT->SearchKey(temp);
            if (h == -1) throw Undeclared(temp->v);
            if (temp->type == "none") throw TypeCannotBeInferred(instruction);
            if (temp->type != "void") throw TypeMismatch(instruction);
            cout<<temp->rebound-1<<endl;
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
            (HT->scope)--;
            break;
        }
        else if (match_command[0] == "LOOKUP"){
            int result = 0;
            for (int j = HT->scope; j >= 0; j--){
                string id = a[0];
                int a = 0;
                int b = 0;
                for (int i = 0; id[i]; i++){
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
    if (HT->scope > 0) throw UnclosedBlock(HT->scope);
    if (HT->scope < 0) throw UnknownBlock();
}
void SymbolTable::run(string filename)
{
    //extract file
    fstream my_file;
    my_file.open(filename, ios::in);
    if (!my_file) cout<<"No such file.";
    //
    //
    //check first line (hash) and extract para 1 (hash table's size)
    //
    //
    regex regex_hash("(LINEAR|QUADRATIC|DOUBLE|INSERT|ASSIGN|BEGIN|LOOKUP|END|PRINT)(\\s[0-9]{1,6})(\\s[0-9]{1,6})(\\s[0-9]{1,6})?");
    regex regex_type("(LINEAR|QUADRATIC|DOUBLE)");
    string hash;
    getline(my_file, hash);
        if (!regex_match(hash, regex_hash)){
            throw InvalidInstruction(hash);
    }
    smatch match_hash;
    regex_search(hash, match_hash, regex_type);

    // cout<<A;
    regex regex_hash_para("([0-9]{1,6})");
    int a[3] = {0,0,0};
    int i = 0;
    for(std::sregex_iterator iter = std::sregex_iterator(hash.begin(), hash.end(), regex_hash_para); iter != std::sregex_iterator(); ++iter )
        {
            std::smatch m = *iter;
            a[i] = std::stoi(m.str());
            i++;
        }
    HashTable* HT = new HashTable(a[0],a[1],a[2]);;
    if (match_hash[0] == "LINEAR")
        HT->probeType = "LINEAR";
    else if (match_hash[0] == "QUADRATIC")
        HT->probeType = "QUADRATIC";
    else if (match_hash[0] == "DOUBLE")
        HT->probeType = "DOUBLE";
    //
    //
    //check instructions and main body
    //
    //
    mainBody(HT, my_file, 0);
    

}
