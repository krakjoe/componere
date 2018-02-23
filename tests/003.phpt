--TEST--
Basic recomposition
--FILE--
<?php 
class A {
	public function thing() {
		return true;
	}
}

compose(A::class, [
	"member" => function() {
		return false;
	}
], A::class);

$a = new A();

var_dump($a->member());
?>
--EXPECT--
bool(false)
