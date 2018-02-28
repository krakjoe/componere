--TEST--
Disallow addConstant duplicates
--FILE--
<?php
use Componere\Definition;
use Componere\Property;
use Componere\Value;

$a = new Definition(A::class);
$a->addConstant("NAME", new Value(0));
$a->addConstant("NAME", new Value(0));
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: cannot redeclare A::NAME in %s:8
Stack trace:
#0 %s(8): Componere\Definition->addConstant('NAME', Object(Componere\Value))
#1 {main}
  thrown in %s on line 8
