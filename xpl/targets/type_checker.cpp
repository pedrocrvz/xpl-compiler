#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void xpl::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

void xpl::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}
//---------------------------------------------------------------------------

inline void xpl::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in argument of unary expression");

  // in XPL, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

inline void xpl::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in right argument of binary expression");

  // in XPL, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

inline void xpl::type_checker::processBinaryExpressionII(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;

  //verify left type
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT 
  && node->left()->type()->name() != basic_type::TYPE_DOUBLE 
  &&node->left()->type()->name() != basic_type::TYPE_POINTER)
    throw std::string("wrong type in left argument of binary expression");

  //verify right
  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT 
  && node->right()->type()->name() != basic_type::TYPE_DOUBLE 
  && node->right()->type()->name() != basic_type::TYPE_POINTER)
    throw std::string("wrong type in right argument of binary expression");

  if(node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_UNSPEC){
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->left()->type(node->type());
    node->right()->type(node->type());
  } 

  //left
  else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(node->left()->type());    
    node->right()->type(node->type());
  } 
  //right
  else if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(node->right()->type());    
    node->left()->type(node->type());
  } 
  else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));  
  else
    node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  //processBinaryExpression(node, lvl);
  ASSERT_UNSPEC;

  //verify left type
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT 
  && node->left()->type()->name() != basic_type::TYPE_DOUBLE 
  && node->left()->type()->name() != basic_type::TYPE_POINTER 
  && node->left()->type()->name() != basic_type::TYPE_UNSPEC)
    throw std::string("wrong type in left argument of binary expression");

  //verify right type
  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT 
  && node->right()->type()->name() != basic_type::TYPE_DOUBLE 
  && node->right()->type()->name() != basic_type::TYPE_POINTER 
  && node->right()->type()->name() != basic_type::TYPE_UNSPEC)
    throw std::string("wrong type in right argument of binary expression");


  else if((node->left()->type()->name() == basic_type::TYPE_INT) && (node->right()->type()->name() == basic_type::TYPE_POINTER)){
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  }
  else if((node->left()->type()->name() == basic_type::TYPE_POINTER) && (node->right()->type()->name() == basic_type::TYPE_INT)){
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  }

  else if((node->left()->type()->name() == basic_type::TYPE_DOUBLE) || (node->right()->type()->name() == basic_type::TYPE_DOUBLE)){
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }
  else if((node->left()->type()->name() == basic_type::TYPE_INT) || (node->right()->type()->name() == basic_type::TYPE_INT)){
    node->type(new basic_type(4, basic_type::TYPE_INT));
  }
  else {
    node->type(new basic_type(0, basic_type::TYPE_ERROR));
    throw std::string("args: wrong add types");
  }
}

void xpl::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  //processBinaryExpression(node, lvl);
  ASSERT_UNSPEC;

  //verify left type
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT 
  && node->left()->type()->name() != basic_type::TYPE_DOUBLE 
  && node->left()->type()->name() != basic_type::TYPE_POINTER 
  && node->left()->type()->name() != basic_type::TYPE_UNSPEC)
    throw std::string("wrong type in left argument of binary expression");

  //verify right tpe
  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT 
  && node->right()->type()->name() != basic_type::TYPE_DOUBLE
  && node->right()->type()->name() != basic_type::TYPE_POINTER 
  && node->right()->type()->name() != basic_type::TYPE_UNSPEC)
    throw std::string("wrong type in right argument of binary expression");
  
  
  if(node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->left()->type(node->type());
    node->right()->type(node->type());
  } 
  else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(node->left()->type());    
    node->right()->type(node->type());
  } 
  else if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(node->right()->type());    
    node->left()->type(node->type());
  } 
  else if(node->left()->type()->name() == basic_type::TYPE_POINTER || node->right()->type()->name() == basic_type::TYPE_POINTER)
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));  
  else 
    node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
    processBinaryExpressionII(node, lvl);
}

//same as mul_node
void xpl::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
    processBinaryExpressionII(node, lvl);
}

