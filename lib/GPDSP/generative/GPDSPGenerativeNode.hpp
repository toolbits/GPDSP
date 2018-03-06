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
**      GPDSPGenerativeNode.hpp
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

#ifndef __GPDSPGENERATIVENODE_HPP
#define __GPDSPGENERATIVENODE_HPP

/*!
    @file
    @brief ir::GPDSPGenerativeNode クラス
 */

#include "../core/GPDSPInputtableNode.hpp"
#include "../core/GPDSPOutputtableNode.hpp"
#include "../core/GPDSPRewindableNode.hpp"
#include "../core/GPDSPRefreshableNode.hpp"
#include "../core/GPDSPNodeRenderer.hpp"

namespace ir {

class GPDSPGenerativeInputNode;
class GPDSPGenerativeOutputNode;

//! 生成的ノードを表す具象クラス
/*!
    GPDSPGenerativeNode クラスは, gpdsp 形式の外部ファイルから XML で記述されたノードの構成を読み込み,
    任意の特性を持つフィルタとして振る舞う生成的ノードを表す具象クラスです.
 
    GPDSPGenerativeInputNode クラス, GPDSPGenerativeOutputNode クラスとともに利用します.
 
    入力ターミナルと出力ターミナルの個数と名前は, 読み込むノードの構成に依存します.
 
    | 番号 | 入力 | 出力 |
    | :----: | :----: | :----: |
    | 0 | ? | ? |
    | 1 | ? | ? |
    | N-1 | ? | ? |
 
    <b>gpdsp ファイルでの記述例</b>
    @code{.xml}
    <GPDSPGenerativeNode>
        <name>ノード名</name>
        <param>
            <file>gpdsp ファイルのパス</file>
        </param>
        <input>
            <::0>
                <node>[入力ターミナル名] に対する入力元のノード名</node>
                <output>::[入力ターミナル名] に対する入力元のターミナル番号</output>
            </::0>
            <::1>
                <node>[入力ターミナル名] に対する入力元のノード名</node>
                <output>::[入力ターミナル名] に対する入力元のターミナル番号</output>
            </::1>
            ...
            <::N-1>
                <node>[入力ターミナル名] に対する入力元のノード名</node>
                <output>::[入力ターミナル名] に対する入力元のターミナル番号</output>
            </::N-1>
        </input>
    </GPDSPGenerativeNode>
    @endcode
 */
class GPDSPGenerativeNode : public GPDSPInputtableNode, public GPDSPOutputtableNode, public virtual GPDSPRewindableNode, public virtual GPDSPRefreshableNode {
    private:
                int                         _rate;
                GPDSPNodeRenderer           _renderer;
                std::string                 _file;
                std::vector<GPDSPGenerativeInputNode*>
                                            _input;
                std::vector<GPDSPGenerativeOutputNode const*>
                                            _output;
    
    public:
        //! コンストラクタです.
        /*!
            リソースを初期化します.
         
            @param[in] rate サンプリングレート >= 0
         */
        explicit                            GPDSPGenerativeNode         (int rate) noexcept;
        //! デストラクタです.
        /*!
            管理しているリソースを解放します.
         */
        virtual                             ~GPDSPGenerativeNode        (void) noexcept;
        //! サンプリングレートを取得します.
        /*!
            @return 現在のサンプリングレート
         */
                int                         getRate                     (void) const noexcept;
        //! 内部で利用している GPDSPNodeRenderer クラスのインスタンスを取得します.
        /*!
            @return インスタンスへの参照
         */
                GPDSPNodeRenderer const&    getRenderer                 (void) const noexcept;
        //! 開いているファイルのパスを取得します.
        /*!
            @return パスへの参照
         */
                std::string const&          getFile                     (void) const noexcept;
        //! gpdsp 形式の外部ファイルを開いて, XML で記述されたノードの構成を読み込みます.
        /*!
            @param[in] file ファイルのパス
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_SUPPORT サポートされていない
            @retval #GPDSPERROR_NO_FILE ファイルが存在しない
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_NO_FOUND 項目が見つからない
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_STATE 不正な状態
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_FORMAT 不正なフォーマット
            @retval #GPDSPERROR_FAILED 失敗
         */
                GPDSPError                  open                        (std::string const& file) noexcept;
        //! 開いているファイルを閉じて, 管理しているリソースを解放します.
                void                        close                       (void) noexcept;
        //! 固定個数の入出力ターミナルを生成します.
        /*!
            何もしません.
         
            @retval #GPDSPERROR_OK 正常
         */
        virtual GPDSPError                  fixate                      (void) noexcept;
        //! 演算結果を無効化し, 再演算を要求します.
        virtual void                        invalidate                  (void) noexcept;
        //! 演算前の準備をします.
        /*!
            @retval #GPDSPERROR_OK 正常 (準備を完了)
            @retval #GPDSPERROR_WAIT [返却なし]
            @retval #GPDSPERROR_IGNORE [返却なし]
            @retval #GPDSPERROR_FRAGMENT [返却なし]
            @retval #GPDSPERROR_LOOP [返却なし]
            @retval その他のエラー
         */
        virtual GPDSPError                  prepare                     (void) noexcept;
        //! 演算を行います.
        /*!
            @retval #GPDSPERROR_OK 正常 (演算を完了)
            @retval #GPDSPERROR_WAIT データフロー入力待ち
            @retval #GPDSPERROR_IGNORE [返却なし]
            @retval #GPDSPERROR_FRAGMENT 一部の演算のみ完了
            @retval #GPDSPERROR_LOOP [返却なし]
            @retval その他のエラー
         */
        virtual GPDSPError                  process                     (void) noexcept;
        //! 入出力バッファの操作位置などを再初期化します.
        virtual void                        rewind                      (void) noexcept;
        //! 入出力バッファの値などを再初期化します.
        virtual void                        refresh                     (void) noexcept;
    protected:
        //! 入力ターミナルの値を対応する内部の GPDSPGenerativeInputNode クラスのインスタンスにコピーします.
        /*!
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  copyInput                   (void) noexcept;
        //! 内部の GPDSPGenerativeOutputNode クラスのインスタンスの値を対応する出力ターミナルにコピーします.
        /*!
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  copyOutput                  (void) noexcept;
    private:
                                            GPDSPGenerativeNode         (GPDSPGenerativeNode const&);
                GPDSPGenerativeNode&        operator=                   (GPDSPGenerativeNode const&);
};

inline int GPDSPGenerativeNode::getRate(void) const noexcept
{
    return _rate;
}

inline GPDSPNodeRenderer const& GPDSPGenerativeNode::getRenderer(void) const noexcept
{
    return _renderer;
}

inline std::string const& GPDSPGenerativeNode::getFile(void) const noexcept
{
    return _file;
}

}// end of namespace

#endif
