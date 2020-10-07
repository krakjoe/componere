/*
  +----------------------------------------------------------------------+
  | componere                                                            |
  +----------------------------------------------------------------------+
  | Copyright (c) Joe Watkins 2018-2020                                  |
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
#ifndef HAVE_COMPONERE_PATCH
#define HAVE_COMPONERE_PATCH

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <php.h>

#include <ext/spl/spl_exceptions.h>
#include <zend_closures.h>
#include <zend_exceptions.h>
#include <zend_inheritance.h>

#include <src/common.h>
#include <src/definition.h>

#if PHP_VERSION_ID < 80000
#include "patch_legacy_arginfo.h"
#else
#include "patch_arginfo.h"
#endif

zend_class_entry *php_componere_patch_ce;
zend_object_handlers php_componere_patch_handlers;

static inline zend_object* php_componere_patch_create(zend_class_entry *ce) {
	php_componere_definition_t *o = 
		(php_componere_definition_t*) 
			ecalloc(1, sizeof(php_componere_definition_t));

	zend_object_std_init(&o->std, ce);

	o->std.handlers = &php_componere_patch_handlers;

	return &o->std;
}

static inline void php_componere_patch_destroy(zend_object *zo) {
	php_componere_definition_t *o = php_componere_definition_from(zo);

	if (!Z_ISUNDEF(o->instance)) {
		if (o->ce) {
			php_componere_destroy_class(o->ce);		
		}

		Z_OBJCE(o->instance) = o->saved;

		php_componere_destroy_class(o->saved);

		zval_ptr_dtor(&o->instance);
	}

	if (!Z_ISUNDEF(o->reflector)) {
		zval_ptr_dtor(&o->reflector);
	}

	zend_object_std_dtor(&o->std);
}

PHP_METHOD(Componere_Patch, __construct)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_class_entry *pce = NULL;
	zval *pd = NULL;
	HashTable *interfaces = NULL;

	switch (ZEND_NUM_ARGS()) {
		case 2: if (php_componere_parse_parameters("oH", &pd, &interfaces) != SUCCESS) {
			php_componere_wrong_parameters("instance and interfaces expected");
			return;
		} break;

		case 1: if (php_componere_parse_parameters("o", &pd) != SUCCESS) {
			php_componere_wrong_parameters("instance expected as single argument");
			return;
		} break;

		default:
			php_componere_wrong_parameters("instance, or instance and interfaces expected");
			return;
	}

	pce = Z_OBJCE_P(pd);

	o->ce = (zend_class_entry*) 
		zend_arena_alloc(&CG(arena), sizeof(zend_class_entry));

	o->ce->type = ZEND_USER_CLASS;
	o->ce->name = zend_string_copy(pce->name);

	zend_initialize_class_data(o->ce, 1);

	if (pce && pce->type == ZEND_USER_CLASS) {
		memcpy(&o->ce->info.user, &pce->info.user, sizeof(pce->info.user));

		if (pce->info.user.doc_comment) {
			o->ce->info.user.doc_comment = zend_string_copy(pce->info.user.doc_comment);
		}

		if (o->ce->info.user.filename) {
			zend_string_addref(o->ce->info.user.filename);
		}
	} else {
		o->ce->info.user.filename = zend_get_executed_filename_ex();
		
		if (o->ce->info.user.filename) {
			zend_string_addref(o->ce->info.user.filename);
		} else {
			o->ce->info.user.filename = zend_string_init(ZEND_STRL("unknown file"), 0);
		}
		
		o->ce->info.user.line_start = zend_get_executed_lineno();
	}

	if (pce->type == ZEND_USER_CLASS) {
		php_componere_definition_copy(o->ce, pce);
	} else {
		php_componere_definition_inherit(o->ce, pce);
	}

	o->ce->ce_flags |= ZEND_ACC_USE_GUARDS;
	o->saved = pce;
	o->saved->refcount++;
#if PHP_VERSION_ID >= 70400
    o->ce->ce_flags |= ZEND_ACC_LINKED;
#endif

	ZVAL_COPY(&o->instance, pd);

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

#if PHP_VERSION_ID >= 70400
    o->ce->ce_flags |= ZEND_ACC_RESOLVED_INTERFACES;
#endif
}

PHP_METHOD(Componere_Patch, apply)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_object *zo;

	php_componere_no_parameters();

	zo = Z_OBJ(o->instance);
	zo->ce = o->ce;
}

PHP_METHOD(Componere_Patch, revert)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_object *zo;

	php_componere_no_parameters();

	zo = Z_OBJ(o->instance);
	zo->ce = o->saved;
}

PHP_METHOD(Componere_Patch, getClosure)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_string *name = NULL;
	zend_string *key = NULL;
	zend_function *function = NULL;

	if (php_componere_parse_parameters("S", &name) != SUCCESS) {
		php_componere_wrong_parameters("name expected");		
		return;
	}

	key = zend_string_tolower(name);
	function = zend_hash_find_ptr(&o->ce->function_table, key);

	if (!function) {
		php_componere_throw(
			"could not find %s::%s", 
			ZSTR_VAL(o->ce->name), ZSTR_VAL(name));
	} else {
		zend_create_closure(return_value, function, o->ce, o->ce, &o->instance);
	}
	zend_string_release(key);
}

PHP_METHOD(Componere_Patch, getClosures)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_function *function = NULL;

	php_componere_no_parameters();

	array_init(return_value);

	ZEND_HASH_FOREACH_PTR(&o->ce->function_table, function) {
		zval closure;

		zend_create_closure(&closure, function, o->ce, o->ce, &o->instance);		

		zend_hash_add(
			Z_ARRVAL_P(return_value), function->common.function_name, &closure);
	} ZEND_HASH_FOREACH_END();
}

PHP_METHOD(Componere_Patch, isApplied)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());

	php_componere_no_parameters();

	RETURN_BOOL(Z_OBJCE(o->instance) == o->ce);
}

PHP_METHOD(Componere_Patch, derive)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	php_componere_definition_t *r;
	zval *instance;

	if (php_componere_parse_parameters("o", &instance) != SUCCESS) {
		php_componere_wrong_parameters("object expected");
		return;
	}

	if (!instanceof_function(Z_OBJCE_P(instance), o->saved)) {
		php_componere_throw_ex(InvalidArgumentException, 
			"%s is not compatible with %s",
			ZSTR_VAL(o->saved->name),
			ZSTR_VAL(Z_OBJCE_P(instance)->name));
		return;
	}

	object_init_ex(return_value, php_componere_patch_ce);

	r = php_componere_definition_fetch(return_value);

	r->ce = (zend_class_entry*) 
			zend_arena_alloc(&CG(arena), sizeof(zend_class_entry));

	r->ce->type = ZEND_USER_CLASS;
	r->ce->name = zend_string_copy(o->ce->name);
	
	zend_initialize_class_data(r->ce, 1);

	if (o->ce && o->ce->type == ZEND_USER_CLASS) {
		memcpy(&r->ce->info.user, &o->ce->info.user, sizeof(o->ce->info.user));

		if (o->ce->info.user.doc_comment) {
			r->ce->info.user.doc_comment = zend_string_copy(o->ce->info.user.doc_comment);
		}

		if (r->ce->info.user.filename) {
			zend_string_addref(r->ce->info.user.filename);
		}
	} else {
		r->ce->info.user.filename = zend_get_executed_filename_ex();
		
		if (r->ce->info.user.filename) {
			zend_string_addref(o->ce->info.user.filename);
		} else {
			r->ce->info.user.filename = zend_string_init(ZEND_STRL("unknown file"), 0);
		}
		
		r->ce->info.user.line_start = zend_get_executed_lineno();
	}

	php_componere_definition_copy(r->ce, o->ce);
	php_componere_definition_parent(r->ce, o->ce);

#if PHP_VERSION_ID >= 70400
    r->ce->ce_flags |= ZEND_ACC_LINKED;
#endif

	r->saved = Z_OBJCE_P(instance);
	r->saved->refcount++;

	ZVAL_COPY(&r->instance, instance);

#if PHP_VERSION_ID >= 70400
    r->ce->ce_flags |= ZEND_ACC_RESOLVED_INTERFACES;
#endif

#if PHP_VERSION_ID >= 70400
    php_componere_definition_properties_table_rebuild(r->ce);
#endif
}

PHP_MINIT_FUNCTION(Componere_Patch) {
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "Componere", "Patch", class_Componere_Patch_methods);
	
	php_componere_patch_ce = zend_register_internal_class_ex(&ce, php_componere_definition_abstract_ce);
	php_componere_patch_ce->create_object = php_componere_patch_create;

	php_componere_setup_handlers(
		&php_componere_patch_handlers, 
		php_componere_deny_debug,
		php_componere_deny_collect,
		php_componere_deny_clone,
		php_componere_patch_destroy, 
		XtOffsetOf(php_componere_definition_t, std));

	return SUCCESS;
}

PHP_RINIT_FUNCTION(Componere_Patch) {
	php_componere_patch_ce->ce_flags |= ZEND_ACC_FINAL;

	return SUCCESS;
}

#endif
