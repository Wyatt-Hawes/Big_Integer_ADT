#include<iostream>
#include<string>
#include<stdexcept>
#include "List.h"
#include "BigInteger.h"

int main(){
    
    std::string num = "13";

    std::string num2 = "17";
    BigInteger a = BigInteger(num);
    BigInteger b = BigInteger(num2);

    BigInteger c = a.mult(b);
    
    std::cout <<"c:";
    std::cout << c;

    

    //printf("a=b  %d\n", a.compare(b));

    
    return 0;
}
