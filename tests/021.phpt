--TEST--
Disallow addMethod after register
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

$definition = new Definition(A::class);
$definition->register();
$definition->addMethod("function", new Method(function(){}));
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: A is already registered, cannot add method function in %s:7
Stack trace:
#0 %s(7): Componere\Abstract\Definition->addMethod('function', Object(Componere\Method))
#1 {main}
  thrown in %s on line 7
