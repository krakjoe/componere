/*
  +----------------------------------------------------------------------+
  | componere                                                            |
  +----------------------------------------------------------------------+
  | Copyright (c) Joe Watkins 2018-2019                                  |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe <krakjoe@php.net>                                    |
  +----------------------------------------------------------------------+
 */
#ifndef HAVE_COMPONERE_DEFINITION
#define HAVE_COMPONERE_DEFINITION

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <php.h>

#include <ext/spl/spl_exceptions.h>
#include <zend_closures.h>
#include <zend_exceptions.h>
#include <zend_inheritance.h>

#include <src/common.h>
#include <src/reflection.h>
#include <src/definition.h>
#include <src/method.h>
#include <src/value.h>

zend_class_entry *php_componere_definition_abstract_ce;
zend_class_entry *php_componere_definition_ce;

zend_object_handlers php_componere_definition_handlers;

typedef struct _zend_closure_t {
	zend_object       std;
	zend_function     func;
	zval              this_ptr;
	zend_class_entry *called_scope;
#if PHP_VERSION_ID <= 70200
	void (*orig_internal_handler)(INTERNAL_FUNCTION_PARAMETERS);
#else
	zif_handler       orig_internal_handler;
#endif
} zend_closure_t;

static inline zend_object* php_componere_definition_create(zend_class_entry *ce) {
	php_componere_definition_t *o = 
		(php_componere_definition_t*) 
			ecalloc(1, sizeof(php_componere_definition_t));

	zend_object_std_init(&o->std, ce);

	o->ce = (zend_class_entry*) 
		zend_arena_alloc(&CG(arena), sizeof(zend_class_entry));

	memset(o->ce, 0, sizeof(zend_class_entry));

	o->std.handlers = &php_componere_definition_handlers;

	return &o->std;
}

static inline void php_componere_definition_property_copy(zval *zv) {
	zend_property_info *info = Z_PTR_P(zv);
	zend_property_info *child = (zend_property_info*)
		zend_arena_alloc(&CG(arena), sizeof(zend_property_info));

	memcpy(child, info, sizeof(zend_property_info));

	if (child->name) {
		zend_string_addref(child->name);
	}

	if (child->doc_comment) {
		zend_string_addref(child->doc_comment);
	}

	Z_PTR_P(zv) = child;
}

static inline void php_componere_definition_method_copy(zval *zv) {
	zend_function *function = Z_PTR_P(zv);
	zend_function *child;

	if (function->type != ZEND_USER_FUNCTION) {
		function_add_ref(function);
		return;
	}

	child = (zend_function*)
			zend_arena_alloc(&CG(arena), sizeof(zend_op_array));

	memcpy(child, function, sizeof(zend_op_array));

	function_add_ref(child);

	Z_PTR_P(zv) = child;
}

static inline void php_componere_definition_constant_copy(zval *zv) {
	zend_class_constant *constant = Z_PTR_P(zv);
	zend_class_constant *child = (zend_class_constant*)
		zend_arena_alloc(&CG(arena), sizeof(zend_class_constant));

	memcpy(child, constant, sizeof(zend_class_constant));

	if (child->doc_comment) {
		zend_string_addref(child->doc_comment);
	}

	ZVAL_COPY(&child->value, &constant->value);

	Z_PTR_P(zv) = child;
}

static inline void php_componere_definition_magic(zend_class_entry *ce, zend_class_entry *parent)
{
#define php_componere_definition_magic_find(c, f) \
	zend_hash_str_find_ptr(&c->function_table, ZEND_STRL(f))
	if (parent->constructor)
		ce->constructor = php_componere_definition_magic_find(ce, "__construct");
	if (parent->destructor)
		ce->destructor = php_componere_definition_magic_find(ce, "__destruct");
	if (parent->clone)
		ce->clone = php_componere_definition_magic_find(ce, "__clone");
	if (parent->__get)
		ce->__get = php_componere_definition_magic_find(ce, "__get");
	if (parent->__set)
		ce->__set = php_componere_definition_magic_find(ce, "__set");
	if (parent->__unset)
		ce->__unset = php_componere_definition_magic_find(ce, "__unset");
	if (parent->__call)
		ce->__call = php_componere_definition_magic_find(ce, "__call");
	if (parent->__callstatic)
		ce->__callstatic = php_componere_definition_magic_find(ce, "__callstatic");
	if (parent->__tostring)
		ce->__tostring = php_componere_definition_magic_find(ce, "__tostring");
	if (parent->__debugInfo)
		ce->__debugInfo = php_componere_definition_magic_find(ce, "__debuginfo");

	ce->serialize_func = php_componere_definition_magic_find(ce, "serialize");
	ce->unserialize_func = php_componere_definition_magic_find(ce, "unserialize");

	ce->serialize = parent->serialize;
	ce->unserialize = parent->unserialize;
#undef php_componere_definition_magic_find
}