void xpl::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  //processBinaryExpression(node, lvl);

  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_UNSPEC)
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT && node->right()->type()->name() != basic_type::TYPE_UNSPEC)
    throw std::string("wrong type in right argument of binary expression");


  node->type(new basic_type(4, basic_type::TYPE_INT));

  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC)
    node->left()->type(node->type());
  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC)
    node->right()->type(node->type());
}

void xpl::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  //processBinaryExpression(node, lvl);
  processBinaryExpressionII(node, lvl);
}

void xpl::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  //processBinaryExpression(node, lvl);
  processBinaryExpressionII(node, lvl);
}

void xpl::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  //processBinaryExpression(node, lvl);
  processBinaryExpressionII(node, lvl);
}

void xpl::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  //processBinaryExpression(node, lvl);
  processBinaryExpressionII(node, lvl);
}

void xpl::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpressionII(node, lvl);
}

//same as ne_node
void xpl::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpressionII(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_not_node(cdk::not_node * const node, int lvl){
  processUnaryExpression(node, lvl);
}

void xpl::type_checker::do_and_node(cdk::and_node * const node, int lvl){
  processBinaryExpression(node, lvl);
}

void xpl::type_checker::do_or_node(cdk::or_node * const node, int lvl){
  processBinaryExpression(node, lvl);
}



void xpl::type_checker::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  }
  else {
    throw id;
  }
}

