//20.12.09
#include <chrono>

#include "Layer_analysis.hpp"
#include "HW.hpp"
#include "Layer_info.hpp"


void layer_analysis(const std::string& dataflow_name, const int& dataflow_count, const int& Generation_limit, std::string evaluation_criteria ,const std::shared_ptr<HW>& hardware_config, const std::shared_ptr<Layer_info>& layer_information, const double Muation_rate ,const double Packing_prob, const double Packing_ratio, const double JudgementDay_prob, const int CrossOver_count);
/****************************************************************************************
parameter explanation
dataflow name is name of the each layer dtaflow
dataflow count is how many dataflow is generated at one mutation - selection
generation limit is limit of generation
evaluation criteria are
Cycles, Energy, Throughput, Area, Power, L2_size, L1_size
HW is hardware
layer information is information about layer info example channel size, filter size...
*******************************************************************************************/
//Layer_info(std::string Layer_config, std::string Layer_type, int stride_x, int stride_y, int K, int C, int R, int S, int Y, int X)
void Making_network(std::list<std::shared_ptr<Layer_info>>& Network, std::string Network_name);
void Network_analysis_inference(const std::list<std::shared_ptr<Layer_info>>& Network,const std::shared_ptr<HW>& mHW, std::string Network_name, std::string evaluation_criteria, const double Packing_prob, const double Packing_ratio, const double JudgementDay_prob, const int CrossOver_count, const double Muation_rate);
//void layer_analysis(const std::string& dataflow_name, const int& dataflow_count, const int& Generation_limit, std::string evaluation_criteria , const std::shared_ptr<HW>& hardware_config, const std::shared_ptr<Layer_info>& layer_information)

int main(int argc, char* argv[])
{
    //Program execution time check
    
    auto start = std::chrono::steady_clock::now();

    
    //Analyzing generation by generation
    //Network.push_back(std::make_shared<Layer_info>("CONV4_1_2","CONV",1,1,256,256,3,3,28,28));
    
    const double Mutation_rate = 0.1;
    const double Packing_prob = 0.6;
    const double Packing_ratio = 0.6;
    const double JudgementDay_prob = 0.4;
    const int CrossOver_count = 1;

    /*
    std::shared_ptr<HW> mHW = std::make_shared<HW>("Target_hardware");
    mHW->making_hw_file();
    std::shared_ptr<Layer_info> mLayer_info = std::make_shared<Layer_info>("Bottleneck2_1_2","DSCONV",2,2,1,96,3,3,112,112);

    const int gen_count = 3;
    const int generation_limit = 10;

    Layer_analysis conv1( "Test" , gen_count, generation_limit ,"Cycles", mHW, mLayer_info, Mutation_rate, Packing_prob, Packing_ratio, JudgementDay_prob, CrossOver_count);    
    
    conv1.Initialization();
    conv1.Evaluation();
    
    
    for(int i = 0; i < generation_limit; i++)
    {
        conv1.Mutation();
        conv1.Evaluation();
        conv1.Selection();
    }    
    */

    
    //std::vector<std::string> Network_name_vector = {"Resnet50_github_version"};
    
    std::vector<std::string> Network_name_vector = {"generation_curve"};

    std::shared_ptr<HW> mHW = std::make_shared<HW>("Target_hardware");
    mHW->making_hw_file();

    std::list<std::shared_ptr<Layer_info>> Network;


    for(auto i = Network_name_vector.begin(); i != Network_name_vector.end(); i++)
    {
        Making_network(Network, *i);
        std::cout<<"Processing Network : "<<(*i)<<std::endl;        
        Network_analysis_inference(Network, mHW, *i, "Cycles" , 0.6, 0.6, 0.4, CrossOver_count, Mutation_rate);        
        Network.clear();
    }    
    
    //Program execution time check
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout<<"prorgram execution time : ";
    std:: cout<<std::chrono::duration <double,std::milli>(diff).count()<<" ms"<<std::endl;
    
    return 0;
}

