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
#include <stddef.h>
#include <stdlib.h>
using namespace std;


template<class T>
class SA {
private:
    int low, high;
    static SA *newlist;
    union{
        SA *freepointer;
        T* p;
    };
    
public:
// default constructor
// allows for writing things like SA a;
    SA() {
        low = 0;
        high = -1;
        p = new T[1];
        *p='\0';
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

    void *operator new[](size_t size) {
        cout<< "Overloading new operator"<< endl;
        if (size != sizeof (SA)) {
        //this check is needed for cases when class
        //derivation is used (Chapter 4) and the
        //derived class has no operator new.
        //String: : operator delete above will field
        //this over-sized block when it is reclaimed.
            return malloc(size);
        }else if (!newlist) {
            newlist = (SA *)new T[100 * sizeof(SA)];
            int i=0;
            for (i=0;i < 99; i++) {
                newlist[i].freepointer = &(newlist[i+1]);
            }
            newlist[i].freepointer =0;
            }
        
        SA *savenew = newlist;
        newlist = newlist->freepointer;
        return savenew;
    }
        
    void operator delete[] (void *p){
        cout<< "Overloading delete operator" << endl;
        SA *s = (SA *)p;
        s->freepointer = newlist;
        newlist = s;
    }
};

template<class T>
SA<T> *SA<T>::newlist=0;


template<class T>
class SM {
    private:
        int xLow;
        int yLow;
        int xHigh;
        int yHigh;
        SA< SA<T>> matrix;

    public:
        SM(){
            xLow = 0;
            xHigh = -1;
            yLow = 0;
            yHigh = -1;
        }

        SM(int x, int y) {
            xLow= 0;
            xHigh = x-1;
            yLow= 0;
            yHigh = y-1;
            matrix = SA<SA<T> >(x);
            
            for(int i=0; i<x; i++) {
                matrix[i] = SA<T>(y);
            }
        }

        SM(int xl, int xh, int yl, int yh) {
            if( (xh-xl+1) <= 0 || (yh-yl+1) <= 0) {
                cout<< "constructor error in bounds definition"<<endl;
                exit(1);
            }
            
            xLow = xl;
            xHigh = xh;
            yLow = yl;
            yHigh = yh;
            matrix = SA< SA<T> >(xl,xh);
            
            for(int i = xl; i <= xh; i++) {
                matrix[i] = SA<T>(yl,yh);
            }
        }

        SM operator*(SM& m) {
            int xlhs = xHigh - xLow + 1;
            int ylhs = yHigh - yLow + 1;
            
            int xrhs = m.xHigh - m.xLow + 1;
            int yrhs = m.yHigh - m.yLow + 1;
            
            if (ylhs != xrhs) {
                cout<< "matrix multiplication error"<<endl;
                exit(0);
            }

            SM<T> temp_m(xlhs, yrhs);

            for(int i = 0; i < xlhs; i++) {
                for(int j = 0; j < yrhs; j++) {
                    int tmp = 0;
                    
                    for(int k = 0; k < ylhs; k++)
                        tmp += (matrix[i][k] * m[k][j]);
                    
                    temp_m[i][j] = tmp;
                }
            }
            return temp_m;
        }
    
        SA<T>& operator[](int i) {
            return matrix[i];
        }
        
        friend ostream& operator<<(ostream& m, SM<T>& nm)    {
            for (int i = nm.xLow; i <= nm.xHigh; i++) {
                m << "[";
                
                for (int j = nm.yLow; j <= nm.yHigh; j++) {
                    m << nm[i][j];
                    if(j < nm.yHigh)
                        m<<",";
                }
                
                m << "]"<<endl;
                }
                return m;
        }
    
    
};



int main(){

//  a = [1,2,3]  2 by 3 matrix
//      [4,5,6]
    SM<int> a(2,3);
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    cout<<"Matrix a:"<<endl;
    cout<<a<<endl;

//  b = [7,8]    3 by 2 matrix
//      [9,10]
//      [11,12]
    SM<int> b(3,2);
    b[0][0] = 7; b[0][1] = 8;
    b[1][0] = 9; b[1][1] = 10;
    b[2][0] = 11; b[2][1] = 12;
    cout<<"Matrix b:"<<endl;
    cout<<b<<endl;
//  After a*b, ans should be :[58,64]
//                            [139,154]
    SM<int> ans = a*b;
    cout<<"After a*b: "<<endl;
    cout<<ans;

}
