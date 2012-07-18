// Copyright 2012 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_STRATEGIES_SELECT_TOP_TREES_STRATEGY_H_
#define LTR_LEARNERS_GP_LEARNER_STRATEGIES_SELECT_TOP_TREES_STRATEGY_H_

#include <math.h>
#include <vector>
#include <algorithm>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/learners/gp_learner/strategies/population_handler.h"

using std::vector;

using Puppy::Tree;
using Puppy::Context;

namespace ltr {
namespace gp {
/**
* Select top fraction of trees for the next generation.
*/
class SelectTopTreesStrategy : public BasePopulationHandler {
 public:
  explicit SelectTopTreesStrategy(double top_fraction = 0.3)
  : top_fraction_(top_fraction) {}

  virtual void HandlePopulation(vector<Tree>& population, Context& context); // NOLINT

 private:
  double top_fraction_;
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_STRATEGIES_SELECT_TOP_TREES_STRATEGY_H_
