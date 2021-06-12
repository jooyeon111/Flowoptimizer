//20.08.21

#pragma once

//C++ header
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include <algorithm>

//C header
#include <cstring>
#include <cstdlib>

//My header
#include "Directive.hpp"
#include "util.hpp"
#include "Layer_info.hpp"
#include "HW.hpp"
#include "compile_option.hpp"


class Dataflow final {

public:

    //Basic functions
    Dataflow() = delete;
    Dataflow(std::string Dataflow_name, const std::shared_ptr<Layer_info>& mLayer_info, const std::shared_ptr<HW>& mHW_ptr);
    ~Dataflow();
    Dataflow(const Dataflow& Another);
    Dataflow& operator=(const Dataflow& Another) = delete;
 
    //Implied functions
    void FC_init(void); //will be deleted
    void Conv_init(void);
    
    void Making_dataflow_file(void) const;
    void Executing_dataflow_file(void) const;
    void Parsing_and_verifying_output(void);
    void Resetting_dataflow(void);
    
    //Setting
    void Set_tile2_tile3_map(void);
    void Set_dataflow_name(std::string Dataflow_name);
    void Reset_Population_count(void);
    void Set_dimension_vector(void);
    //void Clear_Dataflow(void);

    //Getting
    std::string Get_dataflow_name(void) const;
    double Get_cycles(void) const;
    double Get_energy(void) const;
    long int Get_area(void) const;
    double Get_power(void) const;
    int Get_L2_size(void) const;
    double Get_L1_size(void) const;
    double Get_evaluation_value(std::string Evaluation_criteria);
    
    //Actions
    void Action1_Random_swap(void);
    void Action2_Inc_size_offset(void);
    void Action3_Dec_size_offset(void);
    void Action4_changing_spatial_map(void);
    void Action5_chaning_dataflow(std::unique_ptr<Dataflow>& RHS);
    void Action5_rebuilding_dataflow(void);

    //Util
    void Print_all(void) const;
    void Print_all_result(void) const;
    void Print_all_detail(void) const;

private:

    std::string mOriginal_name;
    std::string mDataflow_name;
    std::shared_ptr<HW> mHW_ptr;
    std::shared_ptr<Layer_info> mLayer_info_ptr;
    std::list<std::unique_ptr<Directive>> mDirective_sequence;
    std::vector<Dimension_type> mDimension_vector;
    
    int mX_basis;
    int mY_basis;

    static int mPopulation_count;//Homw many dataflow is generated


    //double mEvaluation_value;
    double mEvaluation_cycles;
    double mEvaluation_activity_count_based_energy;
    double mEvaluation_throughput;
    long int mEvaluation_area;
    double mEvaluation_power;
    int mEvaluation_L2;
    int mEvaluation_L1;

};
