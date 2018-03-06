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
**      GPDSP.hpp
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

#ifndef __GPDSP_HPP
#define __GPDSP_HPP

/*!
    @file
    @brief ライブラリのすべての機能を提供するヘッダファイル
    @dir core
    @brief 基本設計を提供するインターフェースクラスや抽象クラスを含むディレクトリ
    @dir algorithm
    @brief 基本的な演算や入出力などの機能を提供する具象クラスを含むディレクトリ
    @dir synthesis
    @brief 波形合成の機能を提供する具象クラスを含むディレクトリ
    @dir generative
    @brief 生成的ノードの機能を提供する具象クラスを含むディレクトリ
 */

#include "core/GPDSPType.hpp"
#include "core/GPDSPNodeRenderer.hpp"
#include "core/GPDSPSerializable.hpp"
#include "core/GPDSPNode.hpp"
#include "core/GPDSPInputtableNode.hpp"
#include "core/GPDSPFlexInputtableNode.hpp"
#include "core/GPDSPOutputtableNode.hpp"
#include "core/GPDSPFlexOutputtableNode.hpp"
#include "core/GPDSPRewindableNode.hpp"
#include "core/GPDSPRefreshableNode.hpp"
#include "algorithm/GPDSPBufferInputNode.hpp"
#include "algorithm/GPDSPBufferOutputNode.hpp"
#include "algorithm/GPDSPConstantNode.hpp"
#include "algorithm/GPDSPSignNode.hpp"
#include "algorithm/GPDSPGateNode.hpp"
#include "algorithm/GPDSPPeakNode.hpp"
#include "algorithm/GPDSPAmplifyNode.hpp"
#include "algorithm/GPDSPDelayNode.hpp"
#include "algorithm/GPDSPBufferNode.hpp"
#include "algorithm/GPDSPSumNode.hpp"
#include "algorithm/GPDSPMultiplyNode.hpp"
#include "algorithm/GPDSPSquareRootNode.hpp"
#include "synthesis/GPDSPWaveNode.hpp"
#include "synthesis/GPDSPSinWaveNode.hpp"
#include "synthesis/GPDSPTriangleWaveNode.hpp"
#include "synthesis/GPDSPSawtoothWaveNode.hpp"
#include "synthesis/GPDSPSquareWaveNode.hpp"
#include "generative/GPDSPGenerativeNode.hpp"

