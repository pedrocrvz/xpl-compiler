#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (_context)
    _pf.INT(node->value()); 
  else
    _pf.CONST(node->value()); 
}

void xpl::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.STR(node->value()); // output string characters

  /* leave the address on the stack */

  if (_context) {
    // local
    _pf.TEXT(); // return to the TEXT segment
    _pf.ADDR(mklbl(lbl1)); // the string to be printed
  } else {
    // global
    _pf.DATA();
    _pf.ALIGN();    
    _pf.ID(mklbl(lbl1));
  }
}

void xpl::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (_context) {
    int dlbl = ++_lbl;
    _pf.RODATA();
    _pf.ALIGN();
    _pf.LABEL(mklbl(dlbl));
    _pf.DOUBLE(node->value());
    _pf.TEXT();
    _pf.ALIGN();
    _pf.ADDR(mklbl(dlbl));
    _pf.DLOAD();
  }
  else 
    _pf.DOUBLE(node->value());
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // value
  if (node->type()->name() == basic_type::TYPE_INT)
    _pf.NEG();
  else if (node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DNEG();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if(node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.ADD();
  }

  else if(node->type()->name() == basic_type::TYPE_DOUBLE) {

    node->left()->accept(this, lvl);
    if(node->left()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D(); //Converts from integer (32-bit) to double precision (64-bit) floating point
      
    node->right()->accept(this, lvl);
    if(node->right()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D();
    _pf.DADD(); //floats sum
  }

  else if(node->type()->name() == basic_type::TYPE_POINTER) {

    node->left()->accept(this, lvl);
    if(node->left()->type()->name() == basic_type::TYPE_INT) {
      _pf.INT(node->right()->type()->subtype()->size());
      _pf.MUL();     
    }

    node->right()->accept(this, lvl);
    if(node->right()->type()->name() == basic_type::TYPE_INT) {
      _pf.INT(node->left()->type()->subtype()->size());
      _pf.MUL();
    }
    _pf.ADD();
  }
}

void xpl::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  if(node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.SUB();
  }

  else if(node->type()->name() == basic_type::TYPE_DOUBLE) {

    node->left()->accept(this, lvl);
    if(node->left()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D(); //Converts from integer (32-bit) to double precision (64-bit) floating point
      
    node->right()->accept(this, lvl);
    if(node->right()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D();
    _pf.DSUB(); //floats sum
  }

  else if(node->type()->name() == basic_type::TYPE_POINTER) {

    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);

    _pf.INT(node->right()->type()->subtype()->size());
    _pf.MUL();
    _pf.ADD();
  }

}

void xpl::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  if(node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.MUL();
  }
  else if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    if(node->left()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D();
      
    node->right()->accept(this, lvl);
    if(node->right()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D();
    _pf.DMUL();
  }
}

void xpl::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  //same as mul_node, change mul to div
  if(node->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    _pf.DIV();
  }

  else if(node->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    if(node->left()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D();
      
    node->right()->accept(this, lvl);
    if(node->right()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D();
    _pf.DDIV();
  }
}

void xpl::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void xpl::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void xpl::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void xpl::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void xpl::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void xpl::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void xpl::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

void xpl::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.NOT();
}

void xpl::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  /*int endAnd = ++_lbl;

  node->left()->accept(this, lvl);
  _pf.JZ(mklbl(endAnd));
  node->right()->accept(this, lvl);
  
  _pf.AND();
  _pf.LABEL(mklbl(endAnd));*/
}

void xpl::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int endOr = ++_lbl;

  node->left()->accept(this, lvl);
  _pf.JNZ(mklbl(endOr));
  node->right()->accept(this, lvl);
  _pf.OR();
  _pf.LABEL(mklbl(endOr));

}
//---------------------------------------------------------------------------

void xpl::postfix_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::shared_ptr<xpl::symbol> symbol = _symtab.find_local(node->name());
  if (symbol != nullptr) {
    _pf.LOCAL(symbol->offset());
  } 
  else if (symbol=_symtab.find(node->name())) {
    _pf.ADDR(node->name());
  } 
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void xpl::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  _pf.LOAD(); // depends on type size
}

void xpl::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  _pf.DUP();
  if (new_symbol() == nullptr) {
    node->lvalue()->accept(this, lvl); // where to store the value
  } else {
    _pf.DATA(); // variables are all global and live in DATA
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(new_symbol()->name()); // name variable location
    _pf.CONST(0); // initialize it to 0 (zero)
    _pf.TEXT(); // return to the TEXT segment
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }
  _pf.STORE(); // store the value at address
}

//---------------------------------------------------------------------------

