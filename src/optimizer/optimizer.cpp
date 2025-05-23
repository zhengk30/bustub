//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// optimizer.cpp
//
// Identification: src/optimizer/optimizer.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "optimizer/optimizer.h"
#include <optional>
#include "common/util/string_util.h"
#include "execution/plans/abstract_plan.h"

namespace bustub {

auto Optimizer::Optimize(const AbstractPlanNodeRef &plan) -> AbstractPlanNodeRef {
  if (force_starter_rule_) {
    // Use starter rules when `force_starter_rule_` is set to true.
    auto p = plan;
    p = OptimizeMergeProjection(p);
    p = OptimizeMergeFilterNLJ(p);
    p = OptimizeNLJAsIndexJoin(p);
    p = OptimizeOrderByAsIndexScan(p);
    p = OptimizeMergeFilterScan(p);
    p = OptimizeSeqScanAsIndexScan(p);
    return p;
  }
  // By default, use user-defined rules.
  return OptimizeCustom(plan);
}

/**
 * @brief get the estimated cardinality for a table based on the table name. Useful when join reordering. BusTub
 * doesn't support statistics for now, so it's the only way for you to get the table size :(
 *
 * @param table_name
 * @return std::optional<size_t>
 */
auto Optimizer::EstimatedCardinality(const std::string &table_name) -> std::optional<size_t> {
  if (StringUtil::EndsWith(table_name, "_1m")) {
    return std::make_optional(1000000);
  }
  if (StringUtil::EndsWith(table_name, "_100k")) {
    return std::make_optional(100000);
  }
  if (StringUtil::EndsWith(table_name, "_50k")) {
    return std::make_optional(50000);
  }
  if (StringUtil::EndsWith(table_name, "_10k")) {
    return std::make_optional(10000);
  }
  if (StringUtil::EndsWith(table_name, "_1k")) {
    return std::make_optional(1000);
  }
  if (StringUtil::EndsWith(table_name, "_100")) {
    return std::make_optional(100);
  }
  return std::nullopt;
}

}  // namespace bustub
