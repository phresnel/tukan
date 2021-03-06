// (C) 2013 Sebastian Mach (1983), this file is published under the terms of the
// GNU General Public License, Version 3 (a.k.a. GPLv3).
// See COPYING in the root-folder of the excygen project folder.
#ifndef SPECTRUM_HH_INCLUDED_20131017
#define SPECTRUM_HH_INCLUDED_20131017

#include "../detail/tuple.hh"
#include "../Nanometer.hh"
#include "../Interval.hh"
#include <valarray>
#include <stdexcept>

namespace tukan { namespace future {

    //----------------------------------------------------------------------------------------------
    // SpectrumSample
    //----------------------------------------------------------------------------------------------
    struct SpectrumSample {
        Nanometer wavelength;
        float     amplitude = 0;

        constexpr SpectrumSample() = default;
        constexpr SpectrumSample(Nanometer w, float a) : wavelength(w), amplitude(a) {}
    };

    constexpr bool operator== (SpectrumSample const &lhs, SpectrumSample const &rhs) noexcept ;
    constexpr bool operator!= (SpectrumSample const &lhs, SpectrumSample const &rhs) noexcept ;



    //----------------------------------------------------------------------------------------------
    // Spectrum
    //----------------------------------------------------------------------------------------------
    class Spectrum {
    public:
        template <typename Cont>
        Spectrum (Nanometer lambda_min, Nanometer lambda_max, Cont const &bins);

        Nanometer lambda_min() const ;
        Nanometer lambda_max() const ;

        size_t size() const ;
        bool empty() const ;

        float operator[] (size_t i) const noexcept ;
        float at (size_t i) const ;

    private:
        Nanometer lambda_min_, lambda_max_;
        std::valarray<float> bins_;
    };



    //----------------------------------------------------------------------------------------------
    // LinearInterpolator
    //----------------------------------------------------------------------------------------------
    struct LinearInterpolator {

        LinearInterpolator (Spectrum const &spec) : spec(&spec) {}

        SpectrumSample operator() (Interval<float> r) const ;
        SpectrumSample operator() (float f)           const ;
        SpectrumSample operator() (Nanometer g)       const ;

    private:

        Spectrum const *spec;
    };



    //---------------------------------------------------------------------------------------------
    // implementation
    //---------------------------------------------------------------------------------------------

    // SpectrumSample
    constexpr bool operator== (SpectrumSample const &lhs, SpectrumSample const &rhs) noexcept {
        return lhs.wavelength == rhs.wavelength && lhs.amplitude == rhs.amplitude;
    }
    constexpr bool operator!= (SpectrumSample const &lhs, SpectrumSample const &rhs) noexcept {
        return !(lhs == rhs);
    }

    constexpr bool rel_equal(SpectrumSample const &lhs, SpectrumSample const &rhs,
                          float max_rel_diff=std::numeric_limits<float>::epsilon()
                         ) noexcept
    {
        using tukan::rel_equal;
        return rel_equal(lhs.amplitude, rhs.amplitude, max_rel_diff)
            && rel_equal(lhs.wavelength, rhs.wavelength, max_rel_diff);
    }



    // Spectrum
    template <typename Cont>
    Spectrum::Spectrum (Nanometer lambda_min, Nanometer lambda_max, Cont const &bins)
    : lambda_min_(lambda_min), lambda_max_(lambda_max), bins_(bins.size())
    {
        bins_.resize(bins.size());
        for (size_t i=0; i!=bins.size(); ++i)
            bins_[i] = bins[i];
    }


    inline Nanometer Spectrum::lambda_min() const {
        return lambda_min_;
    }


    inline Nanometer Spectrum::lambda_max() const {
        return lambda_max_;
    }


    inline size_t Spectrum::size() const {
        return bins_.size();
    }


    inline bool Spectrum::empty() const {
        return 0==size();
    }


    inline float Spectrum::operator [](size_t i) const noexcept {
        return bins_[i];
    }


    inline float Spectrum::at(size_t i) const {
        if (i < std::numeric_limits<size_t>::min() || i>=size())
            throw std::out_of_range("passed value outside range to Spectrum::at(size_t)");
        return (*this)[i];
    }



