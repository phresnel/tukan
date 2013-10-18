// (C) 2013 Sebastian Mach (1983), this file is published under the terms of the
// GNU General Public License, Version 3 (a.k.a. GPLv3).
// See COPYING in the root-folder of the excygen project folder.
#ifndef RGB_HH_INCLUDED_20131017
#define RGB_HH_INCLUDED_20131017

#include "rel_equal.hh"

namespace gaudy {

    //---------------------------------------------------------------------------------------------
    // RGB
    //---------------------------------------------------------------------------------------------

    // -- structure -------------------------------------------------------------------------------
    struct RGB {
        using value_type = float;

        float r=0, g=0, b=0;

        constexpr RGB() noexcept = default;
        constexpr RGB(float r, float g, float b) noexcept : r(r), g(g), b(b) {}

        RGB& operator+= (RGB rhs) noexcept;
        RGB& operator-= (RGB rhs) noexcept;
        RGB& operator*= (RGB rhs) noexcept;
        RGB& operator/= (RGB rhs) noexcept;

        RGB& operator+= (float rhs) noexcept;
        RGB& operator-= (float rhs) noexcept;
        RGB& operator*= (float rhs) noexcept;
        RGB& operator/= (float rhs) noexcept;
    };


    // -- relation --------------------------------------------------------------------------------
    constexpr bool operator== (RGB lhs, RGB rhs) noexcept;
    constexpr bool operator!= (RGB lhs, RGB rhs) noexcept;
    constexpr bool rel_equal (RGB lhs, RGB rhs,
                              float max_rel_diff=std::numeric_limits<float>::epsilon() ) noexcept;
}

#include <ostream>
namespace gaudy {

    // -- arithmetics -----------------------------------------------------------------------------
    constexpr RGB operator+ (RGB lhs, RGB rhs) noexcept;
    constexpr RGB operator- (RGB lhs, RGB rhs) noexcept;
    constexpr RGB operator* (RGB lhs, RGB rhs) noexcept;
    constexpr RGB operator/ (RGB lhs, RGB rhs) noexcept;

    constexpr RGB operator+ (RGB lhs, float rhs) noexcept;
    constexpr RGB operator- (RGB lhs, float rhs) noexcept;
    constexpr RGB operator* (RGB lhs, float rhs) noexcept;
    constexpr RGB operator/ (RGB lhs, float rhs) noexcept;

    constexpr RGB operator+ (float lhs, RGB rhs) noexcept;
    constexpr RGB operator- (float lhs, RGB rhs) noexcept;
    constexpr RGB operator* (float lhs, RGB rhs) noexcept;
    constexpr RGB operator/ (float lhs, RGB rhs) noexcept;

}


#include <cmath>
namespace gaudy {

    // -- <cmath> ---------------------------------------------------------------------------------

    // TODO: replace all std-qualifcations by a using

    //-- -- -- -- --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- --
    // Info: While glibcxx defines some (many? (all?)) cmath functions as constexpr, this is not
    //       sanctioned  by  the  C++11  standard,  therefore we are not making the  promise  of
    //       constexpr, as such code is non-portable.
    //-- -- -- -- --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- --


    // trigonometric
    inline RGB cos(RGB v) noexcept { return RGB{std::cos(v.r), std::cos(v.g), std::cos(v.b)}; }
    inline RGB sin(RGB v) noexcept { return RGB{std::sin(v.r), std::sin(v.g), std::sin(v.b)}; }
    inline RGB tan(RGB v) noexcept { return RGB{std::tan(v.r), std::tan(v.g), std::tan(v.b)}; }
    inline RGB acos(RGB v) noexcept { return RGB{std::acos(v.r), std::acos(v.g), std::acos(v.b)}; }
    inline RGB asin(RGB v) noexcept { return RGB{std::asin(v.r), std::asin(v.g), std::asin(v.b)}; }
    inline RGB atan(RGB v) noexcept { return RGB{std::atan(v.r), std::atan(v.g), std::atan(v.b)}; }
    inline RGB atan2(RGB v, RGB w) noexcept { return RGB{std::atan2(v.r,w.r),
                                                         std::atan2(v.g,w.g),
                                                         std::atan2(v.b,w.b)}; }
    inline RGB atan2(RGB v, float w) noexcept { return RGB{std::atan2(v.r,w),
                                                           std::atan2(v.g,w),
                                                           std::atan2(v.b,w)}; }
    inline RGB atan2(float v, RGB w) noexcept { return RGB{std::atan2(v,w.r),
                                                           std::atan2(v,w.g),
                                                           std::atan2(v,w.b)}; }

