#include<iostream>
#include<fstream>
#include<string>
#include<stdexcept>
#include "List.h"
#include "BigInteger.h"

int main(int argc, char* argv[]){

    if (argc != 3){
        fprintf(stderr, "Please enter 2 arguments.\n Ex: Lex <input file> <output file>\n");
        return 1;
    }
    std::ifstream input;
    input.open(argv[1]);

    std::ofstream output;
    output.open(argv[2]);

    std::string a,b;
    input >> a;
    input.ignore(0, '\n');
    input >> b;

    BigInteger A = a;
    BigInteger B = b;

    //A and B
    output << A; 
    output << "\n";
    
    output << B;
    output << "\n";

    //A + B
    output << (A + B); 
    output << "\n";

    //A - B
    output << (A - B); 
    output << "\n";

    //A - A
    output << (A - A); 
    output << "\n";


    //3A - 2B
    output << (std::to_string(3) * A)-(std::to_string(2) * B);
    output <<  "\n";

    //AB
    output << (A * B);
    output << "\n";

    //A^2
    output << (A * A);
    output << "\n";


    //B^2
    output << (B * B);
    output << "\n";


    //9A^4 + 16B^5
    BigInteger a_4 = A;
    BigInteger b_5 = B;

    a_4 *= A;
    a_4 *= A;
    a_4 *= A;

    b_5 *= B;
    b_5 *= B;
    b_5 *= B;
    b_5 *= B;
    output << ((std::to_string(9) * a_4)+(std::to_string(16) * b_5));
    //output << "\n";
    return 0;
}
