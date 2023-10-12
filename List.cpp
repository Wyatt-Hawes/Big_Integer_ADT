#include<iostream>
#include<string>
#include<stdexcept>
#include "List.h"

//ListElement is currently INT

//Node constructor
List::Node::Node(ListElement x){
    data = x;
    prev = nullptr;
    next = nullptr;
}

//List Constructor
List::List(){
    //std::cout << "1\n";
    pos_cursor = 0;
    num_elements = 0;
    //std::cout << "a1\n";
    frontDummy = new Node(-1);

    backDummy = new Node(-1);

    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;

    beforeCursor = frontDummy;
    afterCursor = backDummy;
}

//Copy constructor
List::List(const List& L){
    pos_cursor = 0;
    num_elements = 0;

    frontDummy = new Node(-1);
    backDummy = new Node(-1);

    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;

    beforeCursor = frontDummy;
    afterCursor = backDummy;

    if(L.length() == 0){
        return;
    }

    Node* next = L.frontDummy->next;
    while(next != L.backDummy){
        insertBefore(next->data);
        next = next->next;
    }

}


//Destructor
List::~List(){

    moveFront();


    Node* curr = frontDummy;
    Node* next;

    while(curr != nullptr){
        next = curr->next;
        delete(curr);
        curr = next;

    }
}

int List::length() const{
    return num_elements;
}

ListElement List::front() const{
    if(length() == 0){
        throw std::length_error("List: front(): empty List\n");
    }
    return (frontDummy->next->data);
}

ListElement List::back() const{
    if(length() == 0){
        throw std::length_error("List: back(): empty List\n");
    }
    return (backDummy->prev->data);
}

int List::position() const{
    //printf("Len:%d C:%d\n",length(), pos_cursor);
    if(0 > pos_cursor || pos_cursor > length()){
        throw std::length_error("List: position(): pos_cursor < 0 or pos_cursor > length()\n");
    }
    return pos_cursor;
}

ListElement List::peekNext() const{
    if(pos_cursor > length()){
        throw std::length_error("List: peekNext(): position not < length\n");
    }
    //
    // What happens if next is dummynode?
    //
    return afterCursor->data;
}

ListElement List::peekPrev() const{
    if(pos_cursor <= 0){
        throw std::length_error("List: peekPrev(): position <= 0\n");
    }
    //
    // What happens if next is dummynode?
    //
    return beforeCursor->data;
}

//Manipulation Procedures

void List::clear(){
    moveFront();
    while(length() > 0){
        eraseAfter();
    }
}

void List::moveFront(){
    pos_cursor = 0;
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
}

void List::moveBack(){
    pos_cursor = length();
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;
}

ListElement List::moveNext(){
    if(pos_cursor > length()){
        throw std::length_error("List: moveNext(): position not > length\n");
    }

    ListElement L = afterCursor->data;

    beforeCursor = beforeCursor->next;
    afterCursor = afterCursor->next;
    pos_cursor++;
    return L;
}

ListElement List::movePrev(){
    if(pos_cursor <= 0){
        throw std::length_error("List: movePrev(): position not > 0\n");
    }
    ListElement L = beforeCursor->data;

    beforeCursor = beforeCursor->prev;
    afterCursor = afterCursor->prev;

    pos_cursor--;
    return L;
}


void List::insertAfter(ListElement x){

    Node* n = new Node(x);

    afterCursor->prev = n;
    n->next = afterCursor;

    beforeCursor->next = n;
    n->prev = beforeCursor;

    afterCursor = n;
    num_elements++;
}

void List::insertBefore(ListElement x){

    Node* n = new Node(x);

    afterCursor->prev = n;
    n->next = afterCursor;

    beforeCursor->next = n;
    n->prev = beforeCursor;



    beforeCursor = n;

    pos_cursor++;
    num_elements++;
}

void List::setAfter(ListElement x){
    if (pos_cursor >= length()){
        throw std::length_error("List: setAfter(): position not < length()\n");
        return;
    }
    afterCursor->data = x;
    return;
}

void List::setBefore(ListElement x) {
    if (pos_cursor <= 0){
        throw std::length_error("List: setBefore(): position not > 0\n");
        return;
    }

    beforeCursor->data = x;
    return;
}