void Network_analysis_inference(const std::list<std::shared_ptr<Layer_info>>& Network,const std::shared_ptr<HW>& mHW, 
std::string Network_name, std::string evaluation_criteria, const double Packing_prob, const double Packing_ratio, const double JudgementDay_prob, const int CrossOver_count, const double Muation_rate)
{

    int mLayer_count = 0; 
    for(auto iter = Network.begin(); iter != Network.end(); iter++)
    {
        mLayer_count++;
        //std::string dataflow_name = std::to_string(mLayer_count) + Network_name;
        std::string datafflow_name = std::to_string(mLayer_count) + "_" + Network_name + "_" + (*iter)->Get_layer_config() + "_";
        layer_analysis(datafflow_name, 10, 20, evaluation_criteria ,mHW, (*iter), Muation_rate,Packing_prob, Packing_ratio, JudgementDay_prob, CrossOver_count);
    }


}


void layer_analysis(const std::string& dataflow_name, const int& dataflow_count, const int& Generation_limit, std::string evaluation_criteria , const std::shared_ptr<HW>& hardware_config, const std::shared_ptr<Layer_info>& layer_information, const double Muation_rate ,const double Packing_prob, const double Packing_ratio, const double JudgementDay_prob, const int CrossOver_count)
{
    
    Layer_analysis conv1( dataflow_name , dataflow_count, Generation_limit ,evaluation_criteria, hardware_config, layer_information, Muation_rate, Packing_prob, Packing_ratio, JudgementDay_prob, CrossOver_count );    
    
    conv1.Initialization();
    conv1.Evaluation();
    
    
    for(int i = 0; i < Generation_limit; i++)
    {
        conv1.Mutation();
        conv1.Evaluation();
        conv1.Selection();
    }
    
}



