/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 8b591539e52e4484b6b37c90d804e8c9f00534e2 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_Componere_cast, 0, 0, 2)
	ZEND_ARG_INFO(0, Type)
	ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()

#define arginfo_Componere_cast_by_ref arginfo_Componere_cast


ZEND_FUNCTION(cast);
ZEND_FUNCTION(cast_by_ref);


static const zend_function_entry ext_functions[] = {
	ZEND_NS_FE("Componere", cast, arginfo_Componere_cast)
	ZEND_NS_FE("Componere", cast_by_ref, arginfo_Componere_cast_by_ref)
	ZEND_FE_END
};
