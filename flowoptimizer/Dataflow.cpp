#include "Dataflow.hpp"

int Dataflow::mPopulation_count = 0;

Dataflow::Dataflow(std::string Dataflow_name, const std::shared_ptr<Layer_info>& mLayer_info_ptr, const std::shared_ptr<HW>& mHW_ptr)
    :mOriginal_name(Dataflow_name)
    ,mDataflow_name(Dataflow_name + std::to_string(mPopulation_count))
    ,mHW_ptr(mHW_ptr)
    ,mLayer_info_ptr(mLayer_info_ptr)
    ,mEvaluation_cycles(0.0)
    ,mEvaluation_activity_count_based_energy(0.0)
    ,mEvaluation_throughput(0.0)
    ,mEvaluation_area(0)
    ,mEvaluation_power(0.0)
    ,mEvaluation_L2(0)
    ,mEvaluation_L1(0)
    //,mX_basis(Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() +1 ))
    //,mY_basis(Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() +1 ))
{

    switch(mLayer_info_ptr->Get_stride_x())
    {
        case 1:
            //std::cout<< mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() +1 <<std::endl;
            mX_basis = Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() +1 );
            break;
        
        case 2:
            mX_basis = Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S()) * 2;
            while(mX_basis == 1 || mX_basis > mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 2)
                mX_basis = Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 2 ) * 2;
            break;

        default:
            std::cerr<<"Dataflow constructor Switch Malfuncion"<<std::endl;
            exit(EXIT_FAILURE);
            break;        
    }

    
    switch(mLayer_info_ptr->Get_stride_y())
    {
        case 1:
            mY_basis = Random_number(mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() +1 );
            break;
        
        case 2:
            mY_basis = Random_number(mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() +1 ) *2;
            while(mY_basis == 1 || mY_basis > mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 2 )
                mY_basis = Random_number(mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 2 ) * 2;
            break;

        default:
            std::cerr<<"Dataflow constructor Switch Malfuncion"<<std::endl;
            exit(EXIT_FAILURE);
            break;        
    }    

}

Dataflow::Dataflow(const Dataflow& Another)
    :mOriginal_name(Another.mOriginal_name)
    ,mDataflow_name("")
    ,mHW_ptr(Another.mHW_ptr)
    ,mLayer_info_ptr(Another.mLayer_info_ptr)
    ,mEvaluation_cycles(0.0)
    ,mEvaluation_activity_count_based_energy(0.0)
    ,mEvaluation_throughput(0.0)
    ,mEvaluation_area(0)
    ,mEvaluation_power(0.0)
    ,mEvaluation_L2(0)
    ,mEvaluation_L1(0)
    ,mX_basis(Another.mX_basis)
    ,mY_basis(Another.mY_basis)
    //Things about cpoying dataflow vector will be here
{

    //Copy directives
    for(auto it = Another.mDirective_sequence.begin(); it != Another.mDirective_sequence.end(); it++ )
    {
        mDirective_sequence.push_back(std::make_unique<Directive>(**it));
    }


}


Dataflow::~Dataflow()
{
    #if PRINT_INTERMEDIATE_PROCESS 
    //std::cout<<mDataflow_name<<" is shut down"<<std::endl;
    #endif

}

//Will be delete after soon
void Dataflow::FC_init(void)
{
    mPopulation_count++;
    std::cout<<"Exception occurs"<<std::endl;
    exit (EXIT_FAILURE);
}

void Dataflow::Conv_init(void)
{
    //std::cout<<"Conv init"<<std::endl;
    mPopulation_count++;

    Dimension_type R2_spatialmap_dimension = Dimension_type::None;

    Set_dimension_vector();

    std::random_shuffle(mDimension_vector.begin(), mDimension_vector.end() );

    //std::cout<<"Upper region"<<std::endl;
    while(!mDimension_vector.empty())
    {            
        Dimension_type dimension = Dimension_type::None;
        Mapping_type mapping = Mapping_type::None;
        int size = 0;
        int offset = 0;

        dimension = mDimension_vector.back();
        mDimension_vector.pop_back(); 
        //std::unordered_map<Dimension_type, std::pair<int,int>>::iterator iter_tile = mDimension_tile2_tile3_map.find(dimension);
        
        //mapping
        //Spaital map count
        int spatialmap_count = 0;
        for(auto i = mDirective_sequence.begin(); i != mDirective_sequence.end(); i++)
            if((*i)->Get_mapping() == Mapping_type::Smap)
                spatialmap_count++;

        if( spatialmap_count < 1 && true_or_false())
            mapping = Mapping_type::Smap;
        else
            mapping = Mapping_type::Tmap;        

        //preventing no spatial map in cluster level
        if ( spatialmap_count == 0 && mDimension_vector.empty() == true)
        {
            mapping = Mapping_type::Smap;
        }

        //R3 region size offset mapping
        switch (dimension)
        {
            case Dimension_type::C:
                size = Return_random_divisor(mLayer_info_ptr->Get_Dimension_C());
                offset = size;
                break;
            case Dimension_type::K:
                size = Return_random_divisor(mLayer_info_ptr->Get_Dimension_K());
                offset = size;
                break;
            case Dimension_type::X:
                //size = iter_tile->second.first;
                //offset = mX_basis;                        
                switch(mLayer_info_ptr->Get_stride_x())
                {
                    case 1:
                        size = mX_basis + mLayer_info_ptr->Get_Dimension_S() - 1;
                        break;
                    case 2:
                        size = mX_basis + mLayer_info_ptr->Get_Dimension_S() - 2;
                        break;

                    default:
                        std::cerr<<"Conv initialization error"<<std::endl;
                        exit(EXIT_FAILURE);
                        break;
                }
                offset = mX_basis;
                break;

            case Dimension_type::Y:            
                //size = iter_tile->second.first;
                //offset = mY_basis;
                switch(mLayer_info_ptr->Get_stride_y())
                {
                    case 1:
                        size = mY_basis + mLayer_info_ptr->Get_Dimension_R() - 1;
                        break;
                    case 2:
                        size = mY_basis + mLayer_info_ptr->Get_Dimension_R() - 2;
                        break;

                    default:
                        std::cerr<<"Conv initialization error"<<std::endl;
                        exit(EXIT_FAILURE);
                        break;
                }                
                offset = mY_basis;
                break;

            case Dimension_type::S:
                size = mLayer_info_ptr->Get_Dimension_S();
                offset = mLayer_info_ptr->Get_Dimension_S();
                break;

            case Dimension_type::R:
                size = mLayer_info_ptr->Get_Dimension_R();
                offset = mLayer_info_ptr->Get_Dimension_R();
                break;

            default:
                std::cerr<<"switch error : upper region part"<<std::endl;
                exit(EXIT_FAILURE);
                break;                                                                               
        }       
        
        mDirective_sequence.push_back(std::make_unique<Directive>(size, offset, Cluster_type::None , mapping, dimension));  

    }

    //std::cout<<"Lower region"<<std::endl;
    Set_dimension_vector();

    std::random_shuffle(mDimension_vector.begin(), mDimension_vector.end() );    
    

    while(!mDimension_vector.empty())
    {
        Dimension_type dimension = Dimension_type::None;
        Mapping_type mapping = Mapping_type::None;
        int size = 0;
        int offset = 0;

        dimension = mDimension_vector.back();
        mDimension_vector.pop_back();

        std::list<std::unique_ptr<Directive>>::iterator ptr_upper_region;
        for(ptr_upper_region = mDirective_sequence.begin(); ptr_upper_region != mDirective_sequence.end(); ptr_upper_region++ )
            if( (*ptr_upper_region)->Get_dimension() == dimension)
                break;

        //std::unordered_map<Dimension_type, std::pair<int,int>>::iterator iter_tile = mDimension_tile2_tile3_map.find(dimension);

        //mapping
        //Random spatial map
        int spatialmap_count = 0;
        for(auto i = mDirective_sequence.begin(); i != mDirective_sequence.end(); i++)
            if((*i)->Get_mapping() == Mapping_type::Smap)
                spatialmap_count++;

        if( spatialmap_count < 2 && true_or_false())
        {
            mapping = Mapping_type::Smap;
            R2_spatialmap_dimension = dimension;
        }
        else
            mapping = Mapping_type::Tmap;        

        //preventing no spatial map in cluster level
        if ( spatialmap_count == 1 && mDimension_vector.empty() == true)
        {
            mapping = Mapping_type::Smap;
            R2_spatialmap_dimension = dimension;
        }

        switch (dimension)
        {
            case Dimension_type::C:
            case Dimension_type::K:                
                size = Return_random_divisor((*ptr_upper_region)->Get_size());
                offset = size;
                break;
            case Dimension_type::X:
                
                switch (mLayer_info_ptr->Get_stride_x())
                {
                    case 1:
                        {
                        size = mLayer_info_ptr->Get_Dimension_S();
                        offset = 1;
                        }
                        break;
                    case 2:
                        {
                        size = mLayer_info_ptr->Get_Dimension_S();
                        offset = 2;
                        }   
                        break;
                    default:
                        std::cerr<<"CONV init switch error"<<std::endl;
                        exit(EXIT_FAILURE);
                        break;
                }
                
                break;
            case Dimension_type::Y:

                switch (mLayer_info_ptr->Get_stride_y())
                {

                    case 1:
                        {
                        size = mLayer_info_ptr->Get_Dimension_R();
                        offset = 1;
                        }
                        break;
                    case 2:
                        {                        
                        size = mLayer_info_ptr->Get_Dimension_R();          
                        offset = 2;
                        }
                        break;
                    default:
                        std::cerr<<"CONV init switch error"<<std::endl;
                        exit(EXIT_FAILURE);
                        break;                        
                }
                break;                
            case Dimension_type::S:
                size = mLayer_info_ptr->Get_Dimension_S();
                offset = mLayer_info_ptr->Get_Dimension_S();
                break;
            case Dimension_type::R:
                size = mLayer_info_ptr->Get_Dimension_R();
                offset = mLayer_info_ptr->Get_Dimension_R();
                break;                
            default:
                std::cerr<<"switch error : 2 region part"<<std::endl;
                exit(EXIT_FAILURE);
                break;                                                                               
        }       

        mDirective_sequence.push_back(std::make_unique<Directive>(size, offset, Cluster_type::None , mapping, dimension));          
    }
    
    //std::cout<<"cluster"<<std::endl;
    //Cluster
    int cluster_size = 0;

    //Finding Spatial mapping in R2 region
    std::list<std::unique_ptr<Directive>>::reverse_iterator R2_iter_spatial;
    for(R2_iter_spatial = mDirective_sequence.rbegin(); R2_iter_spatial != mDirective_sequence.rend(); R2_iter_spatial++)
        if((*R2_iter_spatial)->Get_dimension() == R2_spatialmap_dimension)
            break;

    //Same dimension in R3 region
    std::list<std::unique_ptr<Directive>>::iterator R3_iter;
    for(R3_iter = mDirective_sequence.begin(); R3_iter != mDirective_sequence.end(); R3_iter++)
        if((*R3_iter)->Get_dimension() == (*R2_iter_spatial)->Get_dimension())
            break;


    //cluster location
    auto it_clutser = mDirective_sequence.begin();
    for(int i = 0; i < mLayer_info_ptr->Get_Dimension_count(); i++    )
        it_clutser++;

    switch((*R2_iter_spatial)->Get_dimension())
    {
        case Dimension_type::C:
        case Dimension_type::K:
        case Dimension_type::R:
        case Dimension_type::S:
            cluster_size = (*R3_iter)->Get_size() / (*R2_iter_spatial)->Get_size();    
            break;
        case Dimension_type::X:
            switch(mLayer_info_ptr->Get_stride_x())
            {
                case 1:                
                    cluster_size = mX_basis;
                    break;
                case 2:
                    cluster_size = mX_basis / 2;
                    break;
                default:
                    std::cerr<<"dataflow constructor cluster size error"<<std::endl;
                    exit(EXIT_FAILURE);
                    break;

            }
            
            break;            
        case Dimension_type::Y:
            switch(mLayer_info_ptr->Get_stride_y())
            {
                case 1:                
                    cluster_size = mY_basis;
                    break;
                case 2:
                    cluster_size = mY_basis / 2;
                    break;
                default:
                    std::cerr<<"dataflow constructor cluster size error"<<std::endl;
                    exit(EXIT_FAILURE);
                    break;
            }

            break;
        default:
            std::cerr<<"switch error : clusterpart"<<std::endl;
            exit(EXIT_FAILURE);        
            break;
    }
    
    mDirective_sequence.insert(it_clutser, std::make_unique<Directive>(cluster_size, 0, Cluster_type::P , Mapping_type::Cluster, Dimension_type::None));  

}


