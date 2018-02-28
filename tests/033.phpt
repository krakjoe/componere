--TEST--
Patch with interfaces after construction
--FILE--
<?php
use Componere\Patch;
use Componere\Method;

class A {}

interface IFace {}

$instance = new A();

$a = new Patch($instance);

var_dump($instance instanceof IFace);

$a->addInterface(IFace::class);
$a->apply();

var_dump($instance instanceof IFace);
?>
--EXPECT--
bool(false)
bool(true)

