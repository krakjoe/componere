--TEST--
Patch::derive incorrect invocation (1)
--FILE--
<?php
use Componere\Patch;

$instance = new stdClass;

$patch = new Patch($instance);

$patch->derive([]);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: object expected in %s:8
Stack trace:
#0 %s(8): Componere\Patch->derive(Array)
#1 {main}
  thrown in %s on line 8

