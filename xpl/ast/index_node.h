#ifndef __XPL_INDEXNODE_H__
#define __XPL_INDEXNODE_H__

#include <cdk/ast/lvalue_node.h>

namespace xpl {

  /**
   * Class for describing index nodes.
   */

  class index_node: public cdk::lvalue_node {
    cdk::expression_node *_lvalue;
    cdk::expression_node *_rvalue;

  public:
    inline index_node(int lineno, cdk::expression_node *lvalue, cdk::expression_node *rvalue) :
       cdk::lvalue_node(lineno), _lvalue(lvalue), _rvalue(rvalue) {
    }

  public:
    inline cdk::expression_node *lvalue() {
      return _lvalue;
    }
    inline cdk::expression_node *rvalue() {
      return _rvalue;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // xpl

#endif