inline void php_componere_definition_parent(zend_class_entry *ce, zend_class_entry *parent) {
	zend_class_entry **pce = &ce->parent;

	do {
		if (!(*pce)) {
			(*pce) = parent;
			break;
		}

		pce = &(*pce)->parent;
	} while ((*pce));
}

inline void php_componere_definition_inherit(zend_class_entry *ce, zend_class_entry *parent) {
	zend_bool is_final = parent->ce_flags & ZEND_ACC_FINAL;

#if PHP_VERSION_ID >= 70400
    ce->ce_flags &= ~ZEND_ACC_LINKED;
#endif
	parent->ce_flags &= ~ ZEND_ACC_FINAL;
	zend_do_inheritance(ce, parent);

	if (is_final) {
		parent->ce_flags |= ZEND_ACC_FINAL;
	}
#if PHP_VERSION_ID >= 70400
    ce->ce_flags |= ZEND_ACC_LINKED;
#endif
}

inline void php_componere_definition_copy(zend_class_entry *ce, zend_class_entry *parent)
{
	if (parent->num_interfaces) {
		ce->interfaces = (zend_class_entry **) 
			ecalloc(parent->num_interfaces, sizeof(zend_class_entry*));

		memcpy(ce->interfaces, parent->interfaces,
			sizeof(zend_class_entry*) * parent->num_interfaces);

		ce->num_interfaces = parent->num_interfaces;
	}

	if (parent->default_properties_count) {
		int i = 0;

		ce->default_properties_table = (zval*)
			ecalloc(sizeof(zval), parent->default_properties_count);

		for (i = 0; i < parent->default_properties_count; i++) {
			ZVAL_DUP(&ce->default_properties_table[i], &parent->default_properties_table[i]);
		}

		ce->default_properties_count = parent->default_properties_count;
	}

	if (parent->default_static_members_count) {
		int i = 0;

		ce->default_static_members_table = (zval*)
			ecalloc(sizeof(zval), parent->default_static_members_count);

		for (i = 0; i < parent->default_static_members_count; i++) {
			ZVAL_DUP(&ce->default_static_members_table[i], &parent->default_static_members_table[i]);
		}

#if PHP_VERSION_ID >= 70400
        if (ce->ce_flags & ZEND_ACC_IMMUTABLE) {
            ZEND_MAP_PTR_NEW(ce->static_members_table);
        } else {
            ZEND_MAP_PTR_INIT(ce->static_members_table, &ce->default_static_members_table);
        }
#else
		ce->static_members_table = ce->default_static_members_table;
#endif
		ce->default_static_members_count = parent->default_static_members_count;
	}
#if PHP_VERSION_ID >= 70400
    else {
        ZEND_MAP_PTR_INIT(ce->static_members_table, &ce->default_static_members_table);
    }
#endif

	if (zend_hash_num_elements(&parent->properties_info)) {
		zend_hash_copy(
			&ce->properties_info, 
			&parent->properties_info, 
			php_componere_definition_property_copy);
	}

	if (zend_hash_num_elements(&parent->constants_table)) {
		zend_hash_copy(
			&ce->constants_table, 
			&parent->constants_table, 
			php_componere_definition_constant_copy);
	}

	if (zend_hash_num_elements(&parent->function_table)) {
		zend_hash_copy(
			&ce->function_table,
			&parent->function_table,
			php_componere_definition_method_copy);
	}

	php_componere_definition_magic(ce, parent);

	ce->ce_flags     |= parent->ce_flags;
	ce->ce_flags     &= ~ZEND_ACC_CONSTANTS_UPDATED;
#if PHP_VERSION_ID >= 70400
    ce->ce_flags     &= ~ZEND_ACC_LINKED;
#endif
	ce->parent        = parent->parent;

	ce->create_object              = parent->create_object;
	ce->get_iterator               = parent->get_iterator;
	ce->interface_gets_implemented = parent->interface_gets_implemented;
	ce->get_static_method          = parent->get_static_method;
}

