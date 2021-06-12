//20.08.25
//Default constructor fix

#pragma once
#include <string>

class HW final {
public:
    //Default constructor
    HW()
        :mBandwidth(32)
        ,mNoC_hops(1)
        ,mNoc_hops_latency(1)
        ,mL1_size(256)
        ,mL2_size(2048)
        ,mNumber_of_PEs(256)
    {

    };
    //Constructor
    HW(std::string HW_type)
    {
        if(HW_type == "Small")
        {
            mBandwidth = 16;
            mNoC_hops = 1;
            mNoc_hops_latency = 1;
            mL1_size = 128;
            mL2_size = 1024;
            mNumber_of_PEs = 128;
        }
        else if(HW_type == "Middle")
        {
            mBandwidth = 32;
            mNoC_hops = 1;
            mNoc_hops_latency = 1;
            mL1_size = 256;
            mL2_size = 2048;
            mNumber_of_PEs = 256;
        }
        else if (HW_type =="Big")
        {
            mBandwidth = 64;
            mNoC_hops = 1;
            mNoc_hops_latency = 1;
            mL1_size = 512;
            mL2_size = 4096;
            mNumber_of_PEs = 512;            
        }
        else if(HW_type =="Target_hardware")
        {
            mBandwidth = 1024;
            mNoC_hops = 1;
            mNoc_hops_latency = 1;
            mL1_size = 512;
            mL2_size = 108000;
            mNumber_of_PEs = 168;            
        }     
        else if(HW_type == "Edge")
        {
            mBandwidth = 64;
            mNoC_hops = 1;
            mNoc_hops_latency = 1;
            mL1_size = 512;
            mL2_size = 108000;
            mNumber_of_PEs = 168;            
        }
        else if(HW_type == "Cloud")
        {
            mBandwidth = 64;
            mNoC_hops = 1;
            mNoc_hops_latency = 1;
            mL1_size = 8192;
            mL2_size = 65536;
            mNumber_of_PEs = 512;            
        }          
        else
        {
            std::cerr<<"HW constructor malfunction"<<std::endl;
            exit(EXIT_FAILURE);
        }            
        
    }
    //Deconstructor
    ~HW() = default;
    //Dataflow(const Dataflow&) = delete;
    HW& operator=(const HW& ) = delete;//Deleting implied function

    int Get_bandwidth(void) { return mBandwidth; }
    int Get_Noc_hops(void) { return mNoC_hops; }
    int Get_hops_latency(void) { return mNoc_hops_latency; }
    int Get_l1_size(void) { return mL1_size; }
    int Get_l2_size(void) { return mL2_size; }
    int Get_number_of_PES(void) {return mNumber_of_PEs;}

    void print_all(void) const
    {
        std::cout<<"Bandwidth : "<<mBandwidth<<std::endl;
        std::cout<<"mNoC_hops : "<<mNoC_hops<<std::endl;
        std::cout<<"mNoc_hops_latency : "<<mNoc_hops_latency<<std::endl;
        std::cout<<"mL1_size : "<<mL1_size<<std::endl;
        std::cout<<"mL2_size : "<<mL2_size<<std::endl;
        std::cout<<"mNumber_of_PEs : "<<mNumber_of_PEs<<std::endl;
    }
    
    void making_hw_file(void) const
    {
        std::ofstream HW;    
        HW.open("../maestro/data/hw/my_HW.m");
        if(!HW.is_open())
        {
            std::cerr<<"[INITIALIZATION] Hardware file open is failed"<<std::endl;            
            exit(EXIT_FAILURE);
        }

        HW<<"NumPEs: "<<mNumber_of_PEs<<std::endl;
        HW<<"L1Size: "<<mL1_size<<std::endl;
        HW<<"L2Size: "<<mL2_size<<std::endl;
        HW<<"NoC_BW: "<<mBandwidth<<std::endl;
        
        HW.close();

    }
    
private:
    int mBandwidth;
    int mNoC_hops;
    int mNoc_hops_latency;
    int mL1_size;
    int mL2_size;
    int mNumber_of_PEs;
};
