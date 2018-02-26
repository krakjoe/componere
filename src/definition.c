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

#include "src/method.h"
#include "src/value.h"

zend_class_entry *php_componere_definition_ce;
zend_object_handlers php_componere_definition_handlers;

typedef struct _php_componere_definition_t {
	zend_class_entry *ce;
	zend_class_entry *saved;
	zend_bool registered;
	zend_object std;
} php_componere_definition_t;

#define php_componere_definition_from(o) \
	((php_componere_definition_t*) \
		((char*) o - XtOffsetOf(php_componere_definition_t, std)))
#define php_componere_definition_fetch(z) php_componere_definition_from(Z_OBJ_P(z))

static inline zend_object* php_componere_definition_create(zend_class_entry *ce) {
	php_componere_definition_t *o = 
		(php_componere_definition_t*) 
			ecalloc(1, sizeof(php_componere_definition_t));

	zend_object_std_init(&o->std, ce);

	object_properties_init(&o->std, ce);

	o->ce = (zend_class_entry*) 
		zend_arena_alloc(&CG(arena), sizeof(zend_class_entry));

	o->std.handlers = &php_componere_definition_handlers;

	return &o->std;
}

static inline void php_componere_definition_destroy(zend_object *zo) {
	php_componere_definition_t *o = php_componere_definition_from(zo);
	zval tmp;

	if (!o->registered) {
		ZVAL_PTR(&tmp, o->ce);

		destroy_zend_class(&tmp);
	} else if (o->saved) {
		zend_string *name = 
			zend_string_tolower(o->saved->name);
		zend_class_entry *ce = 
			zend_hash_find_ptr(CG(class_table), name);

		if (ce) {
			zend_function *function;
			zend_property_info *info;

			zend_hash_del(CG(class_table), name);

			memcpy(ce, o->saved, sizeof(zend_class_entry));

			ZEND_HASH_FOREACH_PTR(&ce->function_table, function) {
				if (zend_string_equals(function->common.scope->name, o->ce->name)) {
					function->common.scope = ce;
				}
			} ZEND_HASH_FOREACH_END();

			ZEND_HASH_FOREACH_PTR(&o->ce->properties_info, info) {
				if (zend_string_equals(info->ce->name, o->ce->name)) {
					info->ce = ce;
				}
			} ZEND_HASH_FOREACH_END();

			zend_hash_update_ptr(
				CG(class_table), name, ce);
		}

		zend_string_release(name);
	}

	zend_object_std_dtor(&o->std);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, parent)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, __construct)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zend_string *parent = NULL;
	HashTable *interfaces = NULL;
	zend_class_entry *pce = NULL;

	switch (ZEND_NUM_ARGS()) {
		case 1: if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "S", &name) != SUCCESS) {
			zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "name expected as single argument");
			return;
		} break;

		case 2: if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "SH", &name, &interfaces) != SUCCESS &&
			    zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "SS", &name, &parent) != SUCCESS) {
			zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "name and interfaces, or name and parent expected");
			return;
		} break;

		case 3: if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "SSH", &name, &parent, &interfaces) != SUCCESS) {
			zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "name, parent, and interfaces expected");
			return;
		} break;
	}
	
	o->ce->type = ZEND_USER_CLASS;
	o->ce->name = zend_string_copy(name);

	zend_initialize_class_data(o->ce, 1);

	if (!parent) {
		pce = zend_lookup_class(name);
	} else {
		pce = zend_lookup_class(parent);
	}

	if (pce) {
		zend_bool is_parent_final = 
			pce->ce_flags & ZEND_ACC_FINAL;

		if (zend_string_equals_ci(o->ce->name, pce->name)) {
			if (pce->type != ZEND_USER_CLASS) {
				zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0,
					"cannot redeclare internal class %s", ZSTR_VAL(pce->name));
				return;
			}

			if (pce->ce_flags & ZEND_ACC_INTERFACE) {
				zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0,
					"cannot redeclare interface %s", ZSTR_VAL(pce->name));
				return;
			}

			if (pce->ce_flags & ZEND_ACC_TRAIT) {
				zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0,
					"cannot redeclare trait %s", ZSTR_VAL(pce->name));
				return;
			}
		}

		pce->ce_flags &= ~ZEND_ACC_FINAL;

		zend_do_inheritance(o->ce, pce);

		if (is_parent_final) {
			pce->ce_flags |= ZEND_ACC_FINAL;
		}

		o->ce->ce_flags = pce->ce_flags;
	} else {
		o->ce->ce_flags |= ZEND_ACC_FINAL;
	}
	o->ce->ce_flags |= ZEND_ACC_USE_GUARDS;

	if (interfaces) {
		zval *interface = NULL;

		ZEND_HASH_FOREACH_VAL(interfaces, interface) {
			zend_class_entry *ce;

			if (Z_TYPE_P(interface) == IS_STRING) {
				ce = zend_lookup_class(Z_STR_P(interface));

				if (!ce) {
					zend_throw_exception_ex(spl_ce_RuntimeException, 0, 
						"could not find interface %s", 
						Z_STRVAL_P(interface));
					break;
				}

				if ((ce->ce_flags & ZEND_ACC_INTERFACE) != ZEND_ACC_INTERFACE) {
					zend_throw_exception_ex(spl_ce_RuntimeException, 0, 
						"%s is not an interface", 
						Z_STRVAL_P(interface));
					break;
				}

				zend_do_implement_interface(o->ce, ce);
			}
		} ZEND_HASH_FOREACH_END();

		o->ce->ce_flags &= ~ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;
	}
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_register, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, register)
{	
	php_componere_definition_t *o = 
		php_componere_definition_fetch(getThis());
	zend_string *name = zend_string_tolower(o->ce->name);

	if (o->registered) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0,
			"could not re-register %s", ZSTR_VAL(o->ce->name));
		zend_string_release(name);
		return;
	} 

	if (o->ce->parent && zend_string_equals_ci(o->ce->name, o->ce->parent->name)) {
		zend_string *name = NULL;
		zend_function *function = NULL;
		zend_property_info *info = NULL;

		ZEND_HASH_FOREACH_STR_KEY_PTR(&o->ce->function_table, name, function) {
			if (function->common.scope == o->ce->parent) {
				function->common.scope = o->ce;
			}
		} ZEND_HASH_FOREACH_END();

		ZEND_HASH_FOREACH_STR_KEY_PTR(&o->ce->properties_info, name, info) {
			if (info->ce == o->ce->parent) {
				info->ce = o->ce;
			}
		} ZEND_HASH_FOREACH_END();

		o->saved = o->ce->parent;
		o->saved->refcount++;

		o->ce->parent = NULL;
	}

	if (!zend_hash_update_ptr(CG(class_table), name, o->ce)) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0,
			"could not register %s", ZSTR_VAL(o->ce->name));
	} else {
		o->registered = 1;
	}

	zend_string_release(name);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_method, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_OBJ_INFO(0, method, Componere\\Method, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, addMethod)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zval *method = NULL;
	zend_function *function;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "SO", &name, &method, php_componere_method_ce) != SUCCESS) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "name and method expected");
		return;	
	}

	if (o->registered) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, 
			"%s is already registered, cannot add method %s", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	function = php_componere_method_function(method);

	if (function->common.scope) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, 
			"method was already added to %s", 
			ZSTR_VAL(function->common.scope->name));
		return;
	}

	function->common.scope = o->ce;
	
	name = zend_string_tolower(name);

	if (!zend_hash_update_ptr(&o->ce->function_table, name, function)) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "failed to add method");
	} else {
		function_add_ref(function);
	}

	if (zend_string_equals_literal(name, "__construct")) {
		o->ce->constructor = function;
	} else if (zend_string_equals_literal(name, "__destruct")) {
		o->ce->destructor = function;
	} else if (zend_string_equals_literal(name, "clone")) {
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

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "C", &trait) != SUCCESS) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "trait expected");
		return;
	}

	if (o->registered) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, 
			"%s is already registered, cannot add trait %s", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(trait->name));
		return;
	}

	zend_do_implement_trait(o->ce, trait);
	zend_do_bind_traits(o->ce);

	RETURN_ZVAL(getThis(), 1, 0);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_property, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_OBJ_INFO(0, property, Componere\\Value, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, addProperty)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zval *value;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "SO", &name, &value, php_componere_value_ce) != SUCCESS) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "name and property expected");
		return;
	}

	if (zend_get_property_info(o->ce, name, 1)) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0,
			"cannot redeclare %s::%s",
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (o->registered) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, 
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
	ZEND_ARG_OBJ_INFO(0, property, Componere\\Value, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, addConstant)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zval *value;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "SO", &name, &value, php_componere_value_ce) != SUCCESS) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "name and property expected");
		return;
	}

	if (zend_get_property_info(o->ce, name, 1)) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0,
			"cannot redeclare %s::%s",
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

	if (o->registered) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, 
			"%s is already registered, cannot add property %s", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
		return;
	}

