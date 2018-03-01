--TEST--
Definition incorrect addInterface
--FILE--
<?php
use Componere\Definition;

$d = new Definition(E::class);
$d->addInterface(null);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: interface expected in %s:5
Stack trace:
#0 %s(5): Componere\Abstract\Definition->addInterface('')
#1 {main}
  thrown in %s on line 5


