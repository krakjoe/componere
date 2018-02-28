--TEST--
Definition::getClosure fail
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

class A {
}

$definition = new Definition(A::class);

$definition->addMethod("thing", new Method(function() {
	
}));

$definition->register();

$definition->getClosure("thing");
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: cannot get closure after registration in %s:16
Stack trace:
#0 %s(16): Componere\Abstract\Definition->getClosure('thing')
#1 {main}
  thrown in %s on line 16