    // hyperbolic
    inline RGB cosh(RGB v) noexcept { return RGB{std::cosh(v.r), std::cosh(v.g), std::cosh(v.b)}; }
    inline RGB sinh(RGB v) noexcept { return RGB{std::sinh(v.r), std::sinh(v.g), std::sinh(v.b)}; }
    inline RGB tanh(RGB v) noexcept { return RGB{std::tanh(v.r), std::tanh(v.g), std::tanh(v.b)}; }
    inline RGB acosh(RGB v) noexcept { return RGB{std::acosh(v.r), std::acosh(v.g), std::acosh(v.b)}; }
    inline RGB asinh(RGB v) noexcept { return RGB{std::asinh(v.r), std::asinh(v.g), std::asinh(v.b)}; }
    inline RGB atanh(RGB v) noexcept { return RGB{std::atanh(v.r), std::atanh(v.g), std::atanh(v.b)}; }

    // exponential and logarithmic
    inline RGB exp(RGB v) noexcept { return RGB{std::exp(v.r), std::exp(v.g), std::exp(v.b)}; }

    // TODO: the second parameter should be RGB<int>, once RGB is generified
    inline RGB frexp(RGB v, RGB *exp) noexcept {
        using std::frexp;
        int exp_r, exp_g, exp_b;
        RGB sig {frexp(v.r, &exp_r),
                 frexp(v.g, &exp_g),
                 frexp(v.b, &exp_b)};
        *exp = RGB(exp_r, exp_g, exp_b);
        return sig;
    }

    // TODO: the second parameter should be RGB<int>, once RGB is generified
    inline RGB frexp(RGB sig, RGB exp) noexcept {
        using std::ldexp;
        return {ldexp(sig.r, exp.r),
                ldexp(sig.g, exp.g),
                ldexp(sig.b, exp.b)};
    }

    inline RGB log(RGB v) noexcept { return {std::log(v.r), std::log(v.g), std::log(v.b)}; }
    inline RGB log10(RGB v)noexcept{return {std::log10(v.r), std::log10(v.g), std::log10(v.b)};}

    inline RGB modf(RGB v, RGB *intpart) noexcept {
        using std::modf;
        return { modf(v.r, &intpart->r),
                 modf(v.g, &intpart->g),
                 modf(v.b, &intpart->b) };
    }

    inline RGB exp2(RGB v)noexcept{return {std::exp2(v.r), std::exp2(v.g), std::exp2(v.b)};}
    inline RGB expm1(RGB v)noexcept{return {std::expm1(v.r), std::expm1(v.g), std::expm1(v.b)};}

    // TODO: the return type should be RGB<int>, once RGB is generified
    inline RGB ilogb(RGB v)noexcept{using std::ilogb; return RGB(ilogb(v.r),ilogb(v.g),ilogb(v.b));}

    inline RGB log1p(RGB v) noexcept{ using std::log1p; return {log1p(v.r),log1p(v.g),log1p(v.b)}; }
    inline RGB log2 (RGB v) noexcept{ using std::log2;  return {log2(v.r),log2(v.g),log2(v.b)}; }

    inline RGB scalbn (RGB v, int n) noexcept {
        using std::scalbn;  return {scalbn(v.r,n), scalbn(v.g,n), scalbn(v.b,n)};
    }
    inline RGB scalbn (RGB v, RGB n) noexcept { // TODO: RGB<int> as rhs
        using std::scalbn;
        return {scalbn(v.r,(int)n.r), scalbn(v.g,(int)n.g), scalbn(v.b,(int)n.b)};
    }

    inline RGB scalbln (RGB v, long n) noexcept {
        using std::scalbln;
        return {scalbln(v.r,n), scalbln(v.g,n), scalbln(v.b,n)};
    }
    inline RGB scalbln (RGB v, RGB n) noexcept { // TODO: RGB<long> as rhs
        using std::scalbln;
        return {scalbln(v.r,(long)n.r), scalbln(v.g,(long)n.g), scalbln(v.b,(long)n.b)};
    }


    // power
    inline RGB pow(RGB v, RGB w) noexcept { return RGB{std::pow(v.r,w.r),
                                                       std::pow(v.g,w.g),
                                                       std::pow(v.b,w.b)}; }
    inline RGB pow(RGB v, float w) noexcept { return RGB{std::pow(v.r,w),
                                                         std::pow(v.g,w),
                                                         std::pow(v.b,w)}; }
    inline RGB pow(float v, RGB w) noexcept { return RGB{std::pow(v,w.r),
                                                         std::pow(v,w.g),
                                                         std::pow(v,w.b)}; }

    inline RGB sqrt(RGB v) noexcept { return RGB{std::sqrt(v.r), std::sqrt(v.g), std::sqrt(v.b)}; }
    inline RGB cbrt(RGB v) noexcept { return RGB{std::cbrt(v.r), std::cbrt(v.g), std::cbrt(v.b)}; }


    inline RGB hypot(RGB v, RGB w) noexcept { return RGB{std::hypot(v.r,w.r),
                                                         std::hypot(v.g,w.g),
                                                         std::hypot(v.b,w.b)}; }
    inline RGB hypot(RGB v, float w) noexcept { return RGB{std::hypot(v.r,w),
                                                           std::hypot(v.g,w),
                                                           std::hypot(v.b,w)}; }
    inline RGB hypot(float v, RGB w) noexcept { return RGB{std::hypot(v,w.r),
                                                           std::hypot(v,w.g),
                                                           std::hypot(v,w.b)}; }

