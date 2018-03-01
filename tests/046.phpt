--TEST--
Definition compose user class that extends internal class
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

class E extends RuntimeException {}

$e = new Definition(E::class);
$e->addMethod("__construct", new Method(function(...$args){
	parent::__construct(...$args);

	var_dump($this instanceof E);
}));
$e->register();

$i = new E();

var_dump($i instanceof RuntimeException);
?>
--EXPECT--
bool(true)
bool(true)

