--TEST--
Definition::addConstant undefined value
--FILE--
<?php
use Componere\Definition;
use Componere\Value;

$d = new Definition(A::class);
$d->addConstant("constant", new Value());
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: A::constant cannot be undefined in %s:6
Stack trace:
#0 %s(6): Componere\Definition->addConstant('constant', Object(Componere\Value))
#1 {main}
  thrown in %s on line 6

