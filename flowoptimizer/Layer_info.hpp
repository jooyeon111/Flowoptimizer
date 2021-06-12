//20.08.21

#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <list>

#include "table.hpp"
#include "util.hpp"

class Layer_info final{

public:
    //Default constructor
    Layer_info()
        :mLayer_config("Early")
        ,mLayer_type("CONV")
        ,mStride{{Stride_type::Stride_X,1},{Stride_type::Stride_Y,1}}
        ,mDimensions{{Dimension_type::K,32},{Dimension_type::C,3},{Dimension_type::R,3},{Dimension_type::S,3},{Dimension_type::Y,224},{Dimension_type::X,224}}
    {
        /*
        //Setting divisor
        for(auto i = mDimensions.begin(); i != mDimensions.end(); i++)
            for(int j = 1; j <= i->second; j++)
                if((i->second) % j == 0)
                    mDimension_divisor_list[i->first].push_back(j);
                    */
        setting_divisor();
    };
    
    //Constructor
    Layer_info(std::string Layer_config, std::string Layer_type, int stride_x, int stride_y, int K, int C, int R, int S, int Y, int X)
        :mLayer_config(Layer_config)
        ,mLayer_type(Layer_type)
        ,mStride{{Stride_type::Stride_X,stride_x},{Stride_type::Stride_Y, stride_y}}
        ,mDimensions{{Dimension_type::K,K},{Dimension_type::C,C},{Dimension_type::R,R},{Dimension_type::S,S},{Dimension_type::Y,Y},{Dimension_type::X,X}}
    {
        //Setting divisor
        /*
        for(auto i = mDimensions.begin(); i != mDimensions.end(); i++)
            for(int j = 1; j <= i->second; j++)
                if((i->second) % j == 0)
                    mDimension_divisor_list[i->first].push_back(j);
        */


       
    }
    ~Layer_info() = default;//Deconstructor
    Layer_info(const Layer_info&) = delete;//Deleting implied function
    Layer_info& operator=(const Layer_info& ) = delete;//Deleting implied function

    void setting_divisor(void)
    {
        mDimension_divisor_list.clear();
        for(auto i = mDimensions.begin(); i != mDimensions.end(); i++)
            for(int j = 1; j <= i->second; j++)
                if((i->second) % j == 0)
                    mDimension_divisor_list[i->first].push_back(j);        
    }


    //will be changed by inline functino later
    std::string Get_layer_config(void) const
    {
        return mLayer_config;
    }

    std::string Get_layer_type(void) const
    {
        return mLayer_type;
    }

    int Get_stride_x(void) const
    {
        
        return mStride.find(Stride_type::Stride_X)->second;
    }

    int Get_stride_y(void) const
    {
        return mStride.find(Stride_type::Stride_Y)->second;
    }

    int Get_Dimension_K(void) const
    {
        return mDimensions.find(Dimension_type::K)->second;
    }

    int Get_Dimension_C(void) const
    {
        return mDimensions.find(Dimension_type::C)->second;
    }

    int Get_Dimension_R(void) const
    {
        return mDimensions.find(Dimension_type::R)->second;
    }

    int Get_Dimension_S(void) const
    {
        return mDimensions.find(Dimension_type::S)->second;
    }
    
    int Get_Dimension_Y(void) const
    {
        return mDimensions.find(Dimension_type::Y)->second;
    }

    int Get_Dimension_X(void) const
    {
        return mDimensions.find(Dimension_type::X)->second;
    }    

    int Get_Dimension(Dimension_type dimension) const
    {
        return mDimensions.find(dimension)->second;
    }

    int Get_Dimension_count(void) const
    {
        int count;

        if(mLayer_type == "CONV")
            count = 6;
        else if(mLayer_type== "DSCONV")
            count = 5;

        return count;
    }

    long Get_product_term_dimension(void) const
    {

        long minimum_cycles = 1;

        if(mStride.find(Stride_type::Stride_X)->second != mStride.find(Stride_type::Stride_Y)->second )
        {
            std::cerr<<"[Layer info head] Stride is not correct "<<std::endl;
            exit(EXIT_FAILURE);
        }

        switch(mStride.find(Stride_type::Stride_X)->second)
        {
            case 1:
                for(auto iter = mDimensions.begin(); iter != mDimensions.end(); iter++)
                    minimum_cycles = minimum_cycles * iter->second;        
                break;
            case 2:
                for(auto iter = mDimensions.begin(); iter != mDimensions.end(); iter++)
                    minimum_cycles = minimum_cycles * iter->second;        
                    minimum_cycles = minimum_cycles / 4;
                break;

        }        
        
        return minimum_cycles;
    }

// added part 20.10.14
    void Set_Dimension_X(int X_value)
    {
        mDimensions.find(Dimension_type::X)->second = X_value;
    }

    void Set_Dimension_Y(int Y_value)
    {
        mDimensions.find(Dimension_type::Y)->second = Y_value;
    }
//////////

    void Reordering_C_K(void)
    {
        mDimension_divisor_list.clear();
        //Reordering dimensions
        //C<->K
        int imsi;
        auto iter_C = mDimensions.find(Dimension_type::C);
        auto iter_K = mDimensions.find(Dimension_type::K);

        imsi = iter_C->second;
        iter_C->second = iter_K->second;
        iter_K->second = imsi;

        //Reset divisor list
        for(auto i = mDimensions.begin(); i != mDimensions.end(); i++)
            for(int j = 1; j <= i->second; j++)
                if((i->second) % j == 0)
                    mDimension_divisor_list[i->first].push_back(j);        
    }

    void Print_all(void) const
    {
        std::cout<<"Layer_info configuration"<<std::endl;
        std::cout<<mLayer_config<<std::endl;
        std::cout<<"Layer type"<<std::endl;
        std::cout<<mLayer_type<<std::endl;        

        std::cout<<"Dimension"<<std::endl;
        for(auto iter_dimension = mDimensions.begin(); iter_dimension != mDimensions.end(); iter_dimension++)
        {
            std::cout<<Dimension_type_to_string( iter_dimension->first )<<" : "<<iter_dimension->second<<std::endl;
        }
        std::cout<<std::endl;

        std::cout<<"Stride"<<std::endl;
        for(auto iter_stride = mStride.begin(); iter_stride != mStride.end(); iter_stride++)
        {
            std::cout<<Stride_type_to_string(iter_stride->first)<<" : "<<iter_stride->second<<std::endl;
        }
        std::cout<<std::endl;        


        std::cout<<"Dimension divisors"<<std::endl;
        for(auto iter_dimension_divisor = mDimension_divisor_list.begin(); iter_dimension_divisor != mDimension_divisor_list.end(); iter_dimension_divisor++)
        {
            std::cout<<Dimension_type_to_string(iter_dimension_divisor->first)<<" : ";
            for(auto iter_dimension_divisor_list =  iter_dimension_divisor->second.begin(); iter_dimension_divisor_list != iter_dimension_divisor->second.end(); iter_dimension_divisor_list++ )
                std::cout<<(*iter_dimension_divisor_list)<<" ";
            std::cout<<std::endl;
        }
        std::cout<<std::endl;  
              
    }

private:
    
    std::string mLayer_config;
    std::string mLayer_type;    
    std::unordered_map<Stride_type,int> mStride;
    std::unordered_map<Dimension_type, int> mDimensions;
    std::unordered_map<Dimension_type, std::list<int>> mDimension_divisor_list;
    

};
