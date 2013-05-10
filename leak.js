var zinnia = require('./build/Release/zinnia.node');

var r = zinnia.Recognizer();
var s = zinnia.Character();

if(r.open('/usr/lib/zinnia/model/tomoe/handwriting-ja.model')){

  var f = function(){
    for(var cnt=0; cnt<100; cnt++){
      s.set_width(300);
      s.set_height(300);

      s.add(0, 51, 29)
      s.add(0, 117, 41)
      s.add(1, 99, 65)
      s.add(1, 219, 77)
      s.add(2, 27, 131)
      s.add(2, 261, 131)
      s.add(3, 129, 17)
      s.add(3, 57, 203)
      s.add(4, 111, 71)
      s.add(4, 219, 173)
      s.add(5, 81, 161)
      s.add(5, 93, 281)
      s.add(6, 99, 167)
      s.add(6, 207, 167)
      s.add(6, 189, 245)
      s.add(7, 99, 227)
      s.add(7, 189, 227)
      s.add(8, 111, 257)
      s.add(8, 189, 245)

      var result = r.classify(s, 10);
      var size = result.size();
      for(var i=0; i<size; i++){
        //console.log(result.value(i) + ' ' + result.score(i));
      }
      result = null;
      size = null;
    }
    console.log('--');
    gc();
    setTimeout(f, 1000);
  }
  setTimeout(f, 1000);

}else{
  console.log('model can\'t open');
}
