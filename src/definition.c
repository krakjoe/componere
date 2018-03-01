/*
  +----------------------------------------------------------------------+
  | componere                                                            |
  +----------------------------------------------------------------------+
  | Copyright (c) Joe Watkins 2018                                       |
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

#include "src/common.h"
#include "src/definition.h"
#include "src/method.h"
#include "src/value.h"

zend_class_entry *php_componere_definition_abstract_ce;
zend_class_entry *php_componere_definition_ce;

zend_object_handlers php_componere_definition_handlers;

static inline zend_object* php_componere_definition_create(zend_class_entry *ce) {
	php_componere_definition_t *o = 
		(php_componere_definition_t*) 
			ecalloc(1, sizeof(php_componere_definition_t));

	zend_object_std_init(&o->std, ce);

	o->ce = (zend_class_entry*) 
		zend_arena_alloc(&CG(arena), sizeof(zend_class_entry));

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

static inline int php_componere_definition_property_fix(zval *zv, zend_class_entry **pair) {
	zend_property_info *info = Z_PTR_P(zv);

	if (info->ce == pair[1]) {
		info->ce = pair[0];
	}

	return ZEND_HASH_APPLY_KEEP;
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

static inline int php_componere_definition_method_fix(zval *zv, zend_class_entry **pair) {
	zend_function *function = Z_PTR_P(zv);

	if (function->common.scope == pair[1]) {
		function->common.scope = pair[0];
	}

	return ZEND_HASH_APPLY_KEEP;
}

#if PHP_VERSION_ID >= 70100
static inline void php_componere_definition_constant_copy(zval *zv) {
	zend_class_constant *constant = Z_PTR_P(zv);
	zend_class_constant *child = (zend_class_constant*)
		zend_arena_alloc(&CG(arena), sizeof(zend_class_constant));

	memcpy(child, constant, sizeof(zend_class_constant));

	if (child->doc_comment) {
		zend_string_addref(child->doc_comment);
	}

	ZVAL_DUP(&child->value, &constant->value);

	Z_PTR_P(zv) = child;
}
#else
static inline void php_componere_definition_constant_copy(zval *zv) {
	ZVAL_DUP(zv, zv);
}
#endif

#if PHP_VERSION_ID >= 70100
static inline int php_componere_definition_constant_fix(zval *zv, zend_class_entry **pair) {
	zend_class_constant *constant = Z_PTR_P(zv);

	if (constant->ce == pair[1]) {
		constant->ce = pair[0];
	}

	return ZEND_HASH_APPLY_KEEP;
}
#endif

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

inline void php_componere_definition_copy(zend_class_entry *ce, zend_class_entry *parent)
{
	zend_class_entry* pair[2] = {ce, parent};

	if (parent->num_interfaces) {
		ce->interfaces = (zend_class_entry **) 
			ecalloc(parent->num_interfaces, sizeof(zend_class_entry*));

		memcpy(ce->interfaces, parent->interfaces,
			sizeof(zend_class_entry*) * parent->num_interfaces);

		ce->num_interfaces = parent->num_interfaces;
	}

	if (parent->default_properties_count) {
		uint32_t i = 0;

		ce->default_properties_table = (zval*)
			ecalloc(sizeof(zval), parent->default_properties_count);

		for (i = 0; i < parent->default_properties_count; i++) {
			ZVAL_DUP(&ce->default_properties_table[i], &parent->default_properties_table[i]);
		}

		ce->default_properties_count = parent->default_properties_count;	
	}

	if (parent->default_static_members_count) {
		uint32_t i = 0;

		ce->default_static_members_table = (zval*)
			ecalloc(sizeof(zval), parent->default_static_members_count);

		for (i = 0; i < parent->default_static_members_count; i++) {
			ZVAL_DUP(&ce->default_static_members_table[i], &parent->default_static_members_table[i]);
		}

		ce->static_members_table = ce->default_static_members_table;
		ce->default_static_members_count = parent->default_static_members_count;
	}

	if (zend_hash_num_elements(&parent->properties_info)) {
		zend_hash_copy(
			&ce->properties_info, 
			&parent->properties_info, 
			php_componere_definition_property_copy);

		zend_hash_apply_with_argument(&ce->properties_info, 
			(apply_func_arg_t) php_componere_definition_property_fix, &pair);
	}

	if (zend_hash_num_elements(&parent->constants_table)) {
		zend_hash_copy(
			&ce->constants_table, 
			&parent->constants_table, 
			php_componere_definition_constant_copy);
#if PHP_VERSION_ID >= 70100
		zend_hash_apply_with_argument(&ce->constants_table, 
			(apply_func_arg_t) php_componere_definition_constant_fix, &pair);
#endif
	}

	if (zend_hash_num_elements(&parent->function_table)) {
		zend_hash_copy(
			&ce->function_table,
			&parent->function_table,
			php_componere_definition_method_copy);

		zend_hash_apply_with_argument(&ce->function_table, 
			(apply_func_arg_t) php_componere_definition_method_fix, &pair);
	}

	php_componere_definition_magic(ce, parent);

	ce->ce_flags |= parent->ce_flags;
	ce->parent = parent->parent;
}

static inline void php_componere_definition_destroy(zend_object *zo) {
	php_componere_definition_t *o = php_componere_definition_from(zo);

	if (o->saved) {
		zend_string *name = 
			zend_string_tolower(o->saved->name);
		zend_class_entry *ce = 
			zend_hash_find_ptr(CG(class_table), name);

		if (ce) {
			zend_class_entry *pair[2] = {ce, o->saved};

			zend_hash_del(CG(class_table), name);

			memcpy(ce, o->saved, sizeof(zend_class_entry));

			zend_hash_apply_with_argument(&ce->function_table, 
				(apply_func_arg_t) php_componere_definition_method_fix, &pair);
			zend_hash_apply_with_argument(&ce->properties_info, 
				(apply_func_arg_t) php_componere_definition_property_fix, &pair);
#if PHP_VERSION_ID >= 70100
			zend_hash_apply_with_argument(&ce->constants_table,
				(apply_func_arg_t) php_componere_definition_constant_fix, &pair);
#endif
			zend_hash_update_ptr(CG(class_table), name, ce);
		}

		zend_string_release(name);
	}

	if (!o->registered || (o->ce && o->ce->refcount > 1)) {
		php_componere_destroy_class(o->ce);
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

	o->ce->type = ZEND_USER_CLASS;
	o->ce->name = zend_string_copy(name);

	zend_initialize_class_data(o->ce, 1);

	if (!parent) {
		pce = zend_lookup_class(name);
	} else {
		pce = zend_lookup_class(parent);
	}

	if (pce && zend_string_equals_ci(o->ce->name, pce->name)) {
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
		o->saved->refcount++;
	} else if(pce) {
		zend_do_inheritance(o->ce, pce);
	}

	if (pce) {
		php_componere_definition_parent(o->ce, pce);
	}

	o->ce->ce_flags |= ZEND_ACC_USE_GUARDS;

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

				if (!instanceof_function(o->ce, ce)) {
					zend_do_implement_interface(o->ce, ce);
				}
			}
		} ZEND_HASH_FOREACH_END();

		o->ce->ce_flags &= ~ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;
	}
}

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

	zend_hash_update_ptr(CG(class_table), name, o->ce);
	zend_string_release(name);
	o->registered = 1;
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

	function->common.scope = o->ce;

	name = zend_string_tolower(name);

	zend_hash_update_ptr(&o->ce->function_table, name, function);
	
	function_add_ref(function);

	if (zend_string_equals_literal(name, "__construct")) {
		o->ce->constructor = function;
	} else if (zend_string_equals_literal(name, "__destruct")) {
		o->ce->destructor = function;
	} else if (zend_string_equals_literal(name, "__clone")) {
		o->ce->clone = function;
	} else if (zend_string_equals_literal(name, "__get")) {
		o->ce->__get = function;
	} else if (zend_string_equals_literal(name, "__set")) {
		o->ce->__set = function;
	} else if (zend_string_equals_literal(name, "__unset")) {
		o->ce->__unset = function;
	} else if (zend_string_equals_literal(name, "__call")) {
		o->ce->__call = function;
	} else if (zend_string_equals_literal(name, "__callstatic")) {
		o->ce->__callstatic = function;
	} else if (zend_string_equals_literal(name, "__tostring")) {
		o->ce->__tostring = function;
	} else if (zend_string_equals_literal(name, "__debuginfo")) {
		o->ce->__debugInfo = function;
	} else if (zend_string_equals_literal(name, "serialize")) {
		o->ce->serialize_func = function;
	} else if (zend_string_equals_literal(name, "unserialize")) {
		o->ce->unserialize_func = function;
	}

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

	zend_do_implement_trait(o->ce, trait);
	zend_do_bind_traits(o->ce);

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

	RETURN_ZVAL(getThis(), 1, 0);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_property, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

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

	zend_declare_property(o->ce, 
		ZSTR_VAL(name), ZSTR_LEN(name), 
		php_componere_value_default(value), 
		php_componere_value_access(value));

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

	if (zend_hash_exists(&o->ce->constants_table, name)) {
		php_componere_throw(
			"cannot redeclare %s::%s",
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (o->registered) {
		php_componere_throw(
			"%s is already registered, cannot add constant %s", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (php_componere_value_access(value) & ZEND_ACC_STATIC) {
		php_componere_throw(
			"%s::%s cannot be declared static", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

#if PHP_VERSION_ID >= 70100
	zend_declare_class_constant_ex(
		o->ce, name, 
		php_componere_value_default(value),
		php_componere_value_access(value), NULL);
#else
	zend_declare_class_constant(o->ce, ZSTR_VAL(name), ZSTR_LEN(name), 
		php_componere_value_default(value));
#endif

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

static zend_function_entry php_componere_definition_abstract_methods[] = {
	PHP_ME(Definition, addMethod, php_componere_definition_method, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addTrait, php_componere_definition_trait, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addInterface, php_componere_definition_interface, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static zend_function_entry php_componere_definition_methods[] = {
	PHP_ME(Definition, __construct, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addProperty, php_componere_definition_property, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addConstant, php_componere_definition_constant, ZEND_ACC_PUBLIC)
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
