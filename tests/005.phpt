--TEST--
Basic composition magic
--FILE--
<?php 
compose(A::class, [
	"__construct" => function(){ echo "ctor\n"; },
	"__destruct" => function(){ echo "dtor\n"; },
	"clone" => function(){ echo "clone\n"; },
	"__get" => function(){ echo "get\n"; },
	"__set" => function(){ echo "set\n"; },
	"__unset" => function(){ echo "unset\n"; },
	"__call" => function(){ echo "call\n"; },
	"__callStatic" => function(){ echo "callstatic\n"; },
	"__toString" => function(){ echo "tostring\n"; return ""; },
	"__debugInfo" => function(){ echo "debuginfo\n"; },
]);

$a = new A();

var_dump($a->get);
$a->set = 10;
unset($a->set);

$a->call();
A::callStatic();

(string) $a;
var_dump($a);
?>
--EXPECT--
ctor
get
NULL
set
unset
call
callstatic
tostring
debuginfo
object(A)#10 (0) {
}
dtor
