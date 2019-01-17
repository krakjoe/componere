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

#include <php.h>
#include <ext/standard/info.h>

#include "php_componere.h"

#include <src/common.h>
#include <src/cast.h>
#include <src/definition.h>
#include <src/patch.h>
#include <src/method.h>
#include <src/reflection.h>
#include <src/value.h>

zend_string *php_componere_name_function;

static inline void php_componere_optimizer_adjust()
{
	zend_string *key = zend_string_init(ZEND_STRL("opcache.optimization_level"), 0);
	zend_string *value;
	zend_long optimizer = zend_ini_long(ZSTR_VAL(key), ZSTR_LEN(key), 0);
	
	if (optimizer) {
		/* @TODO(krakjoe) better */
		value = zend_string_init(ZEND_STRL("0x7fffff0ff"), 0);

		zend_alter_ini_entry(key, value, PHP_INI_SYSTEM, PHP_INI_STAGE_ACTIVATE);

		zend_string_release(value);
	}
	zend_string_release(key);
}

PHP_MINIT_FUNCTION(componere)
{
	PHP_MINIT(Componere_Definition)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Componere_Patch)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Componere_Method)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Componere_Value)(INIT_FUNC_ARGS_PASSTHRU);

#ifndef ZSTR_KNOWN
	php_componere_name_function = zend_string_init(ZEND_STRL("function"), 1);
#else
	php_componere_name_function = ZSTR_KNOWN(ZEND_STR_FUNCTION);
#endif

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(componere)
{
#ifndef ZSTR_KNOWN
	zend_string_release(php_componere_name_function);
#endif
	return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(componere)
{
#if defined(ZTS) && defined(COMPILE_DL_COMPONERE)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	php_componere_optimizer_adjust();

	PHP_RINIT(Componere_Definition)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_RINIT(Componere_Patch)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_RINIT(Componere_Method)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_RINIT(Componere_Value)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_RINIT(Componere_Reflection)(INIT_FUNC_ARGS_PASSTHRU);

	CG(compiler_options) |= ZEND_COMPILE_GUARDS;

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

ZEND_BEGIN_ARG_INFO_EX(php_componere_cast_arginfo, 0, 0, 2)
       ZEND_ARG_INFO(0, Type)
       ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()

PHP_FUNCTION(Componere_cast)
{
       zend_class_entry *target = NULL;
       zval *object = NULL;

       if (php_componere_parse_parameters("Co", &target, &object) != SUCCESS) {
               php_componere_wrong_parameters("Type and object expected");
               return;
       }

       php_componere_cast(return_value, object, target, 0);
}

PHP_FUNCTION(Componere_cast_by_ref)
{
       zend_class_entry *target = NULL;
       zval *object = NULL;

       if (php_componere_parse_parameters("Co", &target, &object) != SUCCESS) {
               php_componere_wrong_parameters("Type and object expected");
               return;
       }

       php_componere_cast(return_value, object, target, 1);
}

/* {{{ componere_functions[]
 */
static const zend_function_entry componere_functions[] = {
        ZEND_NS_NAMED_FE("Componere", cast, zif_Componere_cast, php_componere_cast_arginfo)
        ZEND_NS_NAMED_FE("Componere", cast_by_ref, zif_Componere_cast_by_ref, php_componere_cast_arginfo)
	PHP_FE_END
};
/* }}} */

/* {{{ componere_module_deps[] */
static const zend_module_dep componere_module_deps[] = {
	ZEND_MOD_REQUIRED("reflection")
	{NULL, NULL, NULL}
}; /* }}} */

/* {{{ componere_module_entry
 */
zend_module_entry componere_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	componere_module_deps,
	"componere",
	componere_functions,
	PHP_MINIT(componere),
	PHP_MSHUTDOWN(componere),
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
