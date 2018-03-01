--TEST--
Definition addInterface after registration
--FILE--
<?php
use Componere\Definition;

interface I {}

$d = new Definition(E::class);
$d->addInterface(I::class);
$d->register();
$d->addInterface(I::class);
?>
--EXPECTF--
Fatal error: Uncaught RuntimeException: E is already registered, cannot add interface I in %s:9
Stack trace:
#0 %s(9): Componere\Abstract\Definition->addInterface('I')
#1 {main}
  thrown in %s on line 9


