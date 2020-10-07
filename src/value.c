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
#ifndef HAVE_COMPONERE_VALUE
#define HAVE_COMPONERE_VALUE

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <php.h>
#include <zend_closures.h>
#include <zend_exceptions.h>
#include <ext/spl/spl_exceptions.h>

#include <src/common.h>
#include <src/value.h>

#if PHP_VERSION_ID < 80000
#include "value_legacy_arginfo.h"
#else
#include "value_arginfo.h"
#endif

zend_class_entry *php_componere_value_ce;
zend_object_handlers php_componere_value_handlers;

static inline zend_object* php_componere_value_create(zend_class_entry *ce) {
	php_componere_value_t *o = 
		(php_componere_value_t*) 
			ecalloc(1, sizeof(php_componere_value_t));

	zend_object_std_init(&o->std, ce);

	o->std.handlers = &php_componere_value_handlers;

	return &o->std;
}

static inline void php_componere_value_destroy(zend_object *zo) {
	php_componere_value_t *o = php_componere_value_from(zo);

	if (!Z_ISUNDEF(o->value) && Z_REFCOUNTED(o->value)) {
		zval_ptr_dtor(&o->value);
	}

	zend_object_std_dtor(&o->std);
}

PHP_METHOD(Value, __construct)
{
	php_componere_value_t *o = php_componere_value_fetch(getThis());
	zval *value = NULL;

	if (php_componere_parse_parameters("|z", &value) != SUCCESS) {
		php_componere_wrong_parameters("value expected");
		return;
	}

	o->access = ZEND_ACC_PUBLIC;

	if (!value) {
		return;
	}

	switch (Z_TYPE_P(value)) {
		case IS_STRING:
		case IS_LONG:
		case IS_DOUBLE:
		case IS_TRUE:
		case IS_FALSE:
		case IS_NULL:
			ZVAL_DUP(&o->value, value);
		break;

		case IS_ARRAY:
			if (zend_hash_num_elements(Z_ARRVAL_P(value)) == 0) {
				ZVAL_DUP(&o->value, value);
				break;
			}
		
		default:
			php_componere_wrong_parameters(
				"values of type %s cannot be declared with default values",
				zend_get_type_by_const(Z_TYPE_P(value)));
			return;
	}
}

PHP_METHOD(Value, setProtected)
{
	php_componere_value_t *o = php_componere_value_fetch(getThis());

	php_componere_no_parameters();

	if ((o->access & ZEND_ACC_PPP_MASK) > ZEND_ACC_PUBLIC) {
		php_componere_throw_ex(RuntimeException, "access level already set");
		return;
	}

	o->access |= ZEND_ACC_PROTECTED;

	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(Value, setPrivate)
{
	php_componere_value_t *o = php_componere_value_fetch(getThis());

	php_componere_no_parameters();

	if ((o->access & ZEND_ACC_PPP_MASK) > ZEND_ACC_PUBLIC) {
		php_componere_throw_ex(RuntimeException, "access level already set");
		return;
	}

	o->access |= ZEND_ACC_PRIVATE;

	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(Value, setStatic)
{
	php_componere_value_t *o = php_componere_value_fetch(getThis());

	php_componere_no_parameters();

	o->access |= ZEND_ACC_STATIC;

	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(Value, isPrivate)
{
	php_componere_value_t *o = php_componere_value_fetch(getThis());

	php_componere_no_parameters();

	RETURN_BOOL((o->access & ZEND_ACC_PRIVATE) == ZEND_ACC_PRIVATE);
}

PHP_METHOD(Value, isProtected)
{
	php_componere_value_t *o = php_componere_value_fetch(getThis());

	php_componere_no_parameters();

	RETURN_BOOL((o->access & ZEND_ACC_PROTECTED) == ZEND_ACC_PROTECTED);
}

PHP_METHOD(Value, isStatic)
{
	php_componere_value_t *o = php_componere_value_fetch(getThis());

	php_componere_no_parameters();

	RETURN_BOOL((o->access & ZEND_ACC_STATIC) == ZEND_ACC_STATIC);
}

PHP_METHOD(Value, hasDefault)
{
	php_componere_value_t *o = php_componere_value_fetch(getThis());

	php_componere_no_parameters();

	RETURN_BOOL(!Z_ISUNDEF(o->value));
}

PHP_MINIT_FUNCTION(Componere_Value) {
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "Componere", "Value", class_Value_methods);

	php_componere_value_ce = zend_register_internal_class(&ce);
	php_componere_value_ce->create_object = php_componere_value_create;

	php_componere_setup_handlers(
		&php_componere_value_handlers, 
		php_componere_deny_debug,
		php_componere_deny_collect,
		php_componere_deny_clone,
		php_componere_value_destroy, 
		XtOffsetOf(php_componere_value_t, std));

	return SUCCESS;
}

PHP_RINIT_FUNCTION(Componere_Value) {
	php_componere_value_ce->ce_flags |= ZEND_ACC_FINAL;

	return SUCCESS;
}

#endif
