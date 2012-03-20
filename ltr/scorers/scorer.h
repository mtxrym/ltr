// Copyright 2011 Yandex

#ifndef LTR_SCORERS_SCORER_H_
#define LTR_SCORERS_SCORER_H_

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <string>

#include "ltr/interfaces/aliaser.h"
#include "ltr/data/object.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/interfaces/serializable_functor.h"

using std::string;

namespace ltr {
/*
 * Scorer is a class that can for every object evaluate
 * a value - its rank, or score
 */
class Scorer : public Aliaser, public SerializableFunctor<double> {
  public:
  typedef boost::shared_ptr<Scorer> Ptr;
  typedef boost::shared_ptr<Scorer> BasePtr;

  Scorer(const string& alias,
      const FeatureConverterArray& featureConverters = FeatureConverterArray()):
        Aliaser(alias),
        feature_converters_(featureConverters) {}

  double value(const Object& obj) const {
    return score(obj);
  }

  double score(const Object& obj) const;

  virtual string brief() const = 0;

  const FeatureConverterArray& getFeatureConverters() const {
    return feature_converters_;
  }
  void setFeatureConverters(const FeatureConverterArray& featureConverters) {
    this->feature_converters_ = featureConverters;
  }
  void addFeatureConverter(
    FeatureConverter::Ptr p_feature_converter) {
    this->feature_converters_.push_back(p_feature_converter);
  }

  using SerializableFunctor<double>::generateCppCode;

  string generateCppCode(const string& function_name) const {
    if (feature_converters_.size() == 0)
      return generateCppCodeImpl(function_name);
    string code;
    string implFunctionName(function_name);
    implFunctionName.append("Impl");
    code.append(generateCppCodeImpl(implFunctionName));

    for (size_t featureConverterIdx = 0;
        featureConverterIdx < feature_converters_.size();
        ++featureConverterIdx) {
      code.append(feature_converters_.at(
          featureConverterIdx)->generateCppCode());
    }

    code.append("double ");
    code.append(function_name);
    code.append("(const std::vector<double>& feature) {\n");

    string prevVectorName("feature");

    for (size_t featureConverterIdx = 0;
        featureConverterIdx < feature_converters_.size();
        ++featureConverterIdx) {
      string curVectorName = "feature" +
          boost::lexical_cast<string>(featureConverterIdx);
      string featureConverterFunctionName(feature_converters_.at(
          featureConverterIdx)->getDefaultSerializableObjectName());
      code.append("  std::vector<double> ");
      code.append(curVectorName);
      code.append(";\n  ");
      code.append(featureConverterFunctionName);
      code.append("(");
      code.append(prevVectorName);
      code.append(", &");
      code.append(curVectorName);
      code.append(");\n");

      prevVectorName = curVectorName;
    }
    code.append("  return ");
    code.append(implFunctionName);
    code.append("(");
    code.append(prevVectorName);
    code.append(");\n");
    code.append("}\n");
    return code;
  }

  virtual ~Scorer() {}

  private:
  virtual double scoreImpl(const Object& obj) const = 0;
  virtual string generateCppCodeImpl(const string& function_name) const = 0;

  FeatureConverterArray feature_converters_;
};
}
#endif  // LTR_SCORERS_SCORER_H_