static int php_componere_relink_function(zval *zv, int argc, va_list list, zend_hash_key *key) {
	zend_function *el = Z_FUNC_P(zv);
	zend_class_entry *def = va_arg(list, zend_class_entry*);
	zend_class_entry *parent = va_arg(list, zend_class_entry*);

	if (el->type == ZEND_USER_FUNCTION) {
		if (el->common.scope == parent) {
			el->common.scope = def;
		}

#if PHP_VERSION_ID >= 70400
        if (RUN_TIME_CACHE(&el->op_array)) {
            memset(RUN_TIME_CACHE(&el->op_array), 0, el->op_array.cache_size);
        }
#else
		if (el->op_array.run_time_cache) {
			memset(el->op_array.run_time_cache, 0, el->op_array.cache_size);
		}
#endif
	}

	return ZEND_HASH_APPLY_KEEP;
}

static inline int php_componere_relink_property(zval *zv, int argc, va_list list, zend_hash_key *key) {
	zend_property_info *el = Z_PTR_P(zv);
	zend_class_entry *def = va_arg(list, zend_class_entry*);
	zend_class_entry *parent = va_arg(list, zend_class_entry*);

	if (el->ce == parent) {
		el->ce = def;
	} else if (el->ce == def) {
		el->ce = parent;
	}
	
	return ZEND_HASH_APPLY_KEEP;
}

static inline int php_componere_relink_constant(zval *zv, int argc, va_list list, zend_hash_key *key) {
	zend_class_constant *el = Z_PTR_P(zv);
	zend_class_entry    *def = va_arg(list, zend_class_entry*);
	zend_class_entry    *parent = va_arg(list, zend_class_entry*);	

	if (el->ce == parent) {
		el->ce = def;
	} else if (el->ce == def) {
		el->ce = parent;
	}

	return ZEND_HASH_APPLY_KEEP;
}

static int php_componere_relink_class(zval *zv, int argc, va_list list, zend_hash_key *key) {
	zend_class_entry *el = Z_CE_P(zv);
	zend_class_entry *def = va_arg(list, zend_class_entry*);
	zend_class_entry *parent = va_arg(list, zend_class_entry*);

	if ((parent->type == ZEND_USER_CLASS) &&
	    !(parent->ce_flags & (ZEND_ACC_INTERFACE|ZEND_ACC_TRAIT))) {
		zend_hash_apply_with_arguments(
			&el->function_table, 
			php_componere_relink_function,
			2,
			def, parent);

		zend_hash_apply_with_arguments(
			&el->properties_info, 
			php_componere_relink_property,
			2,
			def, parent);

		zend_hash_apply_with_arguments(
			&el->constants_table, 
			php_componere_relink_constant,
			2,
			def, parent);

		if (el->parent == parent) {
			el->parent = def;
		}
	}

	return ZEND_HASH_APPLY_KEEP;
}

static zend_always_inline void php_componere_relink_objects(zend_objects_store *objects, zend_class_entry *def, zend_class_entry *parent) {
	if (objects->top > 1) {
		uint32_t it = 1,
			 end = objects->top;

		while (it < end) {
			zend_object *object = objects->object_buckets[it];

			if (IS_OBJ_VALID(object)) {
				if (object->ce == parent) {
					object->ce = def;
				} else if (instanceof_function(object->ce, zend_ce_closure)) {
					zend_closure_t *closure = (zend_closure_t*) object;

                    if (closure->func.type == ZEND_USER_FUNCTION) {
#if PHP_VERSION_ID >= 70400
					    if (RUN_TIME_CACHE(&closure->func.op_array)) {
						    memset(RUN_TIME_CACHE(&closure->func.op_array), 0, closure->func.op_array.cache_size);
					    }
#else
					    if (closure->func.op_array.run_time_cache) {
						    memset(closure->func.op_array.run_time_cache, 0, closure->func.op_array.cache_size);
					    }
#endif
                    }

					if (closure->called_scope == parent) {
						closure->called_scope = def;
					}
				}
			}
			it++;
		}
	}
}

