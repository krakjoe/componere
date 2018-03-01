--TEST--
Definition::isRegistered
--FILE--
<?php
use Componere\Definition;

$a = new Definition(A::class);

var_dump($a->isRegistered());

$a->register();

var_dump($a->isRegistered());
?>
--EXPECT--
bool(false)
bool(true)

