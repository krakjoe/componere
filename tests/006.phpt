--TEST--
Disallow re-composition of internal class
--FILE--
<?php 
use Componere\Definition;

(new Definition(RuntimeException::class))->register();
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: cannot redeclare internal class RuntimeException in %s:4
Stack trace:
#0 %s(4): Componere\Definition->__construct('RuntimeExceptio...')
#1 {main}
  thrown in %s on line 4