static zend_always_inline void php_componere_relink_frames(zend_execute_data *execute_data) {
	do {
		if (EX(func) && EX(func)->type == ZEND_USER_FUNCTION) {
#if PHP_VERSION_ID >= 70400
			if (RUN_TIME_CACHE(&EX(func)->op_array)) {
				memset(RUN_TIME_CACHE(&EX(func)->op_array), 0, EX(func)->op_array.cache_size);
			}
#else
			if (EX(func)->op_array.run_time_cache) {
				memset(EX(func)->op_array.run_time_cache, 0, EX(func)->op_array.cache_size);
			}
#endif
		}
	} while ((execute_data = EX(prev_execute_data)));
}

static inline void php_componere_definition_destroy(zend_object *zo) {
	php_componere_definition_t *o = php_componere_definition_from(zo);

	if (o->registered && o->saved) {
		zend_string *name = zend_string_tolower(o->saved->name);
		void        *ce   = o->ce;

		if (EG(current_execute_data)) {
			php_componere_relink_frames(EG(current_execute_data));
		}

		zend_hash_apply_with_arguments(
			CG(class_table), 
			php_componere_relink_class, 2, 
			o->saved,
			ce);

		zend_hash_apply_with_arguments(
			CG(function_table),
			php_componere_relink_function, 2,
			o->saved,
			ce);

		php_componere_relink_objects(&EG(objects_store), o->saved, ce);

		zend_hash_update_ptr(CG(class_table), name, o->saved);

		zend_string_release(name);
	} else if (!o->registered) {
		php_componere_destroy_class(o->ce);
	}

	if (!Z_ISUNDEF(o->reflector)) {
		zval_ptr_dtor(&o->reflector);
	}

	zend_object_std_dtor(&o->std);
}

PHP_METHOD(Definition, __construct)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zend_string *parent = NULL;
	HashTable *interfaces = NULL;
	zend_class_entry *pce = NULL;

	switch (ZEND_NUM_ARGS()) {
		case 1: if (php_componere_parse_parameters("S", &name) != SUCCESS) {
			php_componere_wrong_parameters("name expected as single argument");
			return;
		} break;

		case 2: if (php_componere_parse_parameters("SH", &name, &interfaces) != SUCCESS &&
			    php_componere_parse_parameters("SS", &name, &parent) != SUCCESS) {
			php_componere_wrong_parameters("name and interfaces, or name and parent expected");
			return;
		} break;

		case 3: if (php_componere_parse_parameters("SSH", &name, &parent, &interfaces) != SUCCESS) {
			php_componere_wrong_parameters("name, parent, and interfaces expected");
			return;
		} break;

		default: {
			php_componere_wrong_parameters("name, parent, and interfaces expected");
			return;
		}
	}

	if (name && parent && zend_string_equals_ci(name, parent)) {
		php_componere_throw_ex(InvalidArgumentException,
			"name and parent cannot refer to the same class");
		return;
	}

	o->ce->type = ZEND_USER_CLASS;
	o->ce->name = zend_string_copy(name);

	zend_initialize_class_data(o->ce, 1);

	pce = parent ? zend_lookup_class(parent) : zend_lookup_class(name);
	if (pce) {
		if (zend_string_equals_ci(o->ce->name, pce->name)) {
			if (pce->type != ZEND_USER_CLASS) {
				php_componere_throw_ex(InvalidArgumentException,
					"cannot redeclare internal class %s", ZSTR_VAL(pce->name));
				return;
			}

			if (pce->ce_flags & ZEND_ACC_INTERFACE) {
				php_componere_throw_ex(InvalidArgumentException,
					"cannot redeclare interface %s", ZSTR_VAL(pce->name));
				return;
			}

			if (pce->ce_flags & ZEND_ACC_TRAIT) {
				php_componere_throw_ex(InvalidArgumentException,
					"cannot redeclare trait %s", ZSTR_VAL(pce->name));
				return;
			}

			php_componere_definition_copy(o->ce, pce);

			o->saved = pce;
		} else {
			php_componere_definition_inherit(o->ce, pce);
		}
	}

