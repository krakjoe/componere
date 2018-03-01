--TEST--
Patch incorrect constructor (1)
--FILE--
<?php
use Componere\Patch;

$p = new Patch("", []);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: instance and interfaces expected in %s:4
Stack trace:
#0 %s(4): Componere\Patch->__construct('', Array)
#1 {main}
  thrown in %s on line 4


