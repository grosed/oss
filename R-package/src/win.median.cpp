
#include <Rcpp.h>

using namespace Rcpp;

#include <vector>
#include "oss.h"

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


#include <iostream>

//[[Rcpp::export]]
std::vector<double> win_median(const std::vector<double> X,const int& win_size)
{
  oss_type S;
  std::vector<double> medians(X.size());
  std::transform(X.begin(),X.end(),medians.begin(),
		 [&S,&win_size](auto& x)
		 {
		   S = push_back(std::move(S),x);
		   auto n = S.D.size();
		   if(n > win_size)
		     {
		       S = std::move(std::get<0>(pop_front(std::move(S))));
		     }
		   n = S.D.size();
		   return 0.5*(at(S,(n+1)/2 - 1).value() + at(S,n/2).value()); 
		 });
  return medians;
}
