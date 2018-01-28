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
**      GPDSPWaveNode.hpp
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

#ifndef __GPDSPWAVENODE_HPP
#define __GPDSPWAVENODE_HPP

#include "../core/GPDSPInputtableNode.hpp"
#include "../core/GPDSPOutputtableNode.hpp"
#include "../core/GPDSPRewindableNode.hpp"
#include <math.h>

namespace ir {

//! 波形ノードを表す抽象クラス
/*!
    GPDSPWaveNode クラスは, さまざまな波形を生成して出力する波形ノードを表す抽象クラスです.
 
    あらかじめ指定された解像度で波形バッファを計算しておき演算時に参照する方法と, 演算時に波形を逐次計算する方法を選択することができます.
 
    各種ノードを表す具象クラスは, GPDSPWaveNode クラスの getName(), getWave(), fillWave() 関数を実装する必要があります.
 */
class GPDSPWaveNode : public GPDSPInputtableNode, public GPDSPOutputtableNode, public virtual GPDSPRewindableNode {
    private:
                int                         _rate;
                GPDSPFloat                  _resolution;
                std::vector<std::pair<GPDSPFloat, GPDSPFloat> > const*
                                            _wave;
                GPDSPFloat                  _position;
    
    public:
        //! デフォルトの解像度を取得します.
        /*!
            @retval +INFINITY デフォルトの解像度
         */
        static  GPDSPFloat                  defaultResolution           (void);
        //! 解像度を設定します.
        /*!
            (0.0, +INFINITY) の範囲の値を設定すると, サンプリングレートと解像度に応じた波形バッファがあらかじめ作成され,
            演算時には波形バッファを参照し直線補完にて値を求めます.
         
            +INFINITY を設定すると波形バッファを作成することなく, 演算時に値を逐次計算して求めます.
         
            コンストラクタのサンプリングレート引数に 0 以下の値が設定されているとき, この関数は失敗します.
         
            @param[in] resolution 設定する解像度 (0.0, +INFINITY]
            @retval GPDSPERROR_OK 正常
            @retval GPDSPERROR_NO_MEMORY メモリ不足
            @retval GPDSPERROR_INVALID_STATE 不正な状態
            @retval GPDSPERROR_INVALID_PARAM 不正なパラメータ
            @retval GPDSPERROR_FAILED 失敗
         */
                GPDSPError                  setResolution               (GPDSPFloat resolution);
        //! 解像度を取得します.
        /*!
            @return 現在の解像度
         */
                GPDSPFloat                  getResolution               (void) const;
        //! 入力ターミナルを２つと, 出力ターミナルを１つ生成します.
        /*!
            @retval GPDSPERROR_OK 正常
            @retval GPDSPERROR_NO_MEMORY メモリ不足
         */
        virtual GPDSPError                  fixate                      (void);
        //! 入出力の演算結果を無効化し, 再演算を要求します.
        virtual void                        invalidate                  (void);
        //! 演算前の準備をします.
        /*!
            何もしません.
         
            @retval GPDSPERROR_OK 正常 (準備を完了)
         */
        virtual GPDSPError                  prepare                     (void);
        //! 演算を行います.
        /*!
            @retval GPDSPERROR_OK 正常 (演算を完了)
            @retval GPDSPERROR_WAIT データフロー入力待ち
            @retval GPDSPERROR_INVALID_RANGE 範囲外のパラメータ
         */
        virtual GPDSPError                  process                     (void);
        //! 波形バッファの位相を先頭に再初期化します.
        virtual void                        rewind                      (void);
    protected:
        //! コンストラクタです.
        /*!
            解像度を defaultResolution() に設定し, リソースを初期化します.
         
            @param[in] rate サンプリングレート > 0
         */
        explicit                            GPDSPWaveNode               (int rate);
        //! デストラクタです.
        /*!
            何もしません.
         */
        virtual                             ~GPDSPWaveNode              (void) = 0;
        //! 波形バッファの名前を取得します.
        /*!
            @retval "" [返却禁止]
            @retval その他の名前
         */
        virtual std::string                 getName                     (void) const = 0;
        //! 引数に指定された位相に対応する, 波形の値を取得します.
        /*!
            @param[in] phase 位相 [0.0, 1.0)
            @return 波形の値 [-1.0, +1.0]
         */
        virtual GPDSPFloat                  getWave                     (GPDSPFloat phase) const = 0;
        //! 引数に指定された波形バッファに対して, １波長分の波形の値を計算して書き込みます.
        /*!
            (*wave)[N].first に対して波形の値を設定し, (*wave)[N].second は変更してはいけません.
            また, *wave のサイズを変更したり, *wave を解放してはいけません.
         
            @param[in,out] wave 波形バッファ
         */
        virtual void                        fillWave                    (std::vector<std::pair<GPDSPFloat, GPDSPFloat> >* wave) const = 0;
    private:
                GPDSPError                  makeWave                    (GPDSPFloat resolution, std::vector<std::pair<GPDSPFloat, GPDSPFloat> > const** wave) const;
    private:
                                            GPDSPWaveNode               (GPDSPWaveNode const&);
                GPDSPWaveNode&              operator=                   (GPDSPWaveNode const&);
};

inline GPDSPFloat GPDSPWaveNode::defaultResolution(void)
{
    return +INFINITY;
}

inline GPDSPFloat GPDSPWaveNode::getResolution(void) const
{
    return _resolution;
}

}// end of namespace

#endif