#if PHP_VERSION_ID >= 70400
    o->ce->ce_flags |= ZEND_ACC_LINKED;
#endif

	if (pce && pce->type == ZEND_USER_CLASS) {
		memcpy(&o->ce->info.user, &pce->info.user, sizeof(pce->info.user));

		if (pce->info.user.doc_comment) {
			o->ce->info.user.doc_comment = zend_string_copy(pce->info.user.doc_comment);
		}
	}

	if (interfaces) {
		zval *interface = NULL;

		ZEND_HASH_FOREACH_VAL(interfaces, interface) {
			zend_class_entry *ce;

			if (Z_TYPE_P(interface) == IS_STRING) {
				ce = zend_lookup_class(Z_STR_P(interface));

				if (!ce) {
					php_componere_throw(
						"could not find interface %s", 
						Z_STRVAL_P(interface));
					break;
				}

				if ((ce->ce_flags & ZEND_ACC_INTERFACE) != ZEND_ACC_INTERFACE) {
					php_componere_throw_ex(InvalidArgumentException,
						"%s is not an interface", 
						Z_STRVAL_P(interface));
					break;
				}

				zend_do_implement_interface(o->ce, ce);
			}
		} ZEND_HASH_FOREACH_END();

		o->ce->ce_flags &= ~ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;
	}

	if (!o->ce->info.user.filename) {
		o->ce->info.user.filename = name;
	}

#if PHP_VERSION_ID >= 70400
    o->ce->ce_flags |= ZEND_ACC_RESOLVED_INTERFACES;
#endif
}

#if PHP_VERSION_ID >= 70400
void php_componere_definition_properties_table_rebuild(zend_class_entry *ce)
{
	zend_property_info **table, *prop;
	size_t size;
	if (ce->default_properties_count == 0 || ce->properties_info_table) {
		return;
	}

	size = sizeof(zend_property_info *) * ce->default_properties_count;
	if (ce->type == ZEND_USER_CLASS) {
		ce->properties_info_table = table = zend_arena_alloc(&CG(arena), size);
	} else {
		ce->properties_info_table = table = pemalloc(size, 1);
	}

	/* Dead slots may be left behind during inheritance. Make sure these are NULLed out. */
	memset(table, 0, size);

	if (ce->parent && ce->parent->default_properties_count != 0) {
		zend_property_info **parent_table = ce->parent->properties_info_table;
		memcpy(
			table, parent_table,
			sizeof(zend_property_info *) * ce->parent->default_properties_count
		);

		/* Child did not add any new properties, we are done */
		if (ce->default_properties_count == ce->parent->default_properties_count) {
			return;
		}
	}

	ZEND_HASH_FOREACH_PTR(&ce->properties_info, prop) {
		if (prop->ce == ce && (prop->flags & ZEND_ACC_STATIC) == 0) {
			table[OBJ_PROP_TO_NUM(prop->offset)] = prop;
		}
	} ZEND_HASH_FOREACH_END();
}
#endif

