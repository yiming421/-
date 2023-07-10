#include <fstream>
#include <iostream>
#include <bitset>
#include <cstring>
using namespace std;
    void readBMP(char* input){
        ifstream in(input, std::ios::binary);
        if(!in){
            cerr << "Cannot open the file" << endl; 
            return;
        }
    }
int main(int argc, char** argv){
    readBMP(argv[1]);
    return 0;
}