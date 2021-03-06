// Copyright 2011 Yandex

#include "ltr/scorers/scorer.h"

#include <sstream>

using std::stringstream;

namespace ltr {

double Scorer::score(const ltr::Object& object) const {
  Object sourceObject = object.deepCopy();
  Object convertedObject;
  for (size_t featureConverterIdx = 0;
      featureConverterIdx < feature_converter_.size();
      ++featureConverterIdx) {
    (*feature_converter_[featureConverterIdx]).apply(sourceObject,
        &convertedObject);
    sourceObject = convertedObject;
  }
  return scoreImpl(sourceObject);
}

string Scorer::generateCppCode(const string& function_name) const {
  if (feature_converter_.size() == 0)
    return generateCppCodeImpl(function_name);
  stringstream code;
  string implFunctionName = function_name + "Impl";
  code << generateCppCodeImpl(implFunctionName);

  for (size_t featureConverterIdx = 0;
       featureConverterIdx < feature_converter_.size();
       ++featureConverterIdx) {
    code << feature_converter_.at(featureConverterIdx)->generateCppCode();
  }

  code
    << "double " << function_name << "(const std::vector<double>& feature) {\n";

  string prevVectorName("feature");

  for (size_t featureConverterIdx = 0;
      featureConverterIdx < feature_converter_.size();
      ++featureConverterIdx) {
    string curVectorName = "feature" +
        ltr::utility::lexical_cast<string>(featureConverterIdx);
    string featureConverterFunctionName(feature_converter_.at(
        featureConverterIdx)->getDefaultSerializableObjectName());
    code
      << "  std::vector<double> " << curVectorName << ";\n"
      << "  " << featureConverterFunctionName << "(" << prevVectorName << ",\n"
      << "    &" << curVectorName << ");\n";

    prevVectorName = curVectorName;
  }
  code
    << "  return " << implFunctionName << "(" << prevVectorName << ");\n"
    << "}\n";
  return code.str();
}

string Scorer::generateLocalClassName(size_t index) const {
  return "Local" + ltr::utility::lexical_cast<std::string>(index) +
      "_" + this->alias();
}

string Scorer::toString() const {
  std::stringstream str;
  str << toStringImpl();
  if (feature_converter_.empty()) {
    str << ", has no feature converters";
  } else {
    str << ", initially applies " << feature_converter_.size()
      << " feature converters:\n";
    for (size_t i = 0; i < feature_converter_.size(); ++i) {
      if (i != 0) {
        str << "\n";
      }
      str << "[" << i << "] " << feature_converter_[i]->toString();
    }
  }
  return str.str();
}

string Scorer::toStringImpl() const {
  return "Scorer";
}
}
