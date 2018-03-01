--TEST--
Unsupported clone
--FILE--
<?php
use Componere\Definition;

$d = new Definition(A::class);

clone $d;
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: objects of type Componere\Definition do not support cloning in %s:6
Stack trace:
#0 {main}
  thrown in %s on line 6




