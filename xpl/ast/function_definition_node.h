#ifndef __XPL_FUNCTIONDEFINITIONNODE_H__
#define __XPL_FUNCTIONDEFINITIONNODE_H__

#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "ast/block_node.h"
#include <cdk/basic_type.h>
#include <string>

namespace xpl {

  /**
   * Class for describing function definition nodes.
   */

  class function_definition_node: public cdk::basic_node {
      
      basic_type *_type;
      std::string _id;
      bool _isPublic;
      bool _isExtern;
      cdk::sequence_node *_args;
      cdk::expression_node *_returnValue;
      xpl::block_node *_body;

    public:
      inline function_definition_node(int lineno, basic_type *type, std::string *id, 
        bool isPublic, bool isExtern, cdk::sequence_node *args, cdk::expression_node *returnValue, xpl::block_node *body) :
          cdk::basic_node(lineno), _type(type), _id(*id), 
            _isPublic(isPublic), _isExtern(isExtern), _args(args), _returnValue(returnValue), _body(body) {}

    public:
      inline cdk::sequence_node *args() {
        return _args;
      }

      basic_type *type() {
        return _type;
      }

      void type(basic_type *type) {
        _type = type;
      }

      inline std::string &id() {
        return _id;
      }
      inline bool isPublic() {
        return _isPublic;
      }
      inline  bool isExtern() {
        return _isExtern;
      }

      inline cdk::expression_node *returnValue() {
        return _returnValue;
      }

      inline xpl::block_node *body() {
        return _body;
      }


      void accept(basic_ast_visitor *sp, int level) {
        sp->do_function_definition_node(this, level);
      }

    };


} // xpl

#endif