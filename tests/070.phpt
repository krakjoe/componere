--TEST--
Patch incorrect constructor (3)
--FILE--
<?php
use Componere\Patch;

$p = new Patch();
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: instance, or instance and interfaces expected in %s:4
Stack trace:
#0 %s(4): Componere\Patch->__construct()
#1 {main}
  thrown in %s on line 4