#if PHP_VERSION_ID >= 70100
	zend_declare_class_constant_ex(
		o->ce, name, 
		php_componere_value_default(value),
		php_componere_value_access(value), NULL);
#else
	zend_declare_class_constant(o->ce, name, 
		php_componere_value_default(value), NULL);
#endif

	RETURN_ZVAL(getThis(), 1, 0);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_closure, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, scope)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, getClosure)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zend_string *key = NULL;
	zend_function *function = NULL;
	zval *scope = NULL;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "S|z", &name, &scope) != SUCCESS) {
		return;
	}

	if (o->registered) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0,
			"cannot get closure after registration");
		return;
	}

	key = zend_string_tolower(name);
	function = zend_hash_find_ptr(&o->ce->function_table, key);

	if (!function) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0,
			"could not find %s::%s", ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
	} else {
		zend_create_closure(return_value, function, o->ce, o->ce, scope);
	}
	zend_string_release(key);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_closures, 0, 0, 0)
	ZEND_ARG_INFO(0, scope)
ZEND_END_ARG_INFO()

PHP_METHOD(Definition, getClosures)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zend_function *function = NULL;
	zval *scope = NULL;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "|z", &scope) != SUCCESS) {
		return;
	}

	if (o->registered) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0,
			"cannot get closures after registration");
		return;
	}

	array_init(return_value);

	ZEND_HASH_FOREACH_STR_KEY_PTR(&o->ce->function_table, name, function) {
		zval closure;

		zend_create_closure(&closure, function, o->ce, o->ce, scope);		

		zend_hash_add(
			Z_ARRVAL_P(return_value), name, &closure);
	} ZEND_HASH_FOREACH_END();
}

static zend_function_entry php_componere_definition_methods[] = {
	PHP_ME(Definition, __construct, php_componere_definition_construct, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addMethod, php_componere_definition_method, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addTrait, php_componere_definition_trait, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addProperty, php_componere_definition_property, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, addConstant, php_componere_definition_constant, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, register, php_componere_definition_register, ZEND_ACC_PUBLIC)

	PHP_ME(Definition, getClosure, php_componere_definition_closure, ZEND_ACC_PUBLIC)
	PHP_ME(Definition, getClosures, php_componere_definition_closures, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_MINIT_FUNCTION(Componere_Definition) {
	zend_class_entry ce;
	
	INIT_NS_CLASS_ENTRY(ce, "Componere", "Definition", php_componere_definition_methods);

	php_componere_definition_ce = zend_register_internal_class(&ce);
	php_componere_definition_ce->create_object = php_componere_definition_create;

	memcpy(&php_componere_definition_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_componere_definition_handlers.offset = XtOffsetOf(php_componere_definition_t, std);
	php_componere_definition_handlers.free_obj = php_componere_definition_destroy;

}

#endif
