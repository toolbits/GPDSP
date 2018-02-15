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
    @dir generic
    @brief 汎用ノードの機能を提供する具象クラスを含むディレクトリ
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
#include "generic/GPDSPGenericNode.hpp"

namespace ir {

/*!
    @mainpage GPDSP ライブラリ概要
 
    @tableofcontents
 
    @section sec_overview GPDSP とは
    GPDSP ライブラリは, デジタル信号処理を簡単に行うためのライブラリです.
    増幅や遅延や加算などを表す任意のノードをプログラム上で接続することにより,
    デジタル信号処理のアルゴリズムを簡単に構築して実行することができます.
 
    開発者はデジタル信号処理のデータの計算順序やアルゴリズムの詳細を記述する必要はなく,
    GPDSP ライブラリがデータフローを適切に最適化します. 最適化はデジタル信号処理の実行中にも動的に行われるため,
    ノードの構成をダイナミックに変更しても最速となる計算順序を自動的に探索します.
 
    また, .gpdsp 形式の外部ファイルから XML 形式で記述された任意のノード構成を読み込み,
    １つのノードとして利用することができます. この機能を利用することにより, あらかじめ用意されたノードの動作以外にも,
    任意の特性を持つノードを作成して実行することができるようになります.
 
    @section sec_howtouse 利用方法
    GPDSP ライブラリでは, 基本的には GPDSPNodeRenderer クラスを利用して,
    各種ノードを生成したりノードを接続してデータフローを設計します.
 
    各種ノードに固有の属性や動作を設定したり変更するときは,
    GPDSPNodeRenderer クラスのインスタンスから各種ノードのインスタンスを取得して, ノードに固有の関数を呼び出します.
 
    以下に 100 バイトのデータに対して増幅率 2.0 のデジタル信号処理を行うプログラム例を示します.
 
    <b>GPDSP ライブラリを利用したプログラム例</b>
 
    @code{.cpp}
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
 
    @section sec_lineup ノードの種類
    GPDSP ライブラリには, 以下の表に示すような具象ノードがあらかじめ用意されています.
 
    開発者は, GPDSPGenericNode クラスを利用するか,
    抽象ノードを継承した新しいノードを実装することにより, 独自の機能を持ったノードを追加することもできます.
 
    | クラス名 | 解説 |
    | --------- | --------- |
    | GPDSPBufferInputNode | バッファ入力ノード |
    | GPDSPBufferOutputNode | バッファ出力ノード |
    | GPDSPConstantNode | 定数ノード |
    | GPDSPSignNode | 符号ノード |
    | GPDSPGateNode | 制限ノード |
    | GPDSPPeakNode | 極限ノード |
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
    | GPDSPGenericNode | 汎用ノード |
 
    @section sec_generic 汎用ノードと .gpdsp ファイル
    執筆中
 
    @section sec_feature ライブラリの特徴
    執筆中
 
 
 
    @page page_gpdsp ノード構成の保存と復元
 
    @tableofcontents
 
    @section sec_gpdsp_gpdsp .gpdsp ファイルとは
    執筆中
 
 
 
    @page page_generic 汎用ノード
 
    @tableofcontents
 
    @section sec_generic_overview 汎用ノードとは
    執筆中
 
    @section sec_generic_gpdsp .gpdsp ファイルの記述
    執筆中
 
 
 
    @page page_error エラー処理
 
    @tableofcontents
 
    @section sec_error_overview エラー処理の方針
    GPDSP ライブラリは, エラー値を関数の戻り値として返却し, 例外をもちいたエラー処理を行いません.
    エラー値は #GPDSPError 型で表現され, GPDSPERROR_ プリフィックスで始まるマクロとして定義されています.
 
    各種関数は実行時にエラーが発生した場合, 関数が呼び出される前の状態を保持します.
    引数を介して結果が返却される関数では, 関数の実行が成功しない限り, 引数の内容が書き換えられることはありません.
 
    @section sec_error_print エラー情報の出力
    エラー値は GPDSPNodeRenderer::stringize() 関数を利用することで, エラーの解説を含む文字列に変換することができます.
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
    printf("%s\n", GPDSPNodeRenderer::stringize(error).c_str());
    @endcode
 
    @section sec_error_loop ディレイ・フリー・ループの扱い
    循環する構造を含むノード接続を行う場合, 循環経路の途中に GPDSPDelayNode クラスや
    GPDSPBufferNode クラスなどの遅延ノードを挿入しておく必要があります.
 
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
 */

}// end of namespace

#endif