void Dataflow::Resetting_dataflow(void)
{
    bool check_code = false;

    //Clearing file 
    /*
    std::string deleting_command = "rm " + mDataflow_name + ".txt";
    system(deleting_command.c_str());

    deleting_command = "rm " + mDataflow_name + ".sh";
    system(deleting_command.c_str());

    deleting_command = "rm " + mDataflow_name + ".csv";
    system(deleting_command.c_str());

    std::string deleting_command2 = "cd ../maestro/data/mapping ; rm " + mDataflow_name + ".m";
    system(deleting_command.c_str());
    */
   
    //Clear data
    mDataflow_name.clear();
    mDirective_sequence.clear();

    //Resetting data
    mDataflow_name = mOriginal_name + std::to_string(mPopulation_count);
    Conv_init();
    Making_dataflow_file();
    Executing_dataflow_file();    

    while(!check_code)
    {
        std::string input_line("MAESTRO_OUTPUT");
        std::ifstream MAESTRO_csv;
        MAESTRO_csv.open(mDataflow_name + ".csv");

        if(MAESTRO_csv.is_open())
        {

            check_code = true;
            getline(MAESTRO_csv, input_line);//Aborting first line of MAESTRO ouput csv file
            getline(MAESTRO_csv, input_line);
            MAESTRO_csv.close();

            //Remove_char function

            char* c_str;
            c_str = const_cast<char*>(input_line.c_str());                
            
            char *pDest = c_str;
            char *pStart = c_str;

            while(*pStart)
            {
                if(*pStart != ' ')
                    *pDest++ = *pStart;
                *pStart++;
            }
            *pDest = '\0';
            
            int j = 0;
            double Maestro_output[51] = {0.0};
            char* ptr = strtok(c_str, ",");
            while(ptr != NULL )
            {
                Maestro_output[j++]= atof(ptr);
                ptr = strtok(NULL, ",");
            }    

            mEvaluation_cycles = Maestro_output[3];
            mEvaluation_activity_count_based_energy = Maestro_output[4];
            mEvaluation_throughput = Maestro_output[5];
            mEvaluation_area = Maestro_output[7];
            mEvaluation_power = Maestro_output[8];
            mEvaluation_L2 = Maestro_output[13];
            mEvaluation_L1 = Maestro_output[14];   


            std::string deleting_command2 = "cd ../maestro/data/mapping ; rm " + mDataflow_name + ".m";
            system(deleting_command2.c_str());

            //deleting file in maestro folder
            
            std::string deleting_command = "rm " + mDataflow_name + ".txt";
            system(deleting_command.c_str());

            deleting_command = "rm " + mDataflow_name + ".sh";
            system(deleting_command.c_str());  

            deleting_command = "rm " + mDataflow_name + ".csv";
            system(deleting_command.c_str());
            
            if(check_code && mEvaluation_throughput <= mHW_ptr->Get_number_of_PES()) 
            {
                check_code = true;
            }
            else
            {

                check_code = false;
                //Clearing previous data
                mDataflow_name.clear();
                mDirective_sequence.clear();
                
                //New dataflow name
                mDataflow_name = mOriginal_name + std::to_string(mPopulation_count);

                //Initialization
                Conv_init();

                //Executing
                Making_dataflow_file();
                Executing_dataflow_file();

                //Waiting maestro output
                /*
                bool maestro_execute_done = false;
                std::ifstream check_file;
                while(!maestro_execute_done)
                {
                    check_file.open(mDataflow_name + ".txt");
                    if(check_file.is_open())
                        maestro_execute_done = true;
                }
                */
            }
            
        }
        else
        {

            check_code = false;

            //deleting dataflow file
            std::string deleting_command2 = "cd ../maestro/data/mapping ; rm " + mDataflow_name + ".m";
            system(deleting_command2.c_str());

            //deleting file in maestro folder
            std::string deleting_command = "rm " + mDataflow_name + ".txt";
            system(deleting_command.c_str());

            deleting_command = "rm " + mDataflow_name + ".sh";
            system(deleting_command.c_str());        

            //Clearing previous data
            mDataflow_name.clear();
            mDirective_sequence.clear();


            
            //New dataflow name
            mDataflow_name = mOriginal_name + std::to_string(mPopulation_count);

            //Initialization
            Conv_init();

            //Executing
            Making_dataflow_file();
            Executing_dataflow_file();

            //Waiting maestro output
            bool maestro_execute_done = false;
            std::ifstream check_file;
            while(!maestro_execute_done)
            {
                check_file.open(mDataflow_name + ".txt");
                if(check_file.is_open())
                    maestro_execute_done = true;
            }

        }

    }
}

