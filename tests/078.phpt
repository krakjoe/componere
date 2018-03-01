--TEST--
Unsupported unset
--FILE--
<?php
use Componere\Definition;

$d = new Definition(A::class);

unset($d->not_allowed);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: objects of type Componere\Definition do not support properties in %s:6
Stack trace:
#0 {main}
  thrown in %s on line 6