PHP_METHOD(Definition, register)
{
	php_componere_definition_t *o = 
		php_componere_definition_fetch(getThis());
	zend_string *name = zend_string_tolower(o->ce->name);

	if (o->registered) {
		php_componere_throw(
			"could not re-register %s", ZSTR_VAL(o->ce->name));
		zend_string_release(name);
		return;
	}

	zend_hash_apply_with_arguments(
		&o->ce->function_table,
		php_componere_relink_function, 2,
		o->ce,
		o->saved);

	zend_hash_apply_with_arguments(
		&o->ce->properties_info,
		php_componere_relink_property, 2,
		o->ce,
		o->saved);

	zend_hash_apply_with_arguments(
		&o->ce->constants_table,
		php_componere_relink_constant, 2,
		o->ce,
		o->saved);

	if (o->saved) {
		php_componere_relink_frames(EG(current_execute_data));

		o->saved->refcount++;

		zend_hash_del(CG(class_table), name);

		zend_hash_apply_with_arguments(
			CG(class_table), 
			php_componere_relink_class, 2, 
			o->ce, 	
			o->saved);

		zend_hash_apply_with_arguments(
			CG(function_table),
			php_componere_relink_function, 2,
			o->ce,
			o->saved);

		php_componere_relink_objects(&EG(objects_store), o->ce, o->saved);
	}

	zend_hash_update_ptr(CG(class_table), name, o->ce);

	o->ce->refcount = 1;
	o->registered = 1;

	zend_string_release(name);

#if PHP_VERSION_ID >= 70400
    php_componere_definition_properties_table_rebuild(o->ce);
#endif
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_method, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, method)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, addMethod)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zval *method = NULL;
	zend_function *function;

	if (php_componere_parse_parameters("SO", &name, &method, php_componere_method_ce) != SUCCESS) {
		php_componere_wrong_parameters("name and method expected");
		return;	
	}

	if (o->registered) {
		php_componere_throw(
			"%s is already registered, cannot add method %s", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	function = php_componere_method_function(method);

	if (function->common.scope) {
		php_componere_throw(
			"method was already added to %s", 
			ZSTR_VAL(function->common.scope->name));
		return;
	}

	if (zend_string_equals_literal_ci(name, "__construct")) {
		o->ce->constructor = function;
	} else if (zend_string_equals_literal_ci(name, "__destruct")) {
		o->ce->destructor = function;
	} else if (zend_string_equals_literal_ci(name, "__clone")) {
		o->ce->clone = function;
	} else if (zend_string_equals_literal_ci(name, "__get")) {
		o->ce->__get = function;
		o->ce->ce_flags |= ZEND_ACC_USE_GUARDS;
	} else if (zend_string_equals_literal_ci(name, "__set")) {
		o->ce->__set = function;
		o->ce->ce_flags |= ZEND_ACC_USE_GUARDS;
	} else if (zend_string_equals_literal_ci(name, "__unset")) {
		o->ce->__unset = function;
		o->ce->ce_flags |= ZEND_ACC_USE_GUARDS;
	} else if (zend_string_equals_literal_ci(name, "__isset")) {
		o->ce->__isset = function;
		o->ce->ce_flags |= ZEND_ACC_USE_GUARDS;
	} else if (zend_string_equals_literal_ci(name, "__call")) {
		o->ce->__call = function;
	} else if (zend_string_equals_literal_ci(name, "__callstatic")) {
		o->ce->__callstatic = function;
	} else if (zend_string_equals_literal_ci(name, "__tostring")) {
		o->ce->__tostring = function;
	} else if (zend_string_equals_literal_ci(name, "__debuginfo")) {
		o->ce->__debugInfo = function;
	} else if (zend_string_equals_literal_ci(name, "serialize")) {
		o->ce->serialize_func = function;
	} else if (zend_string_equals_literal_ci(name, "unserialize")) {
		o->ce->unserialize_func = function;
	}

	function->common.scope = o->ce;
	function->common.function_name = zend_string_copy(name);

	name = zend_string_tolower(name);

	zend_hash_update_ptr(&o->ce->function_table, name, function);
	
	function_add_ref(function);

	zend_string_release(name);

	RETURN_ZVAL(getThis(), 1, 0);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_trait, 0, 0, 1)
	ZEND_ARG_INFO(0, trait)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, addTrait)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_class_entry *trait = NULL;

	if (php_componere_parse_parameters("C", &trait) != SUCCESS) {
		php_componere_wrong_parameters("trait expected");
		return;
	}

	if (o->registered) {
		php_componere_throw(
			"%s is already registered, cannot add trait %s", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(trait->name));
		return;
	}

	{
		zend_string *name;

		ZEND_HASH_FOREACH_STR_KEY(&trait->function_table, name) {
			zend_hash_del(&o->ce->function_table, name);
		} ZEND_HASH_FOREACH_END();
	}

