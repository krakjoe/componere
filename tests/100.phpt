--TEST--
relink classes and methods and objects
--FILE--
<?php declare(strict_types=1);

use Componere\Definition;
use Componere\Method;
use Componere\Value;

class A {

    public function foo() : A {
        return $this;
    }
}

class MockA extends A {}

$def      = new Definition(A::class);
$instance = new MockA;

$closure = (new ReflectionClass(MockA::class))
    ->getMethod('foo')
    ->getClosure($instance);

$nested_closure = function(...$args) use ($closure) {
    echo "nested\n";
    return $closure(...$args);
};

$def->addMethod('foo', new Method($nested_closure));
$def->register();

var_dump((new A)->foo());
?>
--EXPECT--
nested
object(MockA)#2 (0) {
}