void Dataflow::Making_dataflow_file(void) const
{

    std::ofstream dataflow_file;    
    //THINGS TO DO : chaning file path to absolute path
    dataflow_file.open("../maestro/data/mapping/" + mDataflow_name + ".m");
    if(!dataflow_file.is_open())
    {
        std::cerr<<"[INITIALIZATION] Dataflow file open is failed"<<std::endl;            
        exit(EXIT_FAILURE);
    }

    dataflow_file<<"Network "<<mDataflow_name<<" {"<<std::endl;
    dataflow_file<<"\t"<<"Layer "<<mLayer_info_ptr->Get_layer_config()<<" {"<<std::endl;
    dataflow_file<<"\t\t"<<"Type: "<<mLayer_info_ptr->Get_layer_type()<<std::endl;
    dataflow_file<<"\t\t"<<"Stride { X: "<<mLayer_info_ptr->Get_stride_x()<<", Y: "<<mLayer_info_ptr->Get_stride_y()<<" }"<<std::endl;
    dataflow_file<<"\t\t"<<"Dimensions { K: "<<mLayer_info_ptr->Get_Dimension_K();
    dataflow_file<<", C: "<<mLayer_info_ptr->Get_Dimension_C();
    dataflow_file<<", R: "<<mLayer_info_ptr->Get_Dimension_R();
    dataflow_file<<", S: "<<mLayer_info_ptr->Get_Dimension_S();
    dataflow_file<<", Y: "<<mLayer_info_ptr->Get_Dimension_Y();
    dataflow_file<<", X: "<<mLayer_info_ptr->Get_Dimension_X()<<" }"<<std::endl;
    dataflow_file<<"\t\t"<<"Dataflow {"<<std::endl;

    for(auto j = mDirective_sequence.begin(); j != mDirective_sequence.end(); j++)
        dataflow_file<<"\t\t\t"<<(*j)->Print_directive()<<std::endl;          

    dataflow_file<<"\t\t"<<"}"<<std::endl;
    dataflow_file<<"\t"<<"}"<<std::endl;
    dataflow_file<<"}"<<std::endl;
    dataflow_file.close();
    
    
}


void Dataflow::Executing_dataflow_file(void) const
{   
    //Added code
    FILE* fp;
    int state;

    std::ofstream run_shell;    
    run_shell.open("../maestro/" + mDataflow_name + ".sh");
    if(!run_shell.is_open())
    {
        std::cerr<<"[Evaluation] Running shell file open is failed"<<std::endl;
        //exit(EXIT_FAILURE);
    }
    //previous version of executing maestro
    /*
    run_shell<<"./maestro --print_res=true \\"<<std::endl;
    run_shell<<"\t\t--print_res_csv_file=true \\"<<std::endl;
    run_shell<<"\t\t--print_log_file=true \\"<<std::endl;
    run_shell<<"\t\t--Mapping_file='data/mapping/"<<mDataflow_name + ".m' \\"<<std::endl; 
    run_shell<<"\t\t--HW_file='./data/hw/my_HW.m' \\"<<std::endl; 
    run_shell<<"\t\t--noc_bw="<<mHW_ptr->Get_bandwidth()<<"\\"<<std::endl;
    run_shell<<"\t\t--noc_hops="<<mHW_ptr->Get_Noc_hops()<<" \\"<<std::endl;
    run_shell<<"\t\t--noc_hop_latency="<<mHW_ptr->Get_hops_latency()<<" \\"<<std::endl;
    run_shell<<"\t\t--l1_size="<<mHW_ptr->Get_l1_size()<<" \\"<<std::endl;
    run_shell<<"\t\t--l2_size="<<mHW_ptr->Get_l2_size()<<" \\"<<std::endl;
    run_shell<<"\t\t--num_pes="<<mHW_ptr->Get_number_of_PES()<<" \\"<<std::endl;
    run_shell<<"\t\t--print_design_space=true \\"<<std::endl;
    run_shell<<"\t\t--msg_print_lv=0 \\"<<std::endl;
    run_shell<<std::endl;
    */

    run_shell<<"./maestro --HW_file="<<"'data/hw/my_HW'.m \\"<<std::endl;
    run_shell<<"\t\t--Mapping_file='data/mapping/"<<mDataflow_name + ".m' \\"<<std::endl;
    run_shell<<"\t\t--print_res=false \\"<<std::endl;
    run_shell<<"\t\t--print_res_csv_file=true \\"<<std::endl;
    run_shell<<"\t\t--print_log_file=false \\"<<std::endl;
    run_shell<<std::endl;
    run_shell.close();

    //executing maestro
    std::string executing_shell ="sh " +  mDataflow_name + ".sh >> " + mDataflow_name + ".txt";
    //system(executing_shell.c_str());
    
    fp = popen(executing_shell.c_str(), "r");
    
    if (fp == NULL)
    {
        perror("[EVLAUATION] Executing MAESTRO failed");
        exit(0);
    }

    state = pclose(fp);
    
    
}

void Dataflow::Parsing_and_verifying_output(void)
{
    bool check_code = false;

    
    while(!check_code)
    {
        std::string input_line("MAESTRO_OUTPUT");
        std::ifstream MAESTRO_csv;
        MAESTRO_csv.open(mDataflow_name + ".csv");

        if(MAESTRO_csv.is_open())
        {

            check_code = true;
            getline(MAESTRO_csv, input_line);//Aborting first line of MAESTRO ouput csv file
            getline(MAESTRO_csv, input_line);
            MAESTRO_csv.close();

            //Remove_char function

            char* c_str;
            c_str = const_cast<char*>(input_line.c_str());                
            
            char *pDest = c_str;
            char *pStart = c_str;

            while(*pStart)
            {
                if(*pStart != ' ')
                    *pDest++ = *pStart;
                *pStart++;
            }
            *pDest = '\0';
            
            int j = 0;
            double Maestro_output[51] = {0.0};
            char* ptr = strtok(c_str, ",");
            while(ptr != NULL )
            {
                Maestro_output[j++]= atof(ptr);
                ptr = strtok(NULL, ",");
            }    

            mEvaluation_cycles = Maestro_output[3];
            mEvaluation_activity_count_based_energy = Maestro_output[4];
            mEvaluation_throughput = Maestro_output[5];
            mEvaluation_area = Maestro_output[7];
            mEvaluation_power = Maestro_output[8];
            mEvaluation_L2 = Maestro_output[13];
            mEvaluation_L1 = Maestro_output[14];   


            std::string deleting_command2 = "cd ../maestro/data/mapping ; rm " + mDataflow_name + ".m";
            system(deleting_command2.c_str());

            //deleting file in maestro folder
            
            std::string deleting_command = "rm " + mDataflow_name + ".txt";
            system(deleting_command.c_str());

            deleting_command = "rm " + mDataflow_name + ".sh";
            system(deleting_command.c_str());  

            deleting_command = "rm " + mDataflow_name + ".csv";
            system(deleting_command.c_str());
            
            if(check_code && mEvaluation_throughput <= mHW_ptr->Get_number_of_PES()) 
            {
                check_code = true;
            }
            else
            {

                check_code = false;
                //Clearing previous data
                mDataflow_name.clear();
                mDirective_sequence.clear();
                
                //New dataflow name
                mDataflow_name = mOriginal_name + std::to_string(mPopulation_count);

                //Initialization
                Conv_init();

                //Executing
                Making_dataflow_file();
                Executing_dataflow_file();

                //Waiting maestro output
                /*
                bool maestro_execute_done = false;
                std::ifstream check_file;
                while(!maestro_execute_done)
                {
                    check_file.open(mDataflow_name + ".txt");
                    if(check_file.is_open())
                        maestro_execute_done = true;
                }
                */
            }
            
        }
        else
        {

            check_code = false;

            //deleting dataflow file
            std::string deleting_command2 = "cd ../maestro/data/mapping ; rm " + mDataflow_name + ".m";
            system(deleting_command2.c_str());

            //deleting file in maestro folder
            std::string deleting_command = "rm " + mDataflow_name + ".txt";
            system(deleting_command.c_str());

            deleting_command = "rm " + mDataflow_name + ".sh";
            system(deleting_command.c_str());        

            //Clearing previous data
            mDataflow_name.clear();
            mDirective_sequence.clear();

            
            //New dataflow name
            mDataflow_name = mOriginal_name + std::to_string(mPopulation_count);

            //Initialization
            Conv_init();

            //Executing
            Making_dataflow_file();
            Executing_dataflow_file();

            //Waiting maestro output
            bool maestro_execute_done = false;
            std::ifstream check_file;
            while(!maestro_execute_done)
            {
                check_file.open(mDataflow_name + ".txt");
                if(check_file.is_open())
                    maestro_execute_done = true;
            }

        }

    }

}


std::string Dataflow::Get_dataflow_name(void) const { return mDataflow_name; }

double Dataflow::Get_cycles(void) const { return mEvaluation_cycles; }

