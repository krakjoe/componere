--TEST--
Basic recomposition non-public
--FILE--
<?php 
class A {
	private function thing() {
		return true;
	}
}

compose(A::class, [
	"member" => function() {
		return self::thing();
	}
], A::class);

$a = new A();

var_dump($a->member());
?>
--EXPECT--
bool(true)
