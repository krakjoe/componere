--TEST--
Basic composition
--FILE--
<?php 
compose(A::class, [
	"member" => function() {
		return true;
	}
]);

$a = new A();

var_dump($a->member());
?>
--EXPECT--
bool(true)