double Dataflow::Get_energy(void) const { return mEvaluation_activity_count_based_energy; }

long int Dataflow::Get_area(void) const { return mEvaluation_area; }

double Dataflow::Get_power(void) const { return mEvaluation_power; }

int Dataflow::Get_L2_size(void) const { return mEvaluation_L2; }

double Dataflow::Get_L1_size(void) const { return mEvaluation_L1; }

double Dataflow::Get_evaluation_value(std::string Evaluation_criteria)
{
    if(Evaluation_criteria == "Cycles")
    {
        return mEvaluation_cycles;
    }
    else if (Evaluation_criteria == "Energy")
    {
        return mEvaluation_activity_count_based_energy;
    }
    else if (Evaluation_criteria == "Throughput")
    {
        return mEvaluation_throughput;;
    }    
    else if (Evaluation_criteria == "Area")
    {
        return static_cast<double>(mEvaluation_area);
    }    
    else if (Evaluation_criteria == "Power")
    {
        return static_cast<double>(mEvaluation_power);
    }
    else if (Evaluation_criteria == "L2_size")
    {
        return static_cast<double>(mEvaluation_L2);
    }    
    else if (Evaluation_criteria == "L1_size")
    {
        return static_cast<double>(mEvaluation_L1);
    }      
    else
    {
        std::cerr<<"Invalid evaluation criteria "<<std::endl;
        exit(EXIT_FAILURE);
    }      
}

void Dataflow::Set_dataflow_name(std::string Dataflow_name)
{
    mDataflow_name = Dataflow_name;
}


void Dataflow::Reset_Population_count(void)
{
    mPopulation_count = 0;
}

void Dataflow::Set_dimension_vector(void)
{
    if(mLayer_info_ptr->Get_layer_type()=="CONV")
    {
        /*
        mDimension_vector = {
        Dimension_type::C,
        Dimension_type::K, 
        Dimension_type::Y, 
        Dimension_type::X, 
        Dimension_type::R, 
        Dimension_type::S,  
        };
        */
       mDimension_vector.push_back(Dimension_type::C);      
       mDimension_vector.push_back(Dimension_type::K);
       mDimension_vector.push_back(Dimension_type::Y);
       mDimension_vector.push_back(Dimension_type::X);
       mDimension_vector.push_back(Dimension_type::R);
       mDimension_vector.push_back(Dimension_type::S);
    }
    else if(mLayer_info_ptr->Get_layer_type()=="DSCONV")
    {
        /*
        mDimension_vector = {
        Dimension_type::C,
        Dimension_type::Y, 
        Dimension_type::X, 
        Dimension_type::R, 
        Dimension_type::S,  
        };
        */
       mDimension_vector.push_back(Dimension_type::C);      
       mDimension_vector.push_back(Dimension_type::Y);
       mDimension_vector.push_back(Dimension_type::X);
       mDimension_vector.push_back(Dimension_type::R);
       mDimension_vector.push_back(Dimension_type::S);       
    }

}

void Dataflow::Print_all(void) const
{
    #if PRINT_INTERMEDIATE_PROCESS
        std::cout<<"********************Dataflow********************"<<std::endl<<std::endl;
        std::cout<<"Datalfow name : " <<mDataflow_name<<std::endl;
        std::cout<<"Cycles : " <<mEvaluation_cycles<<std::endl;
        std::cout<<"Activity count based energy : "<<mEvaluation_activity_count_based_energy<<std::endl;
        std::cout<<"Throughput : "<<mEvaluation_throughput<<std::endl;
        std::cout<<"Area : "<<mEvaluation_area<<std::endl;
        std::cout<<"Power : "<<mEvaluation_power<<std::endl;
        std::cout<<"L2 : "<<mEvaluation_L2<<std::endl;
        std::cout<<"L1 : "<<mEvaluation_L1<<std::endl<<std::endl;
        for(auto i = mDirective_sequence.begin(); i != mDirective_sequence.end(); i++)
            std::cout<<(*i)->Print_directive()<<std::endl;
        std::cout<<"************************************************"<<std::endl<<std::endl;
    #endif
}

void Dataflow::Print_all_result(void) const
{
    std::cout<<"********************Dataflow********************"<<std::endl<<std::endl;
    std::cout<<"Datalfow name : " <<mDataflow_name<<std::endl;
    std::cout<<"Cycles : " <<mEvaluation_cycles<<std::endl;
    std::cout<<"Activity count based energy : "<<mEvaluation_activity_count_based_energy<<std::endl;
    std::cout<<"Throughput : "<<mEvaluation_throughput<<std::endl;
    std::cout<<"Area : "<<mEvaluation_area<<std::endl;
    std::cout<<"Power : "<<mEvaluation_power<<std::endl;
    std::cout<<"L2 : "<<mEvaluation_L2<<std::endl;
    std::cout<<"L1 : "<<mEvaluation_L1<<std::endl<<std::endl;
    for(auto i = mDirective_sequence.begin(); i != mDirective_sequence.end(); i++)
        std::cout<<(*i)->Print_directive()<<std::endl;
    std::cout<<"************************************************"<<std::endl<<std::endl;
}

void Dataflow::Print_all_detail(void) const
{
    std::cout<<"********************Dataflow********************"<<std::endl<<std::endl;
    std::cout<<"Datalfow name : " <<mDataflow_name<<std::endl;
    std::cout<<"Cycles : " <<mEvaluation_cycles<<std::endl;
    std::cout<<"Activity count based energy : "<<mEvaluation_activity_count_based_energy<<std::endl;
    std::cout<<"Throughput : "<<mEvaluation_throughput<<std::endl;
    std::cout<<"Area : "<<mEvaluation_area<<std::endl;
    std::cout<<"Power : "<<mEvaluation_power<<std::endl;
    std::cout<<"L2 : "<<mEvaluation_L2<<std::endl;
    std::cout<<"L1 : "<<mEvaluation_L1<<std::endl;
    std::cout<<"X basis : "<<mX_basis<<std::endl;
    std::cout<<"Y basis : "<<mY_basis<<std::endl;
    
    for(auto i = mDirective_sequence.begin(); i != mDirective_sequence.end(); i++)
        std::cout<<(*i)->Print_directive()<<std::endl;
    std::cout<<"************************************************"<<std::endl<<std::endl;    
}

void Dataflow::Action1_Random_swap(void)
{
    #if PRINT_INTERMEDIATE_PROCESS
    std::cout<<"Action1 swap"<<std::endl;
    #endif
    std::list<std::unique_ptr<Directive>>::iterator it = mDirective_sequence.begin();
    std::list<std::unique_ptr<Directive>>::iterator it1 = mDirective_sequence.begin();    
    
    //Constant value should be changed

    int border_line0;
    int border_line1;
    int border_line2;

    if(mLayer_info_ptr->Get_layer_type() == "CONV")
    {
        border_line0 = 6;
        border_line1 = 13;
        border_line2 = 7;
    }
    else if(mLayer_info_ptr->Get_layer_type() == "DSCONV")
    {
        border_line0 = 5;
        border_line1 = 11;
        border_line2 = 6;
    }

    if(true_or_false())
    {
        //R3 region swap
        int n = Random_number_from_zero(border_line0);
        int n1 = Random_number_from_zero(border_line0);

        std::advance(it,n);
        std::advance(it1,n1);


        //std::cout<<mDataflow_name<<" Region3 Handling dimension : "<<Dimension_type_to_string((*it)->Get_dimension())<<", ";
        //std::cout<<Dimension_type_to_string((*it1)->Get_dimension())<<std::endl;

        swap(*it, (*it1));
    }
    else
    {
        //R2 region swap
        
        int n = Random_number_from_zero(border_line1) ;
        int n1 = Random_number_from_zero(border_line1) ;

        while(n<border_line2)
        {   
            n = Random_number_from_zero(border_line1);
        }

        while(n1<border_line2)
        {
            n1 = Random_number_from_zero(border_line1);
        }

        std::advance(it,n);
        std::advance(it1,n1);

        //std::cout<<mDataflow_name<<" Region2 Handling dimension : "<<Dimension_type_to_string((*it)->Get_dimension())<<", ";
        //std::cout<<Dimension_type_to_string((*it1)->Get_dimension())<<std::endl;

        swap(*it, (*it1));        
        
    }
    
}


