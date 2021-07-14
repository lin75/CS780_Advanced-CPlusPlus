//
//  main.cpp
//  safeMatrix
//
//  Created by Weiting Lin
//  Copyright © 2021 Weiting Lin. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <iterator>
using namespace std;

template<class T>
class SA {
private:
    int low, high;
    T* p;
public:
// default constructor
// allows for writing things like SA a;
    SA() {
        low = 0;
        high = -1;
        p = NULL;
    }
    
// 2 parameter constructor lets us write
// SA x(10,20);
    SA(int l, int h) {
        if ((h - l + 1) <= 0){
            cout << "constructor error in bounds definition" << endl;
            exit(1);
        }
        low = l;
        high = h;
        p = new T[h - l + 1];
    }
    
// single parameter constructor lets us
// create a SA almost like a "standard" one by writing
// SA x(10); and getting an array x indexed from 0 to 9
    SA(int i) {
        low = 0;
        high = i - 1;
        p = new T[i];
    }
    
// copy constructor for pass by value and
// initialization
    SA(const SA & s) {
        int size = s.high - s.low + 1;
        p = new T[size];
        for (int i = 0; i < size; i++)
            p[i] = s.p[i];
        low = s.low;
        high = s.high;
    }
    
// destructor
    ~SA() {
        delete[] p;
    }
    
//overloaded [] lets us write
//SA x(10,20); x[15]= 100;
    T& operator[](int i) {
        if (i < low || i > high) {
            cout << "index " << i << " out of range" << endl;
            exit(1);
        }
        return p[i - low];
    }
    
// overloaded assignment lets us assign
// one SA to another
    SA& operator=(const SA& s) {
        if (this == &s)
            return *this;
        delete[] p;
        int size = s.high - s.low + 1;
        p = new T[size];
        for (int i = 0; i < size; i++)
            p[i] = s.p[i];
        low = s.low;
        high = s.high;
        return *this;
    }


    friend ostream& operator<<(ostream& os, SA<T> s) {
        int size = s.high - s.low + 1;
        for (int i = 0; i < size; i++)
            os << s.p[i] << " ";
        return os;
    }
    
    T* start() {
        return &p[0];
    }
    
    T* end() {
        return &p[high];
    }
};

template<class T>
void test_find (SA<T> A, T item){
    if(find(A.start(), A.end(), item)!= A.end())
        cout << item << " is found at position " << find(A.start(), A.end(), item) - A.start() << endl;
    else
        cout << item << " is not found" << endl;
}

int main(){
//  A1 = {5, 2, 4, 1, 3};
    SA<int> A1(5);
    A1[0]=5;
    A1[1]=2;
    A1[2]=4;
    A1[3]=1;
    A1[4]=3;
    
//  A2 = {8.7, 4.5, 6.7, 3.4, 2.3}
    SA<double> A2(5);
    A2[0]=8.7;
    A2[1]=4.5;
    A2[2]=6.7;
    A2[3]=3.4;
    A2[4]=2.3;
    
//  A3 = {d, b, a, e, c}
    SA<char> A3(5);
    A3[0]='d';
    A3[1]='b';
    A3[2]='a';
    A3[3]='e';
    A3[4]='c';

    cout << "\n------------------- For Safe Array A1 Test for int type -------------------"<< endl;
    cout << "A1: " << A1 << endl;
//  1 should be at position 3
//  6 should not be found
    test_find (A1,1);
    test_find (A1,6);

    cout << "\n------------------- For Safe Array A2 Test for double type -------------------"<< endl;
    cout << "A2: " << A2 << endl;
//  4.5 should be at position 3
//  8.8 should not be found
    test_find (A2,4.5);
    test_find (A2,8.8);

    cout << "\n------------------- For Safe Array A3 Test for char type -------------------"<< endl;
    cout << "A3: " << A3 << endl;
//  e should be at position 3
//  f should not be found
    test_find (A3,'e');
    test_find (A3,'f');
    
    cout<<endl;
}
