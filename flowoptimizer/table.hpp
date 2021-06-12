
#pragma once
#include <map>
#include <string>


//Stride_type
enum class Stride_type { Stride_X, Stride_Y};
std::string Stride_type_to_string (Stride_type e);

//Cluster_type
enum class Cluster_type { None, P, L, };
std::string Cluster_type_to_string (Cluster_type e);

//Mapping_type
enum class Mapping_type { None, Cluster, Tmap, Smap };
std::string Mapping_type_to_string (Mapping_type e);

//Dimension_type
enum class Dimension_type { None, C, K , Y , X , R , S };
std::string Dimension_type_to_string (Dimension_type e);
