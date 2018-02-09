/*
**      General Purpose DSP Library
**
**      Original Copyright (C) 2017 - 2018 HORIGUCHI Junshi.
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
**      Xcode 9.2 (Apple LLVM 9.0.0)
**
**      GPDSPNode.hpp
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

#ifndef __GPDSPNODE_HPP
#define __GPDSPNODE_HPP

#include "GPDSPType.hpp"

namespace ir {

//! ノードを表すインターフェースクラス
/*!
    GPDSPNode クラスは, すべてのノードの基底クラスです.
 
    各種ノードを表す具象クラスは, GPDSPNode クラスを仮想継承し, fixate(), invalidate(), prepare(), process() 関数を実装します.
 */
class GPDSPNode {
    public:
        //! デストラクタです.
        /*!
            何もしません.
         */
        virtual                             ~GPDSPNode                  (void) noexcept = 0;
        //! 固定個数の入出力ターミナルを生成します.
        /*!
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_WAIT [返却禁止]
            @retval #GPDSPERROR_IGNORE [返却禁止]
            @retval #GPDSPERROR_FRAGMENT [返却禁止]
            @retval #GPDSPERROR_LOOP [返却禁止]
            @retval その他のエラー
         */
        virtual GPDSPError                  fixate                      (void) noexcept = 0;
        //! 演算結果を無効化し, 再演算を要求します.
        virtual void                        invalidate                  (void) noexcept = 0;
        //! 演算前の準備をします.
        /*!
            @retval #GPDSPERROR_OK 正常 (準備を完了)
            @retval #GPDSPERROR_WAIT [返却禁止]
            @retval #GPDSPERROR_IGNORE [返却禁止]
            @retval #GPDSPERROR_FRAGMENT [返却禁止]
            @retval #GPDSPERROR_LOOP [返却禁止]
            @retval その他のエラー
         */
        virtual GPDSPError                  prepare                     (void) noexcept = 0;
        //! 演算を行います.
        /*!
            @retval #GPDSPERROR_OK 正常 (演算を完了)
            @retval #GPDSPERROR_WAIT データフロー入力待ち
            @retval #GPDSPERROR_IGNORE 演算対象外として無視
            @retval #GPDSPERROR_FRAGMENT 一部の演算のみ完了
            @retval #GPDSPERROR_LOOP [返却禁止]
            @retval その他のエラー
         */
        virtual GPDSPError                  process                     (void) noexcept = 0;
    protected:
        //! コンストラクタです.
        /*!
            何もしません.
         */
        explicit                            GPDSPNode                   (void) noexcept;
    private:
                                            GPDSPNode                   (GPDSPNode const&);
                GPDSPNode&                  operator=                   (GPDSPNode const&);
};

}// end of namespace

#endif
