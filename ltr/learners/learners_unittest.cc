// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include <vector>
#include <limits>
#include <string>

#include "ltr/data/object.h"
#include "ltr/learners/best_feature_learner/best_feature_learner.h"
#include "ltr/scorers/fake_scorer.h"
#include "ltr/measures/abs_error.h"
#include "ltr/measures/ndcg.h"
#include "ltr/measures/dcg.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/data/data_set.h"
#include "ltr/feature_converters/nan_to_zero_learner.h"
#include "ltr/learners/linear_learner/linear_learner.h"
#include "ltr/scorers/linear_scorer.h"
#include "ltr/utility/boost/path.h"

#include "contrib/include_Eigen.h"

using ltr::utility::FixPathSeparators;

using std::string;

using ltr::io_utility::loadDataSet;

using ltr::FeatureConverter;
using ltr::NanToZeroConverterLearner;
using ltr::Object;
using ltr::DataSet;

class LearnersTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    string learn_data_file_name = FixPathSeparators("data/imat2009/imat2009_learning_small.txt");  // NOLINT
    learn_data = loadDataSet<Object>(learn_data_file_name, "YANDEX");

    string test_data_file_name = FixPathSeparators("data/imat2009/imat2009_test_small.txt");  // NOLINT
    test_data = loadDataSet<Object>(test_data_file_name, "YANDEX");
  }

  DataSet<Object> learn_data;
  DataSet<Object> test_data;
};

TEST_F(LearnersTest, TestingBestFeatureLearner) {
  NanToZeroConverterLearner<Object>::Ptr nan_to_zero_converter(
    new NanToZeroConverterLearner<Object>);
  nan_to_zero_converter->learn(learn_data);
  FeatureConverter::Ptr remove_nan = nan_to_zero_converter->make();
  remove_nan->apply(learn_data, &learn_data);
  remove_nan->apply(test_data, &test_data);

  ltr::Measure<ltr::Object>::Ptr abs_error_measure(new ltr::AbsError());
  ltr::BestFeatureLearner<ltr::Object>::Ptr learner(new
    ltr::BestFeatureLearner<ltr::Object>(abs_error_measure));
  learner->learn(learn_data);

  ltr::FakeScorer::Ptr fake_scorer(
    new ltr::FakeScorer(std::numeric_limits<double>::max()));
  fake_scorer->predict(test_data);
  double measure_before = abs_error_measure->average(test_data);

  learner->make()->predict(test_data);
  double measure_after = abs_error_measure->average(test_data);

  EXPECT_LE(measure_after, measure_before) << "It can't be worth.\n";
};