#if PHP_VERSION_ID >= 70400
    {
        uint32_t num_traits          = o->ce->num_traits;

        o->ce->trait_names = erealloc(o->ce->trait_names, sizeof(zend_class_name) * (num_traits + 1));
        o->ce->num_traits = 1;

        o->ce->trait_names[num_traits].name = zend_string_copy(trait->name);
        o->ce->trait_names[num_traits].lc_name = zend_string_tolower(trait->name);

        o->ce->trait_names += num_traits;

        o->ce->ce_flags |= ZEND_ACC_IMPLEMENT_TRAITS;

        zend_do_link_class(o->ce, NULL);

        o->ce->num_traits  = num_traits + 1;
        o->ce->trait_names -= num_traits;
    }
#else
	zend_do_implement_trait(o->ce, trait);
	zend_do_bind_traits(o->ce);
#endif

	RETURN_ZVAL(getThis(), 1, 0);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_interface, 0, 0, 1)
	ZEND_ARG_INFO(0, interface)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, addInterface)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_class_entry *interface = NULL;

	if (php_componere_parse_parameters("C", &interface) != SUCCESS) {
		php_componere_wrong_parameters("interface expected");
		return;
	}

	if (o->registered) {
		php_componere_throw(
			"%s is already registered, cannot add interface %s", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(interface->name));
		return;
	}

	if (!instanceof_function(o->ce, interface)) {
		zend_do_implement_interface(o->ce, interface);
	}

#if PHP_VERSION_ID >= 70400
    o->ce->ce_flags |= ZEND_ACC_RESOLVED_INTERFACES;
#endif

	RETURN_ZVAL(getThis(), 1, 0);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_property, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

static zend_always_inline zend_bool php_componere_property_check(zend_objects_store *objects, php_componere_definition_t *def) {
	
	if (objects->top > 1) {
		uint32_t it = 1,
			 end = objects->top;

		while (it < end) {
			zend_object *object = objects->object_buckets[it];

			if (IS_OBJ_VALID(object)) {
				if (object->ce == def->saved) {
					if (def->ce->default_properties_count >= object->ce->default_properties_count) {
						return 0;
					}
				}
			}
			it++;
		}
	}
	
	return 1;
}

