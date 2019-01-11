--TEST--
addTrait takes trait as source of truth
--FILE--
<?php
class A {
	private $property = 3;

	public function foo() : A {
		return $this;
	}

	private function method() {
		return 1;
	}
}

trait MockMethodsForA {

	public function foo() : A {
		var_dump($this->method());
		var_dump($this->property);
		return $this;
	}

	private function method() {
		return 2;
	}
}

$definition = new \Componere\Definition(A::class);
$definition->addTrait(MockMethodsForA::class);
$definition->register();

$a = new A();

var_dump($a->foo());
?>
--EXPECT--
int(2)
int(3)
object(A)#2 (1) {
  ["property":"A":private]=>
  int(3)
}




