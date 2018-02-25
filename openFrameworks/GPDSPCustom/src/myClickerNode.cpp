/*
**      GPDSPCustom
**
**      Original Copyright (C) 2017 - 2018 HORIGUCHI Junshi.
**                                          http://iridium.jp/
**                                          zap00365@nifty.com
**      Portions Copyright (C) <year> <author>
**                                          <website>
**                                          <e-mail>
**      Version     openFrameworks
**      Website     http://iridium.jp/
**      E-mail      zap00365@nifty.com
**
**      This source code is for Xcode.
**      Xcode 9.2 (Apple LLVM 9.0.0)
**
**      myClickerNode.cpp
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

#include "myClickerNode.hpp"

myClickerNode::myClickerNode(void) noexcept :

    // 左チャンネルと右チャンネルの同期をデフォルト値に初期化
    _interlock(defaultInterlock()),

    // オーバーフローとなる限界値をデフォルト値に初期化
    _overflow(defaultOverflow())
{
    // 左チャンネルと右チャンネルの積算値を初期化
    _lload = GPDSPFV(0.0);
    _rload = GPDSPFV(0.0);
}

myClickerNode::~myClickerNode(void) noexcept
{
    // 何もしない
}

bool myClickerNode::defaultInterlock(void) noexcept
{
    return false;
}

GPDSPFloat myClickerNode::defaultOverflow(void) noexcept
{
    return GPDSPFV(500.0);
}

void myClickerNode::setInterlock(bool interlock) noexcept
{
    // 左チャンネルと右チャンネルの同期の状態が変更される場合,
    // invalidate() 関数を呼び出して演算結果を無効化し再演算を要求
    if (interlock != _interlock) {
        _interlock = interlock;
        invalidate();
    }
    return;
}

bool myClickerNode::getInterlock(void) const noexcept
{
    return _interlock;
}

void myClickerNode::setOverflow(GPDSPFloat overflow) noexcept
{
    // オーバーフローとなる限界値の状態が変更される場合,
    // invalidate() 関数を呼び出して演算結果を無効化し再演算を要求
    if (overflow != _overflow) {
        _overflow = overflow;
        invalidate();
    }
    return;
}

GPDSPFloat myClickerNode::getOverflow(void) const noexcept
{
    return _overflow;
}

GPDSPError myClickerNode::fixate(void) noexcept
{
    GPDSPError error(GPDSPERROR_OK);
    
    // 初めに入力ターミナルと出力ターミナルをすべて破棄
    clearO();
    clearI();
    
    // 入力ターミナルを作成
    if ((error = appendI("Lch-in")) == GPDSPERROR_OK) {
        if ((error = appendI("Rch-in")) == GPDSPERROR_OK) {
            
            // 出力ターミナルを作成
            if ((error = appendO("Lch-out")) == GPDSPERROR_OK) {
                error = appendO("Rch-out");
            }
        }
    }
    
    // エラーが発生した場合は, 入力ターミナルと出力ターミナルをすべて破棄
    if (error != GPDSPERROR_OK) {
        clearO();
        clearI();
    }
    return error;
}

void myClickerNode::invalidate(void) noexcept
{
    // GPDSPInputtableNode クラスの invalidate() 関数と GPDSPOutputtableNode クラスの
    // invalidate() 関数は, 暗黙には区別がつかないので明示的に両方の関数を呼び出す
    GPDSPInputtableNode::invalidate();
    GPDSPOutputtableNode::invalidate();
    return;
}

GPDSPError myClickerNode::prepare(void) noexcept
{
    // 内部バッファを持たないため何もしない
    return GPDSPERROR_OK;
}

GPDSPError myClickerNode::process(void) noexcept
{
    GPDSPFloat lch;
    GPDSPFloat rch;
    bool lov;
    bool rov;
    GPDSPError error(GPDSPERROR_OK);
    
    // 入力ターミナルの値を取得
    if ((error = getValueI(0, &lch)) == GPDSPERROR_OK) {
        if ((error = getValueI(1, &rch)) == GPDSPERROR_OK) {
            
            // 左チャンネルと右チャンネルのそれぞれの振幅の絶対値を積算値に加算
            _lload += fabs(lch);
            _rload += fabs(rch);
            
            // 積算値がオーバーフローとなる限界値を超えたかどうかを検査
            lov = (_lload >= _overflow);
            rov = (_rload >= _overflow);
            
            // 左チャンネルと右チャンネルが同期されるとき,
            // どちらかのチャンネルが限界値を超えた場合にどちらも限界値を超えたことにする
            if (_interlock) {
                lov |= rov;
                rov |= lov;
            }
            
            // 左チャンネルが限界値を超えた場合
            if (lov) {
                
                // 左チャンネルの積算値を再初期化
                _lload = GPDSPFV(0.0);
                
                // 左チャンネルのクリック音を演算
                if (lch > GPDSPFV(0.0)) {
                    lch = +sqrt(+lch * GPDSPFV(1000.0)) / GPDSPFV(10.0);
                    lch = std::min(lch, GPDSPFV(+1.0));
                }
                else if (lch < GPDSPFV(0.0)) {
                    lch = -sqrt(-lch * GPDSPFV(1000.0)) / GPDSPFV(10.0);
                    lch = std::max(lch, GPDSPFV(-1.0));
                }
            }
            
            // 右チャンネルが限界値を超えた場合
            if (rov) {
                _rload = GPDSPFV(0.0);
                if (rch > GPDSPFV(0.0)) {
                    rch = +sqrt(+rch * GPDSPFV(1000.0)) / GPDSPFV(10.0);
                    rch = std::min(rch, GPDSPFV(+1.0));
                }
                else if (rch < GPDSPFV(0.0)) {
                    rch = -sqrt(-rch * GPDSPFV(1000.0)) / GPDSPFV(10.0);
                    rch = std::max(rch, GPDSPFV(-1.0));
                }
            }
            
            // 出力ターミナルに値を設定
            if ((error = setValueO(0, lch)) == GPDSPERROR_OK) {
                error = setValueO(1, rch);
            }
        }
    }
    return error;
}

void myClickerNode::refresh(void) noexcept
{
    // 左チャンネルと右チャンネルの積算値を再初期化
    _lload = GPDSPFV(0.0);
    _rload = GPDSPFV(0.0);
    return;
}