    // error and gamma
    inline RGB erf (RGB v) noexcept { using std::erf;  return {erf(v.r),  erf(v.g),  erf(v.b)};  }
    inline RGB erfc(RGB v) noexcept { using std::erfc; return {erfc(v.r), erfc(v.g), erfc(v.b)}; }
    inline RGB lgamma(RGB v)noexcept{using std::lgamma;return{lgamma(v.r),lgamma(v.g),lgamma(v.b)};}
    inline RGB tgamma(RGB v)noexcept{using std::tgamma;return{tgamma(v.r),tgamma(v.g),tgamma(v.b)};}


    // rounding and remainder

    // floating point manipulation

    // min, max, difference

    // other

    // classification functions

    // comparison functions


    //---------------------------------------------------------------------------------------------
    // implementation
    //---------------------------------------------------------------------------------------------
    inline RGB& RGB::operator+= (RGB rhs) noexcept {
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        return *this;
    }

    inline RGB& RGB::operator-= (RGB rhs) noexcept {
        r -= rhs.r;
        g -= rhs.g;
        b -= rhs.b;
        return *this;
    }

    inline RGB& RGB::operator*= (RGB rhs) noexcept {
        r *= rhs.r;
        g *= rhs.g;
        b *= rhs.b;
        return *this;
    }

    inline RGB& RGB::operator/= (RGB rhs) noexcept {
        r /= rhs.r;
        g /= rhs.g;
        b /= rhs.b;
        return *this;
    }

    inline RGB& RGB::operator+= (float rhs) noexcept {
        r += rhs;
        g += rhs;
        b += rhs;
        return *this;
    }

    inline RGB& RGB::operator-= (float rhs) noexcept {
        r -= rhs;
        g -= rhs;
        b -= rhs;
        return *this;
    }

    inline RGB& RGB::operator*= (float rhs) noexcept {
        r *= rhs;
        g *= rhs;
        b *= rhs;
        return *this;
    }

    inline RGB& RGB::operator/= (float rhs) noexcept {
        r /= rhs;
        g /= rhs;
        b /= rhs;
        return *this;
    }


    // relation
    constexpr bool operator== (RGB lhs, RGB rhs) noexcept {
        return lhs.r==rhs.r && lhs.g==rhs.g && lhs.b==rhs.b;
    }
    constexpr bool operator!= (RGB lhs, RGB rhs) noexcept {
        return !(lhs == rhs);
    }
    constexpr bool rel_equal (RGB lhs, RGB rhs, float max_rel_diff) noexcept
    {
        return rel_equal (lhs.r, rhs.r, max_rel_diff)
            && rel_equal (lhs.g, rhs.g, max_rel_diff)
            && rel_equal (lhs.b, rhs.b, max_rel_diff)
        ;
    }


    // arithmetics
    constexpr RGB operator+ (RGB lhs, RGB rhs) noexcept {
        return {lhs.r+rhs.r, lhs.g+rhs.g, lhs.b+rhs.b};
    }
    constexpr RGB operator- (RGB lhs, RGB rhs) noexcept {
        return {lhs.r-rhs.r, lhs.g-rhs.g, lhs.b-rhs.b};
    }
    constexpr RGB operator* (RGB lhs, RGB rhs) noexcept {
        return {lhs.r*rhs.r, lhs.g*rhs.g, lhs.b*rhs.b};
    }
    constexpr RGB operator/ (RGB lhs, RGB rhs) noexcept {
        return {lhs.r/rhs.r, lhs.g/rhs.g, lhs.b/rhs.b};
    }

    constexpr RGB operator+ (RGB lhs, float rhs) noexcept {
        return {lhs.r+rhs, lhs.g+rhs, lhs.b+rhs};
    }
    constexpr RGB operator- (RGB lhs, float rhs) noexcept {
        return {lhs.r-rhs, lhs.g-rhs, lhs.b-rhs};
    }
    constexpr RGB operator* (RGB lhs, float rhs) noexcept {
        return {lhs.r*rhs, lhs.g*rhs, lhs.b*rhs};
    }
    constexpr RGB operator/ (RGB lhs, float rhs) noexcept {
        return {lhs.r/rhs, lhs.g/rhs, lhs.b/rhs};
    }

    constexpr RGB operator+ (float lhs, RGB rhs) noexcept {
        return {lhs+rhs.r, lhs+rhs.g, lhs+rhs.b};
    }
    constexpr RGB operator- (float lhs, RGB rhs) noexcept {
        return {lhs-rhs.r, lhs-rhs.g, lhs-rhs.b};
    }
    constexpr RGB operator* (float lhs, RGB rhs) noexcept {
        return {lhs*rhs.r, lhs*rhs.g, lhs*rhs.b};
    }
    constexpr RGB operator/ (float lhs, RGB rhs) noexcept {
        return {lhs/rhs.r, lhs/rhs.g, lhs/rhs.b};
    }
}

#endif // RGB_HH_INCLUDED_20131017
