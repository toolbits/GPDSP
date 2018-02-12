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
**      GPDSPNodeRenderer.hpp
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

#ifndef __GPDSPNODERENDERER_HPP
#define __GPDSPNODERENDERER_HPP

#include <unordered_map>
#include "tinyxml2/tinyxml2.h"
#include "../synthesis/GPDSPSinWaveNode.hpp"
#include "../synthesis/GPDSPTriangleWaveNode.hpp"
#include "../synthesis/GPDSPSawtoothWaveNode.hpp"
#include "../synthesis/GPDSPSquareWaveNode.hpp"

namespace ir {

class GPDSPSerializable;
class GPDSPFlexInputtableNode;
class GPDSPFlexOutputtableNode;
class GPDSPBufferInputNode;
class GPDSPBufferOutputNode;
class GPDSPConstantNode;
class GPDSPSignNode;
class GPDSPGateNode;
class GPDSPPeakNode;
class GPDSPAmplifyNode;
class GPDSPDelayNode;
class GPDSPBufferNode;
class GPDSPSumNode;
class GPDSPMultiplyNode;
class GPDSPSquareRootNode;
class GPDSPGenericNode;

//! ノードを管理してデジタル信号処理の演算を行うクラス
/*!
    GPDSPNodeRenderer クラスは, 各種ノードを生成したり管理し, デジタル信号処理の演算を行うためのクラスです.
 */
class GPDSPNodeRenderer {
    private:
                int                         _rate;
                std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >
                                            _node;
                std::vector<GPDSPNode*>     _wait;
                std::vector<std::pair<GPDSPNode*, GPDSPError> >
                                            _sequence;
                std::vector<std::pair<GPDSPNode*, GPDSPError> >::iterator
                                            _jit;
        mutable std::unordered_map<std::string, std::shared_ptr<GPDSPNode> >::const_iterator
                                            _nit;
    
