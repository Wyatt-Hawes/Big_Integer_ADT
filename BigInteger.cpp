#include<iostream>
#include<string>
#include<cmath>

#include"List.h"
#include"BigInteger.h"

//static bool F = false;
int Power = 9;
//10 to the power of Power
long Base = pow(10,Power);

void delete_zero(List&N){
    N.moveFront();
    while(N.peekNext() == 0){
        N.eraseAfter();
    }
}

BigInteger::BigInteger(){

    signum = 0;
    digits = List();
}

BigInteger::BigInteger(std::string x){

    std::string s = x;

    signum = 0;
    digits = List();

    char f = s.at(0);
    if(f == '+'){
        signum = 1;
        s.erase(0,1);
    }else if(f == '-'){
        signum = -1;
        s.erase(0,1);
    }else{
        signum = 1;
    }
    //std::cout << "|" + s + "|\n";
    int rem_length = s.length();
    //long val;

    digits.moveFront();

    while(rem_length > 0){
        //printf("%d\n", rem_length);
        if(rem_length >= Power){
            //printf("from(%d-%d):",rem_length - Power, rem_length);
            //std::cout << s.substr(rem_length - Power,Power) + "\n";
            digits.insertAfter(std::stol(s.substr(rem_length - Power,Power)));
            rem_length = rem_length - Power;
            continue;
        }
        //printf("bot\n");
        digits.insertAfter(std::stol(s.substr(0,rem_length)));
        rem_length = 0;
        break;
    }

    //time to remove the 0's from the front
    delete_zero(digits);
    //std::cout << digits;

}

BigInteger::BigInteger(const BigInteger& N){
    signum = N.signum;
    digits = List(N.digits);

}

int BigInteger::sign() const{
    return signum;
}

int BigInteger::compare(const BigInteger& N) const{
    //Check signs
    if(signum != N.signum){
        if(signum > N.signum){
            return 1;
        }
        return -1;
    }
    if(signum == 0 || N.signum == 0){
        if(signum == 0 && N.signum == 0){
            return 0;
        }
        if(signum == 0){
            if(N.signum == 1){
                return -1;
            }
            return 1;
        }
        if(N.signum == 0){
            if(signum == 1){
                return 1;
            }
            return -1;
        }
    }


    //compare List lengths
    if(digits.length() != N.digits.length()){
        if(digits.length() < N.digits.length()){
            return -1;
        }
        return 1;
    }
    
    //compare front elements
    if(digits.front() != N.digits.front()){
        if(digits.front() < N.digits.front()){
            return -1;
        }
        return 1;
    }

   
    //If lengths are not the same, the longer number must be larger
    if((digits.to_string()).length() != (N.digits.to_string()).length()){
        if((digits.to_string()).length() < (N.digits.to_string()).length()){
            return -1;
        }
        return 1;
    }
    //Check if toString outputs are the same, if they are then they must
    //be identical
    if((digits.to_string()).compare(N.digits.to_string()) == 0){
        return 0;
    }

    List A = digits;
    List B = N.digits;

    A.moveFront();
    B.moveFront();

    long a,b;

    while(true){
        try{
            a = A.moveNext();
            b = B.moveNext();
        }catch(std::length_error const&){
            throw std::range_error("BigInteger: compare(): issue in while loop, lists may be equal but shouldn't reach this point");
        }


        if(a < b){
            return -1;
        }
        if(a > b){
            return 1;
        }

    }

    return 0;
}   

void BigInteger::makeZero(){
    digits.clear();
    signum = 0;
}

void BigInteger::negate(){
    if(digits.length() == 0){
        return;
    }
    signum = signum * (-1);
}

void carry_val(List& N, int x){
    try{
        N.setBefore(N.peekPrev() + x);
    }catch(std::length_error const&){
        //printf("inserting(carry)\n");
        N.insertBefore((long)x);
    }
}

void mul_n1(List& N){
    N.moveFront();
    for(int i = 0; i < N.length(); i++){
        N.setAfter(N.peekNext() * -1);
        N.moveNext();
    }
}


int normalize(List& N, int s){
    delete_zero(N);
    //printf("N");
    if(N.length() == 0){
        //throw std::length_error("BigInteger: normalize: Normalizing empty bigInt\n");
        return 0;
    }
    
    N.moveBack();
    long carry;
    for(int i = 0; i < N.length(); i++){
        N.movePrev();
        carry = N.peekNext() / Base;
        if(N.peekNext() < 0){
            carry--;
        }
        if(carry != 0){
            if(carry == -1 && N.position() == 0){
                s = s * -1;
                mul_n1(N);
                N.moveBack();
                i = 0;
            }else {
                carry_val(N,carry);
                N.setAfter(N.peekNext() - (carry * Base));
            }
        }
    }
    delete_zero(N);
    return s;
}

