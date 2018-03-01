--TEST--
Definition constructor
--FILE--
<?php
use Componere\Definition;

interface IFace {}

$d = new Definition(E::class, RuntimeException::class, [IFace::class]);
$d->register();

$i = new E();
var_dump($i instanceof E, 
	 $i instanceof RuntimeException, 
	 $i instanceof IFace);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)


