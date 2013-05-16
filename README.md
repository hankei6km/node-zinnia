# node-zinnia

Bindings to Zinnia: Hand writing recognition on Node.js.

## Requirements

* [Node.js](http://nodejs.org)
* [Zinnia](http://zinnia.sourceforge.net/)
* [Zinnia-Tomoe](http://zinnia.sourceforge.net/) (optional requirement)

## Installation

    $ git clone https://github.com/hankei6km/node-zinnia.git
    $ cd node-zinnia
    $ npm install

## API

Use `var zinnia = require('node-zinnia')` to access this module.

This version implemented APIs below from
zinnia([C++ API](http://zinnia.sourceforge.net/doxygen/annotated.html)).

* `Recognizer.open`
* `Recognizer.classify`: this method is the asynchronous version.
* `Character.clear`
* `Character.set_width`
* `Character.set_height`
* `Result.size`
* `Result.value`
* `Result.score`

and

* `Recognizer.classifySync`

## Example

Refer to `test/test.js`.

## License

Copyright (c) 2013 hankei6km (MIT License)
