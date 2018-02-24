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
#ifndef HAVE_COMPONERE_METHOD
#define HAVE_COMPONERE_METHOD

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <php.h>
#include <zend_closures.h>
#include <zend_exceptions.h>
#include <ext/spl/spl_exceptions.h>

#include "src/method.h"

zend_class_entry *php_componere_method_ce;
zend_object_handlers php_componere_method_handlers;

static inline zend_object* php_componere_method_create(zend_class_entry *ce) {
	php_componere_method_t *o = 
		(php_componere_method_t*) 
			ecalloc(1, sizeof(php_componere_method_t));

	zend_object_std_init(&o->std, ce);

	object_properties_init(&o->std, ce);

	o->std.handlers = &php_componere_method_handlers;

	return &o->std;
}

static inline void php_componere_method_destroy(zend_object *zo) {
	php_componere_method_t *o = php_componere_method_from(zo);

	destroy_zend_function(o->function);

	zend_object_std_dtor(&o->std);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_method_construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, closure, Closure, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Method, __construct)
{
	php_componere_method_t *o = php_componere_method_fetch(getThis());
	zval *closure = NULL;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "O", &closure, zend_ce_closure) != SUCCESS) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "closure expected");
		return;
	}

	o->function = zend_arena_alloc(&CG(arena), sizeof(zend_op_array));

	memcpy(o->function, 
		(zend_function*) (((char*)Z_OBJ_P(closure)) + sizeof(zend_object)), 
		sizeof(zend_op_array));

	o->function->common.scope = NULL;
	o->function->common.fn_flags &= ~ ZEND_ACC_CLOSURE;

	function_add_ref(o->function);
}

ZEND_BEGIN_ARG_INFO(php_componere_method_protected, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Method, setProtected)
{
	php_componere_method_t *o = php_componere_method_fetch(getThis());

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "") != SUCCESS) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "no arguments expected");
		return;
	}

	if (o->function->common.fn_flags & ZEND_ACC_PPP_MASK) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0,
			"access level already set");
		return;
	}

	o->function->common.fn_flags |= ZEND_ACC_PROTECTED;

	RETURN_ZVAL(getThis(), 1, 0);
}

ZEND_BEGIN_ARG_INFO(php_componere_method_private, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Method, setPrivate)
{
	php_componere_method_t *o = php_componere_method_fetch(getThis());

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "") != SUCCESS) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "no arguments expected");
		return;
	}

	if (o->function->common.fn_flags & ZEND_ACC_PPP_MASK) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0,
			"access level already set");
		return;
	}

	o->function->common.fn_flags |= ZEND_ACC_PRIVATE;

	RETURN_ZVAL(getThis(), 1, 0);
}

static zend_function_entry php_componere_method_methods[] = {
	PHP_ME(Method, __construct, php_componere_method_construct, ZEND_ACC_PUBLIC)
	PHP_ME(Method, setProtected, php_componere_method_protected, ZEND_ACC_PUBLIC)
	PHP_ME(Method, setPrivate, php_componere_method_private, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_MINIT_FUNCTION(Componere_Method) {
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "Componere", "Method", php_componere_method_methods);

	php_componere_method_ce = zend_register_internal_class(&ce);
	php_componere_method_ce->create_object = php_componere_method_create;

	memcpy(&php_componere_method_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_componere_method_handlers.offset = XtOffsetOf(php_componere_method_t, std);
	php_componere_method_handlers.free_obj = php_componere_method_destroy;

	return SUCCESS;
}

#endif
