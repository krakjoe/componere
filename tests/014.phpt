--TEST--
Definition re-composition with magic
--FILE--
<?php
use Componere\Definition;
use Componere\Method;

class A {
	public function __construct(){ echo "ctor\n"; }
	public function __destruct() { echo "dtor\n"; }
	public function __clone() { echo "clone\n"; }
	public function __get($k) { echo "get\n"; }
	public function __set($k, $v) { echo "set\n"; }
	public function __unset($k) { echo "unset\n"; }
	public function __call($one, $two) { echo "call\n"; }
	public static function __callstatic($one, $two) { echo "callstatic\n"; }
	public function __toString() { echo "tostring\n"; return __CLASS__; }
	public function __debugInfo() { echo "debuginfo\n"; return []; }
}

$definition = new Definition(A::class);

$definition->register();

$a = new A();

$b = clone $a;

$a->get;
$a->set = 1;
unset($a->un);
$a->called();
A::calledStatic();
(string)$a;
var_dump($a);
?>
--EXPECTF--
ctor
clone
get
set
unset
call
callstatic
tostring
debuginfo
object(A)#%d (%d) {
}
dtor
dtor