BigInteger BigInteger::add(const BigInteger& N) const{
    BigInteger A = BigInteger(*this);
    BigInteger B = N;
    if(A.digits.length() == 0){
        return B;
    }
    if(B.digits.length() == 0){
        return A;
    }

    BigInteger ret;
    A.digits.moveFront();
    B.digits.moveFront();

    while(A.digits.length() != B.digits.length()){
        if(A.digits.length() < B.digits.length()){
            A.digits.insertAfter(0);
        }
        if(B.digits.length() < A.digits.length()){
            B.digits.insertAfter(0);
        }
    }
    A.digits.moveBack();
    B.digits.moveBack();
    ret.digits.moveFront();

    for(int i = 0; i < A.digits.length(); i++){
        ret.digits.insertAfter((A.signum * A.digits.peekPrev()) + (B.signum * B.digits.peekPrev()));

        
        A.digits.movePrev();
        B.digits.movePrev();
    }
    ret.signum = 1;
        ret.signum = normalize(ret.digits,ret.signum);
    //}
    return ret;
}
BigInteger BigInteger::sub(const BigInteger& N) const{
    BigInteger A= BigInteger(*this);
    BigInteger B = N;
    if(A.digits.length() == 0){
        return B;
    }
    if(B.digits.length() == 0){
        return A;
    }
    if(A == B){
        BigInteger s;
        return s;
    }

    BigInteger ret;
    A.digits.moveFront();
    B.digits.moveFront();

    while(A.digits.length() != B.digits.length()){
        if(A.digits.length() < B.digits.length()){
            A.digits.insertAfter(0);
        }
        if(B.digits.length() < A.digits.length()){
            B.digits.insertAfter(0);
        }
    }

    A.digits.moveBack();
    B.digits.moveBack();
    ret.digits.moveFront();

    for(int i = 0; i < A.digits.length(); i++){
        ret.digits.insertAfter((A.signum * A.digits.peekPrev()) - (B.signum * B.digits.peekPrev()));

        
        A.digits.movePrev();
        B.digits.movePrev();
    }
    ret.signum = 1;
    ret.signum = normalize(ret.digits,ret.signum);
    return ret;
}

void append_zeroes(List &A, int zeros){
    A.moveBack();
    for(int i = 0; i < zeros; i++){
        A.insertAfter(0);
    }
}

//Helper to try and make mult much faster
List add_no_normalize(List &A, List &B, int sigA, int sigB){
    if(A.length() == 0){
        return B;
    }
    if(B.length() == 0){
        return A;
    }

    List ret;
    A.moveFront();
    B.moveFront();

    while(A.length() != B.length()){
        if(A.length() < B.length()){
            A.insertAfter(0);
        }
        if(B.length() < A.length()){
            B.insertAfter(0);
        }
    }

    A.moveBack();
    B.moveBack();
    ret.moveFront();

    for(int i = 0; i < A.length(); i++){
        ret.insertAfter((sigA * A.peekPrev()) + (sigB * B.peekPrev()));

        
        A.movePrev();
        B.movePrev();
    }
    normalize(ret,1);
    return ret;
}



BigInteger BigInteger::mult(const BigInteger& N) const{

    if(signum == 0 || N.signum == 0){
        BigInteger z;
        return z;
    }
    BigInteger A;
    BigInteger B;
    if(digits.length() < N.digits.length()){
        A = N;
        B = (*this);

    }else{
        A= (*this);
        B = N;
    }

    //A * B

    BigInteger running_sum;
    running_sum.signum = 1;
    BigInteger prod;
    prod.signum = 1;

    B.digits.moveBack();

    long a,b;
    for(int i = 0; i < B.digits.length(); i++){
        A.digits.moveBack();
        b = B.digits.movePrev();
        prod.digits.clear();

        for(int j = 0; j < A.digits.length(); j++){
            a = A.digits.movePrev();
            prod.digits.insertAfter((A.signum * a) * (B.signum * b));
        }

        append_zeroes(prod.digits, i);

        running_sum.digits = add_no_normalize(running_sum.digits, prod.digits, running_sum.signum, prod.signum);
        running_sum.signum = normalize(running_sum.digits,running_sum.signum);
    }
    running_sum.signum = normalize(running_sum.digits, running_sum.signum);
    
    return running_sum;
}


std::string BigInteger::to_string(){
    std::string s;

    if(signum == 0){
        s = "0\n";
        return s;
    }
    if(signum == -1){
        s = "-";
    }

    if(digits.length() == 0){
        s = "0\n";
        return s;
    }

    digits.moveFront();
    for(int i = 0; i < digits.length(); i++){
        std::string t = std::to_string(digits.moveNext());
        while(i != 0 && (long)t.length() < (long)Power){
            t.insert(0,"0");
        }
        s.append(t);
    }
    s.append("\n");
    return s;
}

std::ostream& operator<<(std::ostream& stream, BigInteger N){
    stream << N.to_string();
    return stream; 
}

bool operator==( const BigInteger& A, const BigInteger& B ){
    if(A.compare(B) == 0){
        return true;
    }
    return false;
}

bool operator<( const BigInteger& A, const BigInteger& B ){
    if(A.compare(B) == -1){
        return true;
    }
    return false;
}

bool operator<=( const BigInteger& A, const BigInteger& B ){
    if(A.compare(B) == 1){
        return false;
    }
    return true;
}

bool operator>( const BigInteger& A, const BigInteger& B ){
    if(A.compare(B) == 1){
        return true;
    }
    return false;
}

bool operator>=( const BigInteger& A, const BigInteger& B ){
    if(A.compare(B) == -1){
        return false;
    }
    return true;
}

BigInteger operator+( const BigInteger& A, const BigInteger& B ){
    BigInteger x = A.add(B);
    return x;
}

BigInteger operator+=( BigInteger& A, const BigInteger& B ){
    BigInteger x = A.add(B);
    A = x;
    return A;
}

BigInteger operator-( const BigInteger& A, const BigInteger& B ){
    BigInteger x = A.sub(B);
    return x;
}

BigInteger operator-=( BigInteger& A, const BigInteger& B ){
    BigInteger x = A.sub(B);
    A = x;
    return A;
}
BigInteger operator*( const BigInteger& A, const BigInteger& B ){
    BigInteger x = A.mult(B);
    return x;
}
BigInteger operator*=( BigInteger& A, const BigInteger& B ){
    BigInteger x = A.mult(B);
    A=x;
    return x;
}