void Dataflow::Action2_Inc_size_offset(void)
{
    #if PRINT_INTERMEDIATE_PROCESS
    std::cout<<"Action2 Inc_size_offset"<<std::endl;
    #endif

    std::vector<Dimension_type> dimension_vector;

    if(mLayer_info_ptr->Get_layer_type() == "CONV")
    {
        dimension_vector = {
            Dimension_type::C,
            Dimension_type::K,
            Dimension_type::X,
            Dimension_type::Y,
        };
    }
    else if (mLayer_info_ptr->Get_layer_type() == "DSCONV")
    {
        dimension_vector = {
            Dimension_type::C,
            Dimension_type::X,
            Dimension_type::Y,
        };
    }

    std::random_shuffle(dimension_vector.begin(), dimension_vector.end() );    
    
    Dimension_type dimension = dimension_vector.back();

    #if PRINT_INTERMEDIATE_PROCESS
        std::cout<<mDataflow_name<<" Handling dimension : "<<Dimension_type_to_string(dimension)<<std::endl;
    #endif

    //Things to do about target dimension
    //Searching in R3 region
    std::list<std::unique_ptr<Directive>>::iterator ptr_upper;
    for(ptr_upper = mDirective_sequence.begin(); ptr_upper != mDirective_sequence.end(); ptr_upper++)        
        if(dimension == (*ptr_upper)->Get_dimension())
            break;        

    //Searching in R2 region
    std::list<std::unique_ptr<Directive>>::reverse_iterator ptr_lower;
    for(ptr_lower = mDirective_sequence.rbegin(); ptr_lower != mDirective_sequence.rend(); ptr_lower++)        
        if(dimension == (*ptr_lower)->Get_dimension())
            break;  

    //Finding cluster
    std::list<std::unique_ptr<Directive>>::iterator cluster_pointer;
    for(cluster_pointer = mDirective_sequence.begin(); cluster_pointer != mDirective_sequence.end(); cluster_pointer++)        
        if(Mapping_type::Cluster == (*cluster_pointer)->Get_mapping())
            break;                                  

    //Searching in dimension table


    //resizing size offset
    switch(dimension)
    {
        case Dimension_type::K:
        case Dimension_type::C:        
        {
            //R3
            int increased_size_offset_upper_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension(dimension));
            while(increased_size_offset_upper_region < (*ptr_upper)->Get_size())
            {
                increased_size_offset_upper_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension(dimension));
            }
            (*ptr_upper)->Set_size_offset(increased_size_offset_upper_region, increased_size_offset_upper_region);
            //iter_dim->second.first = increased_size_offset_upper_region; //Not updating

            //R2
            int increased_size_offset_lower_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension(dimension));
            while(increased_size_offset_lower_region < (*ptr_lower)->Get_size())
            {
                increased_size_offset_lower_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension(dimension));
            }
            (*ptr_lower)->Set_size_offset(increased_size_offset_lower_region, increased_size_offset_lower_region);
            //iter_dim->second.second = increased_size_offset_R2_region; //Not updating

            //Cluster
            if((*ptr_lower)->Get_mapping() == Mapping_type::Smap)
            {
                (*cluster_pointer)->Set_size(increased_size_offset_upper_region / increased_size_offset_lower_region);        
            }
            break;
        }
        case Dimension_type::X:
            switch (mLayer_info_ptr->Get_stride_x())
            {
                case 1:
                    {
                        //Things to do about S dimension because of X
                        //Searching in R2 region
                        std::list<std::unique_ptr<Directive>>::reverse_iterator iter_S;
                        for(iter_S = mDirective_sequence.rbegin(); iter_S != mDirective_sequence.rend(); iter_S++)        
                            if(Dimension_type::S == (*iter_S)->Get_dimension())
                                break;  

                        //Searching in dimension table
                        //auto iter_dim_S = mDimension_tile2_tile3_map.find(Dimension_type::S);
                                    
                        //R3
                        //Finding new X basis
                        int increased_X_basis = Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 1 );
                        while(increased_X_basis < mX_basis)
                        {
                            increased_X_basis = Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 1 );
                        }
                        //Updating X basis
                        mX_basis = increased_X_basis;
                        (*ptr_upper)->Set_size(mLayer_info_ptr->Get_Dimension_S() + mX_basis -1 );
                        (*ptr_upper)->Set_offset(mX_basis);
                        //iter_dim->second.first = mLayer_info_ptr->Get_Dimension_S() + mX_basis - 1;
                        /*    
                        //R2
                        int increased_size_offset_R2_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension_S());
                        while(increased_size_offset_R2_region < iter_dim->second.second)
                        {
                            increased_size_offset_R2_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension_S());
                        }
                        //Updating X size
                        (*r2)->Set_size(increased_size_offset_R2_region);            
                        iter_dim->second.second = increased_size_offset_R2_region;

                        //Updating S size
                        (*iter_S)->Set_size(increased_size_offset_R2_region);
                        (*iter_S)->Set_offset(increased_size_offset_R2_region);
                        iter_dim_S->second.second = increased_size_offset_R2_region;
                        */

                        //Cluster
                        if((*ptr_lower)->Get_mapping() == Mapping_type::Smap)
                            (*cluster_pointer)->Set_size(mX_basis);                                     
                        
                    }
                    break;
                case 2:
                    {
                        //Things to do about S dimension because of X
                        //Searching in R2 region
                        std::list<std::unique_ptr<Directive>>::reverse_iterator iter_S;
                        for(iter_S = mDirective_sequence.rbegin(); iter_S != mDirective_sequence.rend(); iter_S++)        
                            if(Dimension_type::S == (*iter_S)->Get_dimension())
                                break;  
                                    
                        //R3
                        //Finding new X basis
                        int increased_X_basis = Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 2 ) * 2;
                        while(increased_X_basis < mX_basis || increased_X_basis > mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 2 || increased_X_basis == 1)
                        {
                            //std::cout<<"Dimension X Jamming INC stride 2 R3"<<std::endl;
                            //std::cout<<"mX basis : "<<mX_basis<<std::endl;
                            //std::cout<<"increased mX basis : "<<increased_X_basis<<std::endl;
                            //std::cout<<"limitation : "<<mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 2<<std::endl;
                            increased_X_basis = Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 2 ) * 2;
                        }
                        //Updating X basis
                        mX_basis = increased_X_basis;                        
                        (*ptr_upper)->Set_size(mLayer_info_ptr->Get_Dimension_S() + mX_basis - 2);
                        (*ptr_upper)->Set_offset(mX_basis);

                        /*
                        //R2
                        int increased_size_offset_R2_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension_S());
                        while(increased_size_offset_R2_region < iter_dim->second.second)
                        {
                            //std::cout<<"Dimension X Jamming INC stride 2 R2"<<std::endl;
                            increased_size_offset_R2_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension_S());
                        }
                        //Updating X size
                        (*r2)->Set_size(increased_size_offset_R2_region);

                        if(increased_size_offset_R2_region == 1)
                            (*r2)->Set_offset(1);
                        else
                            (*r2)->Set_offset(2);

                        //std::cout<<"size : "<<(*r2)->Get_size()<<" offset : "<<(*r2)->Get_offsset()<<std::endl;
                        
                        iter_dim->second.second = increased_size_offset_R2_region;

                        //Updating S size
                        (*iter_S)->Set_size(increased_size_offset_R2_region);
                        (*iter_S)->Set_offset(increased_size_offset_R2_region);
                        iter_dim_S->second.second = increased_size_offset_R2_region;
                        */
                        //Cluster
                        if((*ptr_lower)->Get_mapping() == Mapping_type::Smap)
                            (*cluster_pointer)->Set_size(mX_basis / 2);                                            
                                                             
                    }
                    break;
                default:
                    std::cerr<<"Mutation ince malfunction"<<std::endl;
                    exit(EXIT_FAILURE);
                break;
            }
            break;
        case Dimension_type::Y:
            switch (mLayer_info_ptr->Get_stride_y())
            {
                case 1:
                    {
                    //Things to do about R dimension because of Y
                    //Searching in R2 region
                    std::list<std::unique_ptr<Directive>>::reverse_iterator iter_R;
                    for(iter_R = mDirective_sequence.rbegin(); iter_R != mDirective_sequence.rend(); iter_R++)        
                        if(Dimension_type::R == (*iter_R)->Get_dimension())
                            break;  

                    //Searching in dimension table


                    //R3
                    //Finding new Y basis
                    int increased_Y_basis = Random_number(mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 1 );
                    while(increased_Y_basis < mY_basis)
                    {
                        increased_Y_basis = Random_number(mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 1 );
                    }
                    //Updating Y basis
                    mY_basis = increased_Y_basis;
                    (*ptr_upper)->Set_size(mLayer_info_ptr->Get_Dimension_R() + mY_basis -1 );
                    (*ptr_upper)->Set_offset(mY_basis);


                    /*
                    //R2
                    int increased_size_offset_R2_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension_R());
                    while(increased_size_offset_R2_region < iter_dim->second.second)
                    {
                        increased_size_offset_R2_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension_R());
                    }
                    //Updating Y size
                    (*r2)->Set_size(increased_size_offset_R2_region);            
                    iter_dim->second.second = increased_size_offset_R2_region;

                    //Updating R size
                    (*iter_R)->Set_size(increased_size_offset_R2_region);
                    (*iter_R)->Set_offset(increased_size_offset_R2_region);
                    iter_dim_R->second.second = increased_size_offset_R2_region;
                    */
                    //Cluster
                    if((*ptr_lower)->Get_mapping() == Mapping_type::Smap)
                        (*cluster_pointer)->Set_size(mY_basis);                             

                    
                    }
                    
                    break;
                case 2:
                    {
                    //Things to do about R dimension because of Y
                    //Searching in R2 region
                    std::list<std::unique_ptr<Directive>>::reverse_iterator iter_R;
                    for(iter_R = mDirective_sequence.rbegin(); iter_R != mDirective_sequence.rend(); iter_R++)        
                        if(Dimension_type::R == (*iter_R)->Get_dimension())
                            break;  

                    //R3
                    //Finding new Y basis
                    int increased_Y_basis = Random_number(mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 2 ) * 2;
                    while(increased_Y_basis < mY_basis || increased_Y_basis > mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 2 || increased_Y_basis == 1)
                    {
                        //std::cout<<"Dimension Y Jamming INC stride 2 R3"<<std::endl;
                        //std::cout<<"mY_bsis "<<mY_basis<<std::endl;
                        //std::cout<<"increased Y basis "<<increased_Y_basis<<std::endl;
                        //std::cout<<"limitation : "<<mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 2<<std::endl;;
                        increased_Y_basis = Random_number(mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 2 ) * 2;
                    }
                    //Updating Y basis
                    mY_basis = increased_Y_basis;
                    (*ptr_upper)->Set_size(mLayer_info_ptr->Get_Dimension_R() + mY_basis -2 );
                    (*ptr_upper)->Set_offset(mY_basis);


                    /*
                    //R2
                    int increased_size_offset_R2_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension_R());
                    while(increased_size_offset_R2_region < iter_dim->second.second)
                    {
                        //std::cout<<"Dimension Y Jamming INC stride 2 R2"<<std::endl;
                        increased_size_offset_R2_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension_R());
                    }
                    //Updating Y size
                    (*r2)->Set_size(increased_size_offset_R2_region);            
                    iter_dim->second.second = increased_size_offset_R2_region;

                    (*r2)->Set_size(increased_size_offset_R2_region);
                    if(increased_size_offset_R2_region == 1)
                        (*r2)->Set_offset(1);
                    else
                        (*r2)->Set_offset(2);

                    //std::cout<<"size : "<<(*r2)->Get_size()<<" offset : "<<(*r2)->Get_offsset()<<std::endl;

                    //Updating R size
                    (*iter_R)->Set_size(increased_size_offset_R2_region);
                    (*iter_R)->Set_offset(increased_size_offset_R2_region);
                    iter_dim_R->second.second = increased_size_offset_R2_region;


                    */
                    //Cluster
                    if((*ptr_lower)->Get_mapping() == Mapping_type::Smap)
                        (*cluster_pointer)->Set_size(mY_basis / 2);                                
                    
                    }
                          
                    break;
                default:
                    std::cerr<<"Mutation inc malfunction"<<std::endl;
                    exit(EXIT_FAILURE);
                break;
            }
            break;
        default:
            std::cerr<<"switch error - Action 2 inc part"<<std::endl;
            exit(EXIT_FAILURE);
            break;        
    }    
}



