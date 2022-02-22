//
//  compare.hpp
//  IntegratorLogComparer
//
//  Created by Omid Dehkordi on 4/30/21.
//

#ifndef compare_h
#define compare_h

#include <string>
#include <map>
#include <list>

class CompareInt
{
  public:
    CompareInt(std::string fi1, std::string f2);
    ~CompareInt();
    void StringtoDouble(std::string measurement, std::string tolerance, double& measure, long double& tol, std::string& unit);
    bool loadFile();
    void diffBetMeasurement();
    void diffBetTolerance();
    void outputComponentValues();
    void outputCompare(std::string ouput);
    
  private:
    struct Component
    {
        Component(std::string comp) : m_comp(comp) {}
        std::string m_comp;
        double m_meas;
        std::string m_unit;
        long double m_tol;
        char m_tunit;
    };

    struct Compare
    {
        double m_m;
        std::string mu;
        long double m_t;
        char tu;
    };
    
    std::string f1;
    std::string f2;
    
    std::list<Component*> file1;
    std::list<Component*> file2;
    
    std::map<std::string, Compare*> compare;
    std::string* components;
};

#endif /* compare_h */

