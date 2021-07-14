#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

struct node{
    int coefficent, power;
    node* next; 
};

class linkedlist{
private:
    node* head;
    node* last;

public:
    
    linkedlist() {
        head = new node;
        last = NULL;
    }

    node* getHead() {
        return head;
    }

    void insert_unordered(int coefficent, int power){
        node* temp = new node;
        temp->coefficent = coefficent;
        temp->power = power;

        if (last == NULL) {
            head->next = temp;
            last = temp;
        } else {
            last->next = temp;
            last = temp;
        }
    }

    void insert_ordered(int coefficent, int power) {
        node* tmp_list = head;
        while(tmp_list != NULL) {
            if (tmp_list->next == NULL) {
                node* temp = new node;
                temp->coefficent = coefficent;
                temp->power = power;
                tmp_list->next = temp;
                break;
            }
            if (power == tmp_list->next->power) {
                tmp_list->next->coefficent += coefficent;
                break;
            } else if(power > tmp_list->next->power) {
                node* temp = new node;
                temp->coefficent = coefficent;
                temp->power = power;
                temp->next = tmp_list->next;
                tmp_list->next = temp;
                break;
            }
            
            tmp_list = tmp_list->next;
        }
    }
    

    void print(ofstream& outFile) {
        node* temp = head->next;
        while(temp != NULL) {
            if (temp->coefficent != 0) {
                if (temp->coefficent > 0) {
                    if (temp->coefficent != 1){
                        if(temp == head->next)
                            outFile<<""<<temp->coefficent;
                        else
                            outFile<<"+"<<temp->coefficent;
                    }else if (temp->power == 0){
                        if(temp == head->next)
                            outFile<<"1";
                        else
                            outFile<<"+1";
                    }else{
                        if(temp == head->next)
                            outFile<<"";
                        else
                            outFile<<"+";
                    }
                }else{
                    if (temp->coefficent != -1)
                        outFile<<temp->coefficent;
                    else {
                        if(temp->power == 0)
                            outFile<<"-1";
                        else
                            outFile<<"-";
                    }
                }
                
                if(temp->power == 1)
                    outFile<<"x";
                else if(temp->power != 0)
                    outFile<<"x^"<<temp->power;
            }

            temp = temp->next;
        }
        outFile<<"\n\n";
    }

    friend void sum(node* head1, node* head2,ofstream& outFile);
    friend void difference(node* head1, node* head2,ofstream& outFile);
    friend void product(node* head1, node* head2,ofstream& outFile);
};

void sum(node* head1, node* head2,ofstream& outFile) {
       node* temp = head1->next;
       linkedlist tmp_list;
       while(temp != NULL) {
           tmp_list.insert_ordered(temp->coefficent,temp->power);
           temp = temp->next;
       }
       temp = head2->next;
       while(temp != NULL) {
           tmp_list.insert_ordered(temp->coefficent,temp->power);
           temp = temp->next;
       }
       tmp_list.print(outFile);
   }

void difference(node* head1, node* head2,ofstream& outFile){
    node* temp = head1->next;
    linkedlist tmp_list;
    while(temp != NULL) {
        
        tmp_list.insert_ordered(temp->coefficent,temp->power);
        temp = temp->next;
    }
    temp = head2->next;
    while(temp != NULL) {
        tmp_list.insert_ordered(-1 * temp->coefficent,temp->power);
        temp = temp->next;
    }
    tmp_list.print(outFile);
};

void product(node* head1, node* head2,ofstream& outFile){
    node* temp1 = head1->next;
    linkedlist tmp_list;
    while(temp1 != NULL) {
        int coefficent = temp1->coefficent;
        int power = temp1->power;
        node* temp2 = head2->next;
        while(temp2 != NULL) {
            tmp_list.insert_ordered(coefficent * temp2->coefficent, power + temp2->power);
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
    tmp_list.print(outFile);
}

int main() {
    
    ifstream inFile;
    ofstream outFile;
    
    inFile.open("input.txt");
    outFile.open("output.txt");
    
    linkedlist poly1, poly2;
    linkedlist poly1_canonical, poly2_canonical;

    string line,strCoef,strExpo;
    stringstream ss;
    
    getline(inFile,line);
    ss.str(line);
    while(ss >> strCoef) {
        ss >> strExpo;
        int coefficent = stoi(strCoef);
        int power = stoi(strExpo);
        
        poly1.insert_unordered(coefficent,power);
        poly1_canonical.insert_ordered(coefficent,power);
    }

    ss.clear();

    getline(inFile,line);
    ss.str(line);
    while(ss >> strCoef) {
        ss >> strExpo;
        int coefficent = stoi(strCoef);
        int power = stoi(strExpo);

        poly2.insert_unordered(coefficent,power);
        poly2_canonical.insert_ordered(coefficent,power);
    } 

    outFile<<"First polynomial is: ";
    poly1.print(outFile);
    outFile<<"First polynomial after canonical: ";
    poly1_canonical.print(outFile);
    outFile<<"Second polynomial is: ";
    poly2.print(outFile);
    outFile<<"Second polynomial after canonical: ";
    poly2_canonical.print(outFile);

 
    outFile<<"Their sum is:";
    sum(poly1_canonical.getHead(),poly2_canonical.getHead(),outFile);

    outFile<<"Their difference is:";
    difference(poly1_canonical.getHead(),poly2_canonical.getHead(),outFile);

    outFile<<"Their product is:";
    product(poly1_canonical.getHead(), poly2_canonical.getHead(),outFile);

    inFile.close();
    outFile.close();
    
    return 0;
}
