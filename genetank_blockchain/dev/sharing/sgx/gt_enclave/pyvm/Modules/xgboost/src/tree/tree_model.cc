/*!
 * Copyright 2015 by Contributors
 * \file tree_model.cc
 * \brief model structure for tree
 */
#include "../../include/xgboost/tree_model.h"
#include <sstream>
#include <limits>
#include <iomanip>
#include "./param.h"

namespace xgboost {
// register tree parameter
DMLC_REGISTER_PARAMETER(TreeParam);

namespace tree {
DMLC_REGISTER_PARAMETER(TrainParam);
}
// internal function to dump regression tree to text
void DumpRegTree(std::string& fo,  // NOLINT(*)
                 const RegTree& tree,
                 const FeatureMap& fmap,
                 int nid, int depth, int add_comma,
                 bool with_stats, std::string format) {
  int float_max_precision = std::numeric_limits<bst_float>::max_digits10;
  if (format == "json") {
    if (add_comma) {
      fo += ",";
    }
    if (depth != 0) {
      fo += '\n';
    }
    for (int i = 0; i < depth + 1; ++i) {
      fo += "  ";
    }
  } else {
    for (int i = 0; i < depth; ++i) {
      fo += '\t';
    }
  }
  if (tree[nid].IsLeaf()) {
    if (format == "json") {
      fo += "{ \"nodeid\": " ;
      fo += nid;
      fo += ", \"leaf\": " ;
      fo += tree[nid].LeafValue();
      if (with_stats) {
        fo += ", \"cover\": " ;
        fo += tree.Stat(nid).sum_hess;
      }
      fo += " }";
    } else {
      fo += nid;
      fo += ":leaf=";
      fo += tree[nid].LeafValue();
      if (with_stats) {
        fo += ",cover=" ;
        fo+=tree.Stat(nid).sum_hess;
      }
      fo += '\n';
    }
  } else {
    // right then left,
    bst_float cond = tree[nid].SplitCond();
    const unsigned split_index = tree[nid].SplitIndex();
    if (split_index < fmap.Size()) {
      switch (fmap.type(split_index)) {
        case FeatureMap::kIndicator: {
          int nyes = tree[nid].DefaultLeft() ?
              tree[nid].RightChild() : tree[nid].LeftChild();
          if (format == "json") {
            fo += "{ \"nodeid\": ";
            fo+= nid;
            fo+= ", \"depth\": ";
            fo+= depth;
            fo+=", \"split\": \"" ;
            fo+=fmap.Name(split_index) ;
            fo+="\"";
            fo+= ", \"yes\": ";
            fo+=nyes;
            fo+=", \"no\": ";
            fo+=tree[nid].DefaultChild();
          } else {
            fo+=nid;
            fo+=":[";
            fo+=fmap.Name(split_index);
            fo+="] yes=";
            fo+=nyes;
            fo+=",no=";
            fo+=tree[nid].DefaultChild();
          }
          break;
        }
        case FeatureMap::kInteger: {
          if (format == "json") {
              fo += "{ \"nodeid\": ";
              fo+= nid;
              fo+= ", \"depth\": ";
              fo+= depth;
              fo+=", \"split\": \"" ;
              fo+=fmap.Name(split_index) ;
              fo+="\"";
              fo+=", \"split_condition\": ";
              fo+=int(cond + 1.0);
              fo+= ", \"yes\": ";
              fo+=tree[nid].LeftChild();
              fo+=", \"no\": ";
              fo+=tree[nid].RightChild();
              fo+=", \"missing\": ";
              fo+=tree[nid].DefaultChild();
          } else {
        	fo+=nid;
        	fo+=":[";
        	fo+=fmap.Name(split_index);
        	fo+="<";
        	fo+="] yes=";
        	fo+=tree[nid].LeftChild();
        	fo+=",no=";
        	fo+=tree[nid].RightChild();
        	fo+=",missing=";
        	fo+=tree[nid].DefaultChild();
          }
          break;
        }
        case FeatureMap::kFloat:
        case FeatureMap::kQuantitive: {
          if (format == "json") {
              fo += "{ \"nodeid\": ";
              fo+= nid;
              fo+= ", \"depth\": ";
              fo+= depth;
              fo+=", \"split\": \"" ;
              fo+=fmap.Name(split_index) ;
              fo+="\"";
              fo+=", \"split_condition\": ";
              fo+=cond;
              fo+= ", \"yes\": ";
              fo+=tree[nid].LeftChild();
              fo+=", \"no\": ";
              fo+=tree[nid].RightChild();
              fo+=", \"missing\": ";
              fo+=tree[nid].DefaultChild();
          } else {
        	fo+=nid;
        	fo+=":[";
        	fo+=fmap.Name(split_index);
        	fo+="<";
        	fo+=cond;
        	fo+="] yes=";
        	fo+=tree[nid].LeftChild();
        	fo+=",no=";
        	fo+=tree[nid].RightChild();
        	fo+=",missing=";
        	fo+=tree[nid].DefaultChild();
          }
          break;
        }
        default: break;//LOG(FATAL) << "unknown fmap type";
        }
    } else {
      if (format == "json") {
          fo += "{ \"nodeid\": ";
          fo+= nid;
          fo+= ", \"depth\": ";
          fo+= depth;
          fo+=", \"split\": \"" ;
          fo+=split_index ;
          fo+="\"";
          fo+=", \"split_condition\": ";
          fo+=cond;
          fo+= ", \"yes\": ";
          fo+=tree[nid].LeftChild();
          fo+=", \"no\": ";
          fo+=tree[nid].RightChild();
          fo+=", \"missing\": ";
          fo+=tree[nid].DefaultChild();
      } else {
      	fo+=nid;
      	fo+=":[";
      	fo+=fmap.Name(split_index);
      	fo+="<";
      	fo+=cond;
      	fo+="] yes=";
      	fo+=tree[nid].LeftChild();
      	fo+=",no=";
      	fo+=tree[nid].RightChild();
      	fo+=",missing=";
      	fo+=tree[nid].DefaultChild();
      }
    }
    if (with_stats) {
      if (format == "json") {
    	fo+=", \"gain\": ";
    	fo+=tree.Stat(nid).loss_chg;
        fo+=", \"cover\": ";
        fo+=tree.Stat(nid).sum_hess;
      } else {
    	fo+=",gain=";
    	fo+=tree.Stat(nid).loss_chg;
    	fo+=",cover=";
        fo+=tree.Stat(nid).sum_hess;
      }
    }
    if (format == "json") {
      fo+=", \"children\": [";
    } else {
      fo+= '\n';
    }
    DumpRegTree(fo, tree, fmap, tree[nid].LeftChild(), depth + 1, false, with_stats, format);
    DumpRegTree(fo, tree, fmap, tree[nid].RightChild(), depth + 1, true, with_stats, format);
    if (format == "json") {
      fo += "\n";
      for (int i = 0; i < depth + 1; ++i) {
        fo += "  ";
      }
      fo += "]}";
    }
  }
}

std::string RegTree::DumpModel(const FeatureMap& fmap,
                               bool with_stats,
                               std::string format) const {
  std::string fo("");
  for (int i = 0; i < param.num_roots; ++i) {
    DumpRegTree(fo, *this, fmap, i, 0, false, with_stats, format);
  }
  return fo;
}
}  // namespace xgboost
