#ifndef __XPL_SEMANTICS_SYMBOL_H__
#define __XPL_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>

namespace xpl {

    class symbol {
      basic_type *_type;
      std::string _name;
      int _offset;
      bool _isDeclared;
      std::vector<basic_type*> _args;
      long _value; // hack!

    public:
      inline symbol(basic_type *type, const std::string &name, long value, bool declared = false) :
          _type(type), _name(name), _value(value), _isDeclared(declared), _offset(0) {
      }

      virtual ~symbol() {
        delete _type;
      }

      inline basic_type *type() const {
        return _type;
      }
      inline const std::string &name() const {
        return _name;
      }
      inline long value() const {
        return _value;
      }
      inline long value(long v) {
        return _value = v;
      }
    inline int offset() const {
        return _offset;
      }
      inline int offset(int i) {
        return _offset = i;
      }
    inline bool isDeclared() const {
        return _isDeclared;	
      }
      
      inline bool isDeclared(bool value) {
        return _isDeclared = value;	
      }
      
      inline std::vector<basic_type*> args(std::vector<basic_type*> args) {
        return _args = args;	
      }
      inline std::vector<basic_type*> args() const {
        return _args;	
      }
      
    };

} // xpl

#endif
