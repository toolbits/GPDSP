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
**      GPDSPBufferNode.cpp
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

#include "GPDSPBufferNode.hpp"

namespace ir {

GPDSPBufferNode::GPDSPBufferNode(void)
{
}

GPDSPBufferNode::~GPDSPBufferNode(void)
{
    _queue.clear();
}

GPDSPError GPDSPBufferNode::setSize(int size)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (size < 0) {
        size = 0;
    }
    if (size != _queue.size()) {
        try {
            _queue.assign(size, GPDSPFV(0.0));
        }
        catch (std::bad_alloc const&) {
            error = GPDSPERROR_NO_MEMORY;
        }
        if (error == GPDSPERROR_OK) {
            invalidate();
        }
    }
    return error;
}

GPDSPError GPDSPBufferNode::fixate(void)
{
    GPDSPError error(GPDSPERROR_OK);
    
    clearO();
    clearI();
    if ((error = setCountI(1, "in")) == GPDSPERROR_OK) {
        error = setCountO(1, "out");
    }
    if (error != GPDSPERROR_OK) {
        clearO();
        clearI();
    }
    return error;
}

void GPDSPBufferNode::invalidate(void)
{
    GPDSPInputtableNode::invalidate();
    GPDSPOutputtableNode::invalidate();
    return;
}

GPDSPError GPDSPBufferNode::prepare(void)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (_queue.size() > 0) {
        error = setValueO(0, _queue.back());
    }
    return error;
}

GPDSPError GPDSPBufferNode::process(void)
{
    GPDSPFloat value;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getValueI(0, &value)) == GPDSPERROR_OK) {
        if (_queue.size() > 0) {
            try {
                _queue.push_front(value);
            }
            catch (std::bad_alloc const&) {
                error = GPDSPERROR_NO_MEMORY;
            }
            if (error == GPDSPERROR_OK) {
                _queue.pop_back();
            }
        }
        else {
            error = setValueO(0, value);
        }
    }
    return error;
}

void GPDSPBufferNode::refresh(void)
{
    std::fill_n(_queue.begin(), _queue.size(), GPDSPFV(0.0));
    return;
}

}// end of namespace
