#include "util.hpp"

int Random_number(int limit)
{

      int value = 0;

      if(limit == 1)
            return 1;
      
      while(value == 0)
      {
            value = rand() % limit;
      }

      return value;
}

int Random_number_from_zero(int limit)
{
    int value = 0;

    value = rand() % limit;
    return value;
}

bool true_or_false(void)
{
      if( rand() % 2  == 1)
            return false;
      else
            return true;
}

int Return_random_divisor (int integer)
{
      std::list<int> divisor_list;

      int i;
      for( i = 1; i <= integer; i++)
      {
            if(integer % i == 0)
                  divisor_list.push_back(i);
      }

      std::list<int>::iterator it = divisor_list.begin();
      int n = Random_number_from_zero(divisor_list.size());
      std::advance(it, n);

      return *it;      
}



