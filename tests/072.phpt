--TEST--
Patch incorrect getClosure
--FILE--
<?php
use Componere\Patch;

class A {}

$a = new A();

$p = new Patch($a);
$p->getClosure(new stdClass);
?>
--EXPECTF--
Fatal error: Uncaught InvalidArgumentException: name expected in %s:9
Stack trace:
#0 %s(9): Componere\Patch->getClosure(Object(stdClass))
#1 {main}
  thrown in %s on line 9


