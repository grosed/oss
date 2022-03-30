
   
#include <Rcpp.h>


using namespace Rcpp;


#include <iostream>
#include <vector>

#include <oss.h>

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
//template <typename ValType>
//using DequeType = std::deque<ValType>;

#include <boost/circular_buffer.hpp>
template <typename ValType>
using DequeType = boost::circular_buffer<ValType>;


// simplify reference to the oss type
typedef oss<double,DequeType,OrderStatisticTreeType> oss_type;
