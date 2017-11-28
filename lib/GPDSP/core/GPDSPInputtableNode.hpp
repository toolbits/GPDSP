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
**      GPDSPInputtableNode.hpp
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

#ifndef __GPDSPINPUTTABLENODE_HPP
#define __GPDSPINPUTTABLENODE_HPP

#include "GPDSPNode.hpp"
#include "GPDSPOutputtableNode.hpp"

namespace ir {

class GPDSPInputtableNode : public virtual GPDSPNode {
    private:
        struct SocketRec {
            std::string                     name;
            GPDSPOutputtableNode const*     node;
            int                             index;
        };
    
    private:
                std::vector<SocketRec>      _socket;
    
    public:
                int                         getCountI                   (void) const;
                GPDSPError                  setNameI                    (int index, std::string const& name);
                GPDSPError                  getNameI                    (int index, std::string* name) const;
                GPDSPError                  setLinkI                    (int index, GPDSPOutputtableNode const* node, int which);
                GPDSPError                  getLinkI                    (int index, GPDSPOutputtableNode const** node, int* which) const;
                GPDSPError                  clearLinkI                  (int index);
                void                        clearLinkI                  (GPDSPOutputtableNode const* node, int which);
                void                        clearLinkI                  (GPDSPOutputtableNode const* node);
                void                        clearLinkI                  (void);
                GPDSPError                  getValueI                   (int index, float* value) const;
                int                         findNameI                   (std::string const& name) const;
                int                         findLinkI                   (GPDSPOutputtableNode const* node, int which) const;
                int                         findLinkI                   (GPDSPOutputtableNode const* node) const;
        virtual void                        invalidate                  (void);
    protected:
        explicit                            GPDSPInputtableNode         (void);
        virtual                             ~GPDSPInputtableNode        (void) = 0;
                GPDSPError                  setCountI                   (int count, std::string const& name);
                GPDSPError                  appendI                     (std::string const& name, GPDSPOutputtableNode const* node, int which);
                GPDSPError                  insertI                     (int index, std::string const& name, GPDSPOutputtableNode const* node, int which);
                GPDSPError                  removeI                     (int index);
                void                        clearI                      (void);
    private:
                                            GPDSPInputtableNode         (GPDSPInputtableNode const&);
                GPDSPInputtableNode&        operator=                   (GPDSPInputtableNode const&);
};

inline int GPDSPInputtableNode::getCountI(void) const
{
    return static_cast<int>(_socket.size());
}

inline GPDSPError GPDSPInputtableNode::getValueI(int index, float* value) const
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < _socket.size()) {
        if (_socket[index].node != NULL) {
            error = _socket[index].node->getValueO(_socket[index].index, value);
        }
        else if (value != NULL) {
            *value = 0.0f;
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

}// end of namespace

#endif
