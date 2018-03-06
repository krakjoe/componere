--TEST--
Componere\cast incorrect invocation (3)
--FILE--
<?php
use function Componere\cast;

$instance = new stdClass;

trait ITrait {}

cast(ITrait::class, $instance);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: cannot cast to trait ITrait in %s:8
Stack trace:
#0 %s(8): Componere\cast('ITrait', Object(stdClass))
#1 {main}
  thrown in %s on line 8
