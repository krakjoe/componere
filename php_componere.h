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

#ifndef PHP_COMPONERE_H
# define PHP_COMPONERE_H

extern zend_module_entry componere_module_entry;
# define phpext_componere_ptr &componere_module_entry

# define PHP_COMPONERE_VERSION "2.1.1dev"

# if defined(ZTS) && defined(COMPILE_DL_COMPONERE)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_COMPONERE_H */
