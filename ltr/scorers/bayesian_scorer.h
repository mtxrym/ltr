// Copyright 2012 Yandex

#ifndef LTR_SCORERS_BAYESIAN_SCORER_H_
#define LTR_SCORERS_BAYESIAN_SCORER_H_

#include <map>
#include <string>

#include "ltr/density_estimators/base_probability_density_estimator.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/data/object.h"
#include "ltr/scorers/scorer.h"

using ltr::BaseProbabilityDensityEstimator;
using ltr::Object;

using ltr::utility::lexical_cast;

namespace ltr {

class BayesianScorer : public Scorer {
  ALLOW_SHARED_PTR_ONLY_CREATION(BayesianScorer)
 public:
  typedef map<double, double> LabelToPriorProbability;

  BayesianScorer() {
  }

  BayesianScorer(const LabelToPriorProbability& prior_probability,
                 BaseProbabilityDensityEstimator::Ptr estimator) :
    prior_probability_(prior_probability),
    estimator_(estimator) {
  }

 private:
  virtual double scoreImpl(const Object& object) const;
  virtual string generateCppCodeImpl(const string& function_name) const;
  virtual string getDefaultAlias() const {return "BayesianScorer";}

  LabelToPriorProbability prior_probability_;
  BaseProbabilityDensityEstimator::Ptr estimator_;

  string toStringImpl() const {
    return "Bayesian Scorer";
  };
};
};
#endif  // LTR_SCORERS_BAYESIAN_SCORER_H_