namespace ir {

/*!
    @mainpage GPDSP ライブラリ概要
 
    @tableofcontents
 
    @section sec_overview GPDSP とは
    GPDSP ライブラリは, 音響演算やセンサデータの信号処理などのデジタル信号処理を手軽に行うための C++ 言語で記述されたライブラリです.
    [増幅](@ref GPDSPAmplifyNode)や[遅延](@ref GPDSPDelayNode)や[加算](@ref GPDSPSumNode)などを表す任意のノードをソースコード上で接続することにより,
    デジタル信号処理のアルゴリズムを迅速に構築して実行することができます.
 
    開発者はデジタル信号処理の計算順序やアルゴリズムの詳細を記述する必要はなく,
    GPDSP ライブラリがデータフローを適切に最適化します. 最適化はデジタル信号処理の実行中にも動的に行われるため,
    ノードの構成をダイナミックに変更しても最速となる計算順序を自動的に探索します.
 
    また, [gpdsp 形式](@ref sec_serialize_gpdsp)の外部ファイルから XML で記述された任意のノード構成を読み込み,
    [１つのノード](@ref GPDSPGenerativeNode)として利用することができます.
    この機能を利用することにより, [あらかじめ用意されたノード](@ref sec_kind)の動作以外にも,
    任意の特性を持つノードを作成して実行することができるようになります.
 
    @section sec_environment 推奨環境と開発環境
 
    GPDSP ライブラリは POSIX 準拠の C++ 言語で記述され, ソースコードとして提供されます.
    RTTI (Run-Time Type Information) を有効にした C++ 言語を利用できる環境であれば,
    ワンチップマイコンなどの少資源な環境でも動作させることができます.
 
    一方で, 複雑な多数のノード構成の演算にはマシンパワーが必要であるため,
    リアルタイムにデジタル信号処理を行う場合は十分なマシンパワーを持った環境が推奨されます.
 
    <b>推奨環境</b>
    - C++ 11 (RTTI 有効)
    - ヒープ割り当て利用可能
 
    <b>開発環境</b>
    - macOS High Sierra (10.13.3)
    - Xcode 9.2 (9C40b)
    - Apple LLVM version 9.0.0 (clang-900.0.39.2)
    - MacBook Pro (Retina, 13-inch, Late 2013)
    - 2.8 GHz Intel Core i7
    - 16 GB 1600 MHz DDR3
    - Intel Iris 1536 MB
 
    @note
    RTTI を利用しない設計を行うことも十分に可能ですが,
    インターフェースクラスや抽象クラスに不必要な仮想関数が多くなり, 関数の継承関係の見通しが煩雑になります.
    このため, 現在のバージョンの GPDSP ライブラリでは RTTI を積極的に採用しています.
 
    @section sec_download ダウンロードとインストール
 
    GPDSP ライブラリの最新版は, 以下のサイトからダウンロードすることができます.
 
    http://github.com/toolbits/GPDSP
 
    クローン, もしくは .zip ファイルでダウンロードしたファイルを任意の場所に解凍して配置します.
    実際の開発で利用するライブラリは [lib/GPDSP ディレクトリ](files.html)に含まれており, ソースコードレベルでリンクして利用します.
 
    <b>ディレクトリの構成</b>
    - docs -- [doxygen](http://www.doxygen.org) により生成されたドキュメント
    - <em>lib</em> -- <em>GPDSP ライブラリ本体</em>
    - mcss-doxygen -- [m.css フレームワーク](http://mcss.mosra.cz)用のドキュメントテンプレート
    - openFrameworks -- [openFrameworks](http://openframeworks.cc) を利用したサンプルコード
    - release -- コンパイル済みのサンプルコード
 
    @section sec_howtouse 利用方法
    ダウンロードしたディレクトリに含まれる [lib/GPDSP ディレクトリ](files.html)以下のすべてのヘッダファイルとソースファイルを
    GPDSP ライブラリを利用したいプロジェクトに追加し, ソースコードから次のように読み込みます.
 
    <b>ヘッダファイルの読み込み</b>
 
    @code{.cpp}
    #include "GPDSP.hpp"
    @endcode
 
    GPDSP ライブラリでは, 基本的には GPDSPNodeRenderer クラスを利用して,
    各種ノードを生成したりノードを接続してデータフローを設計します.
 
    各種ノードに固有の属性を設定するときは, GPDSPNodeRenderer クラスのインスタンスから各種ノードのインスタンスを取得して,
    ノードに固有の関数を呼び出します.
 
    次に 100 バイトのデータに対して増幅率 2.0 のデジタル信号処理を行うプログラム例を示します.
 
    <b>GPDSP ライブラリを利用したプログラム例</b>
 
    @code{.cpp}
    #include "GPDSP.hpp"
 
    using namespace ir;
 
    GPDSPNodeRenderer dsp;
    float* in;
    float const* out;
    int i;
 
    // サンプリングレートを 44100Hz に設定
    dsp.setRate(44100);
 
    // 100 バイトの入力バッファを確保して, バッファ入力ノードを作成
    dsp.newNodeBufferInput("data_in", NULL, 100, 1);
 
    // 100 バイトの出力バッファを確保して, バッファ出力ノードを作成
    dsp.newNodeBufferOutput("data_out", NULL, 100, 1);
 
    // 増幅率 2.0 の増幅ノードを作成
    dsp.newNodeAmplify("amp", 2.0f);
 
    // 増幅ノードの入力とバッファ入力ノードの出力を接続
    dsp.setLinkPositiveI("amp", 0, "data_in", 0);
 
    // バッファ出力ノードの入力と増幅ノードの出力を接続
    dsp.setLinkPositiveI("data_out", 0, "amp", 0);
 
    // バッファ入力ノードが確保した 100 バイトのバッファを取得
    in = dsp.getNodeBufferInput("data_in")->getBufferWritable(NULL, NULL);
 
    // バッファに処理前のデータを書き込み
    for (i = 0; i < 100; ++i) {
        in[i] = i / 100.0f;
    }
 
    // 100 バイト分のデジタル信号処理を実行
    dsp.render(100);
 
    // バッファ出力ノードが確保した 100 バイトのバッファを取得
    out = dsp.getNodeBufferOutput("data_out")->getBufferReadonly(NULL, NULL);
 
    // 処理後のデータをバッファから読み出し
    for (i = 0; i < 100; ++i) {
        std::cout << out[i] << std::endl;
    }
    @endcode
 
    @section sec_kind ノードの種類
    GPDSP ライブラリには, 以下の表に示すような具象ノードがあらかじめ用意されています.
 
    開発者は GPDSPGenerativeNode クラスを利用するか, 抽象ノードを継承した新しいノードを実装することにより,
    独自の機能を持った[カスタムノード](@ref sec_extension_overview)を追加することもできます.
 
    | クラス名 | 解説 |
    | --------- | --------- |
    | GPDSPBufferInputNode | バッファ入力ノード |
    | GPDSPBufferOutputNode | バッファ出力ノード |
    | GPDSPConstantNode | 定数ノード |
    | GPDSPSignNode | 符号ノード |
    | GPDSPGateNode | 制限ノード |
    | GPDSPPeakNode | 極値ノード |
    | GPDSPAmplifyNode | 増幅ノード |
    | GPDSPDelayNode | 単位遅延ノード |
    | GPDSPBufferNode | 任意遅延ノード |
    | GPDSPSumNode | 総和ノード |
    | GPDSPMultiplyNode | 総積ノード |
    | GPDSPSquareRootNode | 平方根ノード |
    | GPDSPSinWaveNode | サイン波ノード |
    | GPDSPTriangleWaveNode | 三角波ノード |
    | GPDSPSawtoothWaveNode | 鋸波ノード |
    | GPDSPSquareWaveNode | 矩形波ノード |
    | GPDSPGenerativeNode | 生成的ノード |
 
    @section sec_feature ライブラリの特徴
    GPDSP ライブラリを利用することで, デジタル信号処理の計算順序やアルゴリズムの詳細を開発者が記述する必要がなくなります.
    これは GPDSPNodeRenderer::render() 関数が内部でデータフローを追跡し,
    ノード構成に最適な計算順序を自動的に探索することができるからです.
    一度探索された最適な計算順序は記憶され, 次の演算では負荷のかかる探索を行わずに高速な演算を行います.
 
    一方で GPDSP ライブラリは, 一度接続されたノード構成をデジタル信号処理の途中で動的に再接続することを可能にします.
    GPDSPNodeRenderer::render() 関数は, ノード構成に変更があるかどうかをなるべく負荷のかからない方法で演算のたびに検証します.
    ノード構成に変更がある場合は, 必要最低限の部分について計算順序を再探索して, 新しい順序を記憶し直します.
 
    また, [ディレイ・フリー・ループ](@ref sec_error_loop)などの離散的に演算ができないノード構成や,
    入力を待ち続け演算を完了できない条件なども自動的に判別し, 安全にエラーを返却します.
 
    これらの動作は, GPDSPGenerativeNode クラスによりノード構成が再帰的な入れ子構造を持つ場合にも有効であり,
    ダイナミックに特性の変わる非常に柔軟なデジタル信号処理の実現を可能にします.
 
    @section sec_detail その他の詳細
    上記では記述されていない詳細な情報については, [こちら](pages.html)を参照してください.
 
 
 
    @page page_serialize <執筆中> ノード構成の保存と復元
 
    @tableofcontents
 
    @section sec_serialize_overview 保存と復元とは
    執筆中
 
    @section sec_serialize_gpdsp gpdsp ファイルとは
    執筆中
 
 
 
    @page page_generative <執筆中> 生成的ノードの詳細
 
    @tableofcontents
 
    @section sec_generative_overview 生成的ノードとは
    執筆中
 
    @section sec_generative_gpdsp gpdsp ファイルでの記述
    執筆中
 
 
 
    @page page_extension カスタムノードの実装
 
    @tableofcontents
 
    @section sec_extension_overview カスタムノードとは
    カスタムノードとは, GPDSP ライブラリに[あらかじめ用意されたノード](@ref sec_kind)以外の,
    開発者が独自に定義した具象ノードを指します.
    いくつかの規則に従ってカスタムノードを実装することにより,
    [あらかじめ用意されたノード](@ref sec_kind)と同じ操作性を得ることができます.
 
    @section sec_extension_class 新しいクラスの実装
 
    @note
    ここでは, 特定の条件が満たされると入力された音声の波形に対してクリック音を付加して出力するカスタムノードの作成方法を示します.
 
    カスタムノードを作成するには, 新しいクラスを定義し, 必要な仮想関数をオーバーライドします.
 
    データの入力を受け付けるときには GPDSPInputtableNode クラスを継承し,
    データの出力を行うときには GPDSPOutputtableNode クラスを継承します.
    GPDSPInputtableNode クラスと GPDSPOutputtableNode クラスは, それぞれ,
    入力ターミナルの個数と出力ターミナルの個数がノードの生成時に決定される場合に利用します.
    ノードの生成後に入力ターミナルの個数と出力ターミナルの個数を自由に変更できるようにする場合は,
    GPDSPFlexInputtableNode クラスと GPDSPFlexOutputtableNode クラスを利用します.
    これらのクラスは <em>public 継承</em>して利用します.
 
    また, カスタムノードが何らかの巻き戻しの機能を持つ場合には GPDSPRewindableNode クラスを継承し,
    カスタムノードが何らかの再初期化の機能を持つ場合には GPDSPRefreshableNode クラスを継承します.
    これらのクラスは <em>public 仮想継承</em>して利用します.
 
    新しいクラスで実装しなければいけない仮想関数の種類は継承したクラスに応じて決定されるため,
    必要となる仮想関数をそれぞれ適切に実装します.
 
    次にカスタムノードのプログラム例を示します.
 
    <b>新しいクラスのヘッダファイルの例</b>
 
    @code{.cpp}
    #include "GPDSP.hpp"
 
    using namespace ir;
 
    class myClickerNode : public GPDSPInputtableNode,
    public GPDSPOutputtableNode, public virtual GPDSPRefreshableNode {
        private:
 
                    // 左チャンネルと右チャンネルを同期するかどうか
                    bool            _interlock;
 
                    // オーバーフローとなる限界値
                    GPDSPFloat      _overflow;
 
                    // 左チャンネルの積算値
                    GPDSPFloat      _lload;
 
                    // 右チャンネルの積算値
                    GPDSPFloat      _rload;
 
        public:
 
            // コンストラクタとデストラクタ
            explicit                myClickerNode       (void) noexcept;
            virtual                 ~myClickerNode      (void) noexcept;
 
            // デフォルト値を取得するための関数
            static  bool            defaultInterlock    (void) noexcept;
            static  GPDSPFloat      defaultOverflow     (void) noexcept;
 
            // 左チャンネルと右チャンネルの同期に関連する関数
                    void            setInterlock        (bool interlock) noexcept;
                    bool            getInterlock        (void) const noexcept;
 
            // オーバーフローとなる限界値に関連する関数
                    void            setOverflow         (GPDSPFloat overflow) noexcept;
                    GPDSPFloat      getOverflow         (void) const noexcept;
 
            // 実装しなければいけない仮想関数
            virtual GPDSPError      fixate              (void) noexcept;
            virtual void            invalidate          (void) noexcept;
            virtual GPDSPError      prepare             (void) noexcept;
            virtual GPDSPError      process             (void) noexcept;
 
            // GPDSPRefreshableNode クラスを継承した場合に実装しなければいけない仮想関数
            virtual void            refresh             (void) noexcept;
        private:
 
            // インスタンスのコピーと代入を禁止するための関数宣言
                                    myClickerNode       (myClickerNode const&);
                    myClickerNode&  operator=           (myClickerNode const&);
    };
    @endcode
 
    - fixate(), prepare(), process() 関数は GPDSPInputtableNode クラスや
      GPDSPOutputtableNode クラスを継承する場合に実装が必要になります
    - invalidate() 関数は GPDSPInputtableNode クラスと GPDSPOutputtableNode クラスなど,
      どちらも invalidate() 関数を持つクラスを同時に継承する場合に, 明示的な実装が必要になります
    - refresh() 関数は GPDSPRefreshableNode クラスを継承する場合に実装が必要になります
    - ノードに固有の開発者が操作可能な変数が存在する場合は, セッター, ゲッター,
      デフォルト値のゲッターの３種類の関数をそれぞれの変数について実装します
    - ノードのインスタンスのコピーと代入を禁止するために, コピーコンストラクタと代入演算子を private で宣言します
 
    <b>新しいクラスのソースファイルの例</b>
 
    @code{.cpp}
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
        // 左チャンネルと右チャンネルの同期の状態が変更される場合は,
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
        // オーバーフローとなる限界値の状態が変更される場合は,
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
        // invalidate() 関数は暗黙には区別がつかないので, 明示的に両方の関数を呼び出す
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
 
                    // 右チャンネルの積算値を再初期化
                    _rload = GPDSPFV(0.0);
 
                    // 右チャンネルのクリック音を演算
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
    @endcode
 
    - ノードに固有の開発者が操作可能な変数が更新され, 入力と出力の関係性が変化する場合は,
      invalidate() 関数を呼び出して再演算を要求します
    - fixate() 関数は複数回呼び出される可能性があるために,
      入力ターミナルと出力ターミナルを新しく作成する前にそれぞれのターミナルをすべて破棄します
    - invalidate() 関数の明示的な実装が必要なときは, 親クラスの invalidate() 関数を明示的に呼び出します
    - 内部バッファを持たないノードは prepare() 関数で行うべき処理がないため, 常に #GPDSPERROR_OK を返却します
    - デジタル信号処理の具体的な演算は process() 関数で実装します
 
    最後に, 上記のようにして作成した新しいクラスは, 次のようにして生成して利用することができます.
 
    <b>カスタムノードの生成と利用</b>
 
    @code{.cpp}
    using namespace ir;
 
    GPDSPNodeRenderer dsp;
 
    dsp.appendNode("clicker", std::make_shared<myClickerNode>());
    @endcode
 
    @note
    内部バッファを持つノードでは, データの入力を待たずに内部バッファから出力を演算することが可能な場合があります.
    このような場合は, 内部バッファの値から出力を演算して設定するまでの処理を prepare() 関数で実装し,
    残りの処理を process() 関数で実装します.
 
    <b>内部バッファを持つノードの prepare() 関数と process() 関数の実装例</b>
 
    @code{.cpp}
    GPDSPError GPDSPDelayNode::prepare(void) noexcept
    {
        return setValueO(0, _queue);
    }
 
    GPDSPError GPDSPDelayNode::process(void) noexcept
    {
        GPDSPFloat value;
        GPDSPError error(GPDSPERROR_OK);
 
        if ((error = getValueI(0, &value)) == GPDSPERROR_OK) {
            _queue = value;
        }
        return error;
    }
    @endcode
 
    @section sec_extension_serialize 保存と復元への対応
    新しいクラスを実装することによりカスタムノードを利用することができるようになりますが,
    [保存と復元](@ref sec_serialize_overview)への対応ができていない場合は,
    カスタムノードを含んだノード構成を GPDSPNodeRenderer::load() 関数を利用して復元したり,
    GPDSPNodeRenderer::save() 関数を利用して保存しようとすると #GPDSPERROR_NO_SUPPORT となり失敗します.
 
    カスタムノードを[保存と復元](@ref sec_serialize_overview)に対応させるには,
    GPDSPSerializable クラスを継承したクラスを作成し, GPDSPSerializable::load() 関数と
    GPDSPSerializable::save() 関数をオーバーライドして実装します.
 
    @note
    通常, アプリケーションを表すクラスで GPDSPSerializable クラスを継承するか,
    GPDSPSerializable クラスを継承した新しいクラスを作成して,
    新しいクラスのインスタンスをアプリケーションを表すクラスで保持します.
 
    <b>アプリケーションを表すクラスのヘッダファイルの例</b>
 
    @code{.cpp}
    #include "GPDSP.hpp"
 
    using namespace ir;
 
    class myApp : public GPDSPSerializable {
        ...
        public:
                                    myApp   (void);
                                    ~myApp  (void);
                    void            doCopy  (void);
            ...
 
            // 復元を行うための関数
            virtual GPDSPError      load    (GPDSPNodeRenderer* renderer,
                                                std::string const& type,
                                                std::string const& name,
                                                int format,
                                                tinyxml2::XMLElement const* element) noexcept;
 
            // 保存を行うための関数
            virtual GPDSPError      save    (GPDSPNodeRenderer const& renderer,
                                                std::shared_ptr<GPDSPNode const> const& node,
                                                std::string const& name,
                                                tinyxml2::XMLElement* element) noexcept;
            ...
    }
    @endcode
 
    <b>アプリケーションを表すクラスのソースファイルの例</b>
 
    @code{.cpp}
    using namespace ir;
 
    // 復元を行うための関数
    GPDSPError myApp::load(GPDSPNodeRenderer* renderer,
                              std::string const& type,
                              std::string const& name,
                              int format,
                              tinyxml2::XMLElement const* element) noexcept
    {
        std::shared_ptr<myClickerNode> clicker;
        tinyxml2::XMLElement const* param;
        int interlock;
        GPDSPFloat overflow;
        GPDSPError error(GPDSPERROR_OK);
 
        // ノードの種類が実装したいノードであるかを検証
        if (type == "myClickerNode") {
 
            // gpdsp ファイルの記述内で値が指定されていない場合のためにデフォルト値を設定
            interlock = myClickerNode::defaultInterlock();
            overflow = myClickerNode::defaultOverflow();
 
            // tinyxml2 を利用してノードに固有の値を gpdsp ファイルから復元
            if ((param = element->FirstChildElement("param")) != NULL) {
                if ((error = GPDSPNodeRenderer::readTag(param, "interlock",
                                 true, &interlock)) == GPDSPERROR_OK) {
                    error = GPDSPNodeRenderer::readTag(param, "overflow",
                                true, format, &overflow);
                }
            }
            if (error == GPDSPERROR_OK) {
 
                // 例外を利用しない設計なので try ~ catch 構文で例外を捕捉しエラーに変換
                try {
 
                    // カスタムノードのインスタンスを生成
                    clicker = std::make_shared<myClickerNode>();
                }
                catch (std::bad_alloc const&) {
                    error = GPDSPERROR_NO_MEMORY;
                }
                if (error == GPDSPERROR_OK) {
 
                    // gpdsp ファイルから復元した値を設定
                    clicker->setInterlock(interlock);
                    clicker->setOverflow(overflow);
 
                    // カスタムノードのインスタンスを GPDSPNodeRenderer クラスのインスタンスに登録
                    error = renderer->appendNode(name, clicker);
                }
            }
        }
        else {
 
            // ノードの種類が一致しないときは GPDSPERROR_NO_SUPPORT を必ず返却
            error = GPDSPERROR_NO_SUPPORT;
        }
        return error;
    }
 
    // 保存を行うための関数
    GPDSPError myApp::save(GPDSPNodeRenderer const& renderer,
                              std::shared_ptr<GPDSPNode const> const& node,
                              std::string const& name,
                              tinyxml2::XMLElement* element) noexcept
    {
        std::shared_ptr<myClickerNode const> clicker;
        tinyxml2::XMLElement* param;
        GPDSPError error(GPDSPERROR_OK);
 
        // ノードの種類が実装したいノードであるかを検証
        if ((clicker = std::dynamic_pointer_cast<myClickerNode const>(node)) != NULL) {
 
            // tinyxml2 を利用してノードの種類をタグ名として設定
            element->SetName("myClickerNode");
 
            // tinyxml2 を利用してノードに固有の値を gpdsp ファイルに保存
            if ((error = GPDSPNodeRenderer::addTag(element, "param",
                             &param)) == GPDSPERROR_OK) {
                if ((error = GPDSPNodeRenderer::writeTag(param, "interlock",
                                 clicker->getInterlock())) == GPDSPERROR_OK) {
                    error = GPDSPNodeRenderer::writeTag(param, "overflow",
                                clicker->getOverflow());
                }
            }
        }
        else {
 
            // ノードの種類が一致しないときは GPDSPERROR_NO_SUPPORT を必ず返却
            error = GPDSPERROR_NO_SUPPORT;
        }
        return error;
    }
    @endcode
 
    最後に GPDSPSerializable クラスを継承したクラスのインスタンスへのポインタを
    GPDSPNodeRenderer::load() 関数や GPDSPNodeRenderer::save() 関数の第２引数に設定して呼び出します.
 
    <b>GPDSPNodeRenderer::load() 関数と GPDSPNodeRenderer::save() 関数の呼び出し方</b>
 
    @code{.cpp}
    using namespace ir;
 
    void myApp::doCopy(void)
    {
        GPDSPNodeRenderer dsp;
 
        dsp.load("custom.gpdsp", this);
        dsp.save("custom_copy.gpdsp", this);
        return;
    }
    @endcode
 
    @section sec_extension_gpdsp gpdsp ファイルでの記述
 
    <b>カスタムノードの gpdsp ファイルでの記述例</b>
 
    @code{.xml}
    <myClickerNode>
        <name>ノード名</name>
        <param>
            <interlock>左チャンネルと右チャンネルを同期するかどうか</interlock>
            <overflow>オーバーフローとなる限界値</overflow>
        </param>
        <input>
            <::0>
                <node>Lch-in に対する入力元のノード名</node>
                <output>::Lch-in に対する入力元のターミナル番号</output>
            </::0>
            <::1>
                <node>Rch-in に対する入力元のノード名</node>
                <output>::Rch-in に対する入力元のターミナル番号</output>
            </::1>
        </input>
    </myClickerNode>
    @endcode
 
 
 
    @page page_error エラー処理
 
    @tableofcontents
 
    @section sec_error_overview エラー処理の方針
    GPDSP ライブラリは, エラーを関数の戻り値として返却し, 例外を利用したエラー処理を行いません.
    エラーは #GPDSPError 型で表現され, GPDSPERROR_ プリフィックスで始まるマクロとして定義されます.
 
    各種関数が実行時にエラーを発生した場合は, 関数が呼び出される前の状態が保持されます.
    引数を通して結果が返却される関数では, 関数の実行が成功しない限り, 引数の内容が書き換えられることはありません.
 
    @section sec_error_print エラー情報の出力
    GPDSPNodeRenderer::stringize() 関数を利用することで, エラーからエラーの解説を含む文字列に変換することができます.
    std::cout や printf() 関数とともに利用することでデバッグ作業を行いやすくなります.
 
    <b>std::cout 利用の場合</b>
 
    @code{.cpp}
    using namespace ir;
 
    GPDSPNodeRenderer dsp;
    GPDSPError error;
 
    error = dsp.load("example.gpdsp");
    std::cout << GPDSPNodeRenderer::stringize(error) << std::endl;
    @endcode
 
    <b>printf() 関数利用の場合</b>
 
    @code{.cpp}
    using namespace ir;
 
    GPDSPNodeRenderer dsp;
    GPDSPError error;
 
    error = dsp.load("example.gpdsp");
    printf("%s\n", GPDSPNodeRenderer::stringize(error));
    @endcode
 
    @section sec_error_loop ディレイ・フリー・ループの扱い
    循環する構造を含むノード接続を行う場合は, 循環経路の途中に GPDSPDelayNode クラスや
    GPDSPBufferNode クラスなどの内部バッファを持つ遅延ノードを挿入しておく必要があります.
 
    遅延ノードを含まない循環経路はディレイ・フリー・ループと呼ばれ, 離散的に演算することができません.
    GPDSP ライブラリでは, ディレイ・フリー・ループを自動的に検出し,
    GPDSPNodeRenderer::render() 関数が #GPDSPERROR_LOOP を返却します.
 
 
 
    @page page_copyright ライセンスと著作権
 
    @tableofcontents
 
    @section sec_copyright_license ライセンス
    Original Copyright (C) 2017 - 2018 HORIGUCHI Junshi. http://iridium.jp/ zap00365@nifty.com
 
    The MIT License (MIT)
 
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
    associated documentation files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or
    substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
    BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
    以下に定める条件に従い、本ソフトウェアおよび関連文書のファイル（以下「ソフトウェア」）の複製を
    取得するすべての人に対し、ソフトウェアを無制限に扱うことを無償で許可します。
    これには、ソフトウェアの複製を使用、複写、変更、結合、掲載、頒布、サブライセンス、および、または販売する権利、
    およびソフトウェアを提供する相手に同じことを許可する権利も無制限に含まれます。
    上記の著作権表示および本許諾表示を、ソフトウェアのすべての複製または重要な部分に記載するものとします。
    ソフトウェアは「現状のまま」で、明示であるか暗黙であるかを問わず、何らの保証もなく提供されます。
    ここでいう保証とは、商品性、特定の目的への適合性、および権利非侵害についての保証も含みますが、それに限定されるものではありません。
    作者または著作権者は、契約行為、不法行為、またはそれ以外であろうと、ソフトウェアに起因または関連し、
    あるいはソフトウェアの使用またはその他の扱いによって生じる一切の請求、損害、その他の義務について何らの責任も負わないものとします。
 
    @section sec_copyright_copyright 著作権
    Copyright (C) 2017 - 2018 HORIGUCHI Junshi. All rights reserved.
 
 
 
    @page page_release リリースノート
 
    @tableofcontents
 
    @todo
    - 入力ターミナルと出力ターミナルのそれぞれで, 重複する名前の設定を禁止する.
    - gpdsp ファイルにおいて, 入力ターミナルと出力ターミナルを名前で指定できるように改良する.
 
    @section sec_release_version094 バージョン 0.9.4
 
    - gpdsp ファイルの記述内で GPDSPBufferInputNode クラスの
      interleave パラメータの指定を省略する場合に, 1 を指定した時と同じ動作を行うように変更
    - gpdsp ファイルの記述内で GPDSPBufferOutputNode クラスの
      interleave パラメータの指定を省略する場合に, 1 を指定した時と同じ動作を行うように変更
    - GPDSPBufferInputNode::getBufferReadonly(), GPDSPBufferInputNode::getBufferWritable() 関数が
      NULL を返却する場合にも, 引数の値を設定して返却するように変更
    - GPDSPBufferOutputNode::getBufferReadonly() 関数が NULL を返却する場合にも, 引数の値を設定して返却するように変更
    - GPDSPGenerativeNode クラスに以下の関数を追加
 
    @code{.cpp}
    int getRate(void) const noexcept;
    @endcode
 
    - GPDSPCustom サンプルコードを追加
    - doxygen ドキュメント加筆
 
    @section sec_release_version093 バージョン 0.9.3
    - <em>[バグ修正]</em> gpdsp ファイルの記述内で GPDSPGenerativeNode クラスの
      file パラメータに自分自身を指定する場合に無限ループに陥るバグを修正し,
      #GPDSPERROR_INVALID_STATE を返却するように変更
    - GPDSPGenericNode クラス -> GPDSPGenerativeNode クラスに名称変更
    - GPDSPGenericInputNode クラス -> GPDSPGenerativeInputNode クラスに名称変更
    - GPDSPGenericOutputNode クラス -> GPDSPGenerativeOutputNode クラスに名称変更
    - generic ディレクトリ -> generative ディレクトリに名称変更
    - gpdsp ファイルのフォーマット変更 (index タグ -> output タグ)
 
    @code{.xml}
    // 旧バージョン
    <input>
        <::0>
            <node>入力元のノード名</node>
            <index>入力元のターミナル番号</index>
        </::0>
    </input>
 
    // 新バージョン
    <input>
        <::0>
            <node>入力元のノード名</node>
            <output>::入力元のターミナル番号</output>
        </::0>
    </input>
    @endcode
 
    - gpdsp ファイルのフォーマット変更 (GPDSPGenericInputNode クラス -> GPDSPGenerativeInputNode クラス)
 
    @code{.xml}
    // 旧バージョン
    <GPDSPGenericInputNode>
        <name>公開する入力ターミナル番号.公開する入力ターミナル名</name>
    </GPDSPGenericInputNode>
 
    // 新バージョン
    <GPDSPGenerativeInputNode>
        <name>公開する入力ターミナル名</name>
        <param>
            <index>公開する入力ターミナル番号</index>
        </param>
    </GPDSPGenerativeInputNode>
    @endcode
 
    - gpdsp ファイルのフォーマット変更 (GPDSPGenericOutputNode クラス -> GPDSPGenerativeOutputNode クラス)
 
    @code{.xml}
    // 旧バージョン
    <GPDSPGenericOutputNode>
        <name>公開する出力ターミナル番号.公開する出力ターミナル名</name>
        <input>
            <::0>
                <node>in に対する入力元のノード名</node>
                <index>in に対する入力元のターミナル番号</index>
            </::0>
        </input>
    </GPDSPGenericOutputNode>
 
    // 新バージョン
    <GPDSPGenerativeOutputNode>
        <name>公開する出力ターミナル名</name>
        <param>
            <index>公開する出力ターミナル番号</index>
        </param>
        <input>
            <::0>
                <node>in に対する入力元のノード名</node>
                <output>::in に対する入力元のターミナル番号</output>
            </::0>
        </input>
    </GPDSPGenerativeOutputNode>
    @endcode
 
    - GPDSPNodeRenderer クラスの以下の関数の仕様を変更
 
    @code{.cpp}
    // 旧バージョン
    std::string getNextNode(void) const noexcept;
    std::string findNode(std::shared_ptr<GPDSPNode const> const& node) const noexcept;
    static std::string stringize(GPDSPError error) noexcept;
 
    // 新バージョン
    std::string const& getNextNode(void) const noexcept;
    std::string const& findNode(std::shared_ptr<GPDSPNode const> const& node) const noexcept;
    static char const* stringize(GPDSPError error) noexcept;
    @endcode
 
    - GPDSPWaveNode クラスの以下の関数の仕様を変更
 
    @code{.cpp}
    // 旧バージョン
    virtual std::string getName(void) const noexcept = 0;
 
    // 新バージョン
    virtual char const* getName(void) const noexcept = 0;
    @endcode
 
    - GPDSPWaveNode クラスに以下の関数を追加
 
    @code{.cpp}
    int getRate(void) const noexcept;
    @endcode
 
    - doxygen ドキュメント加筆
 */

}// end of namespace

#endif
