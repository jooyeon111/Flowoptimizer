//20.08.21

#pragma once

#include <iostream>
#include <memory>

#include "table.hpp"

class Directive final {
public:

    //Default constructor
    Directive() = delete;
    Directive(int Size, int Offset, Cluster_type Type, Mapping_type Mapping, Dimension_type Dimension);
    ~Directive() = default;//Deconstructor
    Directive(const Directive& Another);
    Directive& operator=(const Directive& ) = delete;//Deleting implied function


    int Get_size(void) const;
    int Get_offsset(void) const;
    Cluster_type Get_type(void) const;
    Mapping_type Get_mapping(void) const;
    Dimension_type Get_dimension(void) const;

    void Set_size_offset(int size, int offset);
    void Set_size(int size);
    void Set_offset(int offset);
    void Set_mapping(Mapping_type);
    
    std::string Print_directive(void) const;
    

private:
    int mSize;
    int mOffset;
    Cluster_type mType; //Physical or Logical
    Mapping_type mMapping; //Spatial Temporatl Cluster
    Dimension_type mDimension; // RS XY KC
    
};

