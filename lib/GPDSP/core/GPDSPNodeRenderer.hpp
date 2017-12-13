/*
**      General Purpose DSP Library
**
**      Original Copyright (C) 2017 - 2017 HORIGUCHI Junshi.
**                                          http://iridium.jp/
**                                          zap00365@nifty.com
**      Portions Copyright (C) <year> <author>
**                                          <website>
**                                          <e-mail>
**      Version     POSIX / C++11
**      Website     http://iridium.jp/
**      E-mail      zap00365@nifty.com
**
**      This source code is for Xcode.
**      Xcode 9.0 (Apple LLVM 9.0.0)
**
**      GPDSPNodeRenderer.hpp
**
**      ------------------------------------------------------------------------
**
**      The MIT License (MIT)
**
**      Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
**      associated documentation files (the "Software"), to deal in the Software without restriction,
**      including without limitation the rights to use, copy, modify, merge, publish, distribute,
**      sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
**      furnished to do so, subject to the following conditions:
**      The above copyright notice and this permission notice shall be included in all copies or
**      substantial portions of the Software.
**      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
**      BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
**      IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
**      WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
**      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
**      以下に定める条件に従い、本ソフトウェアおよび関連文書のファイル（以下「ソフトウェア」）の複製を
**      取得するすべての人に対し、ソフトウェアを無制限に扱うことを無償で許可します。
**      これには、ソフトウェアの複製を使用、複写、変更、結合、掲載、頒布、サブライセンス、および、または販売する権利、
**      およびソフトウェアを提供する相手に同じことを許可する権利も無制限に含まれます。
**      上記の著作権表示および本許諾表示を、ソフトウェアのすべての複製または重要な部分に記載するものとします。
**      ソフトウェアは「現状のまま」で、明示であるか暗黙であるかを問わず、何らの保証もなく提供されます。
**      ここでいう保証とは、商品性、特定の目的への適合性、および権利非侵害についての保証も含みますが、それに限定されるものではありません。
**      作者または著作権者は、契約行為、不法行為、またはそれ以外であろうと、ソフトウェアに起因または関連し、
**      あるいはソフトウェアの使用またはその他の扱いによって生じる一切の請求、損害、その他の義務について何らの責任も負わないものとします。
*/

#ifndef __GPDSPNODERENDERER_HPP
#define __GPDSPNODERENDERER_HPP

#include <unordered_map>
#include "tinyxml2/tinyxml2.h"
#include "../synthesis/GPDSPSinWaveNode.hpp"
#include "../synthesis/GPDSPTriangleWaveNode.hpp"
#include "../synthesis/GPDSPSawtoothWaveNode.hpp"
#include "../synthesis/GPDSPSquareWaveNode.hpp"

namespace ir {

class GPDSPSerializable;
class GPDSPFlexInputtableNode;
class GPDSPFlexOutputtableNode;
class GPDSPBufferInputNode;
class GPDSPBufferOutputNode;
class GPDSPConstantNode;
class GPDSPSignNode;
class GPDSPGateNode;
class GPDSPPeekNode;
class GPDSPAmplifyNode;
class GPDSPDelayNode;
class GPDSPBufferNode;
class GPDSPSumNode;
class GPDSPMultiplyNode;
class GPDSPSquareRootNode;
class GPDSPGenericNode;

class GPDSPNodeRenderer {
    private:
                int                         _rate;
                std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >
                                            _node;
                std::vector<GPDSPNode*>     _wait;
                std::vector<std::pair<GPDSPNode*, GPDSPError> >
                                            _sequence;
                std::vector<std::pair<GPDSPNode*, GPDSPError> >::iterator
                                            _jit;
        mutable std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator
                                            _nit;
    
