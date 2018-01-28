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
**      GPDSPGenericNode.hpp
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

#ifndef __GPDSPGENERICNODE_HPP
#define __GPDSPGENERICNODE_HPP

#include "../core/GPDSPInputtableNode.hpp"
#include "../core/GPDSPOutputtableNode.hpp"
#include "../core/GPDSPRewindableNode.hpp"
#include "../core/GPDSPRefreshableNode.hpp"
#include "../core/GPDSPNodeRenderer.hpp"

namespace ir {

class GPDSPGenericInputNode;
class GPDSPGenericOutputNode;

//! 任意ノードを表す具象クラス
/*!
    GPDSPGenericNode クラスは, .gpdsp 拡張子を持つ外部ファイルから XML 形式で記述されたノードの構成を読み込み,
    任意の特性を持つフィルタとして振る舞う任意ノードを表す具象クラスです.
 
    GPDSPGenericInputNode クラス, GPDSPGenericOutputNode クラスとともに使用します.
 
    入力ターミナルと出力ターミナルの個数と名前は, 読み込む任意ノードの構成に依存します.
 */
class GPDSPGenericNode : public GPDSPInputtableNode, public GPDSPOutputtableNode, public virtual GPDSPRewindableNode, public virtual GPDSPRefreshableNode {
    private:
                int                         _rate;
                GPDSPNodeRenderer           _renderer;
                std::string                 _file;
                std::vector<GPDSPGenericInputNode*>
                                            _input;
                std::vector<GPDSPGenericOutputNode const*>
                                            _output;
    
    public:
        //! コンストラクタです.
        /*!
            リソースを初期化します.
         
            @param[in] rate サンプリングレート >= 0
         */
        explicit                            GPDSPGenericNode            (int rate);
        //! デストラクタです.
        /*!
            管理しているリソースを解放します.
         */
        virtual                             ~GPDSPGenericNode           (void);
        //! 内部で利用している GPDSPNodeRenderer クラスのインスタンスを取得します.
        /*!
            @return インスタンスへの参照
         */
                GPDSPNodeRenderer const&    getRenderer                 (void) const;
        //! 開いているファイルのパスを取得します.
        /*!
            @return パスへの参照
         */
                std::string const&          getFile                     (void) const;
        //! .gpdsp 拡張子を持つ外部ファイルを開いて, XML 形式で記述されたノードの構成を読み込みます.
        /*!
            @param[in] file ファイルのパス
            @retval GPDSPERROR_OK 正常
            @retval GPDSPERROR_NO_SUPPORT サポートされていない
            @retval GPDSPERROR_NO_FILE ファイルが存在しない
            @retval GPDSPERROR_NO_MEMORY メモリ不足
            @retval GPDSPERROR_NO_FOUND 項目が見つからない
            @retval GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval GPDSPERROR_INVALID_STATE 不正な状態
            @retval GPDSPERROR_INVALID_PARAM 不正なパラメータ
            @retval GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval GPDSPERROR_INVALID_FORMAT 不正なフォーマット
            @retval GPDSPERROR_FAILED 失敗
         */
                GPDSPError                  open                        (std::string const& file);
        //! 開いているファイルを閉じて, 管理しているリソースを解放します.
                void                        close                       (void);
        //! 固定個数の入出力ターミナルを生成します.
        /*!
            何もしません.
         
            @retval GPDSPERROR_OK 正常
         */
        virtual GPDSPError                  fixate                      (void);
        //! 演算結果を無効化し, 再演算を要求します.
        virtual void                        invalidate                  (void);
        //! 演算前の準備をします.
        /*!
            @retval GPDSPERROR_OK 正常 (準備を完了)
            @retval GPDSPERROR_WAIT [返却なし]
            @retval GPDSPERROR_IGNORE [返却なし]
            @retval GPDSPERROR_FRAGMENT [返却なし]
            @retval GPDSPERROR_LOOP [返却なし]
            @retval その他のエラー
         */
        virtual GPDSPError                  prepare                     (void);
        //! 演算を行います.
        /*!
            @retval GPDSPERROR_OK 正常 (演算を完了)
            @retval GPDSPERROR_WAIT データフロー入力待ち
            @retval GPDSPERROR_IGNORE [返却なし]
            @retval GPDSPERROR_FRAGMENT 一部の演算のみ完了
            @retval GPDSPERROR_LOOP [返却なし]
            @retval その他のエラー
         */
        virtual GPDSPError                  process                     (void);
        //! 入出力バッファの操作位置などを再初期化します.
        virtual void                        rewind                      (void);
        //! 入出力バッファの値などを再初期化します.
        virtual void                        refresh                     (void);
    protected:
        //! 入力ターミナルの値を対応する内部の GPDSPGenericInputNode クラスのインスタンスにコピーします.
        /*!
            @retval GPDSPERROR_OK 正常
            @retval GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  copyInput                   (void);
        //! 内部の GPDSPGenericOutputNode クラスのインスタンスの値を対応する出力ターミナルにコピーします.
        /*!
            @retval GPDSPERROR_OK 正常
            @retval GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
                GPDSPError                  copyOutput                  (void);
    private:
        static  GPDSPError                  nameToIndex                 (std::string const& name, int* index);
    private:
                                            GPDSPGenericNode            (GPDSPGenericNode const&);
                GPDSPGenericNode&           operator=                   (GPDSPGenericNode const&);
};

inline GPDSPNodeRenderer const& GPDSPGenericNode::getRenderer(void) const
{
    return _renderer;
}

inline std::string const& GPDSPGenericNode::getFile(void) const
{
    return _file;
}

}// end of namespace

#endif
