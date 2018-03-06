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
**      GPDSPSinWaveNode.hpp
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

#ifndef __GPDSPSINWAVENODE_HPP
#define __GPDSPSINWAVENODE_HPP

/*!
    @file
    @brief ir::GPDSPSinWaveNode クラス
 */

#include "GPDSPWaveNode.hpp"

namespace ir {

//! サイン波ノードを表す具象クラス
/*!
    GPDSPSinWaveNode クラスは, 入力された周波数と位相に従ってサイン波を合成するサイン波ノードを表す具象クラスです.
 
    あらかじめ指定された解像度で波形バッファを計算しておき演算時に参照する方法と, 演算時に波形を逐次計算する方法を選択することができます.
 
    ２つの入力ターミナル "frequency", "phase" と, １つの出力ターミナル "out" を持ちます.
 
    | 番号 | 入力 | 出力 |
    | :----: | :----: | :----: |
    | 0 | frequency | out |
    | 1 | phase | - |
 
    <b>gpdsp ファイルでの記述例</b>
    @code{.xml}
    <GPDSPSinWaveNode>
        <name>ノード名</name>
        <param>
            <resolution>解像度</resolution>
        </param>
        <input>
            <::0>
                <node>frequency に対する入力元のノード名</node>
                <output>::frequency に対する入力元のターミナル番号</output>
            </::0>
            <::1>
                <node>phase に対する入力元のノード名</node>
                <output>::phase に対する入力元のターミナル番号</output>
            </::1>
        </input>
    </GPDSPSinWaveNode>
    @endcode
 */
class GPDSPSinWaveNode : public GPDSPWaveNode {
    public:
        //! コンストラクタです.
        /*!
            何もしません.
         
            @param[in] rate サンプリングレート > 0
         */
        explicit                            GPDSPSinWaveNode            (int rate) noexcept;
        //! デストラクタです.
        /*!
            何もしません.
         */
        virtual                             ~GPDSPSinWaveNode           (void) noexcept;
    protected:
        //! 波形バッファの名前を取得します.
        /*!
            @return "GPDSPSinWaveNode"
         */
        virtual char const*                 getName                     (void) const noexcept;
        //! 引数に指定された位相に対応する, サイン波の値を取得します.
        /*!
            @param[in] phase 位相 [0.0, 1.0)
            @return サイン波の値 [-1.0, +1.0]
         */
        virtual GPDSPFloat                  getWave                     (GPDSPFloat phase) const noexcept;
        //! 引数に指定された波形バッファに対して, １波長分のサイン波の値を計算して書き込みます.
        /*!
            @param[in,out] wave 波形バッファ
         */
        virtual void                        fillWave                    (std::vector<std::pair<GPDSPFloat, GPDSPFloat> >* wave) const noexcept;
    private:
                                            GPDSPSinWaveNode            (GPDSPSinWaveNode const&);
                GPDSPSinWaveNode&           operator=                   (GPDSPSinWaveNode const&);
};

}// end of namespace

#endif
