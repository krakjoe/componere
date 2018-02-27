--TEST--
Disallow getClosures after registration
--FILE--
<?php
use Componere\Definition;

$a = new Definition(A::class);
$a->register();
$a->getClosures();
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: cannot get closures after registration in %s:6
Stack trace:
#0 %s(6): Componere\Definition->getClosures()
#1 {main}
  thrown in %s on line 6