void xpl::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void xpl::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;

  try {
    node->lvalue()->accept(this, lvl);
  } catch (const std::string &id) {
    std::shared_ptr<xpl::symbol> symbol = std::make_shared<xpl::symbol>(new basic_type(4, basic_type::TYPE_INT), id, 0);
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }

  if (node->lvalue()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in left argument of assignment expression");

  node->rvalue()->accept(this, lvl + 2);
  if (node->rvalue()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type in right argument of assignment expression");

  // in XPL, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void xpl::type_checker::do_print_node(xpl::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() == basic_type::TYPE_UNSPEC)
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  
  else if (node->argument()->type()->name() != basic_type::TYPE_INT 
  && node->argument()->type()->name() != basic_type::TYPE_DOUBLE 
  && node->argument()->type()->name() != basic_type::TYPE_STRING)
    throw std::string("cannot print this type!");
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_read_node(xpl::read_node * const node, int lvl) {
  ASSERT_UNSPEC;
  //dont know the input
  node->type(new basic_type(4, basic_type::TYPE_UNSPEC));
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_while_node(xpl::while_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_if_node(xpl::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void xpl::type_checker::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_next_node(xpl::next_node * const node, int lvl) {}

//---------------------------------------------------------------------------

void xpl::type_checker::do_stop_node(xpl::stop_node * const node, int lvl) {} 

//---------------------------------------------------------------------------

void xpl::type_checker::do_return_node(xpl::return_node * const node, int lvl) {}

//---------------------------------------------------------------------------

void xpl::type_checker::do_index_node(xpl::index_node * const node, int lvl) {
  ASSERT_UNSPEC;

  //lvalue
  node->lvalue()->accept(this, lvl + 2);
  if (node->lvalue()->type()->name() != basic_type::TYPE_POINTER)
    throw std::string("args: wrong type in lvalue");
  //rvalue
  node->rvalue()->accept(this, lvl + 2);
  if (node->rvalue()->type()->name() != basic_type::TYPE_INT)
    throw std::string("args: wrong type in rvalue");

  node->type(node->lvalue()->type()->subtype());
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_adress_of_node(xpl::adress_of_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_sweep_down_node(xpl::sweep_down_node * const node, int lvl) {}

//---------------------------------------------------------------------------

void xpl::type_checker::do_sweep_up_node(xpl::sweep_up_node * const node, int lvl) {}

//---------------------------------------------------------------------------

void xpl::type_checker::do_null_node(xpl::null_node * const node, int lvl) {}

//---------------------------------------------------------------------------

void xpl::type_checker::do_allocation_node(xpl::allocation_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->size()->accept(this, lvl + 2);
  if (node->size()->type()->name() != basic_type::TYPE_INT)
    throw std::string("args: wrong type in allocation");
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
  node->type()->_subtype = new basic_type(4, basic_type::TYPE_UNSPEC);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_block_node(xpl::block_node * const node, int lvl) {}

//---------------------------------------------------------------------------

void xpl::type_checker::do_function_definition_node(xpl::function_definition_node * const node, int lvl) {
    std::string id;
    std::vector<basic_type*> args;

    if(node->args()!=NULL) {
      for (size_t i = 0; i < node->args()->size(); i++) {
        xpl::variable_declaration_node *arg = (xpl::variable_declaration_node*) node->args()->node(i);
        args.push_back(arg->type());
      }
    }

    if(node->returnValue()) {
      node->returnValue()->accept(this, lvl);
      if(node->returnValue()->type()->name() != node->type()->name()) {
        throw std::string("diffetents types");
      }
    }
    
    std::shared_ptr<xpl::symbol> symbol = _symtab.find_local(id);
    if (symbol) {
            if (symbol->isDeclared()) {
        throw id + " already declared";
      }

      for (size_t i = 0; i < symbol->args().size()  &&  i < args.size(); i++) {
         if(symbol->args().at(i)->name() != args.at(i)->name())
           throw id + " already declared";
      }

      if(symbol->args().size() != args.size()) {
        throw id + " already declared ";
      }
       
      if(symbol->type()->name() != node->type()->name())
        throw id + " already declared";
      
      symbol->isDeclared(true);
    } 
    else {
      std::shared_ptr<xpl::symbol> symbol = std::make_shared<xpl::symbol>(node->type(), node->id(), 0,true);
    _symtab.insert(node->id(), symbol);
  }
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_function_declaration_node(xpl::function_declaration_node * const node, int lvl) {
  std::vector<basic_type*> args;
  const std::string &id = node->id();

  if(node->args()!=NULL)
    for (size_t i = 0; i < node->args()->size(); i++) {
      xpl::variable_declaration_node *arg = (xpl::variable_declaration_node*) node->args()->node(i);
      args.push_back(arg->type());
  }
  
  if (!_symtab.find_local(id)) {
    std::shared_ptr<xpl::symbol> symbol = std::make_shared<xpl::symbol>(node->type(), id, 0);
    _symtab.insert(id, symbol);
  }
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_function_call_node(xpl::function_call_node * const node, int lvl) {
  ASSERT_UNSPEC;

  std::shared_ptr<xpl::symbol> symbol;
  const std::string &id = node->id();
  
  if( (symbol = _symtab.find(id))) {
    const std::vector<basic_type*> args = symbol->args();
    cdk::expression_node *arg;
    
    if(node->args()!=NULL) {
        if(args.size() != node->args()->size()) {
            throw id + "wrong args size";
        }
      
        for (size_t i = 0; i < args.size()  &&  i < node->args()->size(); i++) {
         arg = (cdk::expression_node*) node->args()->node(i);
         node->args()->node(i)->accept(this, lvl);

          if(arg->type()->name() == basic_type::TYPE_UNSPEC  && args.at(i)->name() == basic_type::TYPE_INT) {
            arg->type(new basic_type(4, basic_type::TYPE_INT));
          } 
          else if(arg->type()->name() == basic_type::TYPE_UNSPEC  &&  args.at(i)->name() == basic_type::TYPE_DOUBLE) {
            arg->type(new basic_type(8, basic_type::TYPE_DOUBLE));
          }
    
          if (args.at(i)->name() != arg->type()->name()) {
            throw id + ": diffetents args types";
          }
        }
    }
  } 
  else {
    throw id + " undeclared";
  } 
  node->type(symbol->type());
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_variable_declaration_node(xpl::variable_declaration_node * const node, int lvl) {
  ASSERT_UNSPEC;
  //done in postfix
  /*std::shared_ptr<xpl::symbol> symbol = _symtab.find(id);
  if (!symbol) {
        _symtab.insert(id, std::make_shared<xpl::symbol>(node->type(), id, 0));
  } 
  else {
        throw id + "redeclared";
  }*/
  if (node->value() != nullptr) {
      node->value()->accept(this, lvl +2);
      if (node->value()->type()->name() != node->type()->name()) {
          throw "wrong type";        
      }
  }
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_identity_node(xpl::identity_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}


