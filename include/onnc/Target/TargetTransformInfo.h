//===- TargetTransformInfo.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TRANSFORM_INFO_H
#define ONNC_TARGET_TRANSFORM_INFO_H
#include <onnc/Config/ONNX.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/Target/TargetMemInfo.h>
#include <vector>

namespace onnc {

using LongInts = std::vector<int64_t>;

/** \class ExeResource
 *  \brief Define a coarse grained execution resource which is used by scheduler
 *         when modeling.
 */
struct ExeResource
{
  unsigned numUnits;
  const char *name;
};

/** \class TargetTransformInfo
 *  \brief TargetTransformInfo provides interfaces to explore target
 *         information which is used by target dependent passes.
 */
class TargetTransformInfo
{
public:
  enum TargetCostKind : unsigned {
    kCycleCount, ///< Get graph (or compute) IR cycle count.
    BUILTIN_COST_KIND_END
  };

  /// Get coarse-grained (approximately) cost of onnx node.
  virtual uint64_t getOperatorCost(const xNode *pNode, unsigned pKind) const {
    return 0;
  }

  /// Get coarse-grained (approximately) cost of onnc IR.
  virtual uint64_t getOperatorCost(const ComputeOperator *pOp,
                                   unsigned pKind) const {
    return 0;
  }

  /// Get coarse-grained (approximately) total memory usage of onnx node.
  ///
  /// Memory usage of onnx node is target dependent. E.g. a gemm node with
  /// A [N x K], B [K x M], C [N x M], its total memory usage is
  /// (N * K + K * N + N * M) * (Element Size), however, target's might
  /// choose to load part of rows and columns to compute a tile, so its
  /// real memory usage will be reduced a lot.
  virtual MemSize getOperatorMemUsage(const xNode *pNode) const {
    return MemSize();
  }

  /// @see xNode version of getOperatorMemUsage.
  virtual MemSize getOperatorMemUsage(const ComputeOperator *pOp) const {
    return MemSize();
  }

  virtual MemSize
  getOperatorMemUsage(const xNode *pNode,
                      const std::vector<LongInts> &pInputSizes,
                      const std::vector<LongInts> &pOutputSizes) const {
    return MemSize();
  }

  /// @param pIdx Get required memory size of input[pIdx].
  /// @param pNewInputSize Based on new input size, get actual required size.
  virtual MemSize getOperatorInputMemUsage(const xNode *pNode,
                                           unsigned pIdx,
                                           const LongInts &pNewInputSize) const
  {
    return MemSize();
  }

  virtual MemSize getOperatorOutputMemUsage(const xNode *pNode,
                                unsigned pIdx,
                                const LongInts &pNewOutputSize) const {
    return MemSize();
  }

  /// Expose coarse grained execution units information, so scheduler can use
  /// it to scheduling graph IR.
  virtual const ExeResource *queryExeResType(const xNode *pNode) const {
    return nullptr;
  }

  virtual const ExeResource *
  queryExeResType(const ComputeOperator *pOp) const { return nullptr; }

  virtual int getWarpSize() const { return 0; }

  virtual int getProcessingUnitCount() const { return 0; }
};

} // namespace of onnc

#endif
