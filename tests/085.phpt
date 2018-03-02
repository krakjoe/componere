--TEST--
Patch::derive incorrect invocation (2)
--FILE--
<?php
use Componere\Patch;

$instance = new stdClass;

$patch = new Patch($instance);

$date = new DateTime();

$patch->derive($date);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: stdClass is not compatible with DateTime in %s:10
Stack trace:
#0 %s(10): Componere\Patch->derive(Object(DateTime))
#1 {main}
  thrown in %s on line 10

