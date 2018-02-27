--TEST--
Definition duplicate registration disallowed
--FILE--
<?php
use Componere\Definition;

$definition = new Definition(A::class);
$definition->register();
$definition->register();
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: could not re-register A in %s:6
Stack trace:
#0 %s(6): Componere\Definition->register()
#1 {main}
  thrown in %s on line 6
