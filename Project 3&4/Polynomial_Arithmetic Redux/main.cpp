//
//  main.cpp
//  Polynomial_Arithmetic_Redux
//
//  Created by Weiting Lin on 4/15/21.
//  Copyright © 2021 Weiting Lin. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

class Polynomial{
    
private:
    map<int, int, greater<int> > polyMap;
    
public:
    Polynomial() = default;
    Polynomial(const string &s){
        stringstream ss(s);
        string str_coefficent, str_power;
        while (ss >> str_coefficent >> str_power){
            int coefficent = stoi(str_coefficent);
            int power = stoi(str_power);

            polyMap[power] += coefficent;
        }
    }
    Polynomial(const Polynomial &other){
        polyMap.insert(other.polyMap.begin(), other.polyMap.end());
    }

    Polynomial operator+(const Polynomial &other){
        Polynomial tmp(*this);
        
        for (auto &i : other.polyMap){
            int power = i.first, coefficent = i.second;
            tmp.polyMap[power] += coefficent;
        }
        return tmp;
    }

    Polynomial operator-(const Polynomial &other){
        Polynomial tmp(*this);
        for (auto &i : other.polyMap){
            int power = i.first, coefficent = i.second;
            tmp.polyMap[power] -= coefficent;
        }
        return tmp;
    }

    Polynomial operator*(const Polynomial &other){
        Polynomial tmp;
        for (auto &i : this->polyMap){
            int power = i.first, coefficent = i.second;
            
            for (auto &j : other.polyMap)
                tmp.polyMap[power + j.first] += coefficent * j.second;
        }
        
        return tmp;
    }

    Polynomial &operator=(const Polynomial &other){
        if (this != &other)
            this->polyMap.insert(other.polyMap.begin(), other.polyMap.end());
        
        return *this;
    }

    friend ostream &operator<<(ostream &ans, const Polynomial &p){
        bool start = true;
        
        for (auto &i : p.polyMap){
            int power = i.first, coefficent = i.second;

            if (coefficent != 0){
                if (coefficent > 0){
                    if (coefficent != 1)
                        ans << (start ? "" : "+") << coefficent;
                    else if (power == 0)
                        ans << (start ? "1" : "+1");
                    else
                        ans << (start ? "" : "+");
                }
                else if (coefficent != -1)
                    ans << coefficent;
                else
                    ans << (power == 0 ? "-1" : "-");
                if (power == 1)
                    ans << "x";
                else if (power != 0)
                    ans << "x^" << power;
            }else
                ans<<"0";
    
            start = false;
        }
        return ans;
    }
};

int main()
{
    ifstream inFile;
    ofstream outFile;
    
    inFile.open("input.txt");
    outFile.open("output.txt");
    
    
    while(inFile){
        string s1, s2;
        
        getline(inFile, s1);
        getline(inFile, s2);
        if(s1=="" || s2 == "") break;
        
        Polynomial p1(s1);
        Polynomial p2(s2);
        
        outFile << "First polynomial in canonical form is: " << p1 << endl;
        outFile << "\nSecond polynomial in canonical form is: " << p2 << endl;
        
        outFile << "\nTheir sum is: " << p1 + p2 << endl;
        outFile << "\nTheir difference is: " << p1 - p2 << endl;
        Polynomial prod = p1 * p2;
        outFile << "\nTheir product is: " << prod <<endl;
        outFile<<"---------------------------------------------------------------------"<<endl;
    }

    inFile.close();
    outFile.close();
}

