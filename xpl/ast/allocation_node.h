#ifndef __XPL_ALLOCATIONNODE_H__
#define __XPL_ALLOCATIONNODE_H__

#include <cdk/ast/expression_node.h>

namespace xpl {

  /**
   * Class for describing memory allocation nodes.
   */
  class allocation_node: public cdk::expression_node {
    cdk::expression_node *_size;

  public:
    inline allocation_node(int lineno, cdk::expression_node *size) :
        cdk::expression_node(lineno), _size(size) {
    }

  public:
    inline cdk::expression_node *size() {
      return _size;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_allocation_node(this, level);
    }

  };

} // xpl

#endif