void Dataflow::Action3_Dec_size_offset(void)
{   
    
    #if PRINT_INTERMEDIATE_PROCESS
        std::cout<<"Action3_Dec_size_offset"<<std::endl;
    #endif

    std::vector<Dimension_type> dimension_vector;

    if(mLayer_info_ptr->Get_layer_type() == "CONV")
    {
        dimension_vector = {
            Dimension_type::C,
            Dimension_type::K,
            Dimension_type::X,
            Dimension_type::Y,
        };
    }
    else if (mLayer_info_ptr->Get_layer_type() == "DSCONV")
    {
        dimension_vector = {
            Dimension_type::C,
            Dimension_type::X,
            Dimension_type::Y,
        };
    }

    std::random_shuffle(dimension_vector.begin(), dimension_vector.end() );    
    
    Dimension_type dimension = dimension_vector.back();


    #if PRINT_INTERMEDIATE_PROCESS
        std::cout<<mDataflow_name<<" Handling dimension : "<<Dimension_type_to_string(dimension)<<std::endl;
    #endif

    //Things to do about target dimension
    //Searching in R3 region
    std::list<std::unique_ptr<Directive>>::iterator ptr_upper;
    for(ptr_upper = mDirective_sequence.begin(); ptr_upper != mDirective_sequence.end(); ptr_upper++)        
        if(dimension == (*ptr_upper)->Get_dimension())
            break;        
            
    //Searching in R2 region
    std::list<std::unique_ptr<Directive>>::reverse_iterator ptr_lower;
    for(ptr_lower = mDirective_sequence.rbegin(); ptr_lower != mDirective_sequence.rend(); ptr_lower++)        
        if(dimension == (*ptr_lower)->Get_dimension())
            break;  

    //Finding cluster
    std::list<std::unique_ptr<Directive>>::iterator cluster_pointer;
    for(cluster_pointer = mDirective_sequence.begin(); cluster_pointer != mDirective_sequence.end(); cluster_pointer++)        
        if(Mapping_type::Cluster == (*cluster_pointer)->Get_mapping())
            break;                                  

    //Searching in dimension table

    switch(dimension)
    {
        case Dimension_type::K:
        case Dimension_type::C:        
        {
            //upper region
            int decreased_size_offset_upper_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension(dimension));
            while(decreased_size_offset_upper_region > (*ptr_upper)->Get_size())
            {
                
                decreased_size_offset_upper_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension(dimension));
            }
            (*ptr_upper)->Set_size_offset(decreased_size_offset_upper_region, decreased_size_offset_upper_region);

            //lower region
            
            int decreased_size_offset_lower_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension(dimension));
            while(decreased_size_offset_lower_region > (*ptr_lower)->Get_size())
            {
                decreased_size_offset_lower_region = Return_random_divisor(mLayer_info_ptr->Get_Dimension(dimension));
            }
            (*ptr_lower)->Set_size_offset(decreased_size_offset_lower_region, decreased_size_offset_lower_region);


            //Cluster
            if((*ptr_lower)->Get_mapping() == Mapping_type::Smap)
            {
                (*cluster_pointer)->Set_size(decreased_size_offset_upper_region / decreased_size_offset_lower_region);        
            }
            
            break;
        }
        case Dimension_type::X:
            switch (mLayer_info_ptr->Get_stride_y())
            {
                case 1:
                {
                    //Things to do about S dimension because of X
                    //Searching in R2 region
                    std::list<std::unique_ptr<Directive>>::reverse_iterator iter_S;
                    for(iter_S = mDirective_sequence.rbegin(); iter_S != mDirective_sequence.rend(); iter_S++)        
                        if(Dimension_type::S == (*iter_S)->Get_dimension())
                            break;  


                    //R3
                    //Finding new X basis
                    int increased_X_basis = Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 1 );
                    while(increased_X_basis > mX_basis)
                    {

                        increased_X_basis = Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 1 );
                    }
                    //Updating X basis
                    mX_basis = increased_X_basis;
                    (*ptr_upper)->Set_size(mLayer_info_ptr->Get_Dimension_S() + mX_basis -1 );
                    (*ptr_upper)->Set_offset(mX_basis);

            
                    if((*ptr_lower)->Get_mapping() == Mapping_type::Smap)
                    {
                        (*cluster_pointer)->Set_size(mX_basis);                                    
                    }
                    
                }            
                break;
            case 2:
                {
                    //Things to do about S dimension because of X
                    //Searching in R2 region
                    std::list<std::unique_ptr<Directive>>::reverse_iterator iter_S;
                    for(iter_S = mDirective_sequence.rbegin(); iter_S != mDirective_sequence.rend(); iter_S++)        
                        if(Dimension_type::S == (*iter_S)->Get_dimension())
                            break;  

                    int increased_X_basis = Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 2 ) * 2;
                    while(increased_X_basis > mX_basis || increased_X_basis > mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 2 || increased_X_basis == 1)
                    {
                        //std::cout<<"Dimension X Jamming DEC stride 2 R3"<<std::endl;
                        increased_X_basis = Random_number(mLayer_info_ptr->Get_Dimension_X() - mLayer_info_ptr->Get_Dimension_S() + 2 ) * 2;
                    }
                    //Updating X basis
                    mX_basis = increased_X_basis;
                    (*ptr_upper)->Set_size(mLayer_info_ptr->Get_Dimension_S() + mX_basis - 2);
                    (*ptr_upper)->Set_offset(mX_basis);
                
                    
                    if((*ptr_lower)->Get_mapping() == Mapping_type::Smap)
                    {
                        (*cluster_pointer)->Set_size(mX_basis / 2);                    
                    }                        
                                                            
                }
                break;  
            default:
                std::cerr<<"Mutation ince malfunction"<<std::endl;
                exit(EXIT_FAILURE);                
                break;
            }  
            break;

        case Dimension_type::Y:
        switch(mLayer_info_ptr->Get_stride_y())
        {
            case 1:
                {

                    //Things to do about R dimension because of Y
                    //Searching in R2 region            
                    std::list<std::unique_ptr<Directive>>::reverse_iterator iter_R;
                    for(iter_R = mDirective_sequence.rbegin(); iter_R != mDirective_sequence.rend(); iter_R++)        
                        if(Dimension_type::R == (*iter_R)->Get_dimension())
                            break;  

                    //Searching in dimension table


                    //R3
                    //Finding new X basis
                    int increased_Y_basis = Random_number(mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 1 );
                    while(increased_Y_basis > mY_basis)
                    {
                        increased_Y_basis = Random_number(mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 1 );
                    }
                    //Updating X basis
                    mY_basis = increased_Y_basis;
                    (*ptr_upper)->Set_size(mLayer_info_ptr->Get_Dimension_R() + mY_basis -1 );
                    (*ptr_upper)->Set_offset(mY_basis);

                    //Cluster
                    if((*ptr_lower)->Get_mapping() == Mapping_type::Smap)
                        (*cluster_pointer)->Set_size(mY_basis);                    
                    break;
                    
                }
                break;    
            case 2:
                {
                    //Things to do about R dimension because of Y
                    //Searching in R2 region
                    std::list<std::unique_ptr<Directive>>::reverse_iterator iter_R;
                    for(iter_R = mDirective_sequence.rbegin(); iter_R != mDirective_sequence.rend(); iter_R++)        
                        if(Dimension_type::R == (*iter_R)->Get_dimension())
                            break;  

                    //Searching in dimension table


                    //R3
                    //Finding new X basis
                    int increased_Y_basis = Random_number(mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 2 ) * 2;
                    while(increased_Y_basis > mY_basis || increased_Y_basis > mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 2 || increased_Y_basis == 1)
                    {
                        //std::cout<<"Dimension Y Jamming DEC stride 2 R3"<<std::endl;
                        increased_Y_basis = Random_number(mLayer_info_ptr->Get_Dimension_Y() - mLayer_info_ptr->Get_Dimension_R() + 2 ) * 2;
                    }
                    //Updating X basis
                    mY_basis = increased_Y_basis;
                    (*ptr_upper)->Set_size(mLayer_info_ptr->Get_Dimension_R() + mY_basis -2 );
                    (*ptr_upper)->Set_offset(mY_basis);


                    if((*ptr_lower)->Get_mapping() == Mapping_type::Smap)
                    {
                        (*cluster_pointer)->Set_size(mY_basis / 2);                                
                    }
                    
                }
                break;
            default:
                std::cerr<<"Mutation ince malfunction"<<std::endl;
                exit(EXIT_FAILURE);                
                break;            
            }
            break;
        default:
            std::cerr<<"switch error - Action 2 inc part"<<std::endl;
            exit(EXIT_FAILURE);
            break;        
    }


}

