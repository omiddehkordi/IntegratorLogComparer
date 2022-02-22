//
//  compare.cpp
//  IntegratorLogComparer
//
//  Created by Omid Dehkordi on 4/30/21.
//

#include "compare.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

CompareInt::CompareInt(string fi1, string fi2)
 : f1(fi1), f2(fi2)
{
    
}

CompareInt::~CompareInt()
{
    delete [] components;
    for(list<Component*>::iterator it1 = file1.begin(); it1 != file1.end();)
    {
        if((*it1) != nullptr)
        {
            delete *it1;
        }
        it1++;
    }
    for(list<Component*>::iterator it2 = file2.begin(); it2 != file2.end();)
    {
        if((*it2) != nullptr)
        {
            delete *it2;
        }
        it2++;
    }
    for(map<string, Compare*>::iterator it3 = compare.begin(); it3 != compare.end();)
    {
        if((it3)->second != nullptr)
        {
            delete (it3)->second;
        }
        it3++;
    }
}

void CompareInt::StringtoDouble(string measurement, string tolerance, double& measure, long double& tol, string& unit)
{
    string tempMn = measurement.substr(0, measurement.find(' '));
    
    unit = measurement.substr(measurement.find(' ') + 1, measurement.size() - 1);
    unit.erase(remove(unit.begin(), unit.end(), '|'), unit.end());
    
    measure = stod(tempMn);

    string tempTn = tolerance.substr(0, tolerance.find('%'));
    stringstream t;
    t.precision(tempTn.size());
    t << tempTn;
    t >> tol;
}

bool CompareInt::loadFile()
{
    bool nextline = false;
    ifstream infile(f1);
    
    if(! infile)
    {
        return false;
    }
    
    string line;
    
    while(getline(infile, line))
    {
        if(! nextline)
        {
            if(line.find("->") != string::npos && line.find(';') != string::npos && line.find("Board Serial Number") == string::npos)
            {
                file1.push_back(new Component(line.substr(line.find('>') + 1, line.find(';') - line.find('>') - 1)));
                nextline = true;
            }
        }
        else if(nextline)
        {
            if(line.find("Out of Range") != string::npos)
            {
                delete file1.back();
                file1.pop_back();
                nextline = false;
            }
            else if(line.find("Measured:") != string::npos)
            {
                string measurement1 = line.substr(line.find("Measured:") + 10, line.find('(') - line.find("Measured:") - 10);
                string tolerance1 = line.substr(line.find('(') + 1, line.find(')') - line.find('(') - 1);
                
                double measure1;
                long double tol1;
                string unit1;
                
                StringtoDouble(measurement1, tolerance1, measure1, tol1, unit1);
                
                (file1.back())->m_meas = measure1;
                (file1.back())->m_unit = unit1;
                (file1.back())->m_tol = tol1;
                (file1.back())->m_tunit = '%';
                nextline = false;
            }
        }
    }
    
    bool nextline2 = false;
    ifstream infile2(f2);
    
    if(! infile2)
    {
        return false;
    }
    
    string line2;
    
    while(getline(infile2, line2))
    {
        if(! nextline2)
        {
            if(line2.find("->") != string::npos && line2.find(';') != string::npos && line2.find("Board Serial Number") == string::npos)
            {
                file2.push_back(new Component(line2.substr(line2.find('>') + 1, line2.find(';') - line2.find('>') - 1)));
                nextline2 = true;
            }
        }
        else if(nextline2)
        {
            if(line2.find("Out of Range") != string::npos)
            {
                delete file2.back();
                file2.pop_back();
                nextline2 = false;
            }
            else if(line2.find("Measured:") != string::npos)
            {
                string measurement2 = line2.substr(line2.find("Measured:") + 10, line2.find('(') - line2.find("Measured:") - 10);
                string tolerance2 = line2.substr(line2.find('(') + 1, line2.find(')') - line2.find('(') - 1);
                
                double measure2;
                long double tol2;
                string unit2;
                
                StringtoDouble(measurement2, tolerance2, measure2, tol2, unit2);
                
                (file2.back())->m_meas = measure2;
                (file2.back())->m_unit = unit2;
                (file2.back())->m_tol = tol2;
                (file2.back())->m_tunit = '%';
                nextline2 = false;
            }
        }
    }
    return true;
}

void CompareInt::diffBetMeasurement()
{
    int i = 0;
    components = new string[file1.size() + 1];
    for(list<Component*>::iterator it1 = file1.begin(); it1 != file1.end(); )
    {
        for(list<Component*>::iterator it2 = file2.begin(); it2 != file2.end(); it2++)
        {
            if((*it1)->m_comp == (*it2)->m_comp)
            {
                components[i] = (*it1)->m_comp;
                i++;
                compare[(*it1)->m_comp] = new Compare;
                (compare[(*it1)->m_comp])->mu = (*it1)->m_unit;
                if((*it1)->m_meas > (*it2)->m_meas)
                {
                    (compare[(*it1)->m_comp])->m_m = (*it1)->m_meas - (*it2)->m_meas;
                }
                else if((*it1)->m_meas < (*it2)->m_meas)
                {
                    (compare[(*it1)->m_comp])->m_m = (*it2)->m_meas - (*it1)->m_meas;
                }
                else if((*it1)->m_meas == (*it2)->m_meas)
                {
                    (compare[(*it1)->m_comp])->m_m = 0;
                }
                it1++;
            }
        }
    }
}

void CompareInt::diffBetTolerance()
{
    int i = 0;
    for(list<Component*>::iterator it1 = file1.begin(); it1 != file1.end();)
    {
        for(list<Component*>::iterator it2 = file2.begin(); it2 != file2.end(); it2++)
        {
            if((*it1)->m_comp == (*it2)->m_comp)
            {
                components[i] = (*it1)->m_comp;
                i++;
                (compare[(*it1)->m_comp])->tu = '%';
                if((*it1)->m_tol > (*it2)->m_tol)
                {
                    (compare[(*it1)->m_comp])->m_t = (*it1)->m_tol - (*it2)->m_tol;
                }
                else if((*it1)->m_tol < (*it2)->m_tol)
                {
                    (compare[(*it1)->m_comp])->m_t = (*it2)->m_tol - (*it1)->m_tol;
                }
                else if((*it1)->m_tol == (*it2)->m_tol)
                {
                    (compare[(*it1)->m_comp])->m_t = 0;
                }
                it1++;
            }
        }
    }
}

void CompareInt::outputComponentValues()
{
    for(list<Component*>::iterator it = file1.begin(); it != file1.end(); it++)
    {
        cout << (*it)->m_comp << ": " << (*it)->m_meas << " " << (*it)->m_unit << " " << (*it)->m_tol << endl;
    }
    for(list<Component*>::iterator it2 = file2.begin(); it2 != file2.end(); it2++)
    {
        cout << (*it2)->m_comp << ": " << (*it2)->m_meas << " " << (*it2)->m_unit << " " << (*it2)->m_tol << endl;
    }
    //Add one tab delimited line per device
}

void CompareInt::outputCompare(string output)
{
    ofstream myout;
    myout.open(output);
    for(int i = 0; i < compare.size(); i++)
    {
        myout << components[i] << ": " << (compare[components[i]])->m_m << " " << (compare[components[i]])->mu << " " << (compare[components[i]])->m_t << (compare[components[i]]) ->tu << endl;
    }
    myout.close();
}

