#include "table.hpp"

std::string Stride_type_to_string (Stride_type e)
{
    const std::map<Stride_type,std::string> MyStrideMap ({
        { Stride_type::Stride_X, "X" },
        { Stride_type::Stride_Y, "Y" }        
    });

    auto   it  = MyStrideMap.find(e);
    return it == MyStrideMap.end() ? "Out of range" : it->second;    
}


std::string Cluster_type_to_string (Cluster_type e)
{
    
    const std::map<Cluster_type,std::string> MyClusterMap ({
        { Cluster_type::None, "None"},
        { Cluster_type::L, "L" },
        { Cluster_type::P, "P" }        
    });

    auto   it  = MyClusterMap.find(e);
    return it == MyClusterMap.end() ? "Out of range" : it->second;
}

std::string Mapping_type_to_string (Mapping_type e)
{
    
    const std::map<Mapping_type,std::string> MyMappingMap ({
        { Mapping_type::None, "None"},
        { Mapping_type::Cluster, "Cluster"},
        { Mapping_type::Tmap, "TemporalMap" },
        { Mapping_type::Smap, "SpatialMap" }        
    });


    auto   it  = MyMappingMap.find(e);
    return it == MyMappingMap.end() ? "Out of range" : it->second;
}

std::string Dimension_type_to_string (Dimension_type e)
{
    
    const std::map<Dimension_type,std::string> MyDimensionMap ({
        { Dimension_type::None, "None"},
        { Dimension_type::C, "C" },
        { Dimension_type::K, "K" },
        { Dimension_type::Y, "Y" },
        { Dimension_type::X, "X" },
        { Dimension_type::R, "R" },
        { Dimension_type::S, "S" }
    });

    auto   it  = MyDimensionMap.find(e);
    return it == MyDimensionMap.end() ? "Out of range" : it->second;
}