//===- TGBackend.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_TG_TG_BACKEND_H
#define TARGET_TG_TG_BACKEND_H
#include "Operator.h"
#include "TGCodeEmitter.h"
#include "TargetLowering.h"
#include <memory>
#include <onnc/Support/Path.h>
#include <onnc/Target/DLATargetBackend.h>
#include <onnx/common/ir.h>
#include <string>
#include <vector>

namespace onnc {

class TargetLowering;
class TGCodeEmitter;

using MemTable = std::map<std::string, uint64_t>;

class TGBackend : public DLATargetBackend
{
public:
  TGBackend(TargetLowering *pTLI, TGCodeEmitter *pCE,
            const TargetOptions &pOptions);

  virtual ~TGBackend();

  void codeEmit();

  void addTensorSel(PassManager &pPM);

  void addCodeEmit(PassManager& pPM, const Path& pOutputFile);

  MemTable &getMemLayout() { return m_globalMemLayout; }

  std::vector<std::unique_ptr<Operator> > &getInsts() { return m_instructions; }

  TargetLowering *getTargetLowering() { return m_pTLI; }

  // default sizeof function
  virtual size_t sizeOfTensorType(::onnx::TensorProto_DataType pType);

  // backend can descript which tensor types are supported
  virtual bool isNativeTensorType(::onnx::TensorProto_DataType pType);

  // calibration table name
  virtual std::string getCtableName() { return ""; }

  // load ctable from onx meta data
  const std::string &getCtable(const Module &pModule);

  // for debug usage
  virtual std::string getBackendName() {
    return "TGBackend";
  };

private:
  std::vector<std::unique_ptr<Operator> > m_instructions;
  MemTable m_globalMemLayout;
  TargetLowering *m_pTLI;
  TGCodeEmitter *m_pCE;
  Path m_outputPath;
};

}  // namespace onnc

#endif  // TARGET_TG_TG_BACKEND_H
