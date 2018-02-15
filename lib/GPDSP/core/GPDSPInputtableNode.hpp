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

/*!
    @file
    @brief ir::GPDSPInputtableNode クラス
 */

#include "GPDSPNode.hpp"
#include "GPDSPOutputtableNode.hpp"

namespace ir {

//! 入力ターミナルを持つノードを表す抽象クラス
/*!
    GPDSPInputtableNode クラスは, 入力ターミナルを持つノードを表す抽象クラスです.
 
    固定個数の入力ターミナルを持つ各種ノードを表す具象クラスは, GPDSPInputtableNode クラスを継承して実装します.
 */
class GPDSPInputtableNode : public virtual GPDSPNode {
    private:
        struct TerminalRec {
            std::string                     name;
            GPDSPMode                       mode;
            union {
                struct {
                    GPDSPOutputtableNode const*
                                            node;
                    int                     index;
                };
                struct {
                    GPDSPFloat              constant;
                };
            };
        };
    
    private:
                std::vector<TerminalRec>    _terminal;
    
    public:
        //! 入力ターミナルの個数を取得します.
        /*!
            @return 現在の個数
         */
                int                         getCountI                   (void) const noexcept;
        //! 入力ターミナルの名前を設定します.
        /*!
            @param[in] index ターミナル番号
            @param[in] what 設定する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  setNameI                    (int index, std::string const& what) noexcept;
        //! 入力ターミナルの名前を取得します.
        /*!
            @param[in] index ターミナル番号
            @param[out] what 名前の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  getNameI                    (int index, std::string* what) const noexcept;
        //! 入力ターミナルの非反転入力のノード接続を設定し, モードを #GPDSPMODE_POSITIVE に変更します.
        /*!
            @param[in] index ターミナル番号
            @param[in] from 設定するノード (NULL 可能)
            @param[in] which 設定するノードのターミナル番号
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  setLinkPositiveI            (int index, GPDSPOutputtableNode const* from, int which) noexcept;
        //! 入力ターミナルの反転入力のノード接続を設定し, モードを #GPDSPMODE_NEGATIVE に変更します.
        /*!
            @param[in] index ターミナル番号
            @param[in] from 設定するノード (NULL 可能)
            @param[in] which 設定するノードのターミナル番号
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  setLinkNegativeI            (int index, GPDSPOutputtableNode const* from, int which) noexcept;
        //! 入力ターミナルの定数入力の定数値を設定し, モードを #GPDSPMODE_CONSTANT に変更します.
        /*!
            @param[in] index ターミナル番号
            @param[in] constant 設定する定数値
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  setLinkConstantI            (int index, GPDSPFloat constant) noexcept;
        //! 入力ターミナルのモードを取得します.
        /*!
            @param[in] index ターミナル番号
            @param[out] mode モードの取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  getModeI                    (int index, GPDSPMode* mode) const noexcept;
        //! 入力ターミナルのモードが #GPDSPMODE_POSITIVE か #GPDSPMODE_NEGATIVE の場合に, ノード接続を取得します.
        /*!
            @param[in] index ターミナル番号
            @param[out] from ノードの取得先 (NULL 可能)
            @param[out] which ノードのターミナル番号の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_STATE 不正な状態
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  getLinkI                    (int index, GPDSPOutputtableNode const** from, int* which) const noexcept;
        //! 入力ターミナルのモードが #GPDSPMODE_CONSTANT の場合に, 定数値を取得します.
        /*!
            @param[in] index ターミナル番号
            @param[out] constant 定数値の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_STATE 不正な状態
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  getLinkI                    (int index, GPDSPFloat* constant) const noexcept;
        //! 入力ターミナルのノード接続と定数値を解除し, モードを #GPDSPMODE_NONE に変更します.
        /*!
            @param[in] index ターミナル番号
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  clearLinkI                  (int index) noexcept;
        //! 入力ターミナルのノード接続と定数値を解除し, モードを #GPDSPMODE_NONE に変更します.
        /*!
            @param[in] mode 解除するモード
         */
                void                        clearLinkI                  (GPDSPMode mode) noexcept;
        //! 入力ターミナルのノード接続を解除し, モードを #GPDSPMODE_NONE に変更します.
        /*!
            @param[in] from 解除するノード (NULL 可能)
            @param[in] which 解除するノードのターミナル番号
         */
                void                        clearLinkI                  (GPDSPOutputtableNode const* from, int which) noexcept;
        //! 入力ターミナルのノード接続を解除し, モードを #GPDSPMODE_NONE に変更します.
        /*!
            @param[in] from 解除するノード (NULL 可能)
         */
                void                        clearLinkI                  (GPDSPOutputtableNode const* from) noexcept;
        //! 入力ターミナルの定数値を解除し, モードを #GPDSPMODE_NONE に変更します.
        /*!
            @param[in] constant 解除する定数値
         */
                void                        clearLinkI                  (GPDSPFloat constant) noexcept;
        //! すべての入力ターミナルのノード接続と定数値を解除し, モードを #GPDSPMODE_NONE に変更します.
                void                        clearLinkI                  (void) noexcept;
        //! 入力ターミナルの値を取得します.
        /*!
            @param[in] index ターミナル番号
            @param[out] value 値の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_WAIT データフロー入力待ち
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  getValueI                   (int index, GPDSPFloat* value) const noexcept;
        //! 引数に指定された名前を持つ入力ターミナルを検索します.
        /*!
            同じ名前を持つ入力ターミナルが複数ある場合には, 最初に見つかった入力ターミナルを返却します.
         
            @param[in] what 検索する名前
            @retval -1 存在しない
            @retval その他 ターミナル番号
         */
                int                         findNameI                   (std::string const& what) const noexcept;
        //! 引数に指定されたモードを持つ入力ターミナルを検索します.
        /*!
            同じモードを持つ入力ターミナルが複数ある場合には, 最初に見つかった入力ターミナルを返却します.
         
            @param[in] mode 検索するモード
            @retval -1 存在しない
            @retval その他 ターミナル番号
         */
                int                         findModeI                   (GPDSPMode mode) const noexcept;
        //! 引数に指定されたノード接続を持つ入力ターミナルを検索します.
        /*!
            同じノード接続を持つ入力ターミナルが複数ある場合には, 最初に見つかった入力ターミナルを返却します.
         
            @param[in] from 検索するノード (NULL 可能)
            @param[in] which 検索するノードのターミナル番号
            @retval -1 存在しない
            @retval その他 ターミナル番号
         */
                int                         findLinkI                   (GPDSPOutputtableNode const* from, int which) const noexcept;
        //! 引数に指定されたノード接続を持つ入力ターミナルを検索します.
        /*!
            同じノード接続を持つ入力ターミナルが複数ある場合には, 最初に見つかった入力ターミナルを返却します.
         
            @param[in] from 検索するノード (NULL 可能)
            @retval -1 存在しない
            @retval その他 ターミナル番号
         */
                int                         findLinkI                   (GPDSPOutputtableNode const* from) const noexcept;
        //! 引数に指定された定数値を持つ入力ターミナルを検索します.
        /*!
            同じ定数値を持つ入力ターミナルが複数ある場合には, 最初に見つかった入力ターミナルを返却します.
         
            @param[in] constant 検索する定数値
            @retval -1 存在しない
            @retval その他 ターミナル番号
         */
                int                         findLinkI                   (GPDSPFloat constant) const noexcept;
        //! 入力の演算結果を無効化し, 再演算を要求します.
        /*!
            何もしません.
         */
        virtual void                        invalidate                  (void) noexcept;
    protected:
        //! コンストラクタです.
        /*!
            何もしません.
         */
        explicit                            GPDSPInputtableNode         (void) noexcept;
        //! デストラクタです.
        /*!
            管理しているリソースを解放します.
         */
        virtual                             ~GPDSPInputtableNode        (void) noexcept = 0;
        //! 入力ターミナルの個数と名前を設定します.
        /*!
            @param[in] count 設定する個数
            @param[in] what 設定する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
         */
                GPDSPError                  setCountI                   (int count, std::string const& what) noexcept;
        //! 引数に指定された名前を持つ入力ターミナルを追加します.
        /*!
            @param[in] what 追加する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
         */
                GPDSPError                  appendI                     (std::string const& what) noexcept;
        //! 引数に指定された名前を持つ入力ターミナルを挿入します.
        /*!
            @param[in] index 挿入する位置
            @param[in] what 挿入する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  insertI                     (int index, std::string const& what) noexcept;
        //! 入力ターミナルを削除します.
        /*!
            @param[in] index ターミナル番号
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  removeI                     (int index) noexcept;
        //! すべての入力ターミナルを削除します.
                void                        clearI                      (void) noexcept;
    private:
                                            GPDSPInputtableNode         (GPDSPInputtableNode const&);
                GPDSPInputtableNode&        operator=                   (GPDSPInputtableNode const&);
};

inline int GPDSPInputtableNode::getCountI(void) const noexcept
{
    return static_cast<int>(_terminal.size());
}

}// end of namespace

#endif
