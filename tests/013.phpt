--TEST--
Definition re-composition with defaults
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

class A {
	/**1
	* @comment
	**/
	public $p = 1;
	static $s = 2;
	/**2
	* @comment
	**/
	const  C  = 3;
}

$definition = new Definition(A::class);

$definition->register();

$a = new A();

var_dump($a->p);
var_dump(A::$s);
var_dump(A::C);
?>
--EXPECT--
int(1)
int(2)
int(3)


