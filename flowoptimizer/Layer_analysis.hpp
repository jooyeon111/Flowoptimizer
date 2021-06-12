//20.08.21

#pragma once

//C++ header
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
#include <iomanip>

//C header
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <cmath>

//My header file
#include "compile_option.hpp"
#include "Dataflow.hpp"
#include "Layer_info.hpp"
#include "HW.hpp"

class Layer_analysis final{

public:
    
    Layer_analysis();//Basic constructor
    Layer_analysis(std::string Dataflow_name, int Dataflow_count, int Generation_limit, 
    std::string Evaluation_criteria, const std::shared_ptr<HW>& HW_ptr, const std::shared_ptr<Layer_info>& Layer_info_ptr,
    const double Muation_rate, const double Packing_prob, const double Packing_ratio, const double JudgementDay_pob, const int CrossOver_count);//Constructor
    
    ~Layer_analysis();//Deconstructor
    Layer_analysis(const Layer_analysis&) = delete;//Deleting implied function
    Layer_analysis& operator=(const Layer_analysis& ) = delete;//Deleting implied function

    void Initialization(void);
    void Evaluation(void);
    void Mutation(void);
    void Selection(void);

    void Packing_and_Judgement(void);
    void Swapping_dataflow(std::unique_ptr<Dataflow>& LHS, std::unique_ptr<Dataflow>& RHS);

private:

    int mGen_count;
    const int mGen_limit;   
    const int mAction_count;
    const int mDataflow_count;

    const double mMuation_rate;
    const double mPacking_prob;
    const double mPacking_ratio;
    const double mJudgementDay_prob;
    
    const int mCrossOver_count;

    const std::string mOriginal_name;
    std::string mEvaluation_criteria;

    std::shared_ptr<HW> mHW_ptr;
    std::shared_ptr<Layer_info> mLayer_info_ptr;
    std::vector<std::unique_ptr<Dataflow>> mDataflow_vector;
    
};
