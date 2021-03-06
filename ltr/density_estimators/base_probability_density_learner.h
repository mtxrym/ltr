// Copyright 2012 Yandex
#ifndef LTR_DENSITY_ESTIMATORS_BASE_PROBABILITY_DENSITY_LEARNER_H_
#define LTR_DENSITY_ESTIMATORS_BASE_PROBABILITY_DENSITY_LEARNER_H_

#include <map>
#include <functional>

#include "contrib/include_Eigen.h"

#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/density_estimators/base_probability_density_estimator.h"
#include "ltr/data/data_set.h"
#include "ltr/data/object.h"
#include "ltr/parameters_container/parameters_container.h"
#include "ltr/interfaces/parameterized.h"

using std::map;

using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::aligned_allocator;

using ltr::BaseProbabilityDensityEstimator;
using ltr::DataSet;
using ltr::Object;
using ltr::ParametersContainer;
using ltr::Parameterized;

namespace ltr {

/**
 * Double value representing actual label of object.
 */
typedef double Label;

/**
 * Map representing the mean of the class with some certain label.
 */
typedef map<Label, VectorXd, std::less<Label>,
            aligned_allocator<std::pair<Label, VectorXd> > > LabelToMean;

/**
 * Map representing the covariance matrix of the class with some certain label.
 */
typedef map<Label, MatrixXd, std::less<Label>,
            aligned_allocator<std::pair<Label, MatrixXd> > >
              LabelToCovarianceMatrix;

/**
 * \brief Simple interface of the probability density estimation learner.
 */
template<class TElement, class TEstimator>
class BaseProbabilityDensityLearner : public Parameterized {
  ALLOW_SHARED_PTR_ONLY_CREATION(BaseProbabilityDensityLearner)
 public:
  BaseProbabilityDensityLearner() {
  }

  explicit BaseProbabilityDensityLearner(
    const ParametersContainer& parameters) {
  }

  virtual void learn(const DataSet<TElement>& data_set) {
    learnImpl(data_set, &estimator_);
  }

  typename TEstimator::Ptr makeSpecific() const {
    return estimator_;
  };

  BaseProbabilityDensityEstimator::Ptr make() const {
    return BaseProbabilityDensityEstimator::Ptr(makeSpecific());
  };

 private:

  /**
   * The function implements learning using given training data set.
   *
   * \param data_set is the training data
   * \param estimator is the pointer to the generated density estimator
   */
  virtual void learnImpl(const DataSet<TElement>& data_set,
                         typename TEstimator::Ptr* estimator) = 0;

  typename TEstimator::Ptr estimator_;
};
};

#endif  // LTR_DENSITY_ESTIMATORS_BASE_PROBABILITY_DENSITY_LEARNER_H_
