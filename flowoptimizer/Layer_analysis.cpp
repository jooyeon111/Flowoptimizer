#include "Layer_analysis.hpp"


//int Layer_analysis::mGen_count = 0;

//Default constructor
Layer_analysis::Layer_analysis()        
    :mOriginal_name("R")
    ,mDataflow_count(3)
    ,mGen_limit(5)
    ,mGen_count(0)
    ,mAction_count(5)
    ,mHW_ptr(std::make_shared<HW>())
    ,mLayer_info_ptr(std::make_shared<Layer_info>())
    ,mEvaluation_criteria("Cycles")
    ,mMuation_rate(0.5)
    ,mPacking_prob(0.6)
    ,mPacking_ratio(0.4)
    ,mJudgementDay_prob(0.2)
    ,mCrossOver_count(1)
{    
    //Random seed
    srand(time(NULL));
    
    //Vector capcity
    mDataflow_vector.reserve(mDataflow_count + mDataflow_count * mAction_count);

    std::cout<<"Layer analysis overveiw"<<std::endl<<std::endl;
    std::cout<<"Dataflow name : "<<mOriginal_name<<std::endl;
    std::cout<<"Dataflow count (Number of intial dataflow ) : "<<mDataflow_count<<std::endl;
    std::cout<<"Generation limit : "<<mGen_limit<<std::endl;
    std::cout<<"Evaluation criteria : "<<mEvaluation_criteria<<std::endl;
    std::cout<<"Packing probability : "<<mPacking_prob<<std::endl;
    std::cout<<"Packing ratio : "<<mPacking_ratio<<std::endl;
    std::cout<<"Judgement probaility : "<<mJudgementDay_prob<<std::endl;
    std::cout<<"Mutation rate : "<<mMuation_rate<<std::endl<<std::endl;

    std::cout<<"Hardware overview"<<std::endl;
    mHW_ptr->print_all();
    std::cout<<std::endl;

    std::cout<<"Layer information overview"<<std::endl;
    mLayer_info_ptr->Print_all();    
    std::cout<<std::endl;

    std::cout<<" Minimum Cycles " <<mLayer_info_ptr->Get_product_term_dimension() / mHW_ptr->Get_number_of_PES()<<std::endl;

}

//Constructor
Layer_analysis::Layer_analysis(std::string Dataflow_name, int Dataflow_count, int Generation_limit ,std::string Evaluation_criteria, const std::shared_ptr<HW>& HW_ptr, const std::shared_ptr<Layer_info>& Layer_info_ptr, const double Muation_rate,const double Packing_prob, const double Packing_ratio, const double JudgementDay_pob, const int CrossOver_count)
    :mOriginal_name(Dataflow_name)
    ,mDataflow_count(Dataflow_count)
    ,mGen_limit(Generation_limit)
    ,mGen_count(0)
    ,mAction_count(5)
    ,mHW_ptr(HW_ptr)
    ,mLayer_info_ptr(Layer_info_ptr)
    ,mEvaluation_criteria(Evaluation_criteria)
    ,mMuation_rate(Muation_rate)
    ,mPacking_prob(Packing_prob)
    ,mPacking_ratio(Packing_ratio)
    ,mJudgementDay_prob(JudgementDay_pob)
    ,mCrossOver_count(CrossOver_count)    
{
    //Random seed
    srand(time(NULL));

    //vector capacity
    mDataflow_vector.reserve(mDataflow_count + mDataflow_count * mAction_count);

    std::cout<<"Layer analysis overveiw"<<std::endl;
    std::cout<<"Dataflow name : "<<mOriginal_name<<std::endl;
    std::cout<<"Dataflow count (Number of intial dataflow ) : "<<mDataflow_count<<std::endl;
    std::cout<<"Generation limit : "<<mGen_limit<<std::endl;
    std::cout<<"Evaluation criteria : "<<mEvaluation_criteria<<std::endl;
    std::cout<<"Packing probability : "<<mPacking_prob<<std::endl;
    std::cout<<"Packing ratio : "<<mPacking_ratio<<std::endl;
    std::cout<<"Judgement probaility : "<<mJudgementDay_prob<<std::endl;
    std::cout<<"Mutation rate : "<<mMuation_rate<<std::endl<<std::endl;

    std::cout<<"Hardware overview"<<std::endl;
    mHW_ptr->print_all();
    std::cout<<std::endl;

    std::cout<<"Layer information overview"<<std::endl;
    mLayer_info_ptr->Print_all();    
    std::cout<<std::endl;

    std::cout<<"Minimum Cycles " <<mLayer_info_ptr->Get_product_term_dimension() / mHW_ptr->Get_number_of_PES()<<std::endl;    


}

