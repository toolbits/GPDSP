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
**      GPDSPDelayNode.hpp
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

#ifndef __GPDSPDELAYNODE_HPP
#define __GPDSPDELAYNODE_HPP

#include "../core/GPDSPInputtableNode.hpp"
#include "../core/GPDSPOutputtableNode.hpp"
#include "../core/GPDSPRefreshableNode.hpp"

namespace ir {

//! 単位遅延ノードを表す具象クラス
/*!
    GPDSPDelayNode クラスは, 入力を単位時間だけ遅延させて出力する単位遅延ノードを表す具象クラスです.
 
    １つの入力ターミナル "in" と, １つの出力ターミナル "out" を持ちます.
 
    <b>.gpdsp ファイルでの記述例</b>
    @code{.xml}
    <GPDSPDelayNode>
        <name>ノード名</name>
        <input>
            <::0>
                <node>in に対する入力元のノード名</node>
                <index>in に対する入力元のターミナル番号</index>
            </::0>
        </input>
    </GPDSPDelayNode>
    @endcode
 */
class GPDSPDelayNode : public GPDSPInputtableNode, public GPDSPOutputtableNode, public virtual GPDSPRefreshableNode {
    private:
                GPDSPFloat                  _queue;
    
    public:
        //! コンストラクタです.
        /*!
            内部バッファの値を 0.0 に初期化します.
         */
        explicit                            GPDSPDelayNode              (void) noexcept;
        //! デストラクタです.
        /*!
            何もしません.
         */
        virtual                             ~GPDSPDelayNode             (void) noexcept;
        //! 入力ターミナルを１つと, 出力ターミナルを１つ生成します.
        /*!
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
         */
        virtual GPDSPError                  fixate                      (void) noexcept;
        //! 入出力の演算結果を無効化し, 再演算を要求します.
        virtual void                        invalidate                  (void) noexcept;
        //! 演算前の準備をします.
        /*!
            @retval #GPDSPERROR_OK 正常 (準備を完了)
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
        virtual GPDSPError                  prepare                     (void) noexcept;
        //! 演算を行います.
        /*!
            @retval #GPDSPERROR_OK 正常 (演算を完了)
            @retval #GPDSPERROR_WAIT データフロー入力待ち
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
        virtual GPDSPError                  process                     (void) noexcept;
        //! 内部バッファの値を 0.0 に再初期化します.
        virtual void                        refresh                     (void) noexcept;
    private:
                                            GPDSPDelayNode              (GPDSPDelayNode const&);
                GPDSPDelayNode&             operator=                   (GPDSPDelayNode const&);
};

}// end of namespace

#endif