void Making_network(std::list<std::shared_ptr<Layer_info>>& Network, std::string Network_name)
{

    if(Network_name == "VGG16") //Verified
    {
        Network.push_back(std::make_shared<Layer_info>("CONV1","CONV",1,1,64,3,3,3,224,224));    
        Network.push_back(std::make_shared<Layer_info>("CONV2","CONV",1,1,64,64,3,3,224,224));
        Network.push_back(std::make_shared<Layer_info>("CONV3","CONV",1,1,128,64,3,3,112,112));
        Network.push_back(std::make_shared<Layer_info>("CONV4","CONV",1,1,128,128,3,3,112,112));        
        Network.push_back(std::make_shared<Layer_info>("CONV5","CONV",1,1,256,128,3,3,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV6","CONV",1,1,256,256,3,3,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV7","CONV",1,1,256,256,3,3,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV8","CONV",1,1,512,256,3,3,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV9","CONV",1,1,512,512,3,3,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV10","CONV",1,1,512,512,3,3,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV11","CONV",1,1,512,512,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV12","CONV",1,1,512,512,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV13","CONV",1,1,512,512,3,3,14,14));
        
    }
    else if (Network_name == "resnet50")
    {
        //This version is maestro github version
        Network.push_back(std::make_shared<Layer_info>("CONV1","CONV",2,2,64,3,7,7,224,224));    

        //CONV2_x
        Network.push_back(std::make_shared<Layer_info>("CONV2_1_1","CONV",1,1,64,64,1,1,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV2_1_2","CONV",1,1,64,64,3,3,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV2_1_3","CONV",1,1,256,64,1,1,56,56));        
        Network.push_back(std::make_shared<Layer_info>("CONV2_1_residual","DSCONV",1,1,1,256,1,1,56,56));

        Network.push_back(std::make_shared<Layer_info>("CONV2_2_1","CONV",1,1,64,256,1,1,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV2_2_2","CONV",1,1,64,64,3,3,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV2_2_3","CONV",1,1,256,64,1,1,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV2_2_residual","DSCONV",1,1,1,256,1,1,56,56));

        Network.push_back(std::make_shared<Layer_info>("CONV2_3_1","CONV",1,1,64,256,1,1,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV2_3_2","CONV",1,1,64,64,3,3,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV2_3_3","CONV",1,1,256,64,1,1,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV2_3_residual","DSCONV",1,1,1,256,1,1,56,56));

        

        //CONV3_x
        Network.push_back(std::make_shared<Layer_info>("CONV3_1_1","CONV",2,2,128,256,1,1,56,56));
        Network.push_back(std::make_shared<Layer_info>("CONV3_1_2","CONV",1,1,128,128,3,3,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV3_1_3","CONV",1,1,512,128,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV3_1_residual","DSCONV",1,1,1,512,1,1,28,28));

        Network.push_back(std::make_shared<Layer_info>("CONV3_2_1","CONV",1,1,128,256,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV3_2_2","CONV",1,1,128,128,3,3,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV3_2_3","CONV",1,1,512,128,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV3_2_residual","DSCONV",1,1,1,512,1,1,28,28));

        Network.push_back(std::make_shared<Layer_info>("CONV3_3_1","CONV",1,1,128,256,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV3_3_2","CONV",1,1,128,128,3,3,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV3_3_3","CONV",1,1,512,128,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV3_3_residual","DSCONV",1,1,1,512,1,1,28,28));

        Network.push_back(std::make_shared<Layer_info>("CONV3_4_1","CONV",1,1,128,256,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV3_4_2","CONV",1,1,128,128,3,3,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV3_4_3","CONV",1,1,512,128,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV3_4_residual","DSCONV",1,1,1,512,1,1,28,28));

        
        
        //CONV4_x
        
        Network.push_back(std::make_shared<Layer_info>("CONV4_1_1","CONV",2,2,256,512,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("CONV4_1_2","CONV",1,1,256,256,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_1_3","CONV",1,1,1024,256,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_1_residual","DSCONV",1,1,1,1024,1,1,14,14));     

        Network.push_back(std::make_shared<Layer_info>("CONV4_2_1","CONV",1,1,256,1024,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_2_2","CONV",1,1,256,256,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_2_3","CONV",1,1,1024,256,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_2_residual","DSCONV",1,1,1,1024,1,1,14,14));     

        Network.push_back(std::make_shared<Layer_info>("CONV4_3_1","CONV",1,1,256,1024,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_3_2","CONV",1,1,256,256,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_3_3","CONV",1,1,1024,256,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_3_residual","DSCONV",1,1,1,1024,1,1,14,14));     

        Network.push_back(std::make_shared<Layer_info>("CONV4_4_1","CONV",1,1,256,1024,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_4_2","CONV",1,1,256,256,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_4_3","CONV",1,1,1024,256,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_4_residual","DSCONV",1,1,1,1024,1,1,14,14));     

        Network.push_back(std::make_shared<Layer_info>("CONV4_5_1","CONV",1,1,256,1024,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_5_2","CONV",1,1,256,256,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_5_3","CONV",1,1,1024,256,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_5_residual","DSCONV",1,1,1,1024,1,1,14,14));     

        Network.push_back(std::make_shared<Layer_info>("CONV4_6_1","CONV",1,1,256,1024,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_6_2","CONV",1,1,256,256,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_6_3","CONV",1,1,1024,256,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV4_6_residual","DSCONV",1,1,1,1024,1,1,14,14));     


        //CONV5_x
        Network.push_back(std::make_shared<Layer_info>("CONV5_1_1","CONV",2,2,512,1024,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("CONV5_1_2","CONV",1,1,512,512,3,3,7,7));
        Network.push_back(std::make_shared<Layer_info>("CONV5_1_3","CONV",1,1,2048,512,1,1,7,7));
        Network.push_back(std::make_shared<Layer_info>("CONV5_1_residual","DSCONV",1,1,1,2048,1,1,7,7));

        Network.push_back(std::make_shared<Layer_info>("CONV5_2_1","CONV",1,1,512,2048,1,1,7,7));
        Network.push_back(std::make_shared<Layer_info>("CONV5_2_2","CONV",1,1,512,512,3,3,7,7));
        Network.push_back(std::make_shared<Layer_info>("CONV5_2_3","CONV",1,1,2048,512,1,1,7,7));
        Network.push_back(std::make_shared<Layer_info>("CONV5_2_residual","DSCONV",1,1,1,2048,1,1,7,7));

        Network.push_back(std::make_shared<Layer_info>("CONV5_3_1","CONV",1,1,512,2048,1,1,7,7));
        Network.push_back(std::make_shared<Layer_info>("CONV5_3_2","CONV",1,1,512,512,3,3,7,7));
        Network.push_back(std::make_shared<Layer_info>("CONV5_3_3","CONV",1,1,2048,512,1,1,7,7));
        Network.push_back(std::make_shared<Layer_info>("CONV5_3_residual","DSCONV",1,1,1,2048,1,1,7,7));

        //FC
        //Network.push_back(std::make_shared<Layer_info>("FC1000","CONV",1,1,1000,2048,7,7,7,7));           
    }
    else if(Network_name == "mbnet")
    {
        //CONV1
        Network.push_back(std::make_shared<Layer_info>("CONV1","CONV",2,2,32,3,1,1,224,224));

        //Bottleneck 1
        Network.push_back(std::make_shared<Layer_info>("Bottleneck1_1_1","CONV",1,1,32,32,1,1,112,112));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck1_1_2","DSCONV",1,1,1,32,3,3,112,112));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck1_1_3","CONV",1,1,16,32,1,1,112,112));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck1_1_Residual","DSCONV",1,1,1,16,1,1,112,112));

        //Bottleneck 2
        Network.push_back(std::make_shared<Layer_info>("Bottleneck2_1_1","CONV",1,1,96,16,1,1,112,112));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck2_1_2","DSCONV",2,2,1,96,3,3,112,112));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck2_1_3","CONV",1,1,96,96,1,1,56,56));

        Network.push_back(std::make_shared<Layer_info>("Bottleneck2_2_1","CONV",1,1,96,96,1,1,56,56));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck2_2_2","DSCONV",1,1,1,96,3,3,56,56));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck2_2_3","CONV",1,1,24,96,1,1,56,56));

        //Bottleneck 3
        Network.push_back(std::make_shared<Layer_info>("Bottleneck3_1_1","CONV",1,1,144,24,1,1,56,56));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck3_1_2","DSCONV",2,2,1,144,3,3,56,56));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck3_1_3","CONV",1,1,144,144,1,1,28,28));

        Network.push_back(std::make_shared<Layer_info>("Bottleneck3_2_1","CONV",1,1,144,144,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck3_2_2","DSCONV",1,1,1,144,3,3,28,28));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck3_2_3","CONV",1,1,144,144,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck3_2_Residual","DSCONV",1,1,1,144,1,1,28,28));

        Network.push_back(std::make_shared<Layer_info>("Bottleneck3_3_1","CONV",1,1,144,144,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck3_3_2","DSCONV",1,1,1,144,3,3,28,28));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck3_3_3","CONV",1,1,32,144,1,1,28,28));        

        //Bottleneck 4
        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_1_1","CONV",1,1,192,32,1,1,28,28));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_1_2","DSCONV",2,2,1,192,3,3,28,28));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_1_3","CONV",1,1,192,192,1,1,14,14));

        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_2_1","CONV",1,1,192,192,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_2_2","DSCONV",1,1,1,192,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_2_3","CONV",1,1,192,192,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_2_Residual","CONV",1,1,1,192,1,1,14,14));

        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_3_1","CONV",1,1,192,192,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_3_2","CONV",1,1,1,192,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_3_3","CONV",1,1,192,192,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_3_Residual","CONV",1,1,1,192,1,1,14,14));

        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_4_1","CONV",1,1,192,192,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_4_2","DSCONV",1,1,1,192,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck4_4_3","CONV",1,1,64,192,1,1,14,14));

        //Bottleneck 5
        Network.push_back(std::make_shared<Layer_info>("Bottleneck5_1_1","CONV",1,1,384,64,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck5_1_2","DSCONV",1,1,1,384,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck5_1_3","CONV",1,1,384,384,1,1,14,14));

        Network.push_back(std::make_shared<Layer_info>("Bottleneck5_2_1","CONV",1,1,384,384,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck5_2_2","DSCONV",1,1,1,384,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck5_2_3","CONV",1,1,96,384,1,1,14,14));

        //Bottleneck 6
        Network.push_back(std::make_shared<Layer_info>("Bottleneck6_1_1","CONV",1,1,576,96,1,1,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck6_1_2","DSCONV",2,2,1,576,3,3,14,14));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck6_1_3","CONV",1,1,576,576,1,1,7,7));

        Network.push_back(std::make_shared<Layer_info>("Bottleneck6_2_1","CONV",1,1,576,576,1,1,7,7));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck6_2_2","DSCONV",1,1,1,576,3,3,7,7));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck6_2_3","CONV",1,1,576,576,1,1,7,7));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck6_2_Residual","CONV",1,1,1,576,1,1,7,7));

        Network.push_back(std::make_shared<Layer_info>("Bottleneck6_3_1","CONV",1,1,576,576,1,1,7,7));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck6_3_2","DSCONV",1,1,1,576,3,3,7,7));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck6_3_3","CONV",1,1,160,576,1,1,7,7));

        //Bottleneck 7
        Network.push_back(std::make_shared<Layer_info>("Bottleneck7_1_1","CONV",1,1,960,160,1,1,7,7));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck7_1_2","DSCONV",1,1,1,960,3,3,7,7));
        Network.push_back(std::make_shared<Layer_info>("Bottleneck7_1_3","CONV",1,1,320,960,1,1,7,7));

        //CONV2D
        Network.push_back(std::make_shared<Layer_info>("CONV2D_2","CONV",1,1,1280,320,1,1,7,7));
        Network.push_back(std::make_shared<Layer_info>("CONV2D_3","CONV",1,1,1000,1280,1,1,1,1));        


    }    
    else if(Network_name == "generation_curve")
    {
        
        //Network.push_back(std::make_shared<Layer_info>("CONV2_3_1","CONV",1,1,64,256,1,1,56,56));        
        Network.push_back(std::make_shared<Layer_info>("CONV3_4_1","CONV",1,1,128,256,1,1,28,28));
        //Network.push_back(std::make_shared<Layer_info>("CONV4_6_2","CONV",1,1,256,256,3,3,14,14));
        
    }
    else if (Network_name == "Extra_mbnet")
    {
        Network.push_back(std::make_shared<Layer_info>("CONV2D_3","CONV",1,1,1000,1280,1,1,1,1));   
    }
    else if (Network_name == "previous_NAS")
    {
        Network.push_back(std::make_shared<Layer_info>("D12211","DSCONV",1,1,1,16,3,3,32,32));
        Network.push_back(std::make_shared<Layer_info>("C12111","CONV",1,1,16,16,1,1,32,32));
        Network.push_back(std::make_shared<Layer_info>("D12311","DSCONV",1,1,1,16,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("C11111","CONV",1,1,8,8,1,1,32,32));
        Network.push_back(std::make_shared<Layer_info>("D13221","DSCONV",1,1,1,32,3,3,16,16));

        Network.push_back(std::make_shared<Layer_info>("C13121","CONV",1,1,32,32,1,1,16,16));
        Network.push_back(std::make_shared<Layer_info>("D13321","DSCONV",1,1,1,32,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("C12121","CONV",1,1,16,16,1,1,16,16));
        Network.push_back(std::make_shared<Layer_info>("D14231","DSCONV",1,1,1,64,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("C14131","CONV",1,1,64,64,1,1,8,8));

        Network.push_back(std::make_shared<Layer_info>("D14331","DSCONV",1,1,1,64,5,5,8,8));   
        Network.push_back(std::make_shared<Layer_info>("C13131","CONV",1,1,32,32,1,1,8,8));   
        Network.push_back(std::make_shared<Layer_info>("D22211","DSCONV",2,2,1,16,3,3,32,32));   
        Network.push_back(std::make_shared<Layer_info>("C22111","CONV",2,2,16,16,1,1,32,32));   
        Network.push_back(std::make_shared<Layer_info>("D22311","DSCONV",2,2,1,16,5,5,32,32));  

        Network.push_back(std::make_shared<Layer_info>("C21111","CONV",2,2,8,8,1,1,32,32));   
        Network.push_back(std::make_shared<Layer_info>("C21211","CONV",2,2,8,8,3,3,32,32));   
        Network.push_back(std::make_shared<Layer_info>("C21311","CONV",2,2,8,8,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("D23221","DSCONV",2,2,1,32,3,3,16,16));
        Network.push_back(std::make_shared<Layer_info>("C23121","CONV",2,2,32,32,1,1,16,16));

        Network.push_back(std::make_shared<Layer_info>("D23321","DSCONV",2,2,1,32,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("C22121","CONV",2,2,16,16,1,1,16,16));
        Network.push_back(std::make_shared<Layer_info>("C22221","CONV",2,2,16,16,3,3,16,16));
        Network.push_back(std::make_shared<Layer_info>("C22321","CONV",2,2,16,16,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("D24231","DSCONV",2,2,1,64,3,3,8,8));

        Network.push_back(std::make_shared<Layer_info>("C24131","CONV",2,2,64,64,1,1,8,8));
        Network.push_back(std::make_shared<Layer_info>("D24331","DSCONV",2,2,1,64,5,5,8,8));
        Network.push_back(std::make_shared<Layer_info>("C23131","CONV",2,2,32,32,1,1,8,8));
        Network.push_back(std::make_shared<Layer_info>("C23231","CONV",2,2,32,32,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("C23331","CONV",2,2,32,32,5,5,8,8));


        
    }    
    else if (Network_name == "NAS1")
    {
        Network.push_back(std::make_shared<Layer_info>("AAAAAA","CONV",1,1,1,1,3,3,32,32));
        Network.push_back(std::make_shared<Layer_info>("AABBAB","CONV",1,1,16,16,1,1,32,32));
        Network.push_back(std::make_shared<Layer_info>("AAACAA","CONV",1,1,1,1,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("AAAABA","CONV",1,1,1,1,3,3,16,16));
        Network.push_back(std::make_shared<Layer_info>("AACBBB","CONV",1,1,32,32,1,1,16,16));

        Network.push_back(std::make_shared<Layer_info>("AAACBA","CONV",1,1,1,1,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("AAAACA","CONV",1,1,1,1,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("AADBCB","CONV",1,1,64,64,1,1,8,8));
        Network.push_back(std::make_shared<Layer_info>("AAACCA","CONV",1,1,1,1,5,5,8,8));
        Network.push_back(std::make_shared<Layer_info>("ABAAAA","CONV",2,2,1,1,3,3,32,32));

        Network.push_back(std::make_shared<Layer_info>("ABACAA","CONV",2,2,1,1,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("ABAABA","CONV",2,2,1,1,3,3,16,16));
        Network.push_back(std::make_shared<Layer_info>("ABACBA","CONV",2,2,1,1,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("ABAACA","CONV",2,2,1,1,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("ABACCA","CONV",2,2,1,1,5,5,8,8));

        Network.push_back(std::make_shared<Layer_info>("AAEBAB","CONV",1,1,8,8,1,1,32,32));
        Network.push_back(std::make_shared<Layer_info>("AAAAAC","CONV",1,1,1,1,3,3,32,32));
        Network.push_back(std::make_shared<Layer_info>("AAACAC","CONV",1,1,1,1,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("AABBBB","CONV",1,1,16,16,1,1,16,16));
        Network.push_back(std::make_shared<Layer_info>("AAAABC","CONV",1,1,1,1,3,3,16,16));

        Network.push_back(std::make_shared<Layer_info>("AAACBC","CONV",1,1,1,1,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("AACBCB","CONV",1,1,32,32,1,1,8,8));
        Network.push_back(std::make_shared<Layer_info>("AAAACC","CONV",1,1,1,1,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("AAACCC","CONV",1,1,1,1,5,5,8,8));
        Network.push_back(std::make_shared<Layer_info>("ABAAAC","CONV",2,2,1,1,3,3,32,32));

        Network.push_back(std::make_shared<Layer_info>("ABEAAB","CONV",2,2,8,8,3,3,32,32));
        Network.push_back(std::make_shared<Layer_info>("ABACAC","CONV",2,2,1,1,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("ABECAB","CONV",2,2,8,8,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("ABAABC","CONV",2,2,1,1,3,3,16,16));
        Network.push_back(std::make_shared<Layer_info>("ABBABB","CONV",2,2,16,16,3,3,16,16));

        Network.push_back(std::make_shared<Layer_info>("ABACBC","CONV",2,2,1,1,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("ABBCBB","CONV",2,2,16,16,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("ABAACC","CONV",2,2,1,1,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("ABCACB","CONV",2,2,32,32,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("ABACCC","CONV",2,2,1,1,5,5,8,8));

        Network.push_back(std::make_shared<Layer_info>("ABCCCB","CONV",2,2,32,32,5,5,8,8));
        
    }  
    else if (Network_name == "NAS2")
    {
        Network.push_back(std::make_shared<Layer_info>("AAAAAA","DSCONV",1,1,1,16,3,3,32,32));
        Network.push_back(std::make_shared<Layer_info>("BAABAA","CONV",1,1,16,16,1,1,32,32));
        Network.push_back(std::make_shared<Layer_info>("AAACAA","DSCONV",1,1,1,16,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("AABABA","DSCONV",1,1,1,32,3,3,16,16));
        Network.push_back(std::make_shared<Layer_info>("BABBBA","CONV",1,1,32,32,1,1,16,16));

        Network.push_back(std::make_shared<Layer_info>("AABCBA","DSCONV",1,1,1,32,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("AACACA","DSCONV",1,1,1,64,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("BACBCA","CONV",1,1,64,64,1,1,8,8));
        Network.push_back(std::make_shared<Layer_info>("AACCCA","DSCONV",1,1,1,64,5,5,8,8));
        Network.push_back(std::make_shared<Layer_info>("ABAAAA","CONV",2,2,1,16,3,3,32,32));

        Network.push_back(std::make_shared<Layer_info>("ABACAA","DSCONV",2,2,1,16,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("ABBABA","DSCONV",2,2,1,32,3,3,16,16));
        Network.push_back(std::make_shared<Layer_info>("ABBCBA","DSCONV",2,2,1,32,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("ABCACA","DSCONV",2,2,1,64,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("ABCCCA","DSCONV",2,2,1,64,5,5,8,8));

        Network.push_back(std::make_shared<Layer_info>("BADBAA","CONV",1,1,8,8,1,1,32,32));
        Network.push_back(std::make_shared<Layer_info>("AADAAA","DSCONV",1,1,1,8,3,3,32,32));
        Network.push_back(std::make_shared<Layer_info>("AADCAA","DSCONV",1,1,1,8,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("BAABBA","CONV",1,1,16,16,1,1,16,16));
        Network.push_back(std::make_shared<Layer_info>("AAAABA","DSCONV",1,1,1,16,3,3,16,16));

        Network.push_back(std::make_shared<Layer_info>("AAACBA","DSCONV",1,1,1,16,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("BABBCA","CONV",1,1,32,32,1,1,8,8));
        Network.push_back(std::make_shared<Layer_info>("AABACA","DSCONV",1,1,1,32,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("AABCCA","DSCONV",1,1,1,32,5,5,8,8));
        Network.push_back(std::make_shared<Layer_info>("ABDAAA","DSCONV",2,2,1,8,3,3,32,32));

        Network.push_back(std::make_shared<Layer_info>("BBDAAA","CONV",2,2,8,8,3,3,32,32));
        Network.push_back(std::make_shared<Layer_info>("ABDCAA","DSCONV",2,2,1,8,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("BBDCAA","CONV",2,2,8,8,5,5,32,32));
        Network.push_back(std::make_shared<Layer_info>("ABAABA","DSCONV",2,2,1,16,3,3,16,16));
        Network.push_back(std::make_shared<Layer_info>("BBAABA","CONV",2,2,16,16,3,3,16,16));

        Network.push_back(std::make_shared<Layer_info>("ABACBA","DSCONV",2,2,1,16,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("BBACBA","CONV",2,2,16,16,5,5,16,16));
        Network.push_back(std::make_shared<Layer_info>("ABBACA","DSCONV",2,2,1,32,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("BBBACA","CONV",2,2,32,32,3,3,8,8));
        Network.push_back(std::make_shared<Layer_info>("ABBCCA","DSCONV",2,2,1,32,5,5,8,8));

        Network.push_back(std::make_shared<Layer_info>("BBBCCA","CONV",2,2,32,32,5,5,8,8));
        
    }              
    else
    {
        std::cerr<<"[MAIN] Making network ERROR"<<std::endl;
        exit(EXIT_FAILURE);
    }


}