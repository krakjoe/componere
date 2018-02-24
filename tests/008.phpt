--TEST--
Disallow re-composition of traits
--FILE--
<?php
trait A {}

compose(A::class, [
	"thing" => function(){}
]);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: cannot redeclare trait A in %s:5
Stack trace:
#0 %s(5): compose('A', Array)
#1 {main}
  thrown in %s on line 5
