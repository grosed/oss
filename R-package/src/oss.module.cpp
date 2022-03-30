
#include "Rcpp.h"

#include "oss.h"

#include <vector>

// use a boost multi index container as an ordered statstics tree
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ranked_index.hpp>
using namespace ::boost;
using namespace ::boost::multi_index;
template <typename ValType>
using OrderStatisticTreeType = multi_index_container<ValType,indexed_by<ranked_non_unique<identity<ValType> > > >;

// use std::deque as a deque
#include <deque>
template <typename ValType>
using DequeType = std::deque<ValType>;

// simplify reference to the oss type
typedef oss<double,DequeType,OrderStatisticTreeType> oss_type;


// non-pure wrapper functions
void push_back_wrapper(oss_type* oss,const std::vector<double>& vals)
{
  for(auto& val : vals)
    {
      *oss = push_back(std::move(*oss),val);
    }
}

void pop_back_wrapper(oss_type* oss)
{
  auto result = pop_back(std::move(*oss));
  *oss = std::move(std::get<0>(result));
}

void push_front_wrapper(oss_type* oss,const std::vector<double>& vals)
{
    for(auto& val : vals)
    {
      *oss = push_front(std::move(*oss),val);
    }
}

void pop_front_wrapper(oss_type* oss)
{
  auto result = pop_front(std::move(*oss));
  *oss = std::move(std::get<0>(result));
}

SEXP at_wrapper(oss_type* oss,const int& index)
{
  if(auto result = at(*oss,index-1))
    {
      return Rcpp::wrap(*result);
    }
  else
    {
      return R_NilValue;
    }
}

SEXP rank_wrapper(oss_type* oss, const double& val)
{
  if(auto result = rank(*oss,val))
    {
      return Rcpp::wrap(*result + 1);
    }
  else
    {
      return R_NilValue;
    }
}

// extensions to the oss interface
std::vector<double> as_vector_extension(oss_type* oss)
{
  return std::vector<double>(oss->D.begin(), oss->D.end());
}

std::vector<double> as_sorted_vector_extension(oss_type* oss)
{
  return std::vector<double>(oss->T.begin(), oss->T.end());
}

int size_extension(oss_type* oss)
{
  return oss->D.size();
}




// create an Rcpp module to wrap the oss type
RCPP_MODULE(oss)
{
  using namespace Rcpp ;

  // expose the oss struct and its default constructor
  class_<oss_type>("oss")
  .constructor()  

  // expose methods
  .method("push_back", &push_back_wrapper)
  .method("pop_back", &pop_back_wrapper)
  .method("push_front", &push_front_wrapper)
  .method("pop_front", &pop_front_wrapper)
  .method("rank", &rank_wrapper)
  .method("at", &at_wrapper)
  .method("as.vector", &as_vector_extension)
  .method("as.sorted.vector", &as_sorted_vector_extension)
  .method("size", &size_extension)

  ;
}
