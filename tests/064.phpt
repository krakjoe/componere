--TEST--
Definition incorrect addConstant (1)
--FILE--
<?php
use Componere\Definition;
use Componere\Value;

$d = new Definition(E::class);
$d->addConstant("CONSTANT", 
	(new Value(1))->setStatic());
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: E::CONSTANT cannot be declared static in %s:7
Stack trace:
#0 %s(7): Componere\Definition->addConstant('CONSTANT', Object(Componere\Value))
#1 {main}
  thrown in %s on line 7


