--TEST--
Definition disallow redeclare trait
--FILE--
<?php
use Componere\Definition;

trait ITrait {}

$componere = new Definition(ITrait::class);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: cannot redeclare trait ITrait in %s:6
Stack trace:
#0 %s(6): Componere\Definition->__construct('ITrait')
#1 {main}
  thrown in %s on line 6
