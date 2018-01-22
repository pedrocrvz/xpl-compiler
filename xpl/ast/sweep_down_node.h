#ifndef __XPL_SWEEPDOWNNODE_H__
#define __XPL_SWEEPDOWNNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace xpl {

  /**
   * Class for describing sweep down nodes.
   */
	class sweep_down_node: public cdk::basic_node{
		cdk::lvalue_node *_lvalue;
		cdk::expression_node *_start;
		cdk::expression_node *_condition;
		cdk::expression_node *_decrement;
		cdk::basic_node *_block;

	public:
		inline sweep_down_node(int lineno, cdk::lvalue_node *lvalue, cdk::expression_node *start, cdk::expression_node *condition, cdk::expression_node *decrement, cdk::basic_node *block) :
		cdk::basic_node(lineno), _lvalue(lvalue), _start(start), _condition(condition), _decrement(decrement), _block(block){
		}

	public:
		inline cdk::lvalue_node *lvalue(){
			return _lvalue;
		}

		inline cdk::expression_node *start(){
			return _start;
		}

		inline cdk::expression_node *condition(){
			return _condition;
		}

		inline cdk::expression_node *decrement(){
			return _decrement;
		}

		inline cdk::basic_node *block(){
			return _block;
		}
		void accept(basic_ast_visitor *sp, int level) {
      		sp->do_sweep_down_node(this, level);
    	}
	};

} // xpl

#endif