#ifndef __XPL_VARIABLEDECLARATIONNODE_H__
#define __XPL_VARIABLEDECLARATIONNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>
#include <string>

namespace xpl {

  /**
   * Class for describing variable declaration nodes.
   */

  class variable_declaration_node: public cdk::basic_node {
	basic_type *_type;
	std::string _id;
	bool _isPublic;
	bool _isExtern;
	cdk::expression_node *_value;

  public:
    inline variable_declaration_node(int lineno, basic_type *type, std::string *id, bool isPublic, bool isExtern, cdk::expression_node *value) :
        cdk::basic_node(lineno), _type(type), _id(*id), _isPublic(isPublic), _isExtern(isExtern), _value(value) {
	}

  public:
	inline basic_type *type() {
		return _type;
	}

    inline std::string &id() {
      return _id;
    }

	inline bool isPublic() {
		return _isPublic;
	}

	inline bool isExtern() {
		return _isExtern;
	}

	inline cdk::expression_node *value() {
		return _value;
	}

    virtual void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_declaration_node(this, level);
    }

  };

} // xpl

#endif