PHP_METHOD(Definition, addProperty)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zval *value;

	if (php_componere_parse_parameters("SO", &name, &value, php_componere_value_ce) != SUCCESS) {
		php_componere_wrong_parameters("name and value expected");
		return;
	}

	if (zend_get_property_info(o->ce, name, 1)) {
		php_componere_throw(
			"cannot redeclare %s::$%s",
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (o->registered) {
		php_componere_throw( 
			"%s is already registered, cannot add property %s", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (o->saved && !php_componere_property_check(&EG(objects_store), o)) {
		php_componere_throw(
			"%s is already in use, cannot add property %s",
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (zend_declare_property(o->ce,
		ZSTR_VAL(name), ZSTR_LEN(name),
		php_componere_value_default(value),
		php_componere_value_access(value)) == SUCCESS) {
		php_componere_value_addref(value);

#if PHP_VERSION_ID >= 70400
        zend_do_link_class(o->ce, NULL);
#endif
	}

	RETURN_ZVAL(getThis(), 1, 0);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_constant, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, addConstant)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zval *value;

	if (php_componere_parse_parameters("SO", &name, &value, php_componere_value_ce) != SUCCESS) {
		php_componere_wrong_parameters("name and value expected");
		return;
	}

	if (o->registered) {
		php_componere_throw(
			"%s is already registered, cannot add constant %s", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (zend_hash_exists(&o->ce->constants_table, name)) {
		php_componere_throw(
			"cannot redeclare %s::%s",
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (php_componere_value_access(value) & ZEND_ACC_STATIC) {
		php_componere_throw(
			"%s::%s cannot be declared static", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (Z_ISUNDEF_P(php_componere_value_default(value))) {
		php_componere_throw(
			"%s::%s cannot be undefined",
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	zend_declare_class_constant_ex(
		o->ce, name, 
		php_componere_value_default(value),
		php_componere_value_access(value), NULL);

	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(Definition, setConstant)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zval *value;
	zend_constant *constant;

	if (php_componere_parse_parameters("SO", &name, &value, php_componere_value_ce) != SUCCESS) {
		php_componere_wrong_parameters("name and value expected");
		return;
	}

	if (o->registered) {
		php_componere_throw(
			"%s is already registered, cannot set constant %s", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (!zend_hash_exists(&o->ce->constants_table, name)) {
		php_componere_throw(
			"%s::%s is not declared",
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (php_componere_value_access(value) & ZEND_ACC_STATIC) {
		php_componere_throw(
			"%s::%s cannot be declared static", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (Z_ISUNDEF_P(php_componere_value_default(value))) {
		php_componere_throw(
			"%s::%s cannot be undefined",
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	constant = zend_hash_find_ptr(&o->ce->constants_table, name);

	if (Z_REFCOUNTED(constant->value)) {
		zval_ptr_dtor(&constant->value);
	}

	ZVAL_COPY(&constant->value, php_componere_value_default(value));

	RETURN_ZVAL(getThis(), 1, 0);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_closure, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, getClosure)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zend_string *key = NULL;
	zend_function *function = NULL;

	if (php_componere_parse_parameters("S", &name) != SUCCESS) {
		return;
	}

	if (o->registered) {
		php_componere_throw("cannot get closure after registration");
		return;
	}

	key = zend_string_tolower(name);
	function = zend_hash_find_ptr(&o->ce->function_table, key);

	if (!function) {
		php_componere_throw("could not find %s::%s", ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
	} else {
		zend_create_closure(return_value, function, o->ce, o->ce, NULL);
	}
	zend_string_release(key);
}

PHP_METHOD(Definition, getClosures)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_function *function = NULL;

	php_componere_no_parameters();

	if (o->registered) {
		php_componere_throw("cannot get closures after registration");
		return;
	}

	array_init(return_value);

	ZEND_HASH_FOREACH_PTR(&o->ce->function_table, function) {
		zval closure;

		zend_create_closure(&closure, function, o->ce, o->ce, NULL);		

		zend_hash_add(
			Z_ARRVAL_P(return_value), function->common.function_name, &closure);
	} ZEND_HASH_FOREACH_END();
}

PHP_METHOD(Definition, isRegistered)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());

	php_componere_no_parameters();

	RETURN_BOOL(o->registered);
}

PHP_METHOD(Definition, getReflector)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());

	php_componere_no_parameters();

	if (!Z_ISUNDEF(o->reflector)) {
		RETURN_ZVAL(&o->reflector, 1, 0);
	}

	php_componere_reflection_object_factory(
		&o->reflector,
		php_componere_reflection_class_ce, 
		PHP_REF_TYPE_OTHER, 
		o->ce,
		o->ce->name);

	RETURN_ZVAL(&o->reflector, 1, 0);
}

static zend_function_entry php_componere_definition_abstract_methods[] = {
	PHP_ME(Definition, addMethod, php_componere_definition_method, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addTrait, php_componere_definition_trait, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addInterface, php_componere_definition_interface, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, getReflector, php_componere_no_arginfo, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static zend_function_entry php_componere_definition_methods[] = {
	PHP_ME(Definition, __construct, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addProperty, php_componere_definition_property, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addConstant, php_componere_definition_constant, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, setConstant, php_componere_definition_constant, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, getClosure, php_componere_definition_closure, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, getClosures, php_componere_no_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, register, php_componere_no_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, isRegistered, php_componere_no_arginfo, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_MINIT_FUNCTION(Componere_Definition) {
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "Componere\\Abstract", "Definition", php_componere_definition_abstract_methods);

	php_componere_definition_abstract_ce = zend_register_internal_class(&ce);
	php_componere_definition_abstract_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

	INIT_NS_CLASS_ENTRY(ce, "Componere", "Definition", php_componere_definition_methods);

	php_componere_definition_ce = zend_register_internal_class_ex(&ce, php_componere_definition_abstract_ce);
	php_componere_definition_ce->create_object = php_componere_definition_create;

	php_componere_setup_handlers(
		&php_componere_definition_handlers,
		php_componere_deny_debug, 
		php_componere_deny_collect,
		php_componere_deny_clone,
		php_componere_definition_destroy, 
		XtOffsetOf(php_componere_definition_t, std));

	return SUCCESS;
}

PHP_RINIT_FUNCTION(Componere_Definition) {
	php_componere_definition_abstract_ce->ce_flags |= ZEND_ACC_FINAL;
	php_componere_definition_ce->ce_flags |= ZEND_ACC_FINAL;

	return SUCCESS;
}
#endif