void Dataflow::Action4_changing_spatial_map(void)
{   
    #if PRINT_INTERMEDIATE_PROCESS
        std::cout<<"Action4_changing_spatial_map"<<std::endl;
    #endif

    //std::cout<<mDataflow_name;
    if(true_or_false())
    {
        
        //std::cout<<" R3 region";
        //changing spatial map in R3 region
        //Finding spatial map in R3 region
        std::list<std::unique_ptr<Directive>>::iterator upper_spatial;
        for(upper_spatial = mDirective_sequence.begin(); upper_spatial != mDirective_sequence.end(); upper_spatial++)        
            if((*upper_spatial)->Get_mapping() == Mapping_type::Smap)
                break;        

        //std::cout<<" previous spatialmap dimension : "<<Dimension_type_to_string((*r3_spatial)->Get_dimension())<<", ";
        //resetting mapping type
        (*upper_spatial)->Set_mapping(Mapping_type::Tmap);
        std::list<std::unique_ptr<Directive>>::iterator new_upper_spatial = mDirective_sequence.begin();
        int n = Random_number_from_zero(mLayer_info_ptr->Get_Dimension_count());
        std::advance(new_upper_spatial,n);
        (*new_upper_spatial)->Set_mapping(Mapping_type::Smap);
        //std::cout<<" subsequent spatialmap dimension : "<<Dimension_type_to_string((*new_r3_spatial)->Get_dimension())<<std::endl;
        
        
    }
    else
    {
        //std::cout<<" R2 region";
            
        //changing spatial map in R2 region
        //Finding spatial map in R2 region
        std::list<std::unique_ptr<Directive>>::reverse_iterator lower_spatial;
        for(lower_spatial = mDirective_sequence.rbegin(); lower_spatial != mDirective_sequence.rend(); lower_spatial++)        
            if((*lower_spatial)->Get_mapping() == Mapping_type::Smap)
                break;        

        //std::cout<<" previous spatialmap dimension : "<<Dimension_type_to_string((*r2_spatial)->Get_dimension())<<", ";;
        //resetting mapping type
        (*lower_spatial)->Set_mapping(Mapping_type::Tmap);
        std::list<std::unique_ptr<Directive>>::reverse_iterator new_lower_spatial = mDirective_sequence.rbegin();
        int n = Random_number_from_zero(mLayer_info_ptr->Get_Dimension_count());
        std::advance(new_lower_spatial,n);
        (*new_lower_spatial)->Set_mapping(Mapping_type::Smap);
        //std::cout<<" subsequent spatialmap dimension : "<<Dimension_type_to_string((*new_r2_spatial)->Get_dimension())<<std::endl;

        //Updating cluster size
        //Finding cluster location
        std::list<std::unique_ptr<Directive>>::iterator cluster_pointer;
        for(cluster_pointer = mDirective_sequence.begin(); cluster_pointer != mDirective_sequence.end(); cluster_pointer++)        
            if((*cluster_pointer)->Get_mapping() == Mapping_type::Cluster)
                break;

        //std::cout<<"here1"<<std::endl;
        Dimension_type dimension = (*new_lower_spatial)->Get_dimension();
        switch(dimension)
        {
            case Dimension_type::C:
            case Dimension_type::K:
            case Dimension_type::R:
            case Dimension_type::S:
            {
                //std::cout<<"here2"<<std::endl;
                //std::unordered_map<Dimension_type, std::pair<int,int>>::iterator iter_table_pointer = mDimension_tile2_tile3_map.find(dimension);

                auto same_dimension_in_upper_region = mDirective_sequence.begin();
                for(same_dimension_in_upper_region = mDirective_sequence.begin(); same_dimension_in_upper_region != mDirective_sequence.end(); same_dimension_in_upper_region++)
                {
                    if((*same_dimension_in_upper_region)->Get_dimension() == dimension)
                        break;
                }
                
                (*cluster_pointer)->Set_size((*same_dimension_in_upper_region)->Get_size() / (*new_lower_spatial)->Get_size());
                break;            
            }
            case Dimension_type::X:
                (*cluster_pointer)->Set_size(mX_basis);
                break;
            case Dimension_type::Y:
                (*cluster_pointer)->Set_size(mY_basis);            
                break;
            default:
                std::cerr<<"switch error - Action 4 chaning spaital mapping"<<std::endl;
        }
        
    }
    //std::cout<<"Ending spatial map chaing"<<std::endl;
}
void Dataflow::Action5_chaning_dataflow(std::unique_ptr<Dataflow>& RHS)
{
    #if PRINT_INTERMEDIATE_PROCESS
        std::cout<<"Action5_changing_dataflow"<<std::endl;
    #endif
    
    //auto iter_lhs = mDirective_sequence.begin();
    //auto iter_rhs = RHS->mDirective_sequence.begin();
/*
    for(auto iter_lhs = mDirective_sequence.begin(); iter_lhs != mDirective_sequence.end(); iter_lhs++)
        std::cout<<(*iter_lhs)->Print_directive()<<std::endl;
        
    for(auto iter_rhs = RHS->mDirective_sequence.begin(); iter_rhs != RHS->mDirective_sequence.end(); iter_rhs++)
        std::cout<<(*iter_rhs)->Print_directive()<<std::endl;
*/  
    #if PRINT_INTERMEDIATE_PROCESS  
        std::cout<<"Switchig dataflow 1 : "<<mDataflow_name<<std::endl;
        std::cout<<"Switchig dataflow 2 : "<<RHS->Get_dataflow_name()<<std::endl;
    #endif

    auto iter_lhs = mDirective_sequence.begin();
    auto iter_rhs = RHS->mDirective_sequence.begin();

    for(int i = 0; i < 6; i++, iter_lhs++, iter_rhs++)
        iter_lhs->swap((*iter_rhs));

/*
    std::unordered_map<Dimension_type, std::pair<int,int>> iter;
    iter = RHS->mDimension_tile2_tile3_map;
    RHS->mDimension_tile2_tile3_map = mDimension_tile2_tile3_map;
    mDimension_tile2_tile3_map = iter;
*/
    int iter_X_basis = RHS->mX_basis;
    RHS->mX_basis = mX_basis;
    mX_basis = iter_X_basis;
    
    int iter_Y_basis = RHS->mY_basis;
    RHS->mY_basis = mY_basis;
    mY_basis = iter_Y_basis;    

}

