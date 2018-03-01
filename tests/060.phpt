--TEST--
Definition incorrect addTrait (1)
--FILE--
<?php
use Componere\Definition;

$d = new Definition(E::class);
$d->addTrait(Nothing::class);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: trait expected in %s:5
Stack trace:
#0 %s(5): Componere\Abstract\Definition->addTrait('Nothing')
#1 {main}
  thrown in %s on line 5


