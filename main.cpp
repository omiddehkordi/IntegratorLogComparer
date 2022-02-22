//
//  main.cpp
//  IntegratorLogComparer
//
//  Created by Omid Dehkordi on 4/5/21.
//

// Change from Folder to Terminal Command that takes in file


#include <iostream>
#include <string>
#include <list>
#include "compare.h"
using namespace std;

int main(int argc, char* argv[])
{
    list<CompareInt*> fcomp;
    
    for(int i = 1; i < argc; i++)
    {
        for(int j = i; j < argc; j++)
        {
            if(argv[i] != argv[j])
            {
                fcomp.push_back(new CompareInt(argv[i], argv[j]));
            }
        }
    }
    
    string out = "output";
    string end = ".log";
    int oNum = 1;
    string output = out + to_string(oNum) + end;
    
    for(list<CompareInt*>::iterator it = fcomp.begin(); it != fcomp.end(); it++)
    {
        output = out + to_string(oNum) + end;
        (*it)->loadFile();
        (*it)->diffBetMeasurement();
        (*it)->diffBetTolerance();
        (*it)->outputCompare(output);
        oNum++;
    }
    
    
}