    public:
        explicit                            GPDSPNodeRenderer           (void);
                                            ~GPDSPNodeRenderer          (void);
                GPDSPError                  setRate                     (int rate);
                int                         getRate                     (void) const;
                std::shared_ptr<GPDSPBufferInputNode>
                                            getNodeBufferInput          (std::string const& name) const;
                std::shared_ptr<GPDSPBufferOutputNode>
                                            getNodeBufferOutput         (std::string const& name) const;
                std::shared_ptr<GPDSPConstantNode>
                                            getNodeConstant             (std::string const& name) const;
                std::shared_ptr<GPDSPSignNode>
                                            getNodeSign                 (std::string const& name) const;
                std::shared_ptr<GPDSPGateNode>
                                            getNodeGate                 (std::string const& name) const;
                std::shared_ptr<GPDSPPeekNode>
                                            getNodePeek                 (std::string const& name) const;
                std::shared_ptr<GPDSPAmplifyNode>
                                            getNodeAmplify              (std::string const& name) const;
                std::shared_ptr<GPDSPDelayNode>
                                            getNodeDelay                (std::string const& name) const;
                std::shared_ptr<GPDSPBufferNode>
                                            getNodeBuffer               (std::string const& name) const;
                std::shared_ptr<GPDSPSumNode>
                                            getNodeSum                  (std::string const& name) const;
                std::shared_ptr<GPDSPMultiplyNode>
                                            getNodeMultiply             (std::string const& name) const;
                std::shared_ptr<GPDSPSquareRootNode>
                                            getNodeSquareRoot           (std::string const& name) const;
                std::shared_ptr<GPDSPSinWaveNode>
                                            getNodeSinWave              (std::string const& name) const;
                std::shared_ptr<GPDSPTriangleWaveNode>
                                            getNodeTriangleWave         (std::string const& name) const;
                std::shared_ptr<GPDSPSawtoothWaveNode>
                                            getNodeSawtoothWave         (std::string const& name) const;
                std::shared_ptr<GPDSPSquareWaveNode>
                                            getNodeSquareWave           (std::string const& name) const;
                std::shared_ptr<GPDSPGenericNode>
                                            getNodeGeneric              (std::string const& name) const;
                std::shared_ptr<GPDSPNode>  getNode                     (std::string const& name) const;
                int                         getSize                     (void) const;
                GPDSPError                  setCountI                   (std::string const& name, int count, std::string const& what);
                GPDSPError                  getCountI                   (std::string const& name, int* count) const;
                GPDSPError                  setCountO                   (std::string const& name, int count, std::string const& what);
                GPDSPError                  getCountO                   (std::string const& name, int* count) const;
                GPDSPError                  setNameI                    (std::string const& name, int index, std::string const& what);
                GPDSPError                  getNameI                    (std::string const& name, int index, std::string* what) const;
                GPDSPError                  setNameO                    (std::string const& name, int index, std::string const& what);
                GPDSPError                  getNameO                    (std::string const& name, int index, std::string* what) const;
                GPDSPError                  getModeI                    (std::string const& name, int index, GPDSPMode* mode) const;
                GPDSPError                  setLinkPositiveI            (std::string const& name, int index, std::string const& from, int which);
                GPDSPError                  setLinkNegativeI            (std::string const& name, int index, std::string const& from, int which);
                GPDSPError                  setLinkConstantI            (std::string const& name, int index, GPDSPFloat constant);
                GPDSPError                  getLinkI                    (std::string const& name, int index, std::string* from, int* which) const;
                GPDSPError                  getLinkI                    (std::string const& name, int index, GPDSPFloat* constant) const;
                GPDSPError                  clearLinkI                  (std::string const& name, int index);
                GPDSPError                  clearLinkI                  (std::string const& name, GPDSPMode mode);
                GPDSPError                  clearLinkI                  (std::string const& name, std::string const& from, int which);
                GPDSPError                  clearLinkI                  (std::string const& name, std::string const& from);
                GPDSPError                  clearLinkI                  (std::string const& name);
                GPDSPError                  clearLinkO                  (std::string const& name, int index);
                GPDSPError                  clearLinkO                  (std::string const& name);
                GPDSPError                  getValueI                   (std::string const& name, int index, GPDSPFloat* value) const;
                GPDSPError                  getValueO                   (std::string const& name, int index, GPDSPFloat* value) const;
                std::string                 getNextNode                 (void) const;
                bool                        hasNextNode                 (void) const;
                std::string                 findNode                    (std::shared_ptr<GPDSPNode const> const& node) const;
                GPDSPError                  findNameI                   (std::string const& name, std::string const& what, int* index) const;
                GPDSPError                  findNameO                   (std::string const& name, std::string const& what, int* index) const;
                GPDSPError                  findLinkI                   (std::string const& name, GPDSPMode mode, int* index) const;
                GPDSPError                  findLinkI                   (std::string const& name, std::string const& from, int which, int* index) const;
                GPDSPError                  findLinkI                   (std::string const& name, std::string const& from, int* index) const;
                GPDSPError                  newNodeBufferInput          (std::string const& name, GPDSPFloat const* buffer, int length, int interleave);
                GPDSPError                  newNodeBufferOutput         (std::string const& name, GPDSPFloat* buffer, int length, int interleave);
                GPDSPError                  newNodeConstant             (std::string const& name, GPDSPFloat constant);
                GPDSPError                  newNodeSign                 (std::string const& name);
                GPDSPError                  newNodeGate                 (std::string const& name, GPDSPFloat minimum, GPDSPFloat maximum);
                GPDSPError                  newNodePeek                 (std::string const& name);
                GPDSPError                  newNodeAmplify              (std::string const& name, GPDSPFloat gain);
                GPDSPError                  newNodeDelay                (std::string const& name);
                GPDSPError                  newNodeBuffer               (std::string const& name, int size);
                GPDSPError                  newNodeSum                  (std::string const& name, int count);
                GPDSPError                  newNodeMultiply             (std::string const& name, int count);
                GPDSPError                  newNodeSquareRoot           (std::string const& name);
                GPDSPError                  newNodeSinWave              (std::string const& name, GPDSPFloat resolution = GPDSPSinWaveNode::defaultResolution());
                GPDSPError                  newNodeTriangleWave         (std::string const& name, GPDSPFloat resolution = GPDSPTriangleWaveNode::defaultResolution());
                GPDSPError                  newNodeSawtoothWave         (std::string const& name, GPDSPFloat resolution = GPDSPSawtoothWaveNode::defaultResolution());
                GPDSPError                  newNodeSquareWave           (std::string const& name, GPDSPFloat resolution = GPDSPSquareWaveNode::defaultResolution());
                GPDSPError                  newNodeGeneric              (std::string const& name, std::string const& file);
                GPDSPError                  newNode                     (std::string const& name, std::shared_ptr<GPDSPNode> const& node);
                GPDSPError                  appendI                     (std::string const& name, std::string const& what);
                GPDSPError                  appendO                     (std::string const& name, std::string const& what);
                GPDSPError                  insertI                     (std::string const& name, int index, std::string const& what);
                GPDSPError                  insertO                     (std::string const& name, int index, std::string const& what);
                GPDSPError                  deleteNode                  (std::string const& name);
                GPDSPError                  removeI                     (std::string const& name, int index);
                GPDSPError                  removeO                     (std::string const& name, int index);
                void                        clearNode                   (void);
                GPDSPError                  clearI                      (std::string const& name);
                GPDSPError                  clearO                      (std::string const& name);
                void                        iterateNode                 (void) const;
                void                        invalidate                  (void);
                GPDSPError                  prepare                     (void);
                GPDSPError                  process                     (void);
                GPDSPError                  render                      (int count, int* remain = NULL);
                GPDSPError                  rewind                      (std::string const& name);
                void                        rewind                      (void);
                GPDSPError                  refresh                     (std::string const& name);
                void                        refresh                     (void);
                GPDSPError                  load                        (std::string const& file, GPDSPSerializable* serializable = NULL);
                GPDSPError                  save                        (std::string const& file, GPDSPSerializable* serializable = NULL) const;
        static  GPDSPError                  addTag                      (tinyxml2::XMLElement* parent, std::string const& tag, tinyxml2::XMLElement** child);
        static  GPDSPError                  writeTag                    (tinyxml2::XMLElement* parent, std::string const& tag, int value);
        static  GPDSPError                  writeTag                    (tinyxml2::XMLElement* parent, std::string const& tag, GPDSPFloat value);
        static  GPDSPError                  writeTag                    (tinyxml2::XMLElement* parent, std::string const& tag, std::string const& value);
        static  GPDSPError                  readTag                     (tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, int* value);
        static  GPDSPError                  readTag                     (tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, int format, GPDSPFloat* value);
        static  GPDSPError                  readTag                     (tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, std::string* value);
        static  std::string                 stringize                   (GPDSPError error);
    private:
                GPDSPError                  getNodeInputtable           (std::string const& name, std::shared_ptr<GPDSPInputtableNode>* node);
                GPDSPError                  getNodeInputtable           (std::string const& name, std::shared_ptr<GPDSPInputtableNode const>* node) const;
                GPDSPError                  getNodeFlexInputtable       (std::string const& name, std::shared_ptr<GPDSPFlexInputtableNode>* node);
                GPDSPError                  getNodeOutputtable          (std::string const& name, std::shared_ptr<GPDSPOutputtableNode>* node);
                GPDSPError                  getNodeOutputtable          (std::string const& name, std::shared_ptr<GPDSPOutputtableNode const>* node) const;
                GPDSPError                  getNodeFlexOutputtable      (std::string const& name, std::shared_ptr<GPDSPFlexOutputtableNode>* node);
                GPDSPError                  newNodeGenericInput         (std::string const& name);
                GPDSPError                  newNodeGenericOutput        (std::string const& name);
                GPDSPError                  makeWait                    (void);
                GPDSPError                  makeSequence                (void);
                GPDSPError                  optimizeSequence            (void);
                GPDSPError                  writeInputTag               (tinyxml2::XMLElement* parent, std::shared_ptr<GPDSPNode const> const& node, std::string const& name) const;
                GPDSPError                  readInputTag                (tinyxml2::XMLElement const* parent, std::string const& name, int format);
        static  GPDSPError                  countInputTag               (tinyxml2::XMLElement const* parent, int* count);
        static  GPDSPError                  writeRateTag                (tinyxml2::XMLElement* parent, int rate);
        static  GPDSPError                  readRateTag                 (tinyxml2::XMLElement const* parent, int rate);
        static  GPDSPError                  writeFormatTag              (tinyxml2::XMLElement* parent);
        static  GPDSPError                  readFormatTag               (tinyxml2::XMLElement const* parent, int* format);
        static  GPDSPError                  writeLinkTag                (tinyxml2::XMLElement* parent, std::string const& node, int index, bool positive);
        static  GPDSPError                  writeLinkTag                (tinyxml2::XMLElement* parent, GPDSPFloat constant);
        static  GPDSPError                  readLinkTag                 (tinyxml2::XMLElement const* parent, std::string* node, int* index);
        static  GPDSPError                  readLinkTag                 (tinyxml2::XMLElement const* parent, int format, GPDSPFloat* constant);
        static  GPDSPError                  stringize                   (std::string const& prefix, int param, std::string* result);
        static  GPDSPError                  stringize                   (std::string const& prefix, GPDSPFloat param, std::string* result);
        static  GPDSPError                  numberize                   (std::string const& prefix, std::string const& param, int* result);
        static  GPDSPError                  numberize                   (std::string const& prefix, std::string const& param, GPDSPFloat* result);
    private:
                                            GPDSPNodeRenderer           (GPDSPNodeRenderer const&);
                GPDSPNodeRenderer&          operator=                   (GPDSPNodeRenderer const&);
};

inline GPDSPNodeRenderer::GPDSPNodeRenderer(void) : _rate(0), _nit(_node.end())
{
}

inline GPDSPNodeRenderer::~GPDSPNodeRenderer(void)
{
    clearNode();
}

inline int GPDSPNodeRenderer::getRate(void) const
{
    return _rate;
}

inline int GPDSPNodeRenderer::getSize(void) const
{
    return static_cast<int>(_node.size());
}

inline bool GPDSPNodeRenderer::hasNextNode(void) const
{
    return (_nit != _node.end());
}

inline void GPDSPNodeRenderer::iterateNode(void) const
{
    _nit = _node.begin();
    return;
}

}// end of namespace

#endif