//Deconstructor
Layer_analysis::~Layer_analysis()
{
    
    //Reset dataflow count at end of layer analysis
    for(auto i = mDataflow_vector.begin(); i != mDataflow_vector.end(); i++)
        (*i)->Reset_Population_count();

    //std::cout<<"Layer deconstructor is callled"<<std::endl;
}

//Intialization
void Layer_analysis::Initialization(void)
{
    #if PRINT_INTERMEDIATE_PROCESS
    std::cout<<"Initialization"<<std::endl;
    #endif

    //mLayer_info_ptr->Print_all();
    //mHW_ptr->print_all();
    
    //Call dataflow constructor
    for(int i = 0; i < mDataflow_count; i++){
        mDataflow_vector.push_back(std::make_unique<Dataflow>(mOriginal_name, mLayer_info_ptr, mHW_ptr));                    
        mDataflow_vector[i]->Conv_init();
    }


    for(int i = 0; i < mDataflow_vector.size(); i++)
    {
        mDataflow_vector[i]->Print_all();
    }

    mHW_ptr->making_hw_file();

    #if PRINT_INTERMEDIATE_PROCESS
    std::cout<<"Intialization is done"<<std::endl;
    #endif

}

//Evaluation
void Layer_analysis::Evaluation()
{   
    mGen_count++;
    #if PRINT_INTERMEDIATE_PROCESS
    std::cout<<"Evaluation"<<std::endl;
    #endif

    //Making execute file and executing maestro    
    for(int i = 0; i < mDataflow_vector.size(); i++)
    {
        mDataflow_vector[i]->Making_dataflow_file();   
        mDataflow_vector[i]->Executing_dataflow_file();        
    }
    
    //Parsing Part    

    for(int i = 0; i < mDataflow_vector.size(); i++)
    {            
        
        mDataflow_vector[i]->Parsing_and_verifying_output();   
    }
    
    for(int i = 0; i < mDataflow_vector.size(); i++)
    {
        mDataflow_vector[i]->Print_all();
    }

    #if PRINT_INTERMEDIATE_PROCESS
    std::cout<<"Evaluation is done"<<std::endl;
    #endif
    
}

//Mutation
void Layer_analysis::Mutation(void)
{
    #if PRINT_INTERMEDIATE_PROCESS
    std::cout<<"***************"<<mGen_count<<"-Generation"<<"***************"<<std::endl;
    std::cout<<"Mutataion"<<std::endl;
    #endif

    for(int i = 0; i<mDataflow_count; i++)
    {
        if ((rand() % 100) < mMuation_rate * 100 )
        {
            mDataflow_vector.push_back(std::make_unique<Dataflow>( (*mDataflow_vector[i]) ) );
            mDataflow_vector.back()->Set_dataflow_name(mDataflow_vector[i]->Get_dataflow_name() +"_G" + std::to_string(mGen_count)+ "A1");        
            mDataflow_vector.back()->Action1_Random_swap();            
        }        
    }

    for(int i = 0; i<mDataflow_count; i++)
    {
        if ((rand() % 100) < mMuation_rate * 100 )
        {        
            mDataflow_vector.push_back(std::make_unique<Dataflow>( (*mDataflow_vector[i]) ) );
            mDataflow_vector.back()->Set_dataflow_name(mDataflow_vector[i]->Get_dataflow_name() + "_G" + std::to_string(mGen_count) + "A2");
            mDataflow_vector.back()->Action2_Inc_size_offset();
        }
    }

    for(int i = 0; i<mDataflow_count; i++)
    {
        if ((rand() % 100) < mMuation_rate * 100 )
        {
            mDataflow_vector.push_back(std::make_unique<Dataflow>( (*mDataflow_vector[i]) ) );
            mDataflow_vector.back()->Set_dataflow_name(mDataflow_vector[i]->Get_dataflow_name() + "_G" + std::to_string(mGen_count) + "A3");
            mDataflow_vector.back()->Action3_Dec_size_offset();
        }
    }

    for(int i = 0; i<mDataflow_count; i++)
    {
        if ((rand() % 100) < mMuation_rate * 100 )
        {
            mDataflow_vector.push_back(std::make_unique<Dataflow>( (*mDataflow_vector[i]) ) );
            mDataflow_vector.back()->Set_dataflow_name(mDataflow_vector[i]->Get_dataflow_name() +"_G" + std::to_string(mGen_count) + "A4");
            mDataflow_vector.back()->Action4_changing_spatial_map();
        }
    }    

    for( int i = 0; i < mCrossOver_count; i++)
    {
        if ((rand() % 100) < mMuation_rate * 100 )
        {
            int rand_index0 = rand() % mDataflow_count;
            int rand_index1 = rand() % mDataflow_count;

            while(rand_index0 == rand_index1)
            {
                rand_index0 = rand() % mDataflow_count;
                rand_index1 = rand() % mDataflow_count;
            }

            mDataflow_vector.push_back(std::make_unique<Dataflow>( (*mDataflow_vector[ rand_index0 ]) ) );
            mDataflow_vector.back()->Set_dataflow_name(mDataflow_vector[ rand_index0 ]->Get_dataflow_name() +"_G" + std::to_string(mGen_count)+ "A5");

            mDataflow_vector.push_back(std::make_unique<Dataflow>( (*mDataflow_vector[ rand_index1 ]) ) );
            mDataflow_vector.back()->Set_dataflow_name(mDataflow_vector[ rand_index1 ]->Get_dataflow_name() +"_G" + std::to_string(mGen_count)+ "A5");

            
            mDataflow_vector[mDataflow_vector.size() - 1]->Action5_chaning_dataflow( mDataflow_vector[ mDataflow_vector.size() - 2 ]);

            mDataflow_vector[mDataflow_vector.size() - 1]->Action5_rebuilding_dataflow();
            mDataflow_vector[mDataflow_vector.size() - 2]->Action5_rebuilding_dataflow();
            
        }        
    }

    for(int i = 0; i < mDataflow_vector.size(); i++)
    {
        mDataflow_vector[i]->Print_all();
    }
    
    #if PRINT_INTERMEDIATE_PROCESS
    std::cout<<"Mutataion is done"<<std::endl;
    #endif

}

