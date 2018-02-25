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
**      GPDSPBufferOutputNode.hpp
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

#ifndef __GPDSPBUFFEROUTPUTNODE_HPP
#define __GPDSPBUFFEROUTPUTNODE_HPP

/*!
    @file
    @brief ir::GPDSPBufferOutputNode クラス
 */

#include "../core/GPDSPInputtableNode.hpp"
#include "../core/GPDSPRewindableNode.hpp"
#include "../core/GPDSPRefreshableNode.hpp"

namespace ir {

//! バッファ出力ノードを表す具象クラス
/*!
    GPDSPBufferOutputNode クラスは, 設定されたバッファにデータを書き込むためのバッファ出力ノードを表す具象クラスです.
 
    １つの入力ターミナル "in" を持ち, 出力ターミナルは持ちません.
 
    | 番号 | 入力 | 出力 |
    | :----: | :----: | :----: |
    | 0 | in | - |
 
    <b>.gpdsp ファイルでの記述例</b>
    @code{.xml}
    <GPDSPBufferOutputNode>
        <name>ノード名</name>
        <param>
            <length>出力バッファのサイズ</length>
            <interleave>データの間隔</interleave>
        </param>
        <input>
            <::0>
                <node>in に対する入力元のノード名</node>
                <output>::in に対する入力元のターミナル番号</output>
            </::0>
        </input>
    </GPDSPBufferOutputNode>
    @endcode
 */
class GPDSPBufferOutputNode : public GPDSPInputtableNode, public virtual GPDSPRewindableNode, public virtual GPDSPRefreshableNode {
    private:
                GPDSPFloat*                 _delegate;
                std::vector<GPDSPFloat>     _buffer;
                int                         _length;
                int                         _interleave;
                int                         _position;
    
    public:
        //! コンストラクタです.
        /*!
            リソースを初期化します.
         */
        explicit                            GPDSPBufferOutputNode       (void) noexcept;
        //! デストラクタです.
        /*!
            管理しているリソースを解放します.
         */
        virtual                             ~GPDSPBufferOutputNode      (void) noexcept;
        //! 既存の出力バッファを参照, または, 新規の出力バッファを自動的に確保します.
        /*!
            buffer 引数に NULL 以外を設定すると, 既存の出力バッファを出力バッファとして参照します.
            このとき, length 引数, もしくは interleave 引数に 0 以下の値を設定するとこの関数は失敗します.
         
            buffer 引数に NULL を設定し length 引数が 0 以下でないとき,
            length 引数と interleave 引数から適切な出力バッファのサイズを計算し, 自動的に出力バッファを確保します.
            このとき, interleave 引数に 0 以下の値を設定するとこの関数は失敗します.
         
            buffer 引数に NULL を設定し length 引数が 0 以下のとき,
            既存の出力バッファが設定されている場合は出力バッファの参照を解除し,
            出力バッファが自動的に確保されている場合は出力バッファを解放します.
         
            @param[in] buffer 既存の出力バッファ (NULL 可能)
            @param[in] length 出力バッファのサイズ
            @param[in] interleave データの間隔
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  setBuffer                   (GPDSPFloat* buffer, int length, int interleave) noexcept;
        //! 読み込み専用の出力バッファを取得します.
        /*!
            @param[out] length 出力バッファのサイズ (NULL 可能)
            @param[out] interleave データの間隔 (NULL 可能)
            @retval NULL 出力バッファが設定されていない
            @retval その他 有効な出力バッファ
         */
                GPDSPFloat const*           getBufferReadonly           (int* length, int* interleave) const noexcept;
        //! 出力バッファの操作位置を設定します.
        /*!
            @param[in] position 設定する操作位置
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_STATE 不正な状態
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  setPosition                 (int position) noexcept;
        //! 出力バッファの操作位置を取得します.
        /*!
            @return 現在の操作位置
         */
                int                         getPosition                 (void) const noexcept;
        //! 出力バッファが既存の出力バッファへの参照であるかどうかを判定します.
        /*!
            @retval true 既存の出力バッファへの参照
            @retval false 出力バッファが設定されていないか自動的に確保された出力バッファ
         */
                bool                        isDelegate                  (void) const noexcept;
        //! 入力ターミナルを１つ生成します.
        /*!
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
         */
        virtual GPDSPError                  fixate                      (void) noexcept;
        //! 演算前の準備をします.
        /*!
            何もしません.
         
            @retval #GPDSPERROR_OK 正常 (準備を完了)
         */
        virtual GPDSPError                  prepare                     (void) noexcept;
        //! 演算を行います.
        /*!
            @retval #GPDSPERROR_OK 正常 (演算を完了)
            @retval #GPDSPERROR_WAIT データフロー入力待ち
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
        virtual GPDSPError                  process                     (void) noexcept;
        //! 出力バッファの操作位置を先頭に再初期化します.
        virtual void                        rewind                      (void) noexcept;
        //! 出力バッファの値を 0.0 に再初期化します.
        virtual void                        refresh                     (void) noexcept;
    private:
                                            GPDSPBufferOutputNode       (GPDSPBufferOutputNode const&);
                GPDSPBufferOutputNode&      operator=                   (GPDSPBufferOutputNode const&);
};

inline int GPDSPBufferOutputNode::getPosition(void) const noexcept
{
    return (_delegate != NULL) ? (_position) : (0);
}

inline bool GPDSPBufferOutputNode::isDelegate(void) const noexcept
{
    return (_delegate != NULL && _buffer.size() <= 0);
}

}// end of namespace

#endif
