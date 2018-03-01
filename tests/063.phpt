--TEST--
Definition incorrect addConstant (1)
--FILE--
<?php
use Componere\Definition;

$d = new Definition(E::class);
$d->addConstant(new stdClass, new stdClass);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: name and value expected in %s:5
Stack trace:
#0 %s(5): Componere\Definition->addConstant(Object(stdClass), Object(stdClass))
#1 {main}
  thrown in %s on line 5


