--TEST--
Definition get non-existent closure
--FILE--
<?php
use Componere\Definition;

$d = new Definition(E::class);
$d->getClosure("none");
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: could not find E::none in %s:5
Stack trace:
#0 %s(5): Componere\Definition->getClosure('none')
#1 {main}
  thrown in %s on line 5


