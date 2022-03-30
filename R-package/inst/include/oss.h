
#ifndef ___OSS_H___
#define ___OSS_H__

#include <optional>
#include <tuple>

template<typename ValType,
         template<class> class DequeType,
         template<class> class OrderStatisticTreeType>
struct oss
{
  typedef ValType value_type;
  OrderStatisticTreeType<ValType> T;
  DequeType<ValType> D; 
};

template<typename T>
concept fillable = requires (T x) { {x.D.full()} -> bool; };


template <class OSSType>
auto push_front(OSSType S, const typename OSSType::value_type& val)
{
  S.D.push_front(val);
  S.T.insert(val);
  return(std::move(S));
}

template <class OSSType>
auto push_back(OSSType S, const typename OSSType::value_type& val)
{
  S.D.push_back(val);
  S.T.insert(val);
  return(std::move(S));
}


template <class OSSType>
requires fillable<OSSType>
auto push_back(OSSType S, const typename OSSType::value_type& val)
{
  if(S.D.full())
    {
      S.T.erase(S.D.front());
    }
  S.D.push_back(val);
  S.T.insert(val);
  return(std::move(S));
}


template <class OSSType>
std::tuple<OSSType,std::optional<typename OSSType::value_type> > pop_back(OSSType S)
{
  if(S.T.size() == 0)
    {
      return std::make_tuple(S,std::nullopt);
    }
  auto it = S.D.rbegin();
  auto popped = *it;
  S.T.erase(*it);
  S.D.pop_back();
  return std::make_tuple(std::move(S),std::optional<typename OSSType::value_type>{popped});
}


template <class OSSType>
std::tuple<OSSType,std::optional<typename OSSType::value_type> > pop_front(OSSType S)
{
  if(S.T.size() == 0)
    {
      return std::make_tuple(S,std::nullopt);
    }
  auto it = S.D.begin();
  auto popped = *it;
  S.T.erase(*it);
  S.D.pop_front();
  return std::make_tuple(std::move(S),std::optional<typename OSSType::value_type>{popped});
}


template <class OSSType>
auto rank(const OSSType& S, const typename OSSType::value_type& val)
{
  auto it = S.T.find(val);
  return it == S.T.end() ? std::nullopt : std::optional<int>{S.T.rank(it)};
}


template <class OSSType>
auto bisect(const OSSType& S, const typename OSSType::value_type& val)
{
  auto pos = S.T.upper_bound(val);
  return S.T.size() == 0 ? std::make_tuple(0,val) : *pos < val ? std::make_tuple((int)S.T.size(),val) : std::make_tuple((int)S.T.rank(pos),*pos); 
}

template <class OSSType>
auto at(const OSSType& S, const int& index)
{
  auto it = S.T.nth(index);
  return it == S.T.end() ? std::nullopt : std::optional<double>{*it};
}


template <class OSSType>
auto resize(OSSType S, const int& size)
{
  S.D.resize(size);
  return std::move(S);
}




#endif