    inline SpectrumSample LinearInterpolator::operator() (float f) const
    {
        if (f<0) throw std::logic_error("passed value < 0 to Spectrum::operator()(float)");
        if (f>1) throw std::logic_error("passed value > 1 to Spectrum::operator()(float)");
        if (f == 0.0) return SpectrumSample(spec->lambda_min(), (*spec)[0]);
        if (f == 1.0) return SpectrumSample(spec->lambda_max(), (*spec)[(*spec).size()-1]);

        Nanometer g = (*spec).lambda_min() + Nanometer(f)*((*spec).lambda_max() - (*spec).lambda_min());

        const int i = f*static_cast<float>((*spec).size()-1);
        const Interval<float> bin_interval (i / float((*spec).size()-1), (1+i) / float((*spec).size()-1));

        float frac = (f-bin_interval.min) / (bin_interval.max-bin_interval.min);
        if (size_t(i+1) >= (*spec).size()) // TODO: can this happen?
                throw std::logic_error("crap");
        return SpectrumSample(g, (*spec)[i]*(1-frac) + (*spec)[i+1]*frac);
    }


    inline SpectrumSample LinearInterpolator::operator() (Nanometer g) const {
        if (g<(*spec).lambda_min()) throw std::logic_error("passed value < lambda_min to "
                                                  "Spectrum::operator()(Nanometer)");
        if (g>(*spec).lambda_max()) throw std::logic_error("passed value > lambda_max to "
                                                  "Spectrum::operator()(Nanometer)");

        float f = static_cast<float>((g - (*spec).lambda_min()) / ((*spec).lambda_max()-(*spec).lambda_min()));
        return (*this)(f);
    }


    inline SpectrumSample LinearInterpolator::operator() (Interval<float> r) const {
        if (r.min<0) throw std::logic_error("passed value < 0 to "
                                              "Spectrum::operator()(Interval<float>)");
        if (r.max>1) throw std::logic_error("passed value > 1 to "
                                              "Spectrum::operator()(Interval<float>)");

        if (r.min == r.max) {
            // We assume that (amp*0)/inf == amp.
            return (*this)(r.min);
        }

        auto avg = [](Interval<float> const &i, float A, float B)
        {
            return 0.5*(A*(1-i.min) + B*i.min)
                 + 0.5*(A*(1-i.max) + B*i.max);
        };

        const auto delta = float(1)/(spec->size()-1);
        auto bin_average = [&delta, &r, &avg, this] (int bin)
        {
            const Interval<float> bin_global (bin*delta, bin*delta+delta);
            const auto overlap_global = intersection(bin_global, r);
            //if (!overlap_global)
            //    throw std::logic_error("in SpectrumSample::operator()(Interval)");

            // translate the spectrum-space overlap-interval to bin-space
            const Interval<float> overlap_local((overlap_global->min - bin_global.min)/delta,
                                                (overlap_global->max - bin_global.min)/delta);

            const auto weight = length(*overlap_global);
            const auto local_avg_amplitude = avg(overlap_local, (*spec)[bin], (*spec)[bin+1]);
            return make_tuple(weight, local_avg_amplitude);
        };

        const int min_i = r.min * (spec->size()-1);
        const int max_i = r.max * (spec->size()-1);

        float weight_total = 0;
        SpectrumSample ret;

        // Given our bins            [    |    |    |    |    ]
        // with the query interval      [______________]
        // we compute the average and weight for the first and last bin in the interval exactly,
        // and then iterate over the inner bins using a constant weight:
        //  [exact|const|const|exact|    ]
        //     [________________]

        // First bin:
        {
            auto ba = bin_average(min_i);
            ret.amplitude += get<1>(ba) * get<0>(ba);
            weight_total  += get<0>(ba);
        }

        // Second bin (if there's only one bin in the interval, we already have it):
        if (min_i != max_i)
        {
            auto ba = bin_average(max_i);
            ret.amplitude += get<1>(ba) * get<0>(ba);
            weight_total  += get<0>(ba);
        }

        // Inner bins with known weight (we could solely use the bin_average() function,
        //                               but this is less performant)
        for (auto i=min_i+1; i<max_i; ++i) {
            auto ba = (*spec)[i]*0.5f + (*spec)[i+1]*0.5f;
            ret.amplitude += ba * delta;
            weight_total  += delta;
        }

        ret.amplitude /= weight_total;
        ret.wavelength = Nanometer(avg(r, float(spec->lambda_min()), float(spec->lambda_max())));

        return ret;
    }
} }

#endif // SPECTRUM_HH_INCLUDED_20131017
