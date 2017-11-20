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
**      GPDSPBufferOutputNode.cpp
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

#include "GPDSPBufferOutputNode.hpp"

namespace ir {

GPDSPBufferOutputNode::GPDSPBufferOutputNode(void) : _delegate(NULL)
{
}

GPDSPBufferOutputNode::~GPDSPBufferOutputNode(void)
{
    _buffer.clear();
}

GPDSPError GPDSPBufferOutputNode::setBuffer(float* buffer, int length, int interleave)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (buffer != NULL) {
        if (length > 0) {
            if (interleave > 0) {
                _buffer.clear();
                _delegate = buffer;
            }
            else {
                error = GPDSPERROR_INVALID_PARAM;
            }
        }
        else {
            error = GPDSPERROR_INVALID_PARAM;
        }
    }
    else if (length > 0) {
        if (interleave > 0) {
            try {
                _buffer.assign(length * interleave, 0.0f);
            }
            catch (std::bad_alloc const&) {
                error = GPDSPERROR_NO_MEMORY;
            }
            if (error == GPDSPERROR_OK) {
                _delegate = _buffer.data();
            }
        }
        else {
            error = GPDSPERROR_INVALID_PARAM;
        }
    }
    else {
        _buffer.clear();
        _delegate = NULL;
    }
    if (error == GPDSPERROR_OK) {
        _length = length;
        _interleave = interleave;
        _position = 0;
        invalidate();
    }
    return error;
}

float const* GPDSPBufferOutputNode::getReadonlyBuffer(int* length, int* interleave) const
{
    if (_delegate != NULL) {
        if (length != NULL) {
            *length = _length;
        }
        if (interleave != NULL) {
            *interleave = _interleave;
        }
    }
    return _delegate;
}

GPDSPError GPDSPBufferOutputNode::setPosition(int position)
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= position && position < _length) {
        _position = position;
        invalidate();
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

GPDSPError GPDSPBufferOutputNode::fixate(void)
{
    return setCountI(1, "in");
}

GPDSPError GPDSPBufferOutputNode::prepare(void)
{
    return GPDSPERROR_OK;
}

GPDSPError GPDSPBufferOutputNode::process(void)
{
    float value;
    GPDSPError error(GPDSPERROR_OK);
    
    if ((error = getValueI(0, &value)) == GPDSPERROR_OK) {
        if (_delegate != NULL) {
            _delegate[_position * _interleave] = value;
            if (++_position >= _length) {
                _position = 0;
            }
        }
    }
    return error;
}

void GPDSPBufferOutputNode::rewind(void)
{
    _position = 0;
    invalidate();
    return;
}

void GPDSPBufferOutputNode::refresh(void)
{
    if (_delegate != NULL) {
        std::fill_n(_delegate, _length * _interleave, 0.0f);
    }
    invalidate();
    return;
}

}// end of namespace
