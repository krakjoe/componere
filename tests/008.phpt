--TEST--
Disallow re-composition of traits
--FILE--
<?php
use Componere\Definition;

trait A {}

(new Definition(A::class))->register();
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: cannot redeclare trait A in %s:6
Stack trace:
#0 %s(6): Componere\Definition->__construct('A')
#1 {main}
  thrown in %s on line 6

