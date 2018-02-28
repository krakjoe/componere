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

#include "src/definition.h"

zend_class_entry *php_componere_definition_patch_ce;
zend_object_handlers php_componere_definition_patch_handlers;

static inline zend_object* php_componere_definition_patch_create(zend_class_entry *ce) {
	php_componere_definition_t *o = 
		(php_componere_definition_t*) 
			ecalloc(1, sizeof(php_componere_definition_t));

	zend_object_std_init(&o->std, ce);

	object_properties_init(&o->std, ce);

	o->ce = (zend_class_entry*) 
		zend_arena_alloc(&CG(arena), sizeof(zend_class_entry));

	o->std.handlers = &php_componere_definition_patch_handlers;

	return &o->std;
}

static inline void php_componere_definition_patch_destroy(zend_object *zo) {
	php_componere_definition_t *o = php_componere_definition_from(zo);
	zval tmp;

	if (!Z_ISUNDEF(o->instance)) {
		ZVAL_PTR(&tmp, o->ce);

		destroy_zend_class(&tmp);

		Z_OBJCE(o->instance) = o->saved;

		zval_ptr_dtor(&o->instance);
	}

	zend_object_std_dtor(&o->std);
}

PHP_METHOD(Patch, __construct)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_class_entry *pce = NULL;
	zval *pd = NULL;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "o", &pd) != SUCCESS) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "name or instance expected as single argument");
		return;
	}

	if (Z_OBJCE_P(pd)->type != ZEND_USER_CLASS) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "cannot patch internal objects");
		return;
	}

	pce = Z_OBJCE_P(pd);

	o->ce->type = ZEND_USER_CLASS;
	o->ce->name = zend_string_copy(pce->name);

	zend_initialize_class_data(o->ce, 1);

	php_componere_definition_copy(o->ce, pce);
	php_componere_definition_parent(o->ce, pce);

	o->ce->ce_flags |= ZEND_ACC_USE_GUARDS;
	o->saved = pce;

	ZVAL_COPY(&o->instance, pd);
}

ZEND_BEGIN_ARG_INFO_EX(php_componere_definition_patch_no_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Patch, apply)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_object *zo;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "") != SUCCESS) {
		return;
	}

	zo = Z_OBJ(o->instance);
	zo->ce = o->ce;
}

PHP_METHOD(Patch, revert)
{
	php_componere_definition_t *o = php_componere_definition_fetch(getThis());
	zend_object *zo;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "") != SUCCESS) {
		return;
	}

	zo = Z_OBJ(o->instance);
	zo->ce = o->saved;
}

static zend_function_entry php_componere_definition_patch_methods[] = {
	PHP_ME(Patch, __construct, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Patch, apply, php_componere_definition_patch_no_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Patch, revert, php_componere_definition_patch_no_arginfo, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_MINIT_FUNCTION(Componere_Patch) {
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "Componere", "Patch", php_componere_definition_patch_methods);
	
	php_componere_definition_patch_ce = zend_register_internal_class_ex(&ce, php_componere_definition_abstract_ce);
	php_componere_definition_patch_ce->create_object = php_componere_definition_patch_create;

	memcpy(&php_componere_definition_patch_handlers, &php_componere_definition_handlers, sizeof(zend_object_handlers));

	php_componere_definition_patch_handlers.free_obj = php_componere_definition_patch_destroy;

	return SUCCESS;
}

#endif
