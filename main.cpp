#include "declare.hpp"

int main(){
    ifstream fp("scandi.csv");
    string line;
    stock mystock;
    mystock.datasize = 17;
    mystock.outputsize = 14;
    double i = 0;
    while(getline(fp,line)){
        mystock.updatemap(line);
    }
    mystock.printoutput();
    return 0;
}
