/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: a932abc5116ff48bc96657b1476a3d73dddc49cf */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Componere_cast, 0, 2, IS_OBJECT, 0)
	ZEND_ARG_TYPE_INFO(0, Type, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, object, IS_OBJECT, 0)
ZEND_END_ARG_INFO()

#define arginfo_Componere_cast_by_ref arginfo_Componere_cast


ZEND_FUNCTION(cast);
ZEND_FUNCTION(cast_by_ref);


static const zend_function_entry ext_functions[] = {
	ZEND_NS_FE("Componere", cast, arginfo_Componere_cast)
	ZEND_NS_FE("Componere", cast_by_ref, arginfo_Componere_cast_by_ref)
	ZEND_FE_END
};
