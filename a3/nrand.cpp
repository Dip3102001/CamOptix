#include<iostream>
#include<fstream>

using namespace std;

int main(){
    ifstream urand("/dev/urandom",ios::in|ios::binary);

    if(!urand){
        cerr<<"Error : Unable to open /dev/urandom."<<endl;
        return 1;
    }else{
        int random_number;
        urand.read((char*)(&random_number),sizeof(random_number));

        urand.close();
        cout<<random_number<<endl;
    }

    return 0;
}