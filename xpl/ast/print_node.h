// $Id: print_node.h,v 1.2 2017/03/23 15:49:00 ist180990 Exp $ -*- c++ -*-
#ifndef __XPL_PRINTNODE_H__
#define __XPL_PRINTNODE_H__

#include <cdk/ast/expression_node.h>

namespace xpl {

  /**
   * Class for describing print nodes.
   */
  class print_node: public cdk::basic_node {
    cdk::expression_node *_argument;
    bool _newLine;

  public:
    inline print_node(int lineno, cdk::expression_node *argument, bool newLine) :
        cdk::basic_node(lineno), _argument(argument), _newLine(newLine) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    inline bool newLine() {
      return _newLine;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_print_node(this, level);
    }

  };

} // xpl

#endif


