--TEST--
Disallow re-compose internal class
--FILE--
<?php 
compose(RuntimeException::class, []);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: cannot redeclare internal class RuntimeException in %s:2
Stack trace:
#0 %s(2): compose('RuntimeExceptio...', Array)
#1 {main}
  thrown in %s on line 2