void List::eraseAfter(){
    if(pos_cursor >= length()){
        throw std::length_error("List: eraseAfter(): position not < length()\n");
    }

    Node* deleted = afterCursor;

    Node* next = afterCursor->next;

    beforeCursor->next = next;
    next->prev = beforeCursor;

    afterCursor = next;

    delete(deleted);
    num_elements--;
}

void List::eraseBefore(){
    if(pos_cursor <= 0){
        throw std::length_error("List: eraseAfter(): position not > 0>\n");
    }

    Node* deleted = beforeCursor;

    Node* prev = beforeCursor->prev;

    afterCursor->prev = prev;
    prev->next = afterCursor;

    beforeCursor = prev;

    delete(deleted);
    num_elements--;
    pos_cursor--;
}

int List::findNext(ListElement x){

    while(pos_cursor < num_elements){

        if(afterCursor->data == x){
            moveNext();
            return pos_cursor;
        }
        moveNext();
    }

    //moveNext();
    return -1;
}

int List::findPrev(ListElement x){

    while(pos_cursor > 1){

        if(beforeCursor->data == x){
            movePrev();
            return pos_cursor;
        }
        movePrev();

    }
    movePrev();
    return -1;
}

//Helper function
bool is_inside(List &L,ListElement  x){

    L.moveFront();

    for(int i = 0; i < L.length() ; i++){
        if(L.moveNext() == x){
            //std::cout << std::to_string(x) + " is in List\n";
            return true;
        }
    }
    L.insertAfter(x);
    //std::cout << std::to_string(x) + " not in List, adding\n";
    return false;
}


void List::cleanup() {
    List dup;

    Node *curr = frontDummy->next;
    int counter = 0;

    while (curr != backDummy) {
        //printf("counter: %d\n", counter);
        if (is_inside(dup, curr->data)) {
            //There is a duplicate and now we need to delete it.
            //standard delete
            Node *before = curr->prev;
            Node *after = curr->next;

            before->next = after;
            after->prev = before;
            if (counter < pos_cursor) {
                pos_cursor--;
            }
            if (curr == beforeCursor) {
                beforeCursor = before;
            } else if (curr == afterCursor) {
                afterCursor = after;
            }

            delete (curr);
            num_elements--;
            curr = after;
        }else {

            curr = curr->next;
            counter++;
        }
    }

}


List List::concat(const List &L) const {
    //Not sure if this works
    List cpy;
    Node *curr = frontDummy->next;
    while(curr != backDummy){
        //printf("added:%d\n", curr->data);
        cpy.insertBefore(curr->data);
        curr = curr->next;
    }
    curr = L.frontDummy->next;

    while(curr != L.backDummy){
        //printf("2added:%d\n", curr->data);
        cpy.insertBefore(curr->data);
        curr = curr->next;
    }
    //("done\n");
    cpy.moveFront();
    return cpy;
}


std::string List::to_string() const {
    std::string s = "(";
    Node* curr = frontDummy->next;
    //printf("1:%p\n", (void *)curr);
    if(curr != backDummy){
        s.append(std::to_string(curr->data));
        curr = curr->next;
    }
    //printf("2\n");
    while(curr != backDummy ){
        s.append("," + std::to_string(curr->data));

        curr = curr->next;
    }
    s.append(")\n");
    //printf("3\n");
    return s;
}

bool List::equals(const List &R) const {

    if(this->length() != R.length()){
        return false;
    }

    Node* A = this->frontDummy->next;
    Node* B = R.frontDummy->next;

    //Might work
    while (A != backDummy){
        if(A->data != B->data){
            return false;
        }
        A = A->next;
        B = B->next;
    }
    return true;
}

std::ostream& operator<<(std::ostream& stream, const List& L){
    return (stream << L.to_string());

};



bool operator==( const List& A, const List& B ){
    return A.equals(B);
}

List& List::operator=( const List& L ){
    clear();
    //printf("called\n");
    Node* curr = L.frontDummy->next;
    while(curr != L.backDummy){
        //printf("inserting %d\n", curr->data);
        insertBefore(curr->data);
        curr = curr->next;
    }
    pos_cursor=L.pos_cursor;
    num_elements = L.num_elements;
    return *this;
}
