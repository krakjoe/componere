componere
=========
*composing classes at runtime since 2018*

[![Build Status](https://travis-ci.org/krakjoe/componere.svg?branch=master)](https://travis-ci.org/krakjoe/componere)

This extension allows the composition and re-composition of classes at runtime.

This extension also allows monkey patching an instance with a runtime composed class.

All of this is done using dark scary magic, but of the kind you might take to production.

API
===

```php
namespace Componere {
	namespace Abstract {
		abstract class Definition {
			public function addTrait(string $name);
			public function addInterface(string $name);
			public function addMethod(string $name, Method $method);
		}
	}

	class Definition extends Abstract\Definition {
		public function __construct(string $name);
		public function __construct(string $name, string $parent);
		public function __construct(string $name, array $interfaces);
		public function __construct(string $name, string $parent, array $interfaces);

		public function addProperty(string $name, Value $property);
		public function addConstant(string $name, Value $constant); 

		public function getClosure(string $name) : Closure;
		public function getClosures() : array;

		public function register();
	}
	class Patch extends Abstract\Definition {
		public function __construct(object $instance);
		public function __construct(object $instance, array $interfaces);

		public function getClosure(string $name) : Closure;
		public function getClosures() : array;

		public function apply();
		public function revert();
	}
	class Method {
		public function __construct(string $name, Closure $method);
		public function setProtected();
		public function setPrivate();
	}
	class Value {
		public function __construct(string $name, $value);
		public function setProtected();
		public function setPrivate();
	}
}
```

Explanation
===========

These are for the weak ...