    public:
        //! コンストラクタです.
        /*!
            リソースを初期化します.
         */
        explicit                            GPDSPNodeRenderer           (void) noexcept;
        //! デストラクタです.
        /*!
            管理しているリソースを解放します.
         */
                                            ~GPDSPNodeRenderer          (void) noexcept;
        //! サンプリングレートを設定します.
        /*!
            サンプリングレートは, ノードが１つも登録されていないときに設定します.
            すでにノードが存在する場合, この関数は失敗します.
         
            0 を設定すると, 明示的なサンプリングレートの考慮を行いません.
            このとき, 明示的なサンプリングレートの指定を必要とするいくつかの種類のノードは生成に失敗することがあります.
            また, load() 関数において .gpdsp ファイルで記述されているサンプリングレートとの比較を行いません.
         
            0 よりも大きな値を設定すると, サンプリングレートが明示的に処理されます.
            いくつかの種類のノードは, 設定されたサンプリングレートを参照して生成されます.
            また, load() 関数において .gpdsp ファイルで記述されているサンプリングレートとの比較を行い,
            一致する項目のノード構成を読み込みます.
         
            @param[in] rate サンプリングレート >= 0
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_INVALID_STATE 不正な状態
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  setRate                     (int rate) noexcept;
        //! サンプリングレートを取得します.
        /*!
            @return 現在のサンプリングレート
         */
                int                         getRate                     (void) const noexcept;
        //! GPDSPBufferInputNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPBufferInputNode>
                                            getNodeBufferInput          (std::string const& name) const noexcept;
        //! GPDSPBufferOutputNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPBufferOutputNode>
                                            getNodeBufferOutput         (std::string const& name) const noexcept;
        //! GPDSPConstantNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPConstantNode>
                                            getNodeConstant             (std::string const& name) const noexcept;
        //! GPDSPSignNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPSignNode>
                                            getNodeSign                 (std::string const& name) const noexcept;
        //! GPDSPGateNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPGateNode>
                                            getNodeGate                 (std::string const& name) const noexcept;
        //! GPDSPPeakNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPPeakNode>
                                            getNodePeak                 (std::string const& name) const noexcept;
        //! GPDSPAmplifyNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPAmplifyNode>
                                            getNodeAmplify              (std::string const& name) const noexcept;
        //! GPDSPDelayNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPDelayNode>
                                            getNodeDelay                (std::string const& name) const noexcept;
        //! GPDSPBufferNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPBufferNode>
                                            getNodeBuffer               (std::string const& name) const noexcept;
        //! GPDSPSumNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPSumNode>
                                            getNodeSum                  (std::string const& name) const noexcept;
        //! GPDSPMultiplyNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPMultiplyNode>
                                            getNodeMultiply             (std::string const& name) const noexcept;
        //! GPDSPSquareRootNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPSquareRootNode>
                                            getNodeSquareRoot           (std::string const& name) const noexcept;
        //! GPDSPSinWaveNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPSinWaveNode>
                                            getNodeSinWave              (std::string const& name) const noexcept;
        //! GPDSPTriangleWaveNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPTriangleWaveNode>
                                            getNodeTriangleWave         (std::string const& name) const noexcept;
        //! GPDSPSawtoothWaveNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPSawtoothWaveNode>
                                            getNodeSawtoothWave         (std::string const& name) const noexcept;
        //! GPDSPSquareWaveNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPSquareWaveNode>
                                            getNodeSquareWave           (std::string const& name) const noexcept;
        //! GPDSPGenericNode クラスのノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPGenericNode>
                                            getNodeGeneric              (std::string const& name) const noexcept;
        //! ノードを取得します.
        /*!
            @param[in] name ノード名
            @retval NULL ノードが見つからない
            @retval その他 有効なノード
         */
                std::shared_ptr<GPDSPNode>  getNode                     (std::string const& name) const noexcept;
        //! ノードの個数を取得します.
        /*!
            @return 現在の個数
         */
                int                         getSize                     (void) const noexcept;
        //! 入力ターミナルの個数と名前を設定します.
        /*!
            @param[in] name ノード名
            @param[in] count 設定する個数
            @param[in] what 設定する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  setCountI                   (std::string const& name, int count, std::string const& what) noexcept;
        //! 入力ターミナルの個数を取得します.
        /*!
            @param[in] name ノード名
            @param[out] count 個数の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
         */
                GPDSPError                  getCountI                   (std::string const& name, int* count) const noexcept;
        //! 出力ターミナルの個数と名前を設定します.
        /*!
            @param[in] name ノード名
            @param[in] count 設定する個数
            @param[in] what 設定する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  setCountO                   (std::string const& name, int count, std::string const& what) noexcept;
        //! 出力ターミナルの個数を取得します.
        /*!
            @param[in] name ノード名
            @param[out] count 個数の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
         */
                GPDSPError                  getCountO                   (std::string const& name, int* count) const noexcept;
        //! 入力ターミナルの名前を設定します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[in] what 設定する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  setNameI                    (std::string const& name, int index, std::string const& what) noexcept;
        //! 入力ターミナルの名前を取得します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[out] what 名前の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  getNameI                    (std::string const& name, int index, std::string* what) const noexcept;
        //! 出力ターミナルの名前を設定します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[in] what 設定する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  setNameO                    (std::string const& name, int index, std::string const& what) noexcept;
        //! 出力ターミナルの名前を取得します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[out] what 名前の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  getNameO                    (std::string const& name, int index, std::string* what) const noexcept;
        //! 入力ターミナルの非反転入力のノード接続を設定し, モードを #GPDSPMODE_POSITIVE に変更します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[in] from 設定するノードのノード名
            @param[in] which 設定するノードのターミナル番号
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  setLinkPositiveI            (std::string const& name, int index, std::string const& from, int which) noexcept;
        //! 入力ターミナルの反転入力のノード接続を設定し, モードを #GPDSPMODE_NEGATIVE に変更します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[in] from 設定するノードのノード名
            @param[in] which 設定するノードのターミナル番号
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  setLinkNegativeI            (std::string const& name, int index, std::string const& from, int which) noexcept;
        //! 入力ターミナルの定数入力の定数値を設定し, モードを #GPDSPMODE_CONSTANT に変更します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[in] constant 設定する定数値
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  setLinkConstantI            (std::string const& name, int index, GPDSPFloat constant) noexcept;
        //! 入力ターミナルのモードを取得します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[out] mode モードの取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  getModeI                    (std::string const& name, int index, GPDSPMode* mode) const noexcept;
        //! 入力ターミナルのモードが #GPDSPMODE_POSITIVE か #GPDSPMODE_NEGATIVE の場合に, ノード接続を取得します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[out] from ノードのノード名の取得先 (NULL 可能)
            @param[out] which ノードのターミナル番号の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_STATE 不正な状態
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  getLinkI                    (std::string const& name, int index, std::string* from, int* which) const noexcept;
        //! 入力ターミナルのモードが #GPDSPMODE_CONSTANT の場合に, 定数値を取得します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[out] constant 定数値の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_STATE 不正な状態
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  getLinkI                    (std::string const& name, int index, GPDSPFloat* constant) const noexcept;
        //! 入力ターミナルのノード接続と定数値を解除し, モードを #GPDSPMODE_NONE に変更します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  clearLinkI                  (std::string const& name, int index) noexcept;
        //! 入力ターミナルのノード接続と定数値を解除し, モードを #GPDSPMODE_NONE に変更します.
        /*!
            @param[in] name ノード名
            @param[in] mode 解除するモード
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  clearLinkI                  (std::string const& name, GPDSPMode mode) noexcept;
        //! 入力ターミナルのノード接続を解除し, モードを #GPDSPMODE_NONE に変更します.
        /*!
            @param[in] name ノード名
            @param[in] from 解除するノードのノード名
            @param[in] which 解除するノードのターミナル番号
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  clearLinkI                  (std::string const& name, std::string const& from, int which) noexcept;
        //! 入力ターミナルのノード接続を解除し, モードを #GPDSPMODE_NONE に変更します.
        /*!
            @param[in] name ノード名
            @param[in] from 解除するノードのノード名
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  clearLinkI                  (std::string const& name, std::string const& from) noexcept;
        //! 入力ターミナルの定数値を解除し, モードを #GPDSPMODE_NONE に変更します.
        /*!
            @param[in] name ノード名
            @param[in] constant 解除する定数値
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  clearLinkI                  (std::string const& name, GPDSPFloat constant) noexcept;
        //! すべての入力ターミナルのノード接続と定数値を解除し, モードを #GPDSPMODE_NONE に変更します.
        /*!
            @param[in] name ノード名
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  clearLinkI                  (std::string const& name) noexcept;
                GPDSPError                  clearLinkO                  (std::string const& name, int index) noexcept;
                GPDSPError                  clearLinkO                  (std::string const& name) noexcept;
        //! 入力ターミナルの値を取得します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[out] value 値の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_WAIT データフロー入力待ち
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  getValueI                   (std::string const& name, int index, GPDSPFloat* value) const noexcept;
        //! 出力ターミナルの値を取得します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @param[out] value 値の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_WAIT データフロー入力待ち
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  getValueO                   (std::string const& name, int index, GPDSPFloat* value) const noexcept;
                std::string                 getNextNode                 (void) const noexcept;
                bool                        hasNextNode                 (void) const noexcept;
                std::string                 findNode                    (std::shared_ptr<GPDSPNode const> const& node) const noexcept;
        //! 引数に指定された名前を持つ入力ターミナルを検索します.
        /*!
            同じ名前を持つ入力ターミナルが複数ある場合には, 最初に見つかった入力ターミナルを返却します.
         
            @param[in] name ノード名
            @param[in] what 検索する名前
            @param[out] index 結果の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_FOUND 項目が見つからない
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  findNameI                   (std::string const& name, std::string const& what, int* index) const noexcept;
        //! 引数に指定された名前を持つ出力ターミナルを検索します.
        /*!
            同じ名前を持つ出力ターミナルが複数ある場合には, 最初に見つかった出力ターミナルを返却します.
         
            @param[in] name ノード名
            @param[in] what 検索する名前
            @param[out] index 結果の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_FOUND 項目が見つからない
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  findNameO                   (std::string const& name, std::string const& what, int* index) const noexcept;
        //! 引数に指定されたモードを持つ入力ターミナルを検索します.
        /*!
            同じモードを持つ入力ターミナルが複数ある場合には, 最初に見つかった入力ターミナルを返却します.
         
            @param[in] name ノード名
            @param[in] mode 検索するモード
            @param[out] index 結果の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_FOUND 項目が見つからない
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  findModeI                   (std::string const& name, GPDSPMode mode, int* index) const noexcept;
        //! 引数に指定されたノード接続を持つ入力ターミナルを検索します.
        /*!
            同じノード接続を持つ入力ターミナルが複数ある場合には, 最初に見つかった入力ターミナルを返却します.
         
            @param[in] name ノード名
            @param[in] from 検索するノードのノード名
            @param[in] which 検索するノードのターミナル番号
            @param[out] index 結果の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_FOUND 項目が見つからない
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  findLinkI                   (std::string const& name, std::string const& from, int which, int* index) const noexcept;
        //! 引数に指定されたノード接続を持つ入力ターミナルを検索します.
        /*!
            同じノード接続を持つ入力ターミナルが複数ある場合には, 最初に見つかった入力ターミナルを返却します.
         
            @param[in] name ノード名
            @param[in] from 検索するノードのノード名
            @param[out] index 結果の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_FOUND 項目が見つからない
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  findLinkI                   (std::string const& name, std::string const& from, int* index) const noexcept;
        //! 引数に指定された定数値を持つ入力ターミナルを検索します.
        /*!
            同じ定数値を持つ入力ターミナルが複数ある場合には, 最初に見つかった入力ターミナルを返却します.
         
            @param[in] name ノード名
            @param[in] constant 検索する定数値
            @param[out] index 結果の取得先 (NULL 可能)
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_FOUND 項目が見つからない
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  findLinkI                   (std::string const& name, GPDSPFloat constant, int* index) const noexcept;
        //! 既存の入力バッファを参照, または, 新規の入力バッファを自動的に確保して GPDSPBufferInputNode クラスのノードを生成します.
        /*!
            buffer 引数に NULL 以外を設定すると, 既存の入力バッファを入力バッファとして参照します.
            このとき, length 引数, もしくは interleave 引数に 0 以下の値を設定するとこの関数は失敗します.
         
            buffer 引数に NULL を設定し length 引数が 0 以下でないとき,
            length 引数と interleave 引数から適切な入力バッファのサイズを計算し, 自動的に入力バッファを確保します.
            このとき, interleave 引数に 0 以下の値を設定するとこの関数は失敗します.
         
            buffer 引数に NULL を設定し length 引数が 0 以下のとき,
            既存の入力バッファが設定されている場合は入力バッファの参照を解除し,
            入力バッファが自動的に確保されている場合は入力バッファを解放します.
         
            @param[in] name ノード名
            @param[in] buffer 既存の入力バッファ (NULL 可能)
            @param[in] length 入力バッファのサイズ
            @param[in] interleave データの間隔
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodeBufferInput          (std::string const& name, GPDSPFloat const* buffer, int length, int interleave) noexcept;
        //! 既存の出力バッファを参照, または, 新規の出力バッファを自動的に確保して GPDSPBufferOutputNode クラスのノードを生成します.
        /*!
            buffer 引数に NULL 以外を設定すると, 既存の出力バッファを出力バッファとして参照します.
            このとき, length 引数, もしくは interleave 引数に 0 以下の値を設定するとこの関数は失敗します.
         
            buffer 引数に NULL を設定し length 引数が 0 以下でないとき,
            length 引数と interleave 引数から適切な出力バッファのサイズを計算し, 自動的に出力バッファを確保します.
            このとき, interleave 引数に 0 以下の値を設定するとこの関数は失敗します.
         
            buffer 引数に NULL を設定し length 引数が 0 以下のとき,
            既存の出力バッファが設定されている場合は出力バッファの参照を解除し,
            出力バッファが自動的に確保されている場合は出力バッファを解放します.
         
            @param[in] name ノード名
            @param[in] buffer 既存の出力バッファ (NULL 可能)
            @param[in] length 出力バッファのサイズ
            @param[in] interleave データの間隔
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodeBufferOutput         (std::string const& name, GPDSPFloat* buffer, int length, int interleave) noexcept;
        //! 定数値を指定して GPDSPConstantNode クラスのノードを生成します.
        /*!
            @param[in] name ノード名
            @param[in] constant 指定する定数値
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodeConstant             (std::string const& name, GPDSPFloat constant) noexcept;
        //! GPDSPSignNode クラスのノードを生成します.
        /*!
            @param[in] name ノード名
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodeSign                 (std::string const& name) noexcept;
        //! 最小値と最大値を指定して GPDSPGateNode クラスのノードを生成します.
        /*!
            @param[in] name ノード名
            @param[in] minimum 指定する最小値
            @param[in] maximum 指定する最大値
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodeGate                 (std::string const& name, GPDSPFloat minimum, GPDSPFloat maximum) noexcept;
        //! GPDSPPeakNode クラスのノードを生成します.
        /*!
            @param[in] name ノード名
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodePeak                 (std::string const& name) noexcept;
        //! 増幅率を指定して GPDSPAmplifyNode クラスのノードを生成します.
        /*!
            @param[in] name ノード名
            @param[in] gain 指定する増幅率
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodeAmplify              (std::string const& name, GPDSPFloat gain) noexcept;
        //! GPDSPDelayNode クラスのノードを生成します.
        /*!
            @param[in] name ノード名
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodeDelay                (std::string const& name) noexcept;
        //! 内部バッファのサイズを指定し, 値を 0.0 に初期化して GPDSPBufferNode クラスのノードを生成します.
        /*!
            @param[in] name ノード名
            @param[in] size 指定するサイズ
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodeBuffer               (std::string const& name, int size) noexcept;
        //! 入力ターミナルの個数を指定して GPDSPSumNode クラスのノードを生成します.
        /*!
            入力ターミナルの名前は, すべて "in" になります.
         
            @param[in] name ノード名
            @param[in] count 指定する個数
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodeSum                  (std::string const& name, int count) noexcept;
        //! 入力ターミナルの個数を指定して GPDSPMultiplyNode クラスのノードを生成します.
        /*!
            入力ターミナルの名前は, すべて "in" になります.
         
            @param[in] name ノード名
            @param[in] count 指定する個数
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodeMultiply             (std::string const& name, int count) noexcept;
        //! GPDSPSquareRootNode クラスのノードを生成します.
        /*!
            @param[in] name ノード名
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  newNodeSquareRoot           (std::string const& name) noexcept;
        //! 解像度を指定して GPDSPSinWaveNode クラスのノードを生成します.
        /*!
            resolution 引数に (0.0, +INFINITY) の範囲の値を設定すると,
            サンプリングレートと解像度に応じた波形バッファがあらかじめ作成され,
            演算時には波形バッファを参照し直線補完にて値を求めます.
         
            resolution 引数に +INFINITY を設定すると波形バッファを作成することなく,
            演算時に値を逐次計算して求めます.
         
            サンプリングレートが 0 に設定されているとき, この関数は失敗します.
         
            @param[in] name ノード名
            @param[in] resolution 設定する解像度 (0.0, +INFINITY]
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_STATE 不正な状態
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
            @retval #GPDSPERROR_FAILED 失敗
         */
                GPDSPError                  newNodeSinWave              (std::string const& name, GPDSPFloat resolution = GPDSPSinWaveNode::defaultResolution()) noexcept;
        //! 解像度を指定して GPDSPTriangleWaveNode クラスのノードを生成します.
        /*!
            resolution 引数に (0.0, +INFINITY) の範囲の値を設定すると,
            サンプリングレートと解像度に応じた波形バッファがあらかじめ作成され,
            演算時には波形バッファを参照し直線補完にて値を求めます.
         
            resolution 引数に +INFINITY を設定すると波形バッファを作成することなく,
            演算時に値を逐次計算して求めます.
         
            サンプリングレートが 0 に設定されているとき, この関数は失敗します.
         
            @param[in] name ノード名
            @param[in] resolution 設定する解像度 (0.0, +INFINITY]
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_STATE 不正な状態
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
            @retval #GPDSPERROR_FAILED 失敗
         */
                GPDSPError                  newNodeTriangleWave         (std::string const& name, GPDSPFloat resolution = GPDSPTriangleWaveNode::defaultResolution()) noexcept;
        //! 解像度を指定して GPDSPSawtoothWaveNode クラスのノードを生成します.
        /*!
            resolution 引数に (0.0, +INFINITY) の範囲の値を設定すると,
            サンプリングレートと解像度に応じた波形バッファがあらかじめ作成され,
            演算時には波形バッファを参照し直線補完にて値を求めます.
         
            resolution 引数に +INFINITY を設定すると波形バッファを作成することなく,
            演算時に値を逐次計算して求めます.
         
            サンプリングレートが 0 に設定されているとき, この関数は失敗します.
         
            @param[in] name ノード名
            @param[in] resolution 設定する解像度 (0.0, +INFINITY]
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_STATE 不正な状態
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
            @retval #GPDSPERROR_FAILED 失敗
         */
                GPDSPError                  newNodeSawtoothWave         (std::string const& name, GPDSPFloat resolution = GPDSPSawtoothWaveNode::defaultResolution()) noexcept;
        //! 解像度を指定して GPDSPSquareWaveNode クラスのノードを生成します.
        /*!
            resolution 引数に (0.0, +INFINITY) の範囲の値を設定すると,
            サンプリングレートと解像度に応じた波形バッファがあらかじめ作成され,
            演算時には波形バッファを参照し直線補完にて値を求めます.
         
            resolution 引数に +INFINITY を設定すると波形バッファを作成することなく,
            演算時に値を逐次計算して求めます.
         
            サンプリングレートが 0 に設定されているとき, この関数は失敗します.
         
            @param[in] name ノード名
            @param[in] resolution 設定する解像度 (0.0, +INFINITY]
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_STATE 不正な状態
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
            @retval #GPDSPERROR_FAILED 失敗
         */
                GPDSPError                  newNodeSquareWave           (std::string const& name, GPDSPFloat resolution = GPDSPSquareWaveNode::defaultResolution()) noexcept;
        //! .gpdsp 拡張子を持つ外部ファイルを開いて, XML 形式で記述されたノードの構成を読み込み GPDSPGenericNode クラスのノードを生成します.
        /*!
            @param[in] name ノード名
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
                GPDSPError                  newNodeGeneric              (std::string const& name, std::string const& file) noexcept;
        //! 既存のノードを追加します.
        /*!
            すでに生成された既存のノードを GPDSPNodeRenderer クラスの管理下に登録します.
         
            追加されるノードは name 引数により名前付けされ, 他のノードのノード名と重複してはいけません.
            名前の検証に成功した場合は, GPDSPNode::fixate() 関数が呼び出されたのち管理下に登録されます.
         
            name 引数が空文字か node 引数が NULL のとき, この関数は失敗します.
         
            @param[in] name ノード名
            @param[in] node 追加するノード
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_ALREADY_EXIST すでに存在している
            @retval #GPDSPERROR_INVALID_PARAM 不正なパラメータ
         */
                GPDSPError                  appendNode                  (std::string const& name, std::shared_ptr<GPDSPNode> const& node) noexcept;
        //! 引数に指定された名前を持つ入力ターミナルを追加します.
        /*!
            @param[in] name ノード名
            @param[in] what 追加する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  appendI                     (std::string const& name, std::string const& what) noexcept;
        //! 引数に指定された名前を持つ出力ターミナルを追加します.
        /*!
            @param[in] name ノード名
            @param[in] what 追加する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  appendO                     (std::string const& name, std::string const& what) noexcept;
        //! 引数に指定された名前を持つ入力ターミナルを挿入します.
        /*!
            @param[in] name ノード名
            @param[in] index 挿入する位置
            @param[in] what 挿入する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  insertI                     (std::string const& name, int index, std::string const& what) noexcept;
        //! 引数に指定された名前を持つ出力ターミナルを挿入します.
        /*!
            @param[in] name ノード名
            @param[in] index 挿入する位置
            @param[in] what 挿入する名前
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_MEMORY メモリが不足している
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  insertO                     (std::string const& name, int index, std::string const& what) noexcept;
                GPDSPError                  removeNode                  (std::string const& name) noexcept;
        //! 入力ターミナルを削除します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  removeI                     (std::string const& name, int index) noexcept;
        //! 出力ターミナルを削除します.
        /*!
            @param[in] name ノード名
            @param[in] index ターミナル番号
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  removeO                     (std::string const& name, int index) noexcept;
                void                        clearNode                   (void) noexcept;
        //! すべての入力ターミナルを削除します.
        /*!
            @param[in] name ノード名
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  clearI                      (std::string const& name) noexcept;
        //! すべての出力ターミナルを削除します.
        /*!
            @param[in] name ノード名
            @retval #GPDSPERROR_OK 正常
            @retval #GPDSPERROR_NO_NODE ノードが見つからない
            @retval #GPDSPERROR_INVALID_NODE 不正なノード
         */
                GPDSPError                  clearO                      (std::string const& name) noexcept;
                GPDSPError                  renameNode                  (std::string const& name, std::string const& alternate) noexcept;
                void                        iterateNode                 (void) const noexcept;
                void                        invalidate                  (void) noexcept;
                GPDSPError                  prepare                     (void) noexcept;
                GPDSPError                  process                     (void) noexcept;
                GPDSPError                  render                      (int count, int* remain = NULL) noexcept;
                GPDSPError                  rewind                      (std::string const& name) noexcept;
                void                        rewind                      (void) noexcept;
                GPDSPError                  refresh                     (std::string const& name) noexcept;
                void                        refresh                     (void) noexcept;
                GPDSPError                  load                        (std::string const& file, GPDSPSerializable* serializable = NULL) noexcept;
                GPDSPError                  save                        (std::string const& file, GPDSPSerializable* serializable = NULL) const noexcept;
        static  GPDSPError                  addTag                      (tinyxml2::XMLElement* parent, std::string const& tag, tinyxml2::XMLElement** child) noexcept;
        static  GPDSPError                  writeTag                    (tinyxml2::XMLElement* parent, std::string const& tag, int value) noexcept;
        static  GPDSPError                  writeTag                    (tinyxml2::XMLElement* parent, std::string const& tag, GPDSPFloat value) noexcept;
        static  GPDSPError                  writeTag                    (tinyxml2::XMLElement* parent, std::string const& tag, std::string const& value) noexcept;
        static  GPDSPError                  readTag                     (tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, int* value) noexcept;
        static  GPDSPError                  readTag                     (tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, int format, GPDSPFloat* value) noexcept;
        static  GPDSPError                  readTag                     (tinyxml2::XMLElement const* parent, std::string const& tag, bool implicit, std::string* value) noexcept;
        //! エラー値を解説を含む文字列に変換します.
        /*!
            @param[in] error エラー値
            @return 変換された文字列
         */
        static  std::string                 stringize                   (GPDSPError error) noexcept;
    private:
                GPDSPError                  getNodeInputtable           (std::string const& name, std::shared_ptr<GPDSPInputtableNode>* node) noexcept;
                GPDSPError                  getNodeInputtable           (std::string const& name, std::shared_ptr<GPDSPInputtableNode const>* node) const noexcept;
                GPDSPError                  getNodeFlexInputtable       (std::string const& name, std::shared_ptr<GPDSPFlexInputtableNode>* node) noexcept;
                GPDSPError                  getNodeOutputtable          (std::string const& name, std::shared_ptr<GPDSPOutputtableNode>* node) noexcept;
                GPDSPError                  getNodeOutputtable          (std::string const& name, std::shared_ptr<GPDSPOutputtableNode const>* node) const noexcept;
                GPDSPError                  getNodeFlexOutputtable      (std::string const& name, std::shared_ptr<GPDSPFlexOutputtableNode>* node) noexcept;
                GPDSPError                  newNodeGenericInput         (std::string const& name) noexcept;
                GPDSPError                  newNodeGenericOutput        (std::string const& name) noexcept;
                GPDSPError                  makeWait                    (void) noexcept;
                GPDSPError                  makeSequence                (void) noexcept;
                GPDSPError                  optimizeSequence            (void) noexcept;
                GPDSPError                  writeInputTag               (tinyxml2::XMLElement* parent, std::shared_ptr<GPDSPNode const> const& node, std::string const& name) const noexcept;
                GPDSPError                  readInputTag                (tinyxml2::XMLElement const* parent, std::string const& name, int format) noexcept;
        static  GPDSPError                  countInputTag               (tinyxml2::XMLElement const* parent, int* count) noexcept;
        static  GPDSPError                  writeRateTag                (tinyxml2::XMLElement* parent, int rate) noexcept;
        static  GPDSPError                  readRateTag                 (tinyxml2::XMLElement const* parent, int rate) noexcept;
        static  GPDSPError                  writeFormatTag              (tinyxml2::XMLElement* parent) noexcept;
        static  GPDSPError                  readFormatTag               (tinyxml2::XMLElement const* parent, int* format) noexcept;
        static  GPDSPError                  writeLinkTag                (tinyxml2::XMLElement* parent, std::string const& node, int index, bool positive) noexcept;
        static  GPDSPError                  writeLinkTag                (tinyxml2::XMLElement* parent, GPDSPFloat constant) noexcept;
        static  GPDSPError                  readLinkTag                 (tinyxml2::XMLElement const* parent, std::string* node, int* index) noexcept;
        static  GPDSPError                  readLinkTag                 (tinyxml2::XMLElement const* parent, int format, GPDSPFloat* constant) noexcept;
        static  GPDSPError                  stringize                   (std::string const& prefix, int param, std::string* result) noexcept;
        static  GPDSPError                  stringize                   (std::string const& prefix, GPDSPFloat param, std::string* result) noexcept;
        static  GPDSPError                  numberize                   (std::string const& prefix, std::string const& param, int* result) noexcept;
        static  GPDSPError                  numberize                   (std::string const& prefix, std::string const& param, GPDSPFloat* result) noexcept;
    private:
                                            GPDSPNodeRenderer           (GPDSPNodeRenderer const&);
                GPDSPNodeRenderer&          operator=                   (GPDSPNodeRenderer const&);
};

inline GPDSPNodeRenderer::GPDSPNodeRenderer(void) noexcept : _rate(0), _nit(_node.end())
{
}

inline GPDSPNodeRenderer::~GPDSPNodeRenderer(void) noexcept
{
    clearNode();
}

inline int GPDSPNodeRenderer::getRate(void) const noexcept
{
    return _rate;
}

inline int GPDSPNodeRenderer::getSize(void) const noexcept
{
    return static_cast<int>(_node.size());
}

inline bool GPDSPNodeRenderer::hasNextNode(void) const noexcept
{
    return (_nit != _node.end());
}

inline void GPDSPNodeRenderer::iterateNode(void) const noexcept
{
    _nit = _node.begin();
    return;
}

}// end of namespace

#endif
