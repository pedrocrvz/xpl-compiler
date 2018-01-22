#ifndef __XPL_ADRESSOFNODE_H__
#define __XPL_ADRESSOFNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace xpl {

  /**
   * Class for describing adress of nodes.
   */

  class adress_of_node: public cdk::unary_expression_node {

  public:
    inline adress_of_node(int lineno, cdk::lvalue_node *lvalue) :
        cdk::unary_expression_node(lineno, lvalue) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_adress_of_node(this, level);
    }

  };

} // xpl

#endif