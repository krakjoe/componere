--TEST--
Basic composition with inheritance
--FILE--
<?php 
class A {
	public function thing() {
		return true;
	}
}

compose(B::class, [
	"member" => function() {
		return parent::thing();
	}
], A::class);

$b = new B();

var_dump($b->member());
?>
--EXPECT--
bool(true)
