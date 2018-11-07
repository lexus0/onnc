//===- CountOperatorsPass.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Module.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Statistics/CountOperatorsPass.h>

#include <algorithm>
#include <iomanip>
#include <unordered_map>

using namespace onnc;

//===----------------------------------------------------------------------===//
// CountOperatorsPass
//===----------------------------------------------------------------------===//
Pass::ReturnType CountOperatorsPass::runOnModule(Module &pModule)
{
  std::unordered_map<std::string, int> &count = *this;
  size_t op_len = 8;

  for (ComputeOperator &cm : *pModule.getRootComputeGraph()) {
    if (dyn_cast<InputOperator>(&cm)) continue;
    if (dyn_cast<Initializer>(&cm)) continue;
    if (dyn_cast<OutputOperator>(&cm)) continue;
    onnc::StringRef name = cm.name(); 
    count[name] += 1;
    op_len = std::max(op_len, name.size());
    ++m_Total;
  }

  m_Width.first = op_len;
  m_Width.second = ((m_Total > 99999) ? 10 : 5) + 1;

  // TODO: Use Statistics facility to print.
  // Move this print into LinearScanMemAlloc
  //print(outs(), nullptr);
  return Pass::kModuleNoChanged;
}


std::pair<int, int> CountOperatorsPass::printHeader(OStream &pOS) const {
  pOS << m_Prefix << std::setw(m_Width.first) << "Operator" << SEP
         << std::setw(m_Width.second) << "Count" << std::endl;
  printSeparator(pOS, m_Width);
  return m_Width;
}
void CountOperatorsPass::printFooter(OStream &pOS) const {
  printSeparator(pOS, m_Width);
  pOS << m_Prefix << std::setw(m_Width.first) << "Total" << SEP
      << std::setw(m_Width.second) << m_Total << std::endl;
}


//===----------------------------------------------------------------------===//
// CountOperatorsPass Factory method
//===----------------------------------------------------------------------===//
char CountOperatorsPass::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(CountOperatorsPass, "CountOperatorsPass")
}

CountOperatorsPass *onnc::CreateCountOperatorsPass(const std::string &pPrefix) {
  return new CountOperatorsPass(pPrefix);
}