/*void xpl::postfix_writer::do_program_node(xpl::program_node * const node, int lvl) {
  // The ProgramNode (representing the whole program) is the
  // main function node.

  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");
  _pf.ENTER(0);  // XPL doesn't implement local variables

  node->statements()->accept(this, lvl);

  // end the main function
  _pf.INT(0);
  _pf.POP();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}*/

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void xpl::postfix_writer::do_print_node(xpl::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } 
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } 
  else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8); // delete the printed value
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  if (node->newLine())
    _pf.CALL("println"); // print a newline
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_read_node(xpl::read_node * const node, int lvl) {
  /* ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.PUSH();
  node->argument()->accept(this, lvl);
  _pf.STORE(); */

  ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.PUSH();
  _pf.STORE();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_while_node(xpl::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_node(xpl::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_next_node(xpl::next_node * const node, int lvl) {}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_stop_node(xpl::stop_node * const node, int lvl) {} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_return_node(xpl::return_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
   _pf.JMP(mklbl(_returnlbl));
} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_index_node(xpl::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);

   _pf.LOAD();
  
  node->rvalue()->accept(this, lvl);
  if (node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.INT(8);

  else if (node->type()->name() == basic_type::TYPE_INT ||
           node->type()->name() == basic_type::TYPE_STRING ||
           node->type()->name() == basic_type::TYPE_POINTER)
    _pf.INT(4);
  
  else {
    std::cerr << "ERROR index: CANNOT HAPPEN! " << std::endl;
    exit(1);
  }
  
  _pf.MUL();
  _pf.ADD();
} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_adress_of_node(xpl::adress_of_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_sweep_down_node(xpl::sweep_down_node * const node, int lvl) {
    int lbl1, lbl2;

  node->start()->accept(this, lvl);
  node->lvalue()->accept(this, lvl);
  _pf.STORE();
  _pf.LABEL(mklbl(lbl1 = ++_lbl));

  node->lvalue()->accept(this, lvl);
  _pf.LOAD();

  node->condition()->accept(this, lvl);
  _pf.GE();
  _pf.JZ(mklbl(lbl2==++_lbl));


  node->block()->accept(this, lvl+2);
  node->lvalue()->accept(this, lvl);
  _pf.LOAD();

  node->decrement()->accept(this, lvl);
  _pf.ADD();

  node->lvalue()->accept(this, lvl);
  _pf.STORE();

  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));

} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_sweep_up_node(xpl::sweep_up_node * const node, int lvl) {
  int lbl1, lbl2;

  node->start()->accept(this, lvl);
  node->lvalue()->accept(this, lvl);
  _pf.STORE();
  _pf.LABEL(mklbl(lbl1 = ++_lbl));

  node->lvalue()->accept(this, lvl);
  _pf.LOAD();

  node->condition()->accept(this, lvl);
  _pf.LE();
  _pf.JZ(mklbl(lbl2==++_lbl));


  node->block()->accept(this, lvl+2);
  node->lvalue()->accept(this, lvl);
  _pf.LOAD();

  node->increment()->accept(this, lvl);
  _pf.ADD();

  node->lvalue()->accept(this, lvl);
  _pf.STORE();

  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_null_node(xpl::null_node * const node, int lvl) {} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_allocation_node(xpl::allocation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->size()->accept(this, lvl); //doubles
  _pf.INT(node->type()->subtype()->size());
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP(); //stackpoint
} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_block_node(xpl::block_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _symtab.push();
  if (node->declarations())
    node->declarations()->accept(this, lvl+2);
  if (node->instructions())
    node->instructions()->accept(this, lvl+2);
  _symtab.pop();
} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_function_definition_node(xpl::function_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _pf.TEXT();
  _pf.ALIGN();
  std::string name = node->id();
  
  if (name == "xpl") 
    name = "_main"; 
  else if (name == "_main")
    name = "__main";

  if (node->isPublic()) 
    _pf.GLOBAL(name, _pf.FUNC());
  _pf.LABEL(name);
  

  _returnlbl = ++_lbl;

  size_t localsize = 0;
  size_t retsize = (node->type()->name() != basic_type::TYPE_VOID) ? node->type()->size() : 0;      
  _pf.ENTER(localsize+retsize);
  _context=true;

  if (node->returnValue() != nullptr) { 
    node->returnValue()->accept(this, lvl);
    
    if (node->type()->name() == basic_type::TYPE_DOUBLE && node->returnValue()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D();

    _pf.LOCAL(-retsize);

    if (node->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.DSTORE();
    else
      _pf.STORE();
  } 

  else if (node->type()->name() == basic_type::TYPE_POINTER ||node->type()->name() == basic_type::TYPE_INT) {
    _pf.INT(0);
    _pf.LOCA(-retsize); 
  }

  node->body()->accept(this, lvl);
  _pf.LABEL(mklbl(_returnlbl));

  if (node->type()->name() != basic_type::TYPE_VOID) {
    _pf.LOCAL(-retsize);

    if (node->type()->name() == basic_type::TYPE_DOUBLE) {
      _pf.DLOAD();
      _pf.DPOP();
    } 
    else {
      _pf.LOAD();
      _pf.POP();
    }
  } 

  _pf.LEAVE();
  _pf.RET();
  _context=false;
} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_function_declaration_node(xpl::function_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::string name = node->id(); 
  _externs[name]=1;
  
} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_function_call_node(xpl::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->args()->accept(this, lvl+1);
  _pf.CALL(node->id());

  if (node->args()) {
    for (size_t i = 0; i < node->args()->size(); i++) {
      _pf.TRASH(((cdk::expression_node*)node->args()->node(i))->type()->size());
    }
  }
  if (node->type()->name()==basic_type::TYPE_DOUBLE)
    _pf.DPUSH();

  // put return value in stack
  else if (node->type()->name() == basic_type::TYPE_INT 
  || node->type()->name() == basic_type::TYPE_STRING 
  || node->type()->name() == basic_type::TYPE_POINTER)
    _pf.PUSH();
  else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DPUSH();
  }
} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_variable_declaration_node(xpl::variable_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::string id = node->id();
  std::shared_ptr<xpl::symbol> symbol = _symtab.find_local(id);
  if (symbol == nullptr) {
    _symtab.insert(id, std::make_shared<xpl::symbol>(node->type(), id, 0));
  }
  
  if (!_context) {
    _pf.DATA();
    _pf.ALIGN();
    if (node->isPublic()) 
      _pf.GLOBAL(node->id(), _pf.OBJ());

    _pf.LABEL(node->id());
    node->value()->accept(this, lvl);
  } 
  else {
    node->value()->accept(this, lvl);
    _pf.LOCAL(symbol->offset());

    if (symbol->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.DSTORE();
    else
      _pf.STORE();
  }
} 

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_identity_node(xpl::identity_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); 
}

