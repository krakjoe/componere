--TEST--
Definition disallow redeclare interface
--FILE--
<?php
use Componere\Definition;

interface IFace {}

$componere = new Definition(IFace::class);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: cannot redeclare interface IFace in %s:6
Stack trace:
#0 %s(6): Componere\Definition->__construct('IFace')
#1 {main}
  thrown in %s on line 6
