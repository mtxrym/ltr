// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_SCORERS_BASE_PROBABILITY_DENSITY_ESTIMATOR_H_
#define LTR_DENSITY_ESTIMATORS_SCORERS_BASE_PROBABILITY_DENSITY_ESTIMATOR_H_

#include "ltr/data/object.h"
#include "ltr/scorers/scorer.h"
#include "ltr/utility/shared_ptr.h"

using ltr::Object;

namespace ltr {

/**
 * \class Simple interface of the probability density estimator
 */
class BaseProbabilityDensityEstimator {
 public:
  typedef ltr::utility::shared_ptr<BaseProbabilityDensityEstimator> Ptr;
  virtual double estimate(const Object& object, const double label) = 0;
  virtual ~BaseProbabilityDensityEstimator() {
  }
};
};

#endif  // LTR_DENSITY_ESTIMATORS_SCORERS_BASE_PROBABILITY_DENSITY_ESTIMATOR_H_
