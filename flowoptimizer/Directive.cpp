#include "Directive.hpp"


Directive::Directive(int Size, int Offset, Cluster_type Type, Mapping_type Mapping, Dimension_type Dimension)
    :mSize(Size)
    ,mOffset(Offset)
    ,mType(Type)
    ,mMapping(Mapping)
    ,mDimension(Dimension)
    
{
    //Classifiying Maping code will be added
}

Directive::Directive(const Directive& Another)
    :mSize(Another.mSize)
    ,mOffset(Another.mOffset)
    ,mType(Another.mType)
    ,mMapping(Another.mMapping)
    ,mDimension(Another.mDimension)
{
}

int Directive::Get_size(void) const
{
    return mSize;
}

int Directive::Get_offsset(void) const
{
    return mOffset;
}

Cluster_type Directive::Get_type(void) const
{
    return mType;
}

Mapping_type Directive::Get_mapping(void) const
{
    return mMapping;
}

Dimension_type Directive::Get_dimension(void) const
{
    return mDimension;
}

void Directive::Set_size_offset(int size, int offset)
{
    mSize = size;
    mOffset = offset;
}

void Directive::Set_size(int size)
{
    mSize = size;
}

void Directive::Set_offset(int offset)
{
    mOffset = offset;
}

void Directive::Set_mapping(Mapping_type Mapping){
    mMapping = Mapping;
}

std::string Directive::Print_directive(void) const
{
    std::string s1;
    if(mType == Cluster_type::None)//Temporal or Spatial
    {
        s1 = Mapping_type_to_string(mMapping) + "(" + std::to_string(mSize) + "," + std::to_string(mOffset) + ") " + Dimension_type_to_string(mDimension) + ";";
        return s1;
    }
    else if (mType != Cluster_type::None ) //Cluster
    {
        s1 = Mapping_type_to_string(mMapping) + "(" + std::to_string(mSize) + "," + Cluster_type_to_string(mType) + ");";
        return s1;
    }
}
