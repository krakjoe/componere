--TEST--
Patch::isApplied
--FILE--
<?php
use Componere\Patch;

class A{}

$instance = new A();

$a = new Patch($instance);

var_dump($a->isApplied());

$a->apply();

var_dump($a->isApplied());
?>
--EXPECT--
bool(false)
bool(true)

