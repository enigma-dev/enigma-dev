/**
 * @file cclass_base.h
 * @brief Source implementing a function to help handle class/structure and union declarations.
 *
 * To use, include after defining the following:
 *   def_kind: The suffix of the definitions_ class to be allocated, and also the name by which to call this construct.
 *   DEF_FLAG: The flag(s) which identifies definitions of this kind.
 *
 * @section License
 *
 * Copyright (C) 2011-2014 Josh Ventura
 * This file is part of JustDefineIt.
 *
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 *
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

// Make sure whoever included this header knew what they were doing.
#if !defined(def_kind) | !defined(DEF_FLAG)
# error Define def_kind and DEF_FLAG to use this header.
#else

#define stringify(x) #x
#define sewtogether(x, y) x ## y
#define gcchax_cat(x, y) sewtogether(x, y)
#define gcchax_str(x) stringify(x)
#define strname gcchax_str(def_kind)

/// This is the current file's definition kind, eg, definition_class, definition_union.
#define definition_name gcchax_cat(definition_, def_kind)


static inline definition_name* insnew(definition_scope *scope, int inherited_flags, const string& classname, const token_t &token, ErrorHandler* const& herr) {
  decpair dins = scope->declare_c_struct(classname, nullptr);
  definition_name* nclass = nullptr;
  if (dins.inserted) {
    auto uclass = std::make_unique<definition_name>(
        classname, scope, (DEF_FLAG) | DEF_TYPENAME | inherited_flags);
    nclass = uclass.get();
    dins.def = std::move(uclass);
  } else if (!(dins.def->flags & (DEF_FLAG))) {
    token.report_error(herr, "Cannot redeclare " + dins.def->kind() + " `" + classname + "' as " strname " in this scope");
    return nullptr;
  }
  return nclass;
}


static unsigned anon_count = 1;
static inline int get_location(definition_name* &nclass, bool &will_redeclare, bool &already_complete, token_t &token, string &classname, definition_scope *scope, context_parser * const cp, ErrorHandler *const herr) {
  if (token.type == TT_IDENTIFIER) {
    classname = token.content.toString();
    token = cp->read_next_token(scope);
  }
  else if (token.type == TT_DEFINITION or token.type == TT_DECLARATOR) {
    definition *od = token.def;
    definition *d = cp->read_qualified_definition(token, scope);
    if (!d)
      return 1;

    if (d->flags & DEF_FLAG)
      nclass = (definition_name*)d;
    else if (d != od) {
      token.report_error(herr, "");
      return 1;
    }

    classname = d->name;
  }
  else {
    char buf[32];
    sprintf(buf, "<anonymous-" strname "-%08d>", anon_count++);
    classname = buf;
  }

  for (definition_scope *s = scope; s; s = s->parent) {
    definition_scope::defiter it = s->c_structs.find(classname);
    if (it != s->c_structs.end()) {
      if (it->second->flags & DEF_FLAG) {
        nclass = (definition_name*)it->second.get();
        will_redeclare = (s != scope);
        already_complete = !(nclass->flags & DEF_INCOMPLETE);
      }
      else if (s == scope) {
        herr->error(token) << "Using " << PQuote(strname) << " token to name "
                           << it->second->kind() << " " << PQuote(classname);
        return 1;
      }
      break;
    }
  }

  return 0;
}

#endif
