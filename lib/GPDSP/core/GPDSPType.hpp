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
**      GPDSPType.hpp
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

#ifndef __GPDSPTYPE_HPP
#define __GPDSPTYPE_HPP

/*!
    @file
    @brief マクロ, 型名, 列挙型などを定義するヘッダファイル
 */

#include "../GPDSPConfig.hpp"

//! IridiumFrameworks を表す名前空間
namespace ir {

//! 演算対象のデータ型に応じた定数値を記述するためのマクロ
/*!
    | 32 ビット | 64 ビット |
    | :------------: | :------------: |
    | param\#\#f | param |
 */
#ifdef __GPDSP64
#define GPDSPFV(param)  (param)
#else
#define GPDSPFV(param)  (param##f)
#endif
//! 演算対象のデータ型に応じた出力書式フォーマットを記述するためのマクロ
/*!
    | 32 ビット | 64 ビット |
    | :------------: | :------------: |
    | "%+.6e" | "%+.14e" |
 */
#ifdef __GPDSP64
#define GPDSPFP         "%+.14e"
#else
#define GPDSPFP         "%+.6e"
#endif
//! 演算対象のデータ型に応じた入力書式フォーマットを記述するためのマクロ
/*!
    | 32 ビット | 64 ビット |
    | :------------: | :------------: |
    | "%f" | "%lf" |
 */
#ifdef __GPDSP64
#define GPDSPFS         "%lf"
#else
#define GPDSPFS         "%f"
#endif

//! 演算対象のデータ型
/*!
    32 ビット浮動小数点をもちいる場合は GPDSPConfig.hpp ファイル内で __GPDSP64 マクロを定義せず,
    64 ビット浮動小数点をもちいる場合は定義します.
 */
#ifdef __GPDSP64
typedef double          GPDSPFloat;
#else
typedef float           GPDSPFloat;
#endif

//! エラーを表す定数
enum GPDSPError {
    //! 正常
    GPDSPERROR_OK,
    //! データフロー入力待ち
    GPDSPERROR_WAIT,
    //! 演算対象外として無視
    GPDSPERROR_IGNORE,
    //! 一部の演算のみ完了
    GPDSPERROR_FRAGMENT,
    //! 演算は無限ループ
    GPDSPERROR_LOOP,
    //! サポートされていない
    GPDSPERROR_NO_SUPPORT,
    //! ファイルが存在しない
    GPDSPERROR_NO_FILE,
    //! メモリが不足している
    GPDSPERROR_NO_MEMORY,
    //! 項目が見つからない
    GPDSPERROR_NO_FOUND,
    //! ノードが見つからない
    GPDSPERROR_NO_NODE,
    //! すでに存在している
    GPDSPERROR_ALREADY_EXIST,
    //! 不正な状態
    GPDSPERROR_INVALID_STATE,
    //! 不正なパラメータ
    GPDSPERROR_INVALID_PARAM,
    //! 範囲外のパラメータ
    GPDSPERROR_INVALID_RANGE,
    //! 不正なフォーマット
    GPDSPERROR_INVALID_FORMAT,
    //! 不正なノード
    GPDSPERROR_INVALID_NODE,
    //! 失敗
    GPDSPERROR_FAILED,
    //! エラーを表す定数の最大値
    GPDSPERROR_LIMIT
};
//! 入力ターミナルのモードを表す定数
enum GPDSPMode {
    //! 無効
    GPDSPMODE_NONE,
    //! 非反転入力
    GPDSPMODE_POSITIVE,
    //! 反転入力
    GPDSPMODE_NEGATIVE,
    //! 定数入力
    GPDSPMODE_CONSTANT,
    //! モードを表す定数の最大値
    GPDSPMODE_LIMIT
};

}// end of namespace

#endif