void Dataflow::Action5_rebuilding_dataflow(void)
{
    #if PRINT_INTERMEDIATE_PROCESS
        std::cout<<"Action5_rebuilding_dataflow"<<std::endl;
    #endif

    //Cleaning preivous data
    /*
    for (auto i = mDimension_tile2_tile3_map.begin(); i != mDimension_tile2_tile3_map.end(); i++)
        i->second.second = 0;        
    */
    if(mLayer_info_ptr->Get_layer_type() == "CONV")
    {
        for (int i = 0; i <7 ; i++)
            mDirective_sequence.pop_back();    
    }
    else if (mLayer_info_ptr->Get_layer_type() == "DSCONV")
    {
        for (int i = 0; i <6 ; i++)
            mDirective_sequence.pop_back();    
    }
    //Regerating half of diemsnion table

    /*
    auto iter_C = mDimension_tile2_tile3_map.find(Dimension_type::C);
    iter_C->second.second = Return_random_divisor(iter_C->second.first);
    
    auto iter_K = mDimension_tile2_tile3_map.find(Dimension_type::K);
    iter_K->second.second = Return_random_divisor(iter_K->second.first);

    auto iter_R = mDimension_tile2_tile3_map.find(Dimension_type::R);
    iter_R->second.second = iter_R->second.first;

    auto iter_S = mDimension_tile2_tile3_map.find(Dimension_type::S);
    iter_R->second.second = iter_R->second.first;


    auto iter_X = mDimension_tile2_tile3_map.find(Dimension_type::X);   
    iter_X->second.second = mLayer_info_ptr->Get_Dimension_S();  


    auto iter_Y = mDimension_tile2_tile3_map.find(Dimension_type::Y);
    iter_Y->second.second = mLayer_info_ptr->Get_Dimension_R();  
    */

    Dimension_type R2_spatialmap_dimension = Dimension_type::None;

    //Reordering directive
    Dimension_type dimension = Dimension_type::None;
    /*
    std::vector<Dimension_type> dimension_vector = {
        Dimension_type::C,
        Dimension_type::K,
        Dimension_type::Y,
        Dimension_type::X,
        Dimension_type::R,
        Dimension_type::S
    };
    */
   Set_dimension_vector();


    std::random_shuffle(mDimension_vector.begin(), mDimension_vector.end());

    while(!mDimension_vector.empty())
    {
        Dimension_type dimension = Dimension_type::None;
        Mapping_type mapping = Mapping_type::None;
        int size = 0;
        int offset = 0;

        dimension = mDimension_vector.back();
        mDimension_vector.pop_back();
        //std::unordered_map<Dimension_type, std::pair<int,int>>::iterator iter_tile = mDimension_tile2_tile3_map.find(dimension);

        std::list<std::unique_ptr<Directive>>::iterator ptr_upper_region;
        for(ptr_upper_region = mDirective_sequence.begin(); ptr_upper_region != mDirective_sequence.end(); ptr_upper_region++ )
            if( (*ptr_upper_region)->Get_dimension() == dimension)
                break;        

        //mapping
        int spatialmap_count = 0;
        for(auto i = mDirective_sequence.begin(); i != mDirective_sequence.end(); i ++)
            if((*i)->Get_mapping() == Mapping_type::Smap)
                spatialmap_count++;

        if( spatialmap_count < 2 && true_or_false())
        {
            mapping = Mapping_type::Smap;
            R2_spatialmap_dimension  = dimension;
        }
        else
            mapping = Mapping_type::Tmap; 

        if(mDimension_vector.empty() == true && spatialmap_count == 1)          
        {
            mapping = Mapping_type::Smap;
            R2_spatialmap_dimension = dimension;
        }

        switch (dimension)
        {
            case Dimension_type::C:
            case Dimension_type::K:
                size = Return_random_divisor((*ptr_upper_region)->Get_size());
                offset = size;
                break;
            case Dimension_type::X:
                switch(mLayer_info_ptr->Get_stride_x())
                {
                    case 1:
                        {
                        size = mLayer_info_ptr->Get_Dimension_S();
                        offset = 1;
                        }
                        break;
                    case 2:
                        {
                        size = mLayer_info_ptr->Get_Dimension_S();
                        offset = 2;
                        }   
                        break;
                    default:
                        std::cerr<<"CONV init switch error"<<std::endl;
                        exit(EXIT_FAILURE);
                        break;
                }
                
                break;
            case Dimension_type::Y:
   
                switch (mLayer_info_ptr->Get_stride_y())
                {

                    case 1:
                        {
                        size = mLayer_info_ptr->Get_Dimension_R();
                        offset = 1;
                        }
                        break;
                    case 2:
                        {                        
                        size = mLayer_info_ptr->Get_Dimension_R();          
                        offset = 2;
                        }
                        break;
                    default:
                        std::cerr<<"CONV init switch error"<<std::endl;
                        exit(EXIT_FAILURE);
                        break;                        
                }

                break;            

            case Dimension_type::S:
                size = mLayer_info_ptr->Get_Dimension_S();
                offset = mLayer_info_ptr->Get_Dimension_S();
                break;
            case Dimension_type::R:
                size = mLayer_info_ptr->Get_Dimension_R();
                offset = mLayer_info_ptr->Get_Dimension_R();
                break;
            default:
                std::cerr<<"Action5 switch error"<<std::endl;
                exit(EXIT_FAILURE);
                break;
        }
        mDirective_sequence.push_back(std::make_unique<Directive>(size, offset, Cluster_type::None, mapping, dimension ));
    }

    //resizing cluster
    int cluster_size = 0;

    //Finding Spatial mapping in R2 region
    std::list<std::unique_ptr<Directive>>::reverse_iterator R2_iter_spatial;
    for(R2_iter_spatial = mDirective_sequence.rbegin(); R2_iter_spatial != mDirective_sequence.rend(); R2_iter_spatial++)
        if((*R2_iter_spatial)->Get_dimension() == R2_spatialmap_dimension)
            break;

    //Same dimension in R3 region
    std::list<std::unique_ptr<Directive>>::iterator R3_iter;
    for(R3_iter = mDirective_sequence.begin(); R3_iter != mDirective_sequence.end(); R3_iter++)
        if((*R3_iter)->Get_dimension() == (*R2_iter_spatial)->Get_dimension())
            break;

    auto it_cluster = mDirective_sequence.begin();
    for(int i = 0; i < mLayer_info_ptr->Get_Dimension_count(); i++)
        it_cluster++;

    switch((*R2_iter_spatial)->Get_dimension())
    {
        case Dimension_type::C:
        case Dimension_type::K:
        case Dimension_type::R:
        case Dimension_type::S:
            cluster_size = (*R3_iter)->Get_size() / (*R2_iter_spatial)->Get_size();
            break;
        case Dimension_type::X:
            switch(mLayer_info_ptr->Get_stride_x())
            {
                case 1:
                    cluster_size = mX_basis;
                    break;
                case 2:
                    cluster_size = mX_basis / 2;
                    break;
                default:
                    std::cerr<<"Action5 rebuildign dataflow swtich error in cluster size region "<<std::endl;
                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        case Dimension_type::Y:
            switch(mLayer_info_ptr->Get_stride_y())
            {
                case 1:
                    cluster_size = mY_basis;
                    break;
                case 2:
                    cluster_size = mY_basis / 2;
                    break;
                default:
                    std::cerr<<"Action5 rebuildign dataflow swtich error in cluster size region "<<std::endl;
                    exit(EXIT_FAILURE);
                    break;
            }
            break;        
        default:
            std::cerr<<"Action5 rebuildign dataflow swtich error in cluster size region "<<std::endl;
            exit(EXIT_FAILURE);
            break;
    }

    mDirective_sequence.insert(it_cluster, std::make_unique<Directive>(cluster_size, 0, Cluster_type::P , Mapping_type::Cluster, Dimension_type::None));  
       
}
