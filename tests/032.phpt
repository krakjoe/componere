--TEST--
Patch with interfaces
--FILE--
<?php
use Componere\Patch;
use Componere\Method;

class A {}

interface IFace {}

$instance = new A();

$a = new Patch($instance, [IFace::class]);

var_dump($instance instanceof IFace);

$a->apply();

$reflector = $a->getReflector();

var_dump($instance instanceof IFace,
	 $reflector->implementsInterface(IFace::class));
?>
--EXPECT--
bool(false)
bool(true)
bool(true)