//Selection    
void Layer_analysis::Selection(void)
{    
    
    #if PRINT_INTERMEDIATE_PROCESS
    std::cout<<"Selection"<<std::endl;
    #endif

    //Quick sort
    std::sort( mDataflow_vector.begin(), mDataflow_vector.end(),
        [this](const std::unique_ptr<Dataflow>& lhs, const std::unique_ptr<Dataflow>& rhs) {return lhs->Get_evaluation_value(mEvaluation_criteria) < rhs->Get_evaluation_value(mEvaluation_criteria); } );

    while(mDataflow_vector.size() != mDataflow_count)
        mDataflow_vector.pop_back();

/*
    for(int i = 0; i < mDataflow_vector.size(); i++)
    {
        mDataflow_vector[i]->Print_all();
    }
*/
    Packing_and_Judgement();

    for(int i = 0; i < mDataflow_vector.size(); i++)
    {
        mDataflow_vector[i]->Print_all();
    }

    if(mGen_count == mGen_limit)
    {    
        //for(int i = 0; i < mDataflow_vector.size(); i++)            
        //    mDataflow_vector[i]->Print_all_result();            
        mDataflow_vector[0]->Print_all_result();
    }

    #if PRINT_INTERMEDIATE_PROCESS
    std::cout<<"Selection is done"<<std::endl;    
    #endif
}

void Layer_analysis::Packing_and_Judgement(void)
{
    #if PRINT_INTERMEDIATE_PROCESS
        std::cout<<"Pack and Judge"<<std::endl;
    #endif

    double same_evaluation_ratio;
    double best_evaluation_value = 0.0;
    double same_evaluation_count = 1.0;

    best_evaluation_value = mDataflow_vector[0]->Get_evaluation_value(mEvaluation_criteria);

    if(best_evaluation_value == 0.0)
    {
        std::cerr<<"Packing and judging malfunction"<<std::endl;
        exit(EXIT_FAILURE);
    }

    //Counting same evaluation value
    for(int i = 1; i < mDataflow_vector.size(); i++)
    {
        if(best_evaluation_value == mDataflow_vector[i]->Get_evaluation_value(mEvaluation_criteria))
            same_evaluation_count = same_evaluation_count + 1.0;
    }

    same_evaluation_ratio = same_evaluation_count / static_cast<float>(mDataflow_count);

    if(same_evaluation_ratio > mPacking_ratio && (rand() % 100 < 100 * mPacking_prob) )
    {
        #if PRINT_INTERMEDIATE_PROCESS
            std::cout<<"Packing"<<std::endl;
        #endif
        /*
        std::cout<<"Same evluation count :"<<same_evaluation_count<<std::endl;
        std::cout<<"mDataflow count :"<<mDataflow_count<<std::endl;
        std::cout<<"Same evluation ratio :"<<same_evaluation_ratio<<std::endl;
        std::cout<<"Packing ratio"<<mPacking_ratio<<std::endl;
        */

        //for(int i = same_evaluation_count; i < mDataflow_vector.size(); i ++)
        for(int i = 1; i < mDataflow_vector.size(); i ++)
        {
            mDataflow_vector[i]->Resetting_dataflow();
        }

    }
    else if ((rand() % 100) < mJudgementDay_prob * 100 )
    {
        #if PRINT_INTERMEDIATE_PROCESS
            std::cout<<"Judgement"<<std::endl;
        #endif
        
        for(int i = 1; i < mDataflow_vector.size(); i++)
        {
            mDataflow_vector[i]->Resetting_dataflow();
        }
 
    }
    
    
}


