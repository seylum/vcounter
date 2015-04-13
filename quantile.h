#ifndef QUANTILE_H
#define QUANTILE_H

#include "boost/config.hpp"
#include <algorithm>
#include <vector>
#include <cmath>
#include "boost/lexical_cast.hpp"

namespace clx {
    /* --------------------------------------------------------------------- */
    //  quantile
    /* --------------------------------------------------------------------- */
    template <class InIter>
    inline double quantile(InIter first, InIter last, unsigned int k, unsigned int q) {
        if (k > q) return static_cast<double>(0);

        std::vector<double> v;
        while (first != last) v.push_back(boost::lexical_cast<double>(*first++));
        std::sort(v.begin(), v.end());

        unsigned int idx = q + k * v.size() - k;
        if (idx % q == 0) return v.at(idx / q - 1);

        double t = idx / static_cast<double>(q);
        unsigned int ceil = static_cast<unsigned int>(std::ceil(t));
        unsigned int floor = static_cast<unsigned int>(std::floor(t));

        return (ceil - t) * v.at(floor - 1) + (t - floor) * v.at(ceil - 1);
    }


    /* --------------------------------------------------------------------- */
    //  percentile
    /* --------------------------------------------------------------------- */
    template <class InIter>
    inline double percentile(InIter first, InIter last, unsigned int k) {
        return quantile(first, last, k, 100);
    }
}

#endif // QUANTILE_H
