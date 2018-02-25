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
#include "php_componere.h"

#include "src/definition.h"
#include "src/method.h"
#include "src/value.h"

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
	PHP_MINIT(Componere_Method)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Componere_Value)(INIT_FUNC_ARGS_PASSTHRU);

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
	PHP_FE_END
};
/* }}} */

/* {{{ componere_module_entry
 */
zend_module_entry componere_module_entry = {
	STANDARD_MODULE_HEADER,
	"componere",
	componere_functions,
	PHP_MINIT(componere),
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
