var zinnia = require('./build/Release/zinnia.node');

var r = zinnia.Recognizer();
console.log(r.open('/usr/lib/zinnia/model/tomoe/handwriting-ja.model'));
