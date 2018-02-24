--TEST--
Disallow re-composition of interfaces
--FILE--
<?php
interface A {}

compose(A::class, [
	"thing" => function(){}
]);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: cannot redeclare interface A in %s:5
Stack trace:
#0 %s(5): compose('A', Array)
#1 {main}
  thrown in %s on line 5
