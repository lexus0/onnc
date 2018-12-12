//===- SkyGlobal.h --------------------------------------------------------===//
//
//                               Skymizer
//
// Copyright (C) 2013, Skymizer Inc.. All rights reserved.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ANALYSIS_GLOBALSTATISTICS_H
#define ONNC_ANALYSIS_GLOBALSTATISTICS_H
#include <onnc/Analysis/Statistics.h>
#include <onnc/Analysis/StatisticsGroup.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

namespace global {

Statistics* stat();
class Scalar
{
  public:
    Scalar(const std::string& pScalarName);
    Scalar& operator=(int intValue){
      StatisticsGroup tmpGroup = global::stat()->group("global_scalar");
      tmpGroup.writeEntry(m_ScalarName, intValue);
      return *this;
    }
    Scalar& operator++(){
      StatisticsGroup tmpGroup = global::stat()->group("global_scalar");
      errs() << "value: " << tmpGroup.readEntry(m_ScalarName, 0) << std::endl;
      errs() << "name: " << name() << std::endl;
      tmpGroup.writeEntry(m_ScalarName, tmpGroup.readEntry(m_ScalarName, 0)+1);
      return *this;
    }
    const std::string& name() const{
      return m_ScalarName;
    }
    //TODO: impelemnt ++ for postfix
  
  private:
    std::string m_ScalarName;
};


} // namespace of global


/// Clear global::config()
void ClearStats();


} // namespace of skymizer

#endif

