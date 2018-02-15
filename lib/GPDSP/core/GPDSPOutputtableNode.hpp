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
**      GPDSPOutputtableNode.hpp
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

#ifndef __GPDSPOUTPUTTABLENODE_HPP
#define __GPDSPOUTPUTTABLENODE_HPP

/*!
    @file
    @brief ir::GPDSPOutputtableNode クラス
 */

#include "GPDSPNode.hpp"
#include <string>
#include <vector>

namespace ir {

//! 出力ターミナルを持つノードを表す抽象クラス
/*!
    GPDSPOutputtableNode クラスは, 出力ターミナルを持つノードを表す抽象クラスです.
 
    固定個数の出力ターミナルを持つ各種ノードを表す具象クラスは, GPDSPOutputtableNode クラスを継承して実装します.
 */
class GPDSPOutputtableNode : public virtual GPDSPNode {
    private:
        struct TerminalRec {
            std::string                     name;
            bool                            valid;
            GPDSPFloat                      value;
        };
    
    private:
                std::vector<TerminalRec>    _terminal;
    
    public:
        //! 出力ターミナルの個数を取得します.
        /*!
            @return 現在の個数
         */
                int                         getCountO                   (void) const noexcept;
        //! 出力ターミナルの名前を設定します.
        /*!
            @param[in] index ターミナル番号
            @param[in] what 設定する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  setNameO                    (int index, std::string const& what) noexcept;
        //! 出力ターミナルの名前を取得します.
        /*!
            @param[in] index ターミナル番号
            @param[out] what 名前の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  getNameO                    (int index, std::string* what) const noexcept;
        //! 出力ターミナルの値を取得します.
        /*!
            @param[in] index ターミナル番号
            @param[out] value 値の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_WAIT データフロー入力待ち
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  getValueO                   (int index, GPDSPFloat* value) const noexcept;
        //! 引数に指定された名前を持つ出力ターミナルを検索します.
        /*!
            同じ名前を持つ出力ターミナルが複数ある場合には, 最初に見つかった出力ターミナルを返却します.
         
            @param[in] what 検索する名前
            @retval -1 存在しない
            @retval その他 ターミナル番号
         */
                int                         findNameO                   (std::string const& what) const noexcept;
        //! 出力の演算結果を無効化し, 再演算を要求します.
        virtual void                        invalidate                  (void) noexcept;
    protected:
        //! コンストラクタです.
        /*!
            何もしません.
         */
        explicit                            GPDSPOutputtableNode        (void) noexcept;
        //! デストラクタです.
        /*!
            管理しているリソースを解放します.
         */
        virtual                             ~GPDSPOutputtableNode       (void) noexcept = 0;
        //! 出力ターミナルの個数と名前を設定します.
        /*!
            @param[in] count 設定する個数
            @param[in] what 設定する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
         */
                GPDSPError                  setCountO                   (int count, std::string const& what) noexcept;
        //! 出力ターミナルの値を設定します.
        /*!
            @param[in] index ターミナル番号
            @param[in] value 設定する値
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  setValueO                   (int index, GPDSPFloat value) noexcept;
        //! 引数に指定された名前を持つ出力ターミナルを追加します.
        /*!
            @param[in] what 追加する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
         */
                GPDSPError                  appendO                     (std::string const& what) noexcept;
        //! 引数に指定された名前を持つ出力ターミナルを挿入します.
        /*!
            @param[in] index 挿入する位置
            @param[in] what 挿入する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  insertO                     (int index, std::string const& what) noexcept;
        //! 出力ターミナルを削除します.
        /*!
            @param[in] index ターミナル番号
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  removeO                     (int index) noexcept;
        //! すべての出力ターミナルを削除します.
                void                        clearO                      (void) noexcept;
    private:
                                            GPDSPOutputtableNode        (GPDSPOutputtableNode const&);
                GPDSPOutputtableNode&       operator=                   (GPDSPOutputtableNode const&);
};

inline int GPDSPOutputtableNode::getCountO(void) const noexcept
{
    return static_cast<int>(_terminal.size());
}

inline GPDSPError GPDSPOutputtableNode::getValueO(int index, GPDSPFloat* value) const noexcept
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < static_cast<int>(_terminal.size())) {
        if (_terminal[index].valid) {
            if (value != NULL) {
                *value = _terminal[index].value;
            }
        }
        else {
            error = GPDSPERROR_WAIT;
        }
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

inline GPDSPError GPDSPOutputtableNode::setValueO(int index, GPDSPFloat value) noexcept
{
    GPDSPError error(GPDSPERROR_OK);
    
    if (0 <= index && index < static_cast<int>(_terminal.size())) {
        _terminal[index].valid = true;
        _terminal[index].value = value;
    }
    else {
        error = GPDSPERROR_INVALID_RANGE;
    }
    return error;
}

}// end of namespace

#endif
