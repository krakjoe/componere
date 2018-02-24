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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "ext/spl/spl_exceptions.h"
#include "zend_exceptions.h"
#include "zend_inheritance.h"
#include "php_componere.h"

static inline void componere_magic(zend_class_entry *composed, zend_string *fname, zend_function *handler) {
	if (EG(exception)) {
		return;
	}

	if (zend_string_equals_literal(fname, "__construct")) {
		composed->constructor = handler;
	} else if (zend_string_equals_literal(fname, "__destruct")) {
		composed->destructor = handler;
	} else if (zend_string_equals_literal(fname, "clone")) {
		composed->clone = handler;
	} else if (zend_string_equals_literal(fname, "__get")) {
		composed->__get = handler;
	} else if (zend_string_equals_literal(fname, "__set")) {
		composed->__set = handler;
	} else if (zend_string_equals_literal(fname, "__unset")) {
		composed->__unset = handler;
	} else if (zend_string_equals_literal(fname, "__call")) {
		composed->__call = handler;
	} else if (zend_string_equals_literal(fname, "__callstatic")) {
		composed->__callstatic = handler;
	} else if (zend_string_equals_literal(fname, "__tostring")) {
		composed->__tostring = handler;
	} else if (zend_string_equals_literal(fname, "__debuginfo")) {
		composed->__debugInfo = handler;
	}
}

static inline zend_string* componere_inheritance(zend_class_entry *composed, zend_class_entry *parent) {
	zend_string *name = NULL;
	
	if (parent) {
		zend_bool is_parent_final = 
			parent->ce_flags & ZEND_ACC_FINAL;

		parent->ce_flags &= ~ZEND_ACC_FINAL;

		if (zend_string_equals_ci(composed->name, parent->name)) {
			name = zend_string_copy(parent->name);
		} else {
			name = zend_string_copy(composed->name);
		}

		zend_do_inheritance(composed, parent);

		if (is_parent_final) {
			parent->ce_flags |= ZEND_ACC_FINAL;
		}

		composed->ce_flags = parent->ce_flags;
	} else {
		composed->ce_flags |= ZEND_ACC_FINAL;
		
		name = zend_string_copy(composed->name);
	}
	composed->ce_flags |= ZEND_ACC_USE_GUARDS;

	return name;
}

static inline zend_class_entry* componere_initialize(zend_string *name) {
	zend_class_entry *composed = (zend_class_entry*) 
		zend_arena_alloc(&CG(arena), sizeof(zend_class_entry));

	composed->type = ZEND_USER_CLASS;
	composed->name = zend_string_copy(name);

	zend_initialize_class_data(composed, 1);

	return composed;
}

static inline void componere_register_class(zend_class_entry *composed, zend_string *name) {
	zend_string *key = zend_string_tolower(name);

	if (!zend_hash_update_ptr(CG(class_table), key, composed)) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, "could not register %s", ZSTR_VAL(name));
	}

	zend_string_release(key);
}

static inline void componere_register_function(zend_class_entry *composed, zend_string *name, zval *function) {
	zend_string *fname = zend_string_tolower(name);
	zend_function *closure = 
		(zend_function*) 
			(((char*)Z_OBJ_P(function)) + sizeof(zend_object));
	zend_function *handler = zend_arena_alloc(&CG(arena), sizeof(zend_op_array));

	memcpy(handler, closure, sizeof(zend_op_array));

	handler->common.scope = composed;
	handler->common.fn_flags &= ~ ZEND_ACC_CLOSURE;

	if (!zend_hash_update_ptr(
		&composed->function_table,
		fname,
		handler)) {
		zend_throw_exception_ex(spl_ce_RuntimeException, 0, 
			"could not register %s::%s", 
			ZSTR_VAL(composed->name), ZSTR_VAL(name));
	}

	zend_string_release(fname);

	componere_magic(composed, fname, handler);

	function_add_ref(handler);
}

static inline void componere_register_functions(zend_class_entry *composed, HashTable *functions) {
	zend_string *name = NULL;
	zval *function = NULL;

	if (!functions) {
		return;
	}

	ZEND_HASH_FOREACH_STR_KEY_VAL(functions, name, function) {
		componere_register_function(composed, name, function);

		if (EG(exception)) {
			break;
		}
	} ZEND_HASH_FOREACH_END();
}

static inline void componere_register_properties(zend_class_entry *composed, HashTable *properties) {
	zend_string *property;
	zval *value;

	if (EG(exception) || !properties) {
		return;
	}

	ZEND_HASH_FOREACH_STR_KEY_VAL(properties, property, value) {
		if (zend_get_property_info(composed, property, 1)) {
			zend_throw_exception_ex(spl_ce_RuntimeException, 0,
				"cannot redeclare %s::%s",
				ZSTR_VAL(composed->name), ZSTR_VAL(property));
			return;
		}

		switch (Z_TYPE_P(value)) {
			case IS_STRING:
			case IS_LONG:
			case IS_DOUBLE:
			case IS_TRUE:
			case IS_FALSE:
			case IS_NULL:
				zend_declare_property(composed, 
					ZSTR_VAL(property), ZSTR_LEN(property), 
					value, ZEND_ACC_PUBLIC);
			break;
			
			default:
				zend_throw_exception_ex(spl_ce_RuntimeException, 0,
					"%s::%s cannot be declared as %s",
					ZSTR_VAL(composed->name), ZSTR_VAL(property),
					Z_TYPE_P(value) == IS_OBJECT ?
						ZSTR_VAL(Z_OBJCE_P(value)->name) :
						zend_get_type_by_const(Z_TYPE_P(value)));
		}
	} ZEND_HASH_FOREACH_END();
}

static inline void componere_scope_functions(zend_class_entry *composed, zend_class_entry *parent) {
	if (parent && zend_string_equals_ci(composed->name, parent->name)) {
		zend_string *name = NULL;
		zend_function *function = NULL;

		ZEND_HASH_FOREACH_STR_KEY_PTR(&composed->function_table, name, function) {
			if (function->common.scope == parent) {
				function->common.scope = composed;
			}
		} ZEND_HASH_FOREACH_END();

		composed->parent = NULL;
	}
}

/* {{{ proto bool compose(string name, array functions, [class parent, [array properties]]) */
PHP_FUNCTION(compose)
{
	zend_string		*name = NULL;
	HashTable		*functions = NULL;
	zend_class_entry	*parent = NULL;
	HashTable               *properties = NULL;
	zend_class_entry	*composed;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SH|CH", &name, &functions, &parent, &properties) != SUCCESS) {
		return;
	}

	composed = componere_initialize(name);
	name = componere_inheritance(composed, parent);
	componere_register_class(composed, name);
	componere_register_functions(composed, functions);
	componere_register_properties(composed, properties);
	componere_scope_functions(composed, parent);

	zend_string_release(name);

	RETURN_TRUE;
} /* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(componere)
{
#if defined(ZTS) && defined(COMPILE_DL_COMPONERE)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(componere)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "componere support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(compose_arginfo, 0)
ZEND_END_ARG_INFO()


/* {{{ componere_functions[]
 */
static const zend_function_entry componere_functions[] = {
	PHP_FE(compose, compose_arginfo)
	PHP_FE_END
};
/* }}} */

/* {{{ componere_module_entry
 */
zend_module_entry componere_module_entry = {
	STANDARD_MODULE_HEADER,
	"componere",
	componere_functions,
	NULL,
	NULL,
	PHP_RINIT(componere),
	NULL,
	PHP_MINFO(componere),
	PHP_COMPONERE_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_COMPONERE
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(componere)
#endif
