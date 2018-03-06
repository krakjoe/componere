--TEST--
Componere\cast incorrect invocation (4)
--FILE--
<?php
use function Componere\cast;

$instance = new stdClass;

abstract class IAbstract {}

cast(IAbstract::class, $instance);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: cannot cast to abstract IAbstract in %s:8
Stack trace:
#0 %s(8): Componere\cast('IAbstract', Object(stdClass))
#1 {main}
  thrown in %s on line 8
