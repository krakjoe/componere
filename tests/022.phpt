--TEST--
Disallow addMethod in two definitions
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

$m = new Method(function(){});

$a = new Definition(A::class);
$a->addMethod("function", $m);

$b = new Definition(B::class);
$b->addMethod("function", $m);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: method was already added to A in %s:11
Stack trace:
#0 %s(11): Componere\Abstract\Definition->addMethod('function', Object(Componere\Method))
#1 {main}
  thrown in %s on line 11
