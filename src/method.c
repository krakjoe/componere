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
#ifndef HAVE_COMPONERE_METHOD
#define HAVE_COMPONERE_METHOD

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <php.h>
#include <zend_closures.h>
#include <zend_exceptions.h>
#include <ext/spl/spl_exceptions.h>

#include <src/common.h>
#include <src/reflection.h>
#include <src/method.h>

#if PHP_VERSION_ID < 80000
#include "method_legacy_arginfo.h"
#else
#include "method_arginfo.h"
#endif

zend_class_entry *php_componere_method_ce;
zend_object_handlers php_componere_method_handlers;

extern zend_string* php_componere_name_function;

#define ZEND_ACC_CHECK(f, a) (((f)->common.fn_flags & (a)))

static inline zend_object* php_componere_method_create(zend_class_entry *ce) {
	php_componere_method_t *o = 
		(php_componere_method_t*) 
			ecalloc(1, sizeof(php_componere_method_t));

	zend_object_std_init(&o->std, ce);

	o->std.handlers = &php_componere_method_handlers;

	return &o->std;
}

#if PHP_VERSION_ID >= 80000
static inline zend_object* php_componere_method_clone(zend_object *object) {
#else
static inline zend_object* php_componere_method_clone(zval *zo) {
    zend_object *object = Z_OBJ_P(zo);
#endif
	php_componere_method_t *o = 
		(php_componere_method_t*)
			ecalloc(1, sizeof(php_componere_method_t));

	zend_object_std_init(&o->std, object->ce);

	o->function = (zend_function*) 
		zend_arena_alloc(&CG(arena), sizeof(zend_op_array));

	memcpy(o->function, 
		php_componere_method_from(object)->function, sizeof(zend_op_array));

	o->function->common.scope = NULL;
	o->function->common.function_name = zend_string_copy(php_componere_name_function);

	function_add_ref(o->function);

	o->std.handlers = &php_componere_method_handlers;
	
	return &o->std;
}

static inline void php_componere_method_destroy(zend_object *zo) {
	php_componere_method_t *o = php_componere_method_from(zo);

	if (o->function) {
		destroy_zend_function(o->function);
	}

	if (!Z_ISUNDEF(o->reflector)) {
		zval_ptr_dtor(&o->reflector);
	}

	zend_object_std_dtor(&o->std);
}

PHP_METHOD(Componere_Method, __construct)
{
	php_componere_method_t *o = php_componere_method_fetch(getThis());
	zval *closure = NULL;
	uint32_t flags = 0;

	if (php_componere_parse_parameters("O", &closure, zend_ce_closure) != SUCCESS) {
		php_componere_wrong_parameters("closure expected");
		return;
	}

	o->function = zend_arena_alloc(&CG(arena), sizeof(zend_op_array));

	memcpy(o->function, 
		(zend_function*) (((char*)Z_OBJ_P(closure)) + sizeof(zend_object)), 
		sizeof(zend_op_array));

	o->function->op_array.function_name = zend_string_copy(php_componere_name_function);
	o->function->op_array.refcount = NULL;
	o->function->op_array.scope = NULL;
	o->function->op_array.prototype = NULL;

	flags = 
		(o->function->op_array.fn_flags & ZEND_ACC_STATIC) ?
			(ZEND_ACC_STATIC|ZEND_ACC_PUBLIC) :
			(ZEND_ACC_PUBLIC);
	if (o->function->op_array.fn_flags & ZEND_ACC_VARIADIC) {
		flags |= ZEND_ACC_VARIADIC;
	}

	o->function->op_array.fn_flags = flags;
	
	function_add_ref(o->function);
}

PHP_METHOD(Componere_Method, setProtected)
{
	php_componere_method_t *o = php_componere_method_fetch(getThis());

	php_componere_no_parameters();

	if (ZEND_ACC_CHECK(o->function, ZEND_ACC_PROTECTED|ZEND_ACC_PRIVATE)) {
		php_componere_throw_ex(RuntimeException, "access level already set");
		return;
	}

	o->function->common.fn_flags |= ZEND_ACC_PROTECTED;

	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(Componere_Method, setPrivate)
{
	php_componere_method_t *o = php_componere_method_fetch(getThis());

	php_componere_no_parameters();

	if (ZEND_ACC_CHECK(o->function, ZEND_ACC_PRIVATE|ZEND_ACC_PROTECTED)) {
		php_componere_throw_ex(RuntimeException, "access level already set");
		return;
	}

	o->function->common.fn_flags |= ZEND_ACC_PRIVATE;

	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(Componere_Method, setStatic)
{
	php_componere_method_t *o = php_componere_method_fetch(getThis());

	php_componere_no_parameters();

	o->function->common.fn_flags |= ZEND_ACC_STATIC;

	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(Componere_Method, setFinal)
{
	php_componere_method_t *o = php_componere_method_fetch(getThis());

	php_componere_no_parameters();

	o->function->common.fn_flags |= ZEND_ACC_FINAL;

	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(Componere_Method, getReflector)
{
	php_componere_method_t *o = php_componere_method_fetch(getThis());

	php_componere_no_parameters();

	if (!Z_ISUNDEF(o->reflector)) {
		RETURN_ZVAL(&o->reflector, 1, 0);
	}

	php_componere_reflection_object_factory(
		&o->reflector,
		php_componere_reflection_method_ce, 
		PHP_REF_TYPE_FUNCTION,
		o->function,
		o->function->common.function_name);

	RETURN_ZVAL(&o->reflector, 1 , 0);
}

PHP_MINIT_FUNCTION(Componere_Method) {
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "Componere", "Method", class_Componere_Method_methods);

	php_componere_method_ce = zend_register_internal_class(&ce);
	php_componere_method_ce->create_object = php_componere_method_create;

	php_componere_setup_handlers(
		&php_componere_method_handlers,
		php_componere_deny_debug,
		php_componere_deny_collect,
		php_componere_method_clone, 
		php_componere_method_destroy, 
		XtOffsetOf(php_componere_method_t, std));

	return SUCCESS;
}

PHP_RINIT_FUNCTION(Componere_Method) {
	php_componere_method_ce->ce_flags |= ZEND_ACC_FINAL;

	return SUCCESS;
}

#